
#include "StudentWorld.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

Boulder* StudentWorld::CreateBoulder()
{
    int x = rand() % (VIEW_WIDTH-4+1);
    int y = 20 + rand() % (56 - 20 + 1);
    
    Boulder* newboulder = new Boulder(this, x, y);
    return newboulder;
    
}

Barrel* StudentWorld::CreateBarrel()
{
    int x = rand() % (VIEW_WIDTH-4+1);
    int y = rand() % (56);

    Barrel* newoil = new Barrel(this, x, y);
    return newoil;
    
}

Gold_Nugget* StudentWorld::CreateGold()
{
    int x = rand() % (VIEW_WIDTH-4+1);
    int y = rand() % (56);
    
    Gold_Nugget* newgold = new Gold_Nugget(this, 1, x, y);
    return newgold;
}

Protester* StudentWorld::CreateProtester()
{
    if (tick_counter > 0) {
        tick_counter--;
        return nullptr;
    }
    int P = getLevel()*1.5+2 < 15? getLevel()*1.5+2 : 15;
    
    if (protester_counter >= P) {
        return nullptr;
    }
    
    int probabilityOfHardcore = getLevel()*10+30 < 90? getLevel()*10+30 : 90;
    int p = rand() % probabilityOfHardcore;
    
    Protester* newprotester;
    
    if (p == 1) {
        newprotester = new Hardcore_Protester(this, 60, 60);
    }
    else{
        newprotester = new Regular_Protester(this, 60, 60);
    }
    
    tick_counter = (200 - getLevel()) > 25? (200-getLevel()):25;
    protester_counter++;
    
    return newprotester;
    
}

int StudentWorld::BarrelsLeft()
{
    int count = 0;
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->AmIPickable()) {
            count++;
        }
    }
    return count;
}

void StudentWorld::iniMaze(Coord dest)
{
    // initialize the maze
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            DirectionToExit[i][j] = Actor::none;
        }
    }
    
    queue<Coord> maze;
    Coord position(60, 60);
//    DirectionToExit[60][60] = 0;
    
//    if (dest.x() == 63 && dest.y() == 63) {
//        for (int i = 0; i < 4; i++) {
//            for (int j = 0; j < 3; j++) {
//                DirectionToExit[60+i][60+j] = 0;
//            }
//            
//        }
//    }
    
    
    maze.push(dest);
    while (!maze.empty()) {
        position = maze.front();
        maze.pop();
        
        int x = position.x();
        int y = position.y();
        
        if (x == 0 && y == 0) {
            break;
        }
        if (position.x() + 1 < 64 && DirectionToExit[position.x()+1][position.y()] == Actor::none) {
            if (NoDirtInSquare(position.x(), position.y()) && NoDirtInSquare(position.x()+1, position.y()) && !HitBoulder(position.x()+1, position.y(), 3.0))
            {
                Coord east(position.x()+1, position.y());
                maze.push(east);
                DirectionToExit[position.x()+1][position.y()]=Actor::left;
            }
        }
        if (position.x() - 1 >= 0 && DirectionToExit[position.x()-1][position.y()] == Actor::none) {
            if (NoDirtInSquare(position.x(), position.y()) && NoDirtInSquare(position.x()-1, position.y()) && !HitBoulder(position.x()-1, position.y(), 3.0))
            {
                Coord west(position.x()-1, position.y());
                maze.push(west);
                DirectionToExit[position.x()-1][position.y()] = Actor::right;
            }
        }
        if (position.y() + 1 < 64 && DirectionToExit[position.x()][position.y()+1] == Actor::none) {

            if (NoDirtInSquare(position.x(), position.y()) && NoDirtInSquare(position.x(), position.y()+1) && !HitBoulder(position.x(), position.y()+1, 3.0))
            {
                Coord north(position.x(), position.y()+1);
                maze.push(north);
                DirectionToExit[position.x()][position.y()+1] = Actor::down;
            }
        }
        if (position.y() - 1 >= 0 && DirectionToExit[position.x()][position.y()-1] == Actor::none) {
            
            if (NoDirtInSquare(position.x(), position.y()) && NoDirtInSquare(position.x(), position.y()-1) && !HitBoulder(position.x(), position.y()-1, 3.0))
            {
                //                if (position.x() < 33) {
                //                    cout<<"below passed"<<endl;
                //                }
                Coord south(position.x(), position.y()-1);
                maze.push(south);
                DirectionToExit[position.x()][position.y()-1] = Actor::up;
            }
        }
    }
    

}

