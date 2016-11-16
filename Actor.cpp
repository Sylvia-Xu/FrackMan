#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Actor::WillICollide(int x, int y, double radius)
{
    double distance_sq;
    int my_x = getX();
    int my_y = getY();
    distance_sq = double(pow(my_x - x, 2.0)+ pow(my_y - y, 2.0));
    if (pow(distance_sq, 0.5) < radius) {
        return true;
    }
    else return false;
}


Dirt::Dirt(StudentWorld* world, int startX, int startY)
:Actor(world, IID_DIRT, startX, startY, right, 0.25, 3)
{
    setVisible(true);
};

Dirt::~Dirt()
{
    setVisible(false);
}

Boulder::Boulder(StudentWorld* world, int startX, int startY)
:Actor(world, IID_BOULDER, startX, startY, down, 1.0, 1)
{
    setVisible(true);
    m_alive = true;
    m_state = 0;
}

Boulder::~Boulder()
{
    setVisible(false);
    
    
}

void Boulder::doSomething()
{
    StudentWorld* gw = getWorld();
    if (IsDead()) return;
    switch (m_state) {
        case 0:{
            if (getY() == 0) {
                m_state = 1;
                wait_remain = 30;
                break;
            }
            bool EmptyBelow = true;
            for (int x = getX(); x < getX()+4; x++) {
                if (gw->ExistDirt(x, getY()-1)) {
                    EmptyBelow = false;
                }
            }
            if (EmptyBelow == true) {
                // turn into waiting state
                m_state = 1;
                wait_remain = 30;
            }
            break;
        }
           
        case 1:
        {
            wait_remain--;
            if (wait_remain == 0) {
                m_state = 2;
                gw->playSound(SOUND_FALLING_ROCK);
            }
            break;
        }
        case 2:
        {
            bool collide = false;
            // check if the boulder hit the bottom
            if (getY() == 0) {
                setDead();
                collide = true;
            }
            
            // only when the y is larger than 3, it is possible that there is another boulder below
            if (getY() - 4 >= 0) {
                for (int i = 0; i < 4; i++) {
                    // check if the boulder run into another boulder
                    if (gw->CheckBlock(getX()+i, getY()-4)) {
                        setDead();
                        collide = true;
                    }
                    // hit player
                    if (gw->HitPlayer(getX()+i, getY()-4, 3.0)) {
                        gw->AnnoyPlayer(0, true);
                    }

                }
            }
            // check if the boulder run into the dirt
            if (getY() - 1 >= 0) {
                for (int x = getX(); x < getX()+4; x++) {
                    if (gw->ExistDirt(x, getY()-1)) {
                        setDead();
                        collide = true;
                    }
                }
                
            }
            //TODO: run into protester or frackman
            if (gw->HitProtester(getX(), getY(), 3.0, true)) {
                
            }
            
            if (gw->HitPlayer(getX(), getY(), 3.0)) {
//                setDead();
//                collide = true;
            }
            
            if (collide == false) {
                moveTo(getX(), getY()-1);
            }
            
            
            break;
        }
        default: break;
            

    }
    
}

bool Boulder::IsDead()
{
    if (m_alive == false) {
        return true;
    }
    else return false;
}

void Boulder::setDead()
{
    m_alive = false;
}

bool Boulder::WillIBlock(int x, int y)
{
    int my_x = getX();
    int my_y = getY();
    
    // first make sure we are not considering a boulder collide with itself
    if(x != my_x || y != my_y) {
        if ((x >= my_x && x <= my_x + 3)&& (y >= my_y && y <= my_y + 3)) {
                return true;
        }
        else return false;
    }
    
    return false;
    
}

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction dir = right)
:Actor(world, IID_WATER_SPURT, startX, startY, dir, 1.0, 1)
{
    setVisible(true);
    travel_distance = 4;
}

