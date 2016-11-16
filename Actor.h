#ifndef ACTOR_H_
#define ACTOR_H_

class StudentWorld;

#include "GraphObject.h"
#include "GameWorld.h"
#include "GameConstants.h"

#include <list>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor:public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject(imageID, startX, startY, dir, size, depth)
    {
        m_world = world;
    };
    ~Actor(){};
    
    virtual void doSomething() = 0;
    virtual void getAnnoyed(int point, bool completelyAnnoyed){};
    virtual void setDead(){};
    virtual bool IsDead(){return false;};
    virtual bool WillIBlock(int x, int y){return false;};
    virtual bool WillICollide(int x, int y, double radius);
    virtual bool AmIBoulder(){return false;};
    virtual bool AmIProtester(){return false;};
    // this function is called by the studentworld to see whether there is any pickable oil
    // if no, then the level is completed
    virtual bool AmIPickable(){return false;};
    virtual int StateOfGold(){return 0;};
    virtual void ProtesterPickUpGold(){};

    StudentWorld* getWorld()
    {
        return m_world;
    };
private:
    StudentWorld* m_world;
};




class Dirt:public Actor
{
public:
    Dirt(StudentWorld* world, int startX, int startY);
    ~Dirt();
    
    virtual void doSomething(){};
    
};

class Boulder:public Actor
{
public:
    Boulder(StudentWorld* world, int startX, int startY);
    ~Boulder();
    
    virtual void doSomething();
    virtual bool IsDead();
    virtual void setDead();
    virtual bool WillIBlock(int x, int y);
    virtual bool AmIBoulder(){return true;};

private:
    bool m_alive;
    // there are three states for the boulder, 0 is 'stable', 1 is 'waiting', 2 is 'falling' 
    int m_state;
    int wait_remain;
    
    
};

class Squirt:public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction dir);
    ~Squirt();
    
    virtual void doSomething();
    virtual void setDead();
    virtual bool IsDead();
    
private:
    int travel_distance;
};

class Barrel:public Actor
{
public:
    Barrel(StudentWorld* world, int startX, int startY);
    ~Barrel();
    
    virtual void doSomething();
    virtual bool IsDead();
    virtual void setDead(){m_alive = false;};
    virtual bool AmIPickable(){return true;};
private:
    bool discovered;
    bool m_alive;
};

class Goodie:public Actor
{
public:
    Goodie(StudentWorld* world, int imageID, int startX, int startY);
    ~Goodie();
    
    virtual void doSomething(){};
    virtual void setDead(){m_alive = false;};
    virtual bool IsDead();
    virtual void setLifetime(bool isGold);
    virtual void lifetimeElapse(){lifetime--;};
    virtual bool TimeOut();
    
private:
    bool m_alive;
    int lifetime;

};

class Gold_Nugget:public Goodie
{
public:
    Gold_Nugget(StudentWorld* world, int my_state, int startX, int startY);
    ~Gold_Nugget(){};
    
    virtual void doSomething();
    virtual int StateOfGold();
private:
    // there are 2 states, 1 for permanent and 2 for temporary
    int m_state;
    bool discovered;
    
    
};

class Sonar_Kit:public Goodie
{
public:
    Sonar_Kit(StudentWorld* world, int startX, int startY);
    ~Sonar_Kit(){};
    virtual void doSomething();
    
};

class Water_Pool:public Goodie
{
public:
    Water_Pool(StudentWorld* world, int startX, int startY);
    ~Water_Pool(){};
    virtual void doSomething();
    
};


class Protester:public Actor
{
public:
    Protester(StudentWorld* world, int imageID, int startX, int startY);
    virtual void doSomething() = 0;
    virtual void getAnnoyed(int point, bool completelyAnnoyed){};
    virtual bool IsDead();
    virtual void setDead(){ m_alive = false;};
    virtual bool AmIProtester(){return true;};
    virtual bool AmIHard() = 0;
    
    void ReasonOfAnnoyance(int reason) {whyAnnoyed = reason;};
    
    int getReason() { return whyAnnoyed;};
    void setSquaresToMove(int num) { numSquaresToMoveInCurrentDirection = num;} ;
    void decreaseSquaresToMove() {numSquaresToMoveInCurrentDirection--;};
    int getSquaresToMove(){ return numSquaresToMoveInCurrentDirection;};
    void setRestTime(int mode);
    int getRestTime(){return ticksToWaitBetweenMoves;};
    void updateCounters();
//    void setHitPoint();
//    int getState() {return m_state;};
//    void changeState(int newstate) { m_state = newstate;};
    
    bool facingFrackMan();
    bool upviable(int num_steps);
    bool downviable(int num_steps);
    bool leftviable(int num_steps);
    bool rightviable(int num_steps);
    bool MoveToFrackMan();
    void ShoutAtFrackMan();
//    void GiveUp();
    
    void MakeATurn();
    void TakeOneStep();
    void WalkToExit();
    void MazeSearching();
    void StalkPlayer();

private:
//    int hit_point_num;
    int numSquaresToMoveInCurrentDirection;
    int ticksToWaitBetweenMoves;
    int shout_counter;
    int perpendicular_counter;
    bool m_alive;
//    int m_state;
//
    // there are two reasons why the protester is annoyed, 0 for bonked by boulder, 1 for squirted
    int whyAnnoyed;
};

class Regular_Protester:public Protester
{
public:
    Regular_Protester(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void getAnnoyed(int point, bool completelyAnnoyed);
    virtual void ProtesterPickUpGold();
    virtual bool AmIHard() { return false;};
    
private:
    int hit_point_num;
    // two states, 0 for rest state, 1 for leave the oil field state
    int m_state;
    int rest_time;
    
};


class Hardcore_Protester:public Protester
{
public:
    Hardcore_Protester(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void getAnnoyed(int point, bool completelyAnnoyed);
    virtual bool AmIHard() { return true;};
    virtual void ProtesterPickUpGold();
    
private:
    int hit_point_num;
    // two states, 0 for rest state, 1 for leave the oil field state
    int m_state;
    int rest_time;
};

class FrackMan: public Actor
{
public:
    FrackMan(StudentWorld* world);
    ~FrackMan();
    virtual void doSomething();
    virtual void getAnnoyed(int point, bool completelyAnnoyed);
    virtual void setDead();
    virtual bool IsDead();
//    virtual bool WillIBlock(int x, int y){};
    
    void getNugget(){ Nuggets++;};
    void getSonar() { SonarCharge++;};
    void getWater() { water_count += 5;};
    
    int myPoint(){ return hit_point_num;};
    int mySquirt(){ return water_count;};
    int mySonar(){ return SonarCharge;};
    int myNuggets(){ return Nuggets;};

private:
    bool m_alive;
    int hit_point_num;
    int water_count;
    int SonarCharge;
    int Nuggets;
};

#endif // ACTOR_H_