void StudentWorld::UpdateMaze(Coord dest, bool goToExit)
{
    
    queue<Coord> maze;
//    Coord Exit(60, 60);
    int current_x = dest.x();
    int current_y = dest.y();
    Coord position(current_x, current_y);
    
    if (goToExit) {
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                DistanceToExit[i][j].distance = -1;
                DistanceToExit[i][j].previous_x = -1;
                DistanceToExit[i][j].previous_y = -1;
            }
        }
        
        DistanceToExit[current_x][current_y].distance = 0;
        maze.push(dest);
        while (!maze.empty()) {
            position = maze.front();
            maze.pop();
            
            int x = position.x();
            int y = position.y();
            
            if (x == 0 && y == 0) {
                break;
            }
            if (position.x() + 1 < 64 && NoDirtInSquare(position.x()+1, position.y()) && !HitBoulder(position.x()+1, position.y(), 3.0)) {
                if (DistanceToExit[position.x()+1][position.y()].distance < 0)
                {
                    Coord east(position.x()+1, position.y());
                    maze.push(east);
                    DistanceToExit[position.x()+1][position.y()].distance=DistanceToExit[position.x()][position.y()].distance+1;
                    DistanceToExit[position.x()+1][position.y()].previous_x = position.x();
                    DistanceToExit[position.x()+1][position.y()].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()+1][position.y()]>DirectionToExit[position.x()][position.y()]+1)
                //            {
                //                Coord east(position.x()+1, position.y());
                //                maze.push(east);
                //                DirectionToExit[position.x()+1][position.y()]=DirectionToExit[position.x()][position.y()]+1;
                //            }
            }
            if (position.x() - 1 >= 0 && NoDirtInSquare(position.x()-1, position.y()) && !HitBoulder(position.x()-1, position.y(), 3.0) ) {
                if (DistanceToExit[position.x()-1][position.y()].distance < 0)
                {
                    Coord west(position.x()-1, position.y());
                    maze.push(west);
                    DistanceToExit[position.x()-1][position.y()].distance=DistanceToExit[position.x()][position.y()].distance+1;
                    DistanceToExit[position.x()-1][position.y()].previous_x = position.x();
                    DistanceToExit[position.x()-1][position.y()].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()-1][position.y()]<DirectionToExit[position.x()][position.y()] + 1)
                //            {
                //                Coord west(position.x()-1, position.y());
                //                maze.push(west);
                //                DirectionToExit[position.x()-1][position.y()]=DirectionToExit[position.x()][position.y()]+1;
                //
                //            }
            }
            if (position.y() + 4 < 64 && NoDirtInSquare(position.x(), position.y()+4) && !HitBoulder(position.x(), position.y()+1, 3.0)) {
                
                if (DistanceToExit[position.x()][position.y()+1].distance < 0)
                {
                    Coord north(position.x(), position.y()+1);
                    maze.push(north);
                    DistanceToExit[position.x()][position.y()+1].distance = DistanceToExit[position.x()][position.y()].distance + 1;
                    DistanceToExit[position.x()][position.y()+1].previous_x = position.x();
                    DistanceToExit[position.x()][position.y()+1].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()][position.y()+1] < DirectionToExit[position.x()][position.y()] + 1)
                //            {
                //                Coord north(position.x(), position.y()+1);
                //                maze.push(north);
                //                DirectionToExit[position.x()][position.y()+1] = DirectionToExit[position.x()][position.y()] + 1;
                //            }
            }
            if (position.y() - 1 >= 0 && NoDirtInSquare(position.x(), position.y()-1) && !HitBoulder(position.x(), position.y()-1, 3.0)) {
                
                if (DistanceToExit[position.x()][position.y()-1].distance < 0)
                {
                    Coord south(position.x(), position.y()-1);
                    maze.push(south);
                    DistanceToExit[position.x()][position.y()-1].distance = DistanceToExit[position.x()][position.y()].distance + 1;
                    DistanceToExit[position.x()][position.y()-1].previous_x = position.x();
                    DistanceToExit[position.x()][position.y()-1
                                                 ].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()][position.y()-1] < DirectionToExit[position.x()][position.y()] + 1)
                //            {
                //                Coord south(position.x(), position.y()-1);
                //                maze.push(south);
                //                DirectionToExit[position.x()][position.y()-1] = DirectionToExit[position.x()][position.y()] + 1;
                //            }
            }
        }
    
    }
    else{
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                DistanceToPlayer[i][j].distance = -1;
                DistanceToPlayer[i][j].previous_x = -1;
                DistanceToPlayer[i][j].previous_y = -1;
            }
        }
        
        DistanceToPlayer[current_x][current_y].distance = 0;
        maze.push(dest);
        while (!maze.empty()) {
            position = maze.front();
            maze.pop();
            
            int x = position.x();
            int y = position.y();
            
            if (x == 0 && y == 0) {
                break;
            }
            if (position.x() + 1 < 64 && NoDirtInSquare(position.x()+1, position.y()) && !HitBoulder(position.x()+1, position.y(), 3.0)) {
                if (DistanceToPlayer[position.x()+1][position.y()].distance < 0)
                {
                    Coord east(position.x()+1, position.y());
                    maze.push(east);
                    DistanceToPlayer[position.x()+1][position.y()].distance=DistanceToPlayer[position.x()][position.y()].distance+1;
                    DistanceToPlayer[position.x()+1][position.y()].previous_x = position.x();
                    DistanceToPlayer[position.x()+1][position.y()].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()+1][position.y()]>DirectionToExit[position.x()][position.y()]+1)
                //            {
                //                Coord east(position.x()+1, position.y());
                //                maze.push(east);
                //                DirectionToExit[position.x()+1][position.y()]=DirectionToExit[position.x()][position.y()]+1;
                //            }
            }
            if (position.x() - 1 >= 0 && NoDirtInSquare(position.x()-1, position.y()) && !HitBoulder(position.x()-1, position.y(), 3.0) ) {
                if (DistanceToPlayer[position.x()-1][position.y()].distance < 0)
                {
                    Coord west(position.x()-1, position.y());
                    maze.push(west);
                    DistanceToPlayer[position.x()-1][position.y()].distance=DistanceToPlayer[position.x()][position.y()].distance+1;
                    DistanceToPlayer[position.x()-1][position.y()].previous_x = position.x();
                    DistanceToPlayer[position.x()-1][position.y()].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()-1][position.y()]<DirectionToExit[position.x()][position.y()] + 1)
                //            {
                //                Coord west(position.x()-1, position.y());
                //                maze.push(west);
                //                DirectionToExit[position.x()-1][position.y()]=DirectionToExit[position.x()][position.y()]+1;
                //
                //            }
            }
            if (position.y() + 4 < 64 && NoDirtInSquare(position.x(), position.y()+4) && !HitBoulder(position.x(), position.y()+1, 3.0)) {
                
                if (DistanceToPlayer[position.x()][position.y()+1].distance < 0)
                {
                    Coord north(position.x(), position.y()+1);
                    maze.push(north);
                    DistanceToPlayer[position.x()][position.y()+1].distance = DistanceToPlayer[position.x()][position.y()].distance + 1;
                    DistanceToPlayer[position.x()][position.y()+1].previous_x = position.x();
                    DistanceToPlayer[position.x()][position.y()+1].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()][position.y()+1] < DirectionToExit[position.x()][position.y()] + 1)
                //            {
                //                Coord north(position.x(), position.y()+1);
                //                maze.push(north);
                //                DirectionToExit[position.x()][position.y()+1] = DirectionToExit[position.x()][position.y()] + 1;
                //            }
            }
            if (position.y() - 1 >= 0 && NoDirtInSquare(position.x(), position.y()-1) && !HitBoulder(position.x(), position.y()-1, 3.0)) {
                
                if (DistanceToExit[position.x()][position.y()-1].distance < 0)
                {
                    Coord south(position.x(), position.y()-1);
                    maze.push(south);
                    DistanceToPlayer[position.x()][position.y()-1].distance = DistanceToPlayer[position.x()][position.y()].distance + 1;
                    DistanceToPlayer[position.x()][position.y()-1].previous_x = position.x();
                    DistanceToPlayer[position.x()][position.y()-1].previous_y = position.y();
                }
                //            else if(DirectionToExit[position.x()][position.y()-1] < DirectionToExit[position.x()][position.y()] + 1)
                //            {
                //                Coord south(position.x(), position.y()-1);
                //                maze.push(south);
                //                DirectionToExit[position.x()][position.y()-1] = DirectionToExit[position.x()][position.y()] + 1;
                //            }
            }
        }

    }
}