void Squirt::doSomething()
{
    int x = getX();
    int y = getY();
    StudentWorld* gw = getWorld();
    
    bool collision = false;
    if (gw->HitProtester(x, y, 3.0, false)){
        //TODO: the squirt will make protester get annoyed
        setDead();
        collision = true;
    }
    
    if (getDirection() == up) {
        // hit the boundary
        if (getY()+4>=64) {
            collision = true;
            setDead();
        }
        // hit the dirt or boulder
        else {
            for (int i = 0; i < 4; i++) {
                if(gw->CheckBlock(getX()+i, getY()+4) || gw->ExistDirt(getX()+i, getY()+4))
                {
                    collision = true;
                    setDead();
                }
                
            }

        }
    }
    if (getDirection() == down) {
        if (getY()-1<0) {
            collision = true;
            setDead();
        }
        else {
            for (int i = 0; i < 4; i++) {
                if (gw->CheckBlock(getX()+i, getY()-1) || gw->ExistDirt(getX()+i, getY()-1)) {
                    collision = true;
                    setDead();
                }
            }
        }
        
    }
    if (getDirection() == left) {
        if (getX()-1<0) {
            collision = true;
            setDead();
        }
        else if (getY() <= 60) {
            for (int i = 0; i < 4; i++) {
                if (gw->CheckBlock(getX()-1, getY()+i) || gw->ExistDirt(getX()-1, getY()+i)) {
                    collision = true;
                    setDead();
                }
            }
        }
        
    }
    if (getDirection() == right) {
        if (getX()+4>VIEW_WIDTH) {
            collision = true;
            setDead();
        }
        else if (getY() < 60) {
            for (int i = 0; i < 4; i++) {
                if (gw->CheckBlock(getX()+4, getY()+i) || gw->ExistDirt(getX()+4, getY()+i)) {
                    collision = true;
                    setDead();
                }
            }
        }
        
    }
    if (collision == false) {
        switch (getDirection()) {
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            default:
                break;
        }
        travel_distance--;
    }
    
}

void Squirt::setDead()
{
    travel_distance = 0;
}

bool Squirt::IsDead()
{
    if (travel_distance == 0) {
        return true;
    }
    else return false;
}

Squirt::~Squirt()
{
    setVisible(false);
}

Barrel::Barrel(StudentWorld* world, int startX, int startY)
:Actor(world, IID_BARREL, startX, startY, right, 1.0, 2)
{
    discovered = false;
    m_alive = true;
//    setVisible(true);
};

Barrel::~Barrel()
{
    setVisible(false);
}

void Barrel::doSomething()
{
    StudentWorld* gw = getWorld();
    int my_x = getX();
    int my_y = getY();

    
    if(IsDead()) return;
    else if(discovered  == false && gw->PickUpItem(my_x, my_y, 4.0))
    {
        setVisible(true);
        discovered = true;
        return;
    }
    else if (gw->PickUpItem(getX(), getY(), 3.0)) {
        setDead();
        gw->playSound(SOUND_FOUND_OIL);
        gw->increaseScore(1000);
    }
    return;
}

bool Barrel::IsDead()
{
    if (m_alive == false) {
        return true;
    }
    else return false;
}

Goodie::Goodie(StudentWorld* world, int imageID, int startX, int startY)
:Actor(world, imageID, startX, startY, right, 1.0, 2)
{
    m_alive = true;
}
Goodie::~Goodie()
{
    setVisible(false);
}

bool Goodie::IsDead()
{
    if (m_alive == false) {
        return true;
    }
    else return false;
}

void Goodie::setLifetime(bool isGold)
{
    StudentWorld* gw = getWorld();
    if (isGold == false && 300 - 10 * gw->getLevel() > 100) {
        lifetime = 300 - 10 * gw->getLevel();
    }
    else lifetime = 100;
}

bool Goodie::TimeOut()
{
    if (lifetime == 0) {
        return true;
    }
    else return false;
}
Gold_Nugget::Gold_Nugget(StudentWorld* world, int my_state, int startX, int startY)
:Goodie(world, IID_GOLD, startX, startY)
{
    m_state = my_state;
    switch (my_state) {
            // if it is buried within dirt
        case 1:
        {
            discovered = false;
            setVisible(false);
            break;
        }
            // if it is dropped by the frackman
        case 2:
        {
            discovered = true;
            setVisible(true);
            setLifetime(true);
        }
        default:
            break;
    }
};

int Gold_Nugget::StateOfGold()
{
    return m_state;
}

void Gold_Nugget::doSomething()
{
    StudentWorld* gw = getWorld();
    int my_x = getX();
    int my_y = getY();
    
    
    if(IsDead()) return;
    else if(discovered  == false && gw->PickUpItem(my_x, my_y, 4.0))
    {
        setVisible(true);
        discovered = true;
        return;
    }
    else if (m_state == 1 && gw->PickUpItem(getX(), getY(), 3.0)) {
        setDead();
        gw->playSound(SOUND_GOT_GOODIE);
        gw->increaseScore(10);
        gw->PlayerGetNugget();
        
    }
    else if (m_state == 2){
        lifetimeElapse();
        if (gw->PickUpByProtester(getX(), getY(), 3.0)) {
            setDead();
        }
        if (TimeOut()) {
            setDead();
        }
    
    }
    return;

}


Sonar_Kit::Sonar_Kit(StudentWorld* world, int startX, int startY)
:Goodie(world, IID_SONAR, startX, startY)
{
    setVisible(true);
    setLifetime(false);
};


void Sonar_Kit::doSomething()
{
    StudentWorld* gw = getWorld();
    int my_x = getX();
    int my_y = getY();
    
    
    if(IsDead()) return;
    lifetimeElapse();
    if(gw->PickUpItem(my_x, my_y, 3.0))
    {
        setDead();
        gw->playSound(SOUND_GOT_GOODIE);
        gw->PlayerGetSonar();
        gw->increaseScore(75);
        return;
    }
    
    if (TimeOut()) {
        setDead();
    }
}


