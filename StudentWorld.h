#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

class FrackMan;

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Coord{
public:
    Coord(int x, int y):m_x(x), m_y(y){};
    int x() const { return m_x;};
    int y() const { return m_y;};
private:
    int m_x;
    int m_y;
};

struct backtrack_node{
    int distance;
    int previous_x;
    int previous_y;
};

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
    };

    virtual int init();

    virtual int move();
	

    virtual void cleanUp();
    
    void setDisplayText();
    
    void AddActor(Actor* newbee);
//    void MakeSound(int soundtype);
    bool ExistDirt(int x, int y);
    bool NoDirtInSquare(int x, int y);
    void RemoveDirt(int x, int y);
    bool CheckBlock(int x, int y);
    bool CheckCollision(int x, int y, double radius);
    bool HitBoulder(int x, int y, double radius);
    bool HitPlayer(int x, int y, double radius);
    bool HitProtester(int x, int y, double radius, bool AmIBoulder);
    bool PickUpItem(int x, int y, double radius);
    bool PickUpByProtester(int x, int y, double radius);
    Actor::Direction getDirection(int x, int y);
    void FindHidden(double radius);
    FrackMan* access_player() const;
//    void PlayerGetPoint(int point);
    void PlayerGetNugget();
    void PlayerGetSonar();
    void PlayerGetWater();
    void AnnoyPlayer(int point, bool completelyAnnoy);
    
    Actor::Direction DirectionToPlayer(int x, int y);
    int getDistanceToPlayer(int x, int y);
    Coord goBackward(int x, int y);
    Coord sensePlayer(int x, int y);
    
//    void AnnoyProtester(int point, bool completelyAnnoy);
//    bool ExistActor(int actortype,int x, int y, double radius);
//    // this function is to make sure no distributed object overlaps with other distributed object
//    bool ExistAnyActor(int x, int y, double radius);
    
private:
    void iniMaze(Coord dest);
    void UpdateMaze(Coord dest, bool goToExit);
    Protester* CreateProtester();
    Boulder* CreateBoulder();
    Barrel* CreateBarrel();
    Gold_Nugget* CreateGold();
    int BarrelsLeft();
    std::vector<Actor*> m_actors;
    Dirt* m_dirt[VIEW_WIDTH][VIEW_HEIGHT];
    FrackMan* m_player;
    int tick_counter;
    int protester_counter;
    Actor::Direction DirectionToExit[VIEW_WIDTH][VIEW_HEIGHT];
    backtrack_node DistanceToExit[VIEW_WIDTH][VIEW_HEIGHT];
    backtrack_node DistanceToPlayer[VIEW_WIDTH][VIEW_HEIGHT];
    
};



#endif // STUDENTWORLD_H_