void StudentWorld::setDisplayText()
{
    //get current score
    
    stringstream ss_score;
    int score_num = getScore();
    ss_score<<setw(6)<<setfill('0')<<score_num;
    string score = ss_score.str();
    
    stringstream ss_level;
    int level_num = getLevel();
    ss_level<<setw(2)<<setfill(' ')<<level_num;
    string level = ss_level.str();
    
    stringstream ss_lives;
    int lives_num = getLives();
    ss_lives<<setw(1)<<lives_num;
    string lives = ss_lives.str();
    
    stringstream ss_health;
    int health_num = m_player->myPoint() * 10;
    ss_health<<setw(3)<<setfill(' ')<<health_num;
    string health = ss_health.str();
    
    stringstream ss_squirt;
    int squirt_num = m_player->mySquirt();
    ss_squirt<<setw(2)<<setfill(' ')<<squirt_num;
    string squirt = ss_squirt.str();
    
    stringstream ss_gold;
    int gold_num = m_player->myNuggets();
    ss_gold<<setw(2)<<setfill(' ')<<gold_num;
    string gold = ss_gold.str();
    
    stringstream ss_sonar;
    int sonar_num = m_player->mySonar();
    ss_sonar<<setw(2)<<setfill(' ')<<sonar_num;
    string sonar = ss_sonar.str();
    
    stringstream ss_oil;
    int oil_num = BarrelsLeft();
    ss_oil<<setw(2)<<setfill(' ')<<oil_num;
    string oil = ss_oil.str();
    
    stringstream ss;
    ss<<"Scr: "<<score<<"  "<<"Lvl: "<<level<<"  "<<"Lives: "<<lives<<"  "<<"Hlth: "<<health<<"%  "<<"Wtr: "<<squirt<<"  "<<"Gld: "<<gold<<"  "<<"Sonar: "<<sonar<<"  "<<"Oil Left: "<<oil;
    setGameStatText(ss.str());
    
}
int StudentWorld::move()
{
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    setDisplayText();
    
    Coord Exit(60, 60);
//    iniMaze(Exit);
    UpdateMaze(Exit, true);
//
//    Protester* newprotester = CreateProtester();
//    if (newprotester != nullptr) {
//        m_actors.push_back(newprotester);
//    }
    
    m_player->doSomething();
    Coord player_pos(m_player->getX(), m_player->getY());
    UpdateMaze(player_pos, false);
    
    //manage all the actors in the game
    for (int i = 0; i < m_actors.size(); i++) {
        //check if it is dead
        if (!m_actors[i]->IsDead()) {
//            Coord Exit(60, 60);
//            UpdateMaze(Exit);
            m_actors[i]->doSomething();
        }
        
        if (m_player->IsDead()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (BarrelsLeft() == 0) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    // remove newly dead actors
    for (int i = 0; i < m_actors.size(); i++) {
        //check if it is dead
        if (m_actors[i]->IsDead()) {
            if (m_actors[i]->AmIProtester()) {
                protester_counter--;
            }
            delete m_actors[i];
            m_actors.erase(m_actors.begin()+i);
            i--;
        }
        
    }
    
    
    
    if (m_player->IsDead()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    //check whether all the barrels are discovered, if yes, then level completed
    if (BarrelsLeft() == 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //add new actor to the game
    int G = getLevel()*25 +300;
    int chance = rand() % G;
    
    if (chance == 1) {
        int isSonar = rand() % 5;
        if (isSonar == 1) {
            Sonar_Kit* newsonar = new Sonar_Kit(this, 0, 60);
            m_actors.push_back(newsonar);
            
        }
        else{
            int x = 0;
            int y = 0;
            bool valid = false;
            while (valid == false){
                x = rand() % (VIEW_WIDTH-4);
                y = rand() % (56);
                bool dirtless = true;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if(ExistDirt(x+i, y+j)){
                            dirtless = false;
                        }
                    }
                    
                }
                if (dirtless == true) {
                    valid = true;
                }
                
            }
            
            Water_Pool* newpool = new Water_Pool(this, x, y);
            m_actors.push_back(newpool);
        }
    }

    
    return GWSTATUS_CONTINUE_GAME;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::init()
{
    tick_counter = 0;
    protester_counter = 0;
    
    // create dirt
    for (int i = 0; i<VIEW_WIDTH; i++) {
        if (i < 30 or i > 33) {
            for (int j = 0; j<60; j++) {
                m_dirt[i][j] = new Dirt(this, i, j);
            }
        }
        else{
            for (int j = 0; j < 4; j++) {
                m_dirt[i][j] = new Dirt(this, i, j);
            }
            for (int j = 4; j < 60; j++) {
                m_dirt[i][j] = nullptr;
            }
        }
        for (int j = 60; j < VIEW_HEIGHT; j++) {
            m_dirt[i][j] = nullptr;
        }
    }
    
    // create Frackman
    m_player = new FrackMan(this);
    Coord player_pos(m_player->getX(), m_player->getY());
    UpdateMaze(player_pos, false);

    Hardcore_Protester* test = new Hardcore_Protester(this, 60, 60);
    m_actors.push_back(test);
    
    //create protester
    Protester* newprotester = CreateProtester();
    if (newprotester != nullptr) {
        m_actors.push_back(newprotester);
    }
    

    //create boulder
    int B;
    if (getLevel()/2 + 2 <  6) {
        B = getLevel()/2 + 2;
    }
    else B = 6;
    
    for (int k = 0; k < B; k++) {
        Boulder* temp;
        temp = CreateBoulder();
        int newx = temp->getX();
        int newy = temp->getY();
        //check if the new boulder overlap other actors
        bool inShaft = false;
        for (int i = 0; i < 4; i++) {
            if(!ExistDirt(newx+i, newy+3)){
                inShaft = true;
            }
        }
        if (CheckCollision(newx, newy, 6.0) || inShaft) {
            delete temp;
            k--;
        }
        else{
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (newx+i < VIEW_WIDTH && newy+j < 60) {
                        RemoveDirt(newx + i, newy + j);

                    }
                }
            }
            m_actors.push_back(temp);
        }
    }
    
    // create barrel of oil
    int L;
    if (getLevel() + 2 < 20) {
        L = getLevel() + 2;
    }
    else L = 20;
    
    for (int k = 0; k < L; k++) {
        Barrel* temp;
        temp = CreateBarrel();
        int newx = temp->getX();
        int newy = temp->getY();
        //check if the new boulder overlap other actors
        bool inShaft = false;
        for (int i = 0; i < 4; i++) {
            if(!ExistDirt(newx+i, newy+3)){
                inShaft = true;
            }
        }
        if (CheckCollision(newx, newy, 6.0) || inShaft) {
            delete temp;
            k--;
        }
        else{
            m_actors.push_back(temp);
        }
    }

    // create gold nuggets
    int M;
    if (5 - getLevel()/2 > 2) {
        M = 5 - getLevel()/2;
    }
    else M = 2;
    
    for (int k = 0; k < M; k++) {
        Gold_Nugget* temp;
        temp = CreateGold();
        int newx = temp->getX();
        int newy = temp->getY();
        //check if the new boulder overlap other actors
        bool inShaft = false;
        for (int i = 0; i < 4; i++) {
            if(!ExistDirt(newx+i, newy+3)){
                inShaft = true;
            }
        }
        if (CheckCollision(newx, newy, 6.0) || inShaft) {
            delete temp;
            k--;
        }
        else{
            m_actors.push_back(temp);
        }
    }

    Coord Exit(60, 60);
//    iniMaze(Exit);
    UpdateMaze(Exit, true);

    cout<<DistanceToExit[58][60].previous_x<<endl;
    cout<<DistanceToExit[54][60].previous_x<<endl;
    cout<<DistanceToExit[34][60].previous_x<<endl;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i<VIEW_WIDTH; i++) {
        for (int j = 0; j < 60; j++) {
            if (m_dirt[i][j] != nullptr) {
                delete m_dirt[i][j];
            }
        }
    }
    delete m_player;
    while (!m_actors.empty()) {
        if (m_actors.back()!=nullptr) {
            delete m_actors.back();
            m_actors.pop_back();
        }
    }
}
void StudentWorld::AddActor(Actor* newbee)
{
    m_actors.push_back(newbee);
}