Water_Pool::Water_Pool(StudentWorld* world, int startX, int startY)
:Goodie(world, IID_WATER_POOL, startX, startY)
{
    setVisible(true);
    setLifetime(false);
}


void Water_Pool::doSomething()
{
    StudentWorld* gw = getWorld();
    int my_x = getX();
    int my_y = getY();
    
    
    if(IsDead()) return;
    lifetimeElapse();
    if(gw->PickUpItem(my_x, my_y, 3.0))
    {
        setDead();
        gw->playSound(SOUND_GOT_GOODIE);
        gw->PlayerGetWater();
        gw->increaseScore(100);
        return;
    }
    
    if (TimeOut()) {
        setDead();
    }

}

Protester::Protester(StudentWorld* world, int imageID, int startX, int startY)
:Actor(world, imageID, startX, startY, left, 1.0, 0)
{
    setVisible(true);
    m_alive = true;
    numSquaresToMoveInCurrentDirection = 8 + rand() % (60 - 8 + 1);
    setRestTime(0);
    shout_counter = 0;
    perpendicular_counter = 0;

}

//int Protester::WaitingTime()
//{
//    return ticksToWaitBetweenMoves;
//}

bool Protester::IsDead()
{
    if (m_alive == false) {
        return true;
    }
    else return false;
}



void Protester::setRestTime(int mode)
{
    StudentWorld* gw = getWorld();
    switch (mode) {
        // normal mode
        case 0:
            ticksToWaitBetweenMoves = (3 - gw->getLevel() / 4) > 0 ? 3 - gw->getLevel() / 4 : 0;
            break;
        // regular protester stunned
        case 1:
            ticksToWaitBetweenMoves = (100 - gw->getLevel()*10) <= 50 ? (100 - gw->getLevel()*10) : 50;
            break;
        
        default:
            break;
    }
    
}

void Protester::updateCounters()
{
    shout_counter--;
    // if the protester does not turn to perpendicular direction, counter increase
    if (perpendicular_counter != 200) {
        perpendicular_counter--;
    }
}

bool Protester::facingFrackMan()
{
    StudentWorld* gw = getWorld();
    
    bool facing = false;
    switch (getDirection()) {
        case up:
            if (gw->access_player()->getY() >= getY()) {
                facing = true;
            }
            break;
        case down:
            if (gw->access_player()->getY() <= getY() ) {
                facing = true;
            }
            break;
        case left:{
            if (gw->access_player()->getX() <= getX()) {
                facing = true;
            }
            break;
        }
        case right:
            if (gw->access_player()->getX() >= getX() ) {
                facing = true;
            }
            break;
        default:
            break;
    }
    return facing;

}

void Protester::ShoutAtFrackMan()
{
    StudentWorld* gw = getWorld();
    if (shout_counter <= 0) {
        gw->playSound(SOUND_PROTESTER_YELL);
        gw->AnnoyPlayer(2, false);
        shout_counter = 15;
    }
    else{
        shout_counter--;
        perpendicular_counter--;
    }

}

bool Protester::upviable(int num_steps)
{
    StudentWorld* gw = getWorld();
    bool up_viable = true;
    if (getY()+3+num_steps>=64) {
        up_viable = false;
    }
    else{
        for (int j = 1; j <= num_steps; j++) {
            for (int i = 0; i < 4; i++) {
                if(gw->HitBoulder(getX()+i, getY()+j, 3.0)
                   || gw->ExistDirt(getX()+i, getY()+3+j))
                {
                    up_viable = false;
                }
                
            }
            
        }
    }
    
    return up_viable;
}

bool Protester::downviable(int num_steps)
{
    StudentWorld* gw = getWorld();
    bool down_viable = true;
    if (getY()-num_steps<0) {
        down_viable = false;
    }
    else{
        for (int j = 1; j <= num_steps; j++) {
            for (int i = 0; i < 4; i++) {
                if (gw->HitBoulder(getX()+i, getY()-j, 3.0)
                    || gw->ExistDirt(getX()+i, getY()-j)) {
                    down_viable = false;
                }
            }
        }
    }
    return down_viable;
}

bool Protester::leftviable(int num_steps)
{
    StudentWorld* gw = getWorld();
    bool left_viable = true;
    
    if (getX()-num_steps<0) {
        left_viable = false;
    }
    else if (getY() >= 60) {
        left_viable = true;
    }
    else{
        for (int j = 1; j <= num_steps; j++) {
            for (int i = 0; i < 4; i++) {
                if (gw->HitBoulder(getX()-j, getY()+i, 3.0)
                    || gw->ExistDirt(getX()-j, getY()+i)) {
                    left_viable = false;
                }
            }
        }
    }
    return left_viable;
}

