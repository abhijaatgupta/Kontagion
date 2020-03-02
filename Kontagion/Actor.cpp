#include "Actor.h"
#include "StudentWorld.h"
using namespace std;
#define PI 3.14159265
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//implementing Actor class
Actor::Actor(int imID, int lives, Direction dir, double startX, double startY, int depth, double size, int health, StudentWorld* sWorld) : GraphObject(imID, startX, startY, dir, depth, size){
    m_health = health;
    m_lives = lives;
    m_sworld = sWorld;
}
Actor::~Actor(){
}
void Actor::doSomething(){
}
StudentWorld* Actor::getAPointerToMyStudentWorld(){
    return m_sworld;
}
int Actor::getHealth(){
    return m_health;
}
int Actor::getLives(){
    return m_lives;
}
void Actor::setHealth(int health){
    m_health = health;
}
void Actor::setLives(int lives){
    m_lives = lives;
}
//implementing Socrates class
Socrates::Socrates(StudentWorld* sWorld) : Actor(IID_PLAYER, 3, 0, 0.0, 128.0, 0, 1, 100, sWorld){
    m_flames = 5;
    m_sprays = 20;
}
Socrates::~Socrates(){
}
void Socrates::doSomething(){
    if (getLives() == 0){
        return;
    }
    double x = getX();
    double y = getY();
    int ch;
    if (getAPointerToMyStudentWorld()->getKey(ch)) {
    // user hit a key during this tick!
        switch (ch)
        {
        case KEY_PRESS_LEFT:
        //... move Socrates counterclockwise ...;
                setDirection(getDirection()+5);
                moveTo(VIEW_RADIUS * (1-cos(getDirection()*PI/180)), (VIEW_RADIUS * (1-sin(getDirection()*PI/180))));
                break;
        case KEY_PRESS_RIGHT:
        //... move Socrates clockwise...;
                setDirection(getDirection()-5);
                moveTo(VIEW_RADIUS * (1 -cos(getDirection()*PI/180)), (VIEW_RADIUS * (1 -sin(getDirection()*PI/180))));
                break;
        case KEY_PRESS_SPACE:
                if (m_sprays > 0){
                    getPositionInThisDirection(getDirection(), 1, x, y);
                    Spray* spray = new Spray(getAPointerToMyStudentWorld(), x, y, getDirection());
                    getAPointerToMyStudentWorld()->addToAllActors(spray);
                    m_sprays--;
                    getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
        case KEY_PRESS_ENTER:
                if (m_flames > 0){
                    Flame* flame;
                    for (int i = 0; i < 16; i++){
                        getPositionInThisDirection(getDirection() + (i*22), 2*SPRITE_RADIUS, x, y);
                        flame = new Flame(getAPointerToMyStudentWorld(), x, y, getDirection()+(i*22));
                        getAPointerToMyStudentWorld()->addToAllActors(flame);
                    }
                    m_flames--;
                    getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
        }
    }
    else {
        if (m_sprays < 20){
            m_sprays++;
        }
    }
}
int Socrates::getSprays(){
    return m_sprays;
}
void Socrates::useSpray(){
    m_sprays--;
}
int Socrates::getFlames(){
    return m_flames;
}
void Socrates::useFlame(){
    m_flames--;
}
void Socrates::gainFlame(){
    m_flames += 5;
}
bool Socrates::isDamageable(){
    return true;
}
//implementing dirt class
Dirt::Dirt(StudentWorld* sWorld, double x, double y) : Actor(IID_DIRT, 1, 0, x, y, 1, 1, 1, sWorld){
}
Dirt::~Dirt(){
}
bool Dirt::isDamageable(){
    return true;
}
//implementing Projectile class
Projectile::Projectile(StudentWorld* sWorld, double x, double y, Direction dir, int dam, int pix, int imid) : Actor(imid, 1, dir, x, y, 1, 1, 1, sWorld){
    m_pixels = pix;
    m_damage = dam;
}
Projectile::~Projectile(){
}
void Projectile::doSomething(){
    if (getLives() == 0){
        return;
    }
    Actor* damagedAct;
    if (getAPointerToMyStudentWorld()->damageableObjectAt(getX(), getY(), damagedAct)){
        //damage the object
        damagedAct->setHealth(damagedAct->getHealth()-m_damage);
        if (damagedAct->getHealth() > 0){
            if (damagedAct->isSalmonella()){
                getAPointerToMyStudentWorld()->playSound(SOUND_SALMONELLA_HURT);
            }
            else if(isBacteria()){
                getAPointerToMyStudentWorld()->playSound(SOUND_ECOLI_HURT);
            }
        }
        if (damagedAct->getHealth() <= 0){
            if (damagedAct->isSalmonella()){
                getAPointerToMyStudentWorld()->playSound(SOUND_SALMONELLA_DIE);
            }
            else if(isBacteria()){
                getAPointerToMyStudentWorld()->playSound(SOUND_ECOLI_DIE);
            }
            if (damagedAct->isBacteria()){
                if (damagedAct->getLives() > 0){
                    getAPointerToMyStudentWorld()->increaseScore(100);
                    if (randInt(0, 1) == 0){
                        Food* newFood = new Food(getAPointerToMyStudentWorld(), damagedAct->getX(), damagedAct->getY());
                        getAPointerToMyStudentWorld()->addToAllActors(newFood);
                    }
                }
            }
            damagedAct->setLives(damagedAct->getLives()-1);
        }
        setLives(0);
        m_damage = 0;
        return;
    }
    else{
        moveAngle(getDirection(), SPRITE_RADIUS*2);
        m_pixels -= SPRITE_RADIUS*2;
    }
    if (m_pixels <= 0){
        setLives(0);
        m_damage = 0;
    }
}
//implementing spray class
Spray::Spray(StudentWorld* sWorld, int x, int y, Direction dir) : Projectile(sWorld, x, y, dir, 2, 112, IID_SPRAY){
}
Spray::~Spray(){
}
//implementing flame class
Flame::Flame(StudentWorld* sWorld, int x, int y, Direction dir) : Projectile(sWorld, x, y, dir, 5, 32, IID_FLAME){
}
Flame::~Flame(){
}
//implementing food class
Food::Food(StudentWorld* sWorld, int x, int y) : Actor(IID_FOOD, 1, 90, x, y, 1, 1, 1, sWorld){
}
//implementing droppedItem class
droppedItem::droppedItem(StudentWorld* sWorld, double x, double y, int Imid, double life) : Actor(Imid, 1, 0, x, y, 1, 1, 1, sWorld){
    m_lifetime = life;
}
//implementing Fungus class
Fungus::Fungus(StudentWorld* sWorld, double x, double y, double life) : droppedItem(sWorld, x, y, IID_FUNGUS, life){
}
void Fungus::doSomething(){
    if (getLives() <= 0){
        return;
    }
    if (getAPointerToMyStudentWorld()->overlapWithSocrates(getX(), getY())){
        getAPointerToMyStudentWorld()->increaseScore(-50);
        setLives(0);
        getAPointerToMyStudentWorld()->getSocrates()->setHealth(getAPointerToMyStudentWorld()->getSocrates()->getHealth() - 20);
        getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_HURT);
        return;
    }
    if (getlifetime() <= 0){
        setLives(0);
    }
    lifeLoss();
}
//implementing ExtraLife class
ExtraLife::ExtraLife(StudentWorld* sWorld, double x, double y, double life) : droppedItem(sWorld, x, y, IID_EXTRA_LIFE_GOODIE, life){
}
void ExtraLife::doSomething(){
    if (getLives() <= 0){
        return;
    }
    if (getAPointerToMyStudentWorld()->overlapWithSocrates(getX(), getY())){
        getAPointerToMyStudentWorld()->increaseScore(500);
        setLives(0);
        getAPointerToMyStudentWorld()->playSound(SOUND_GOT_GOODIE);
        getAPointerToMyStudentWorld()->incLives();
//        getAPointerToMyStudentWorld()->getSocrates()->detLives(getAPointerToMyStudentWorld()->getSocrates()->getLives() + 1);
        return;
    }
    if (getlifetime() <= 0){
        setLives(0);
    }
    lifeLoss();
}
//implementing FlameCharge class
FlameCharge::FlameCharge(StudentWorld* sWorld, double x, double y, double life) : droppedItem(sWorld, x, y, IID_FLAME_THROWER_GOODIE, life){
}
void FlameCharge::doSomething(){
    if (getLives() <= 0){
        return;
    }
    if (getAPointerToMyStudentWorld()->overlapWithSocrates(getX(), getY())){
        getAPointerToMyStudentWorld()->increaseScore(300);
        setLives(0);
        getAPointerToMyStudentWorld()->playSound(SOUND_GOT_GOODIE);
        //socrates gains five flames
        getAPointerToMyStudentWorld()->getSocrates()->gainFlame();
        return;
    }
    if (getlifetime() <= 0){
        setLives(0);
    }
    lifeLoss();
}
//implementing RestoreHealth class
RestoreHealth::RestoreHealth(StudentWorld* sWorld, double x, double y, double life) : droppedItem(sWorld, x, y, IID_RESTORE_HEALTH_GOODIE, life){
}
void RestoreHealth::doSomething(){
    if (getLives() <= 0){
        return;
    }
    if (getAPointerToMyStudentWorld()->overlapWithSocrates(getX(), getY())){
        getAPointerToMyStudentWorld()->increaseScore(250);
        setLives(0);
        getAPointerToMyStudentWorld()->playSound(SOUND_GOT_GOODIE);
        //restore socrates health to 100
        getAPointerToMyStudentWorld()->getSocrates()->setHealth(100);
        return;
    }
    if (getlifetime() <= 0){
        setLives(0);
    }
    lifeLoss();
}
// implementing Bacteria class
Bacteria::Bacteria(StudentWorld* sWorld, double x, double y, int health, int IID) : Actor(IID, 1, 90, x, y, 0, 1, health, sWorld){
    m_movePlanDist = 0;
    m_foodEaten = 0;
    sWorld->playSound(SOUND_BACTERIUM_BORN);
}
void Bacteria::resetMovement(){
    setDirection(randInt(0, 359));
    m_movePlanDist = 10;
}
bool Bacteria::canMoveForward(double direction, double& x, double& y, double units){
    for (double i = 0; i < units; i++){
        getPositionInThisDirection(direction, 1.0, x, y);
        if (!getAPointerToMyStudentWorld()->overlapWithDirt(x, y) && sqrt((VIEW_WIDTH/2.0 - x)*(VIEW_WIDTH/2.0 - x) + (VIEW_HEIGHT/2.0 - y)*(VIEW_HEIGHT/2.0 - y)) < VIEW_RADIUS){
            moveTo(x, y);
        }
        else return false;
    }
    return true;
}
void Bacteria::reproduce(int Case){
    int x = getX();
    int y = getY();
    if (y < VIEW_HEIGHT/2){
        y += SPRITE_WIDTH/2;
    }
    else if (y > VIEW_HEIGHT/2){
        y -= SPRITE_WIDTH/2;
    }
    if (x < VIEW_HEIGHT/2){
        x += SPRITE_WIDTH/2;
    }
    else if (x > VIEW_HEIGHT/2){
        x -= SPRITE_WIDTH/2;
    }
    Bacteria* newBacteria;
    switch (Case) {
        case 1:
            newBacteria = new regSalmonella(getAPointerToMyStudentWorld(), x, y);
            getAPointerToMyStudentWorld()->addToAllActors(newBacteria);
            break;
        case 2:
            newBacteria = new aggroSalmonella(getAPointerToMyStudentWorld(), x, y);
            getAPointerToMyStudentWorld()->addToAllActors(newBacteria);
            break;
        case 3:
            newBacteria = new Ecoli(getAPointerToMyStudentWorld(), x, y);
            getAPointerToMyStudentWorld()->addToAllActors(newBacteria);
            break;
    }
}
void Bacteria::salMoveAlgorithm(){
    if (getmovePlan() > 0){
        setmovePlan(getmovePlan()-1);
        double x;
        double y;
        //getPositionInThisDirection(getDirection(), 3, x, y);
        if (canMoveForward(getDirection(), x, y, 3)){
            moveTo(x, y);
        }
        else {
            resetMovement();
        }
        return;
    }
    else {
        double x;
        double y;
        if (getAPointerToMyStudentWorld()->findNearestFood(x, y, 128, getX(), getY())){
            double direction = atan2(y - getY(), x - getX())* 180/PI;
            if (canMoveForward(direction, x, y, 3)){
                setDirection(direction);
                setmovePlan(9);
            }
            else resetMovement();
        }
        else resetMovement();
    }
}
// implementing regular salmonella
regSalmonella::regSalmonella(StudentWorld* sWorld, double x, double y) : Bacteria(sWorld, x, y, 4, IID_SALMONELLA){
    
}
void regSalmonella::doSomething(){
    if (getLives() <= 0){
        return;
    }
    if (getAPointerToMyStudentWorld()->overlapWithSocrates(getX(), getY())){
        getAPointerToMyStudentWorld()->getSocrates()->setHealth(getAPointerToMyStudentWorld()->getSocrates()->getHealth() - 1);
        getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_HURT);
    }
    else if(getfoodEaten() == 3){
        reproduce(1);
        setFoodEaten(0);
    }
    else if(getAPointerToMyStudentWorld()->overlapWithFood(getX(), getY())){
        setFoodEaten(getfoodEaten()+1);
    }
    salMoveAlgorithm();
}
//implementing aggro salmonella
aggroSalmonella::aggroSalmonella(StudentWorld* sWorld, double x, double y) : Bacteria(sWorld, x, y, 10, IID_SALMONELLA){
    
}
void aggroSalmonella::doSomething(){
    //bool stuckBehindDirt = false;
    bool lockedOnSocrates = false;
    double x = getAPointerToMyStudentWorld()->getSocrates()->getX();
    double y = getAPointerToMyStudentWorld()->getSocrates()->getY();
    if (getLives() <= 0){
        return;
    }
    if (getAPointerToMyStudentWorld()->distActors(getX(), getY(), x, y) <= 72){
        //        cout << x << " " << y << endl;
        //        cout << getX() << " " << getY() << endl;
        double direction = atan2(y - getY(), x - getX())* 180.0/PI;
        if (canMoveForward(direction, x, y, 3)){
            moveTo(x, y);
            //cout << direction << endl;
            setDirection(direction);
            setmovePlan(9);
        }
        else {
            setDirection(direction);
        }
        lockedOnSocrates = true;
    }
    if (getAPointerToMyStudentWorld()->overlapWithSocrates(getX(), getY())){
        getAPointerToMyStudentWorld()->getSocrates()->setHealth(getAPointerToMyStudentWorld()->getSocrates()->getHealth() - 2);
        getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_HURT);
    }
    else if(getfoodEaten() == 3){
        reproduce(2);
        setFoodEaten(0);
    }
    else if(getAPointerToMyStudentWorld()->overlapWithFood(getX(), getY())){
        setFoodEaten(getfoodEaten()+1);
    }
    if (!lockedOnSocrates){
        salMoveAlgorithm();
    }
}
//implementing Ecoli
Ecoli::Ecoli(StudentWorld* sWorld, double x, double y) : Bacteria(sWorld, x, y, 5, IID_ECOLI){
    
}
void Ecoli::doSomething(){
    double x = getAPointerToMyStudentWorld()->getSocrates()->getX();
    double y = getAPointerToMyStudentWorld()->getSocrates()->getY();
    if (getLives() <= 0){
        return;
    }
    if (getAPointerToMyStudentWorld()->overlapWithSocrates(getX(), getY())){
        getAPointerToMyStudentWorld()->getSocrates()->setHealth(getAPointerToMyStudentWorld()->getSocrates()->getHealth() - 4);
        getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_HURT);
    }
    else if(getfoodEaten() == 3){
        reproduce(3);
        setFoodEaten(0);
    }
    else if(getAPointerToMyStudentWorld()->overlapWithFood(getX(), getY())){
        setFoodEaten(getfoodEaten()+1);
    }
    if (getAPointerToMyStudentWorld()->distActors(getX(), getY(), x, y) <= 256){
        double direction = atan2(y - getY(), x - getX())* 180/PI;
        setDirection(direction);
        for (int i = 0; i < 10; i++){
            if (canMoveForward(getDirection(), x, y, 2)){
                setmovePlan(9);
                return;
            }
            else{
                setDirection(getDirection()+10);
            }
        }
    }
}
//implementing pit class
pit::pit(StudentWorld* sWorld, double x, double y) : Actor(IID_PIT, 1, 0, x, y, 0, 1, 1, sWorld) {
    numEcoli = 2;
    numAggroSal = 3;
    numRegSal = 5;
}
void pit::doSomething(){
    if (numRegSal+numAggroSal+numEcoli == 0){
        setLives(0);
    }
    if (randInt(0, 49) == 0){
        int bac = randInt(0, 2);
        if (numRegSal == 0){
            if (numEcoli == 0){
                bac = 1;
            }
            else {
                while (bac == 0)
                    bac = randInt(0, 2);
            }
        }
        else if (numAggroSal == 0){
            if (numEcoli == 0){
                bac = 0;
            }
            else {
                while (bac == 1)
                    bac = randInt(0, 2);
            }
        }
        else if (numEcoli == 0){
                bac = randInt(0, 1);
        }
        Bacteria* newBacteria;
        switch (bac) {
            case 0:
                newBacteria = new regSalmonella(getAPointerToMyStudentWorld(), getX(), getY());
                getAPointerToMyStudentWorld()->addToAllActors(newBacteria);
                numRegSal--;
                break;
            case 1:
                newBacteria = new aggroSalmonella(getAPointerToMyStudentWorld(), getX(), getY());
                getAPointerToMyStudentWorld()->addToAllActors(newBacteria);
                numAggroSal--;
                break;
            case 2:
                newBacteria = new Ecoli(getAPointerToMyStudentWorld(), getX(), getY());
                getAPointerToMyStudentWorld()->addToAllActors(newBacteria);
                numEcoli--;
                break;
        }
    }
}