//void StudentWorld::MakeSound(int soundtype)
//{
//    
//}
bool StudentWorld::ExistDirt(int x, int y)
{
    if (m_dirt[x][y] != nullptr) {
        return true;
    }
    else return false;
}

bool StudentWorld::NoDirtInSquare(int x, int y)
{
    if (x < 0 || x > 60 || y < 0 || y > 60) return false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            
            if (m_dirt[x+i][y+i] != nullptr) {
                return false;
            }
        }
    }
    return true;
}
void StudentWorld::RemoveDirt(int x, int y)
{
    if (m_dirt[x][y] != nullptr) {
        delete m_dirt[x][y];
        m_dirt[x][y] = nullptr;
        playSound(SOUND_DIG);
    }
}

bool StudentWorld::CheckBlock(int x, int y)
{
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->WillIBlock(x, y)) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::CheckCollision(int x, int y, double radius)
{
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->WillICollide(x, y, radius)) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::HitBoulder(int x, int y, double radius)
{
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->AmIBoulder() && m_actors[i]->WillICollide(x, y, radius)) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::HitPlayer(int x, int y, double radius)
{
    if (m_player->WillICollide(x, y, radius)) {
        return true;
    }
    else return false;
}
bool StudentWorld::HitProtester(int x, int y, double radius, bool AmIBoulder)
{
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->AmIProtester() && m_actors[i]->WillICollide(x, y, radius)) {
            if (AmIBoulder) {
                m_actors[i]->getAnnoyed(100, true);
            }
            else m_actors[i]->getAnnoyed(2, false);
            return true;
        }
    }
    return false;
}