bool Protester::rightviable(int num_steps)
{
    StudentWorld* gw = getWorld();
    bool right_viable = true;
    
    if (getX()+3+num_steps>VIEW_WIDTH) {
        right_viable = false;
    }
    else if (getY() >= 60){
        right_viable = true;
    }
    else{
        for (int j = 1; j <= num_steps; j++) {
            for (int i = 0; i < 4; i++) {
                if (gw->HitBoulder(getX()+j, getY()+i, 3.0)
                    || gw->ExistDirt(getX()+3+j, getY()+i)) {
                    right_viable = false;
                }
            }
        }
    }
    return right_viable;
}

bool Protester::MoveToFrackMan()
{
    StudentWorld* gw = getWorld();
    bool HorizontalInLine = (getY() == gw->access_player()->getY());
    bool VerticalInLine = (getX() == gw->access_player()->getX());
    
    bool HaveAPath = false;
    
    if (HorizontalInLine && getX() < gw->access_player()->getX() &&
        rightviable(gw->access_player()->getX() - getX()))
    {
        setDirection(right);
        moveTo(getX()+1, getY());
        setSquaresToMove(0);
        HaveAPath = true;
        
    }
    else if(HorizontalInLine && getX() > gw->access_player()->getX() &&
            leftviable(getX() - gw->access_player()->getX()))
    {
        //                gw->playSound(SOUND_GOT_GOODIE);
        setDirection(left);
        moveTo(getX()-1, getY());
        setSquaresToMove(0);
        HaveAPath = true;
        
    }
    else if(VerticalInLine && getY() < gw->access_player()->getY() &&
            upviable(gw->access_player()->getY() - getY()))
    {
        setDirection(up);
        moveTo(getX(), getY()+1);
        setSquaresToMove(0);
        HaveAPath = true;
    }
    else if(VerticalInLine && getY() > gw->access_player()->getY() &&
            downviable(getY() - gw->access_player()->getY()))
    {
        setDirection(down);
        moveTo(getX(), getY()-1);
        setSquaresToMove(0);
        HaveAPath = true;
    }

    if (HaveAPath) {
        shout_counter--;
        perpendicular_counter--;
    }
    return HaveAPath;
}

void Protester::MakeATurn()
{
    if (numSquaresToMoveInCurrentDirection <= 0) {
        bool valid = false;
        int newdirection = 0;
        while (valid == false) {
            //randomly pick a new direction
            //0 for up, 1 for down, 2 for left, 3 for right
            newdirection = rand() % 4;
            
            //check whether the new direction is blocked
            if (newdirection == 0) {
                if (upviable(1) == true) valid = true;
            }
            if (newdirection == 1) {
                if (downviable(1) == true) valid = true;
            }
            if (newdirection == 2) {
                if (leftviable(1) == true) valid = true;
                
            }
            if (newdirection == 3) {
                if (rightviable(1) == true) valid = true;
            }
        }
        
        switch (newdirection) {
            case 0:
                setDirection(up);
                break;
            case 1:
                setDirection(down);
                break;
            case 2:
                setDirection(left);
                break;
            case 3:
                setDirection(right);
                break;
            default:
                break;
        }
        numSquaresToMoveInCurrentDirection = 8 + rand() % (60 - 8 + 1);
        
    }
    else if((getDirection() == up ||getDirection() == down) && (leftviable(1) || rightviable(1)) &&perpendicular_counter <= 0){
        int newdirection = rand() % 2;
        if (leftviable(1) && !rightviable(1)) {
            setDirection(left);
        }
        if (!leftviable(1) && rightviable(1)) {
            setDirection(right);
        }
        if (leftviable(1) && rightviable(1) && newdirection == 0) {
            setDirection(left);
        }
        if (leftviable(1) && rightviable(1) && newdirection == 1) {
            setDirection(right);
        }
        perpendicular_counter = 200;
        numSquaresToMoveInCurrentDirection = 8 + rand() % (60 - 8 + 1);
        
    }
    else if((getDirection() == left ||getDirection() == right) && (upviable(1) || downviable(1)) &&perpendicular_counter <= 0){
        int newdirection = rand() % 2;
        if (upviable(1) && !downviable(1)) {
            setDirection(up);
        }
        if (!upviable(1) && downviable(1)) {
            setDirection(down);
        }
        if (upviable(1) && downviable(1) && newdirection == 0) {
            setDirection(up);
        }
        if (upviable(1) && downviable(1) && newdirection == 1) {
            setDirection(down);
        }
        perpendicular_counter = 200;
        numSquaresToMoveInCurrentDirection = 8 + rand() % (60 - 8 + 1);
    }

}
void Protester::TakeOneStep()
{
    StudentWorld* gw = getWorld();
    if(gw->HitPlayer(getX(), getY(), 4.0)) return;
       
    if (getDirection() == up) {
        moveTo(getX(), getY()+1);
    }
    if (getDirection() == down) {
        moveTo(getX(), getY()-1);
    }
    if (getDirection() == left) {
        moveTo(getX()-1, getY());
    }
    if (getDirection() == right) {
        moveTo(getX()+1, getY());
    }
}

void Protester::StalkPlayer()
{
    StudentWorld* gw = getWorld();
    
    Coord previous = gw->sensePlayer(getX(), getY());
    if (previous.x() == getX()-1) {
        setDirection(left);
    }
    else if(previous.x() == getX()+1){
        setDirection(right);
    }
    else if (previous.y() == getY()-1){
        setDirection(down);
    }
    else if (previous.y() == getY()+1){
        setDirection(up);
    }
    moveTo(previous.x(), previous.y());

    return;
}
void Protester::WalkToExit()
{
    StudentWorld* gw = getWorld();
    if(gw->HitPlayer(getX(), getY(), 4.0)) return;
    
    Coord previous = gw->goBackward(getX(), getY());
    if (previous.x() == getX()-1) {
        setDirection(left);
    }
    else if(previous.x() == getX()+1){
        setDirection(right);
    }
    else if (previous.y() == getY()-1){
        setDirection(down);
    }
    else if (previous.y() == getY()+1){
        setDirection(up);
    }
    moveTo(previous.x(), previous.y());
    
    
//    gw->playSound(SOUND_GOT_GOODIE);
//    int up_distance = 500;
//    int down_distance = 500;
//    int left_distance = 500;
//    int right_distance = 500;
//    
//    int x = getX();
//    int y = getY();
//    
//    // if there is barrier in this way, return a large enough number to prevent protester walk this way
//    if (gw->getDistance(x, y+1) >=0 && !gw->ExistDirt(x, y+4) && !gw->ExistDirt(x+1, y+4) && !gw->ExistDirt(x+2, y+4) && !gw->ExistDirt(x+3, y+4)) {
////        gw->playSound(SOUND_GOT_GOODIE);
//        up_distance = gw->getDistance(x, y+1);
////        std::cout<<"up"<<up_distance<<std::endl;
//        
//    }
//    
//    if (gw->getDistance(x, y-1) >=0 && !gw->ExistDirt(x, y-1) && !gw->ExistDirt(x+1, y-1) && !gw->ExistDirt(x+2, y-1) && !gw->ExistDirt(x+3, y-1)) {
//        down_distance = gw->getDistance(x, y-1);
//        std::cout<<"down"<<down_distance<<std::endl;
//    }
//    
//    if (gw->getDistance(x-1, y) >=0 && !gw->ExistDirt(x-1, y) && !gw->ExistDirt(x-1, y+1) && !gw->ExistDirt(x-1, y+2) && !gw->ExistDirt(x-1, y+3)) {
//        left_distance = gw->getDistance(x-1, y);
//        std::cout<<"left"<<left_distance<<std::endl;
//    }
//    
//    if (gw->getDistance(x+4, y) >=0 && !gw->ExistDirt(x+4, y) && !gw->ExistDirt(x+4, y+1) && !gw->ExistDirt(x+4, y+2) && !gw->ExistDirt(x+4, y+3)) {
//        right_distance = gw->getDistance(x-1, y);
//        std::cout<<"right"<<right_distance<<std::endl;
//    }
//    
//    switch (getDirection()) {
//        case up:
//        {
//            if (down_distance > left_distance)down_distance = 500;
//            break;
//        }
//        case down:
//        {
//            if(up_distance > down_distance)up_distance = 500;
//            break;
//        }
//            
//        case left:
//        {
//            if(right_distance > left_distance) right_distance = 500;
//            break;
//        }
//            
//            
//        case right:
//        {
//            if(left_distance > right_distance) left_distance = 500;
//            break;
//        }
//            
//        default:
//            break;
//    }
//
//    
//    int min;
//    if (left_distance < down_distance) {
//        min = left_distance;
//    }
//    else min = down_distance;
//    if (up_distance < min) {
//        min = up_distance;
//    }
//    if (right_distance < min) {
//        min = right_distance;
//    }
//    
//    
//
//    if (up_distance < 500 || down_distance < 500 || left_distance < 500 || right_distance < 500) {
//        bool changedirection = false;
//
//        switch (getDirection()) {
//            case up:
//            {
//                
//                if (min != up_distance) {
//                    changedirection = true;
//                }
////                if(min == left_distance){
////                    perpendicular = left;
////                }
////                else if(min == right_distance){
////                    perpendicular = right;
////                }
//                break;
//            }
//            case down:
//            {
//                if (min != down_distance) {
//                    changedirection = true;
//                }
////                if(min == left_distance){
////                    perpendicular = left;
////                }
////                else if(min == right_distance){
////                    perpendicular = right;
////                }
//                break;
//            }
//                
//            case left:
//            {
//                if (min != left_distance) {
//                    changedirection = true;
//                }
////                if(min == up_distance){
////                    perpendicular = up;
////                }
////                else if(min == down_distance){
////                    perpendicular = down;
////                }
//                break;
//            }
//                
//                
//            case right:
//            {
//                if (min != right_distance) {
//                    changedirection = true;
//                }
////                if(min == up_distance){
////                    perpendicular = up;
////                }
////                else if(min == down_distance){
////                    perpendicular = down;
////                }
//                break;
//            }
//                
//            default:
//                break;
//        }
//        if (changedirection) {
////            if (perpendicular != none) {
////                setDirection(perpendicular);
////            }
//            if (min == up_distance) {
//                setDirection(up);
//            }
//            else if (min == right_distance){
//                setDirection(right);
//            }
//            else if (min == down_distance) {
//                setDirection(down);
//            }
//            else if (min == left_distance) {
//                setDirection(left);
//            }
//        }
//        
//        TakeOneStep();
//        std::cout<<"new direction"<<getDirection()<<std::endl;
////        switch (getDirection()) {
////            case up:
////                moveTo(x, y+1);
////                break;
////            case down:
////                moveTo(x, y-1);
////                break;
////            case left:
////                moveTo(x-1, y);
////                break;
////            case right:
////                moveTo(x+1, y);
////                break;
////            default:
////                break;
////        }
//        
//    }
    return;
}

void Protester::MazeSearching()
{
    int x = getX();
    int y = getY();
    
    Direction dir = getWorld()->getDirection(x, y);
    setDirection(dir);
    TakeOneStep();
}

Regular_Protester::Regular_Protester(StudentWorld* world, int startX, int startY)
:Protester(world, IID_PROTESTER, startX, startY)
{
//
//    addHitPoint(5);
    
    hit_point_num = 5;
    rest_time = getRestTime();
    m_state = 0;

}


void Regular_Protester::doSomething()
{
    StudentWorld* gw = getWorld();
    
    if (IsDead()) return;
    if (rest_time > 0) {
        rest_time--;
        return;
    }
    if (m_state == 1) {
        setRestTime(0);
        rest_time = getRestTime();
        if (getX() == 60 && getY() == 60) {
            setDead();
        }
        //TODO: WALK TO THE EXIT
//        MazeSearching();
        WalkToExit();
        return;
    }
    if (m_state == 0) {
        setRestTime(0);
        rest_time = getRestTime();
        // check whether the protester is facing the FrackMan
        
        // check whether the protester is in the same line with FrackMan
        bool HorizontalInLine = (getY() == gw->access_player()->getY());
        bool VerticalInLine = (getX() == gw->access_player()->getX());
        
        //if the protester is facing FrackMan, it will shout at FrackMan
        if (gw->HitPlayer(getX(), getY(), 4.0) && facingFrackMan()) {
            ShoutAtFrackMan();
            return;
        }

        //if the protester is in line of FrackMan, then it will face the Frackman, and move forward.
        
        else if (( HorizontalInLine || VerticalInLine ) && !gw->HitPlayer(getX(), getY(), 4.0))
        {
            if (MoveToFrackMan() == true) {
                return;
            }
            
        }

        //if the protester can not directly see FrackMan, then it will step forward
        else{
            decreaseSquaresToMove();
            // if it has finished the steps in the current direction, then choose a new direction
            MakeATurn();

        }
        
        //take one step in current direction if possible
        if ((getDirection() == up && upviable(1)) || (getDirection() == down && downviable(1)) ||
            (getDirection() == left && leftviable(1)) || (getDirection() == right && rightviable(1))) {
            TakeOneStep();
        }
        else{
            setSquaresToMove(0);
        }
        
        // if the protester does not shout at this tick, counter increase
        
        updateCounters();
        return;
    }
    
    
}

void Regular_Protester::getAnnoyed(int point, bool completelyAnnoyed)
{
    StudentWorld* gw = getWorld();
    if (m_state == 1) {
        return;
    }
    hit_point_num -= point;
    if (hit_point_num > 0) {
        gw->playSound(SOUND_PROTESTER_ANNOYED);
        setRestTime(1);
        rest_time = getRestTime();
        return;
    }
    else{
        m_state = 1;
        gw->playSound(SOUND_PROTESTER_GIVE_UP);
        rest_time = 0;
        if (getReason() == 0) {
            gw->increaseScore(500);
        }
        else gw->increaseScore(100);
    }
}

void Regular_Protester::ProtesterPickUpGold()
{
    StudentWorld* gw = getWorld();
    gw->playSound(SOUND_PROTESTER_FOUND_GOLD);
    gw->increaseScore(25);
    m_state = 1;
}