bool StudentWorld::PickUpItem(int x, int y, double radius)
{
    //if the item is within radius 3.0 of the player, then the player will pick up
    if (m_player->WillICollide(x, y, radius)) {
        return true;
    }
    else return false;
}

bool StudentWorld::PickUpByProtester(int x, int y, double radius)
{
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->AmIProtester() && m_actors[i]->WillICollide(x, y, radius)) {
            m_actors[i]->ProtesterPickUpGold();
            return true;
        }
    }
    return false;
}

Actor::Direction StudentWorld::getDirection(int x, int y)
{
    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT) {
        return Actor::none;
    }
    return DirectionToExit[x][y];
}

FrackMan* StudentWorld::access_player() const
{
    return m_player;
}

void StudentWorld::PlayerGetNugget()
{
    m_player->getNugget();
}
void StudentWorld::PlayerGetSonar()
{
    m_player->getSonar();
}
void StudentWorld::PlayerGetWater()
{
    m_player->getWater();
}

void StudentWorld::AnnoyPlayer(int point, bool completelyAnnoy)
{
    m_player->getAnnoyed(point, completelyAnnoy);
}
void StudentWorld::FindHidden(double radius)
{
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->WillICollide(m_player->getX(), m_player->getY(), radius)) {
            m_actors[i]->setVisible(true);
        }
    }
}