Hardcore_Protester::Hardcore_Protester(StudentWorld* world, int startX, int startY)
:Protester(world, IID_HARD_CORE_PROTESTER, startX, startY)
{
    hit_point_num = 20;
    rest_time = getRestTime();
    m_state = 0;

}

void Hardcore_Protester::doSomething()
{
    StudentWorld* gw = getWorld();
    if (IsDead()) return;
    if (rest_time > 0) {
        rest_time--;
        return;
    }
    if (m_state == 1) {
        setRestTime(0);
        rest_time = getRestTime();
        
        if (getX() == 60 && getY() == 60) {
            setDead();
        }
        //TODO: WALK TO THE EXIT
//        MazeSearching();
        WalkToExit();
        return;
    }
    if (m_state == 0) {
        setRestTime(0);
        rest_time = getRestTime();
        // check whether the protester is facing the FrackMan
        
        // check whether the protester is in the same line with FrackMan
        bool HorizontalInLine = (getY() == gw->access_player()->getY());
        bool VerticalInLine = (getX() == gw->access_player()->getX());
        int M = gw->getLevel()*2 + 16;
        
        //if the protester is facing FrackMan, it will shout at FrackMan
        if (gw->HitPlayer(getX(), getY(), 4.0) && facingFrackMan()) {
            ShoutAtFrackMan();
            return;
        }
        
        // TODO: M move reachable
        else if(!gw->HitPlayer(getX(), getY(), 4.0) && gw->getDistanceToPlayer(getX(), getY()) <= M){
            StalkPlayer();
            
            updateCounters();
            return;
        }
        
        
        //if the protester is in line of FrackMan, then it will face the Frackman, and move forward.
        
        else if (( HorizontalInLine || VerticalInLine ) && !gw->HitPlayer(getX(), getY(), 4.0))
        {
            if (MoveToFrackMan() == true) {
                return;
            }
            
        }
        
        //if the protester can not directly see FrackMan, then it will step forward
        else{
            decreaseSquaresToMove();
            // if it has finished the steps in the current direction, then choose a new direction
            MakeATurn();
            
        }
        
        //take one step in current direction if possible
        if ((getDirection() == up && upviable(1)) || (getDirection() == down && downviable(1)) ||
            (getDirection() == left && leftviable(1)) || (getDirection() == right && rightviable(1))) {
            TakeOneStep();
        }
        else{
            setSquaresToMove(0);
        }
        
        // if the protester does not shout at this tick, counter increase
        
        updateCounters();
        return;
        
        
        
    }
    
}

void Hardcore_Protester::getAnnoyed(int point, bool completelyAnnoyed)
{
    StudentWorld* gw = getWorld();
    if (m_state == 1) {
        return;
    }
    hit_point_num -= point;
    if (hit_point_num > 0) {
        gw->playSound(SOUND_PROTESTER_ANNOYED);
        setRestTime(1);
        rest_time = getRestTime();
        return;
    }
    else{
        m_state = 1;
        gw->playSound(SOUND_PROTESTER_GIVE_UP);
        rest_time = 0;
        if (getReason() == 0) {
            gw->increaseScore(500);
        }
        else gw->increaseScore(250);
    }
    
}

void Hardcore_Protester::ProtesterPickUpGold()
{
    StudentWorld* gw = getWorld();
    gw->playSound(SOUND_PROTESTER_FOUND_GOLD);
    gw->increaseScore(50);
    setRestTime(1);
    rest_time = getRestTime();
}

FrackMan::FrackMan(StudentWorld* world)
:Actor(world, IID_PLAYER, 30, 60, right, 1.0, 0)
{
    m_alive = true;
    hit_point_num = 10;
    water_count = 5;
    SonarCharge = 1;
    Nuggets = 0;
    
    
    setVisible(true);
};

FrackMan::~FrackMan()
{
    
    setVisible(false);
    
}

void FrackMan::doSomething() {
    //        Try to get user input (if any is available)
    //        If the user pressed the UP key and that square is open then
    //        Increase my y location by one
    //        If the user pressed the DOWN key and that square is open then
    //        Decrease my y location by one ...
    //        If the user pressed the space bar to fire and the FrackMan has water, then
    //        Introduce a new Squirt object into the game facing the same
    //        direction as the player
    
    StudentWorld* gw = getWorld();
    //    check alive
    if (hit_point_num <= 0) {
        setDead();
    }
    if (IsDead()) {
        return;
    }
    
    //    remove dirt if necessary
    for (int i = getX(); i < getX() + 4; i++) {
        for (int j = getY(); j < getY() + 4; j++) {
            if (i >= 0 && i < VIEW_WIDTH && j >= 0 && j < VIEW_HEIGHT) {
                if (gw->ExistDirt(i, j)) {
                    gw->RemoveDirt(i,j);
                }

            }
        }
    }
    
    //     get user input
    int keyval;
    if (gw->getKey(keyval) == true) {
        switch (keyval) {
            case KEY_PRESS_LEFT:
            {
                if (getDirection() != left)
                {
                    setDirection(left);
                }
                
                else if (getX()-1 >= 0) {
                    bool collision = false;
                    if (getX() >= 7) {
                        for (int i = 0; i < 4; i++) {
                            if(gw->CheckBlock(getX()-1, getY()+i))
                            {
                                collision = true;
                            }
                            
                        }
                    }
                    if (collision == false) {
                        moveTo(getX()-1, getY());
                    }
                }
                
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                if (getDirection() != right)
                {
                    setDirection(right);
                }
                else if (getX()+4 < VIEW_WIDTH) {
                    bool collision = false;
                    if (getX() <= VIEW_WIDTH-7) {
                        for (int i = 0; i < 4; i++) {
                            if(gw->CheckBlock(getX()+4, getY()+i))
                            {
                                collision = true;
                            }
                            
                        }
                    }
                    if (collision == false) {
                        moveTo(getX()+1, getY());
                    }
                }


                break;
            }
            case KEY_PRESS_UP:
            {
                if (getDirection() != up)
                {
                    setDirection(up);
                }
                
                else if (getY()+4 < VIEW_HEIGHT) {
                    bool collision = false;
                    if (getY() <= 60-7) {
                        for (int i = 0; i < 4; i++) {
                            if(gw->CheckBlock(getX()+i, getY()+4))
                            {
                                collision = true;
                            }
                            
                        }
                    }
                    if (collision == false) {
                        moveTo(getX(), getY()+1);
                    }
                }
                
                
                break;

            }
            case KEY_PRESS_DOWN:
            {
                if (getDirection() != down)
                {
                    setDirection(down);
                }
                else if (getY()-1 >= 0) {
                    bool collision = false;
                    if (getY() >= 7) {
                        for (int i = 0; i < 4; i++) {
                            if(gw->CheckBlock(getX()+i, getY()-1))
                            {
                                collision = true;
                            }
                            
                        }
                    }
                    if (collision == false) {
                        moveTo(getX(), getY()-1);
                    }
                }
                break;
            }
            case KEY_PRESS_SPACE:
            {
                if(water_count > 0){
                    water_count--;
                    gw->playSound(SOUND_PLAYER_SQUIRT);
                    
                    int x = getX();
                    int y = getY();
                    Squirt* newsquirt = nullptr;
                    if(getDirection() == left){
                        if (x-4 >= 0 && (gw->ExistDirt(x-4, y) == false)) {
                            newsquirt  = new Squirt(gw, (x - 4), y, left);
                        }
                        
                    }
                    if(getDirection() == right){
                        if (x+4 < VIEW_WIDTH && (gw->ExistDirt(x+4, y) == false)) {
                            newsquirt  = new Squirt(gw, (x + 4), y, right);
                        }
                    }
                    if(getDirection() == up){
                        if (y+4 < 60 && (gw->ExistDirt(x, y+4) == false)) {
                            newsquirt  = new Squirt(gw, x, (y + 4), up);
                        }
                    }
                    if(getDirection() == down){
                        if (y-4 >= 0 && (gw->ExistDirt(x, y-4) == false)) {
                            newsquirt  = new Squirt(gw, x, (y - 4), down);
                        }
                    }
                    
                    //  give the object to StudentWorld to manage
                    if(newsquirt != nullptr)
                    {
                        if ( gw->HitBoulder(newsquirt->getX(), newsquirt->getY(), 3.0)) {
                            delete newsquirt;
                        }
                        else gw->AddActor(newsquirt);

                    }

                   
                }
                break;
                
            }
            case KEY_PRESS_ESCAPE:
            {
                hit_point_num = 0;
                break;
            }
            case 'z': case 'Z':
            {
                if (SonarCharge > 0) {
                    SonarCharge--;
                    gw->FindHidden(12.0);
                    gw->playSound(SOUND_SONAR);
                  
                }
                
            }
            case KEY_PRESS_TAB:
            {
                if (Nuggets > 0) {
                    Gold_Nugget* newgold = new Gold_Nugget(gw, 2, getX(), getY());
                    gw->AddActor(newgold);
                    Nuggets--;
                }
                
            }
            default:
                break;
        }
    }
    
}

void FrackMan::getAnnoyed(int point, bool completelyAnnoyed)
{
    if (completelyAnnoyed == false) {
        hit_point_num -= point;
    }
    else hit_point_num = 0;
}

void FrackMan::setDead()
{
    m_alive = false;
}

bool FrackMan::IsDead()
{
    if (m_alive == false) {
        return true;
    }
    else return false;
}