Actor::Direction StudentWorld::DirectionToPlayer(int x, int y)
{
    Coord PlayerPos(m_player->getX(), m_player->getY());
    iniMaze(PlayerPos);
    
    Actor::Direction dir = DirectionToExit[x][y];
    return dir;
    
}

int StudentWorld::getDistanceToPlayer(int x, int y)
{
    int dist = DistanceToPlayer[x][y].distance;
    
    if (dist>=0) {
        return dist;
    }
    else return 500;
    
}

Coord StudentWorld::goBackward(int x, int y)
{
    int previous_x = DistanceToExit[x][y].previous_x;
    int previous_y = DistanceToExit[x][y].previous_y;
    Coord previous(previous_x, previous_y);
    return previous;
}

Coord StudentWorld::sensePlayer(int x, int y)
{
//    cout<<"distance to player"<<DistanceToPlayer[x][y].distance<<endl;

    int previous_x = DistanceToPlayer[x][y].previous_x;
    int previous_y = DistanceToPlayer[x][y].previous_y;
    Coord previous(previous_x, previous_y);
    return previous;
}
//bool StudentWorld::ExistActor(int actortype, int x, int y, double radius)
//{
//    for (int i = 0; i < m_actors.size(); i++) {
//        int existx = m_actors[i]->getX();
//        int existy = m_actors[i]->getY();
//        double distance_sq;
//        distance_sq = double((existx - x)^2 + (existy - y)^2);
//        if (pow(distance_sq, 0.5) <= radius && m_actors[i]->getID() == actortype) {
//            return true;
//        }
//    }
//    return false;
//}
//
//bool StudentWorld::ExistAnyActor(int x, int y, double radius)
//{
//    for (int i = 0; i < m_actors.size(); i++) {
//        int existx = m_actors[i]->getX();
//        int existy = m_actors[i]->getY();
//        double distance_sq;
//        distance_sq = double((existx - x)^2 + (existy - y)^2);
//        if (pow(distance_sq, 0.5) <= radius) {
//            return true;
//        }
//    }
//    return false;
//}