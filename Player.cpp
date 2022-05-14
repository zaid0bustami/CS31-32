#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <set>
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);

    virtual bool isHuman() const { return true; }

    virtual bool placeShips(Board& b);
    virtual void recordAttackByOpponent(Point p);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
    : Player(nm, g)
{}

bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " must place " << game().nShips() << " ships." << endl;
    for (int i = 0; i < game().nShips(); i++)
    {
        b.display(false);

        Direction dir;
        string dirStr;
        for (;;)
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            getline(cin, dirStr);
            if (dirStr[0] == 'h')
            {
                dir = HORIZONTAL;
                break;
            }
            else if (dirStr[0] == 'v')
            {
                dir = VERTICAL;
                break;
            }
            else
                cout << "Direction must be h or v." << endl;
        }

        int r = 0, c = 0;
        for (;;)
        {
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            if (getLineWithTwoIntegers(r, c))
            {
                Point p(r, c);
                if (b.placeShip(p, i, dir))
                    break;
                else
                    cout << "The ship can not be placed there." << endl;
            }
            else
                cout << "You must enter two integers." << endl;
        }        
    }
    return true;
}

void HumanPlayer::recordAttackByOpponent(Point p)
{
    //doesn't do anything
}

Point HumanPlayer::recommendAttack()
{
    int r = 0, c = 0;
    for (;;)
    {
        cout << "Enter the row and column to attack (e.g., 3 5): ";
        if (getLineWithTwoIntegers(r, c))
        {
            Point p(r, c);
            return p;
        }
        else
            cout << "You must enter two integers." << endl;
    }
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    //doesn't do anything
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    bool placeShips(Board& b, int nShips);
    virtual void recordAttackByOpponent(Point p);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    bool attackedBefore(const Point& p);
private:
    vector<Point> m_attacks;
    struct lastAttack
    {
        Point p;
        Point hitP;
        bool validShot = false;
        bool shotHit = false;
        bool shipDestroyed = false;
        int shipId;
    };
    lastAttack m_lastAttack;
    int m_state = 1;
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
    : Player(nm, g)
{}

bool MediocrePlayer::placeShips(Board& b, int shipId)
{
    if (shipId == game().nShips()) //base case - if all the ships have been placed
        return true;
    for (int r = 0; r != game().rows(); r++)
    {
        for (int c = 0; c != game().cols(); c++)
        {
            Point p(r, c);
            if (b.placeShip(p, shipId, VERTICAL))
                if (placeShips(b, shipId + 1))
                    return true;
                else
                    b.unplaceShip(p, shipId, VERTICAL);
            if(b.placeShip(p, shipId, HORIZONTAL))
                if (placeShips(b, shipId + 1))
                    return true;
                else
                    b.unplaceShip(p, shipId, HORIZONTAL);
        }
    }
    return false;
}

bool MediocrePlayer::placeShips(Board& b)
{
    b.block();
    for (int i = 0; i < 50; i++)
    {
        if (placeShips(b, 0))
        {
            b.unblock();
            return true;
        }
    }
    b.unblock();
    return false;
}

void MediocrePlayer::recordAttackByOpponent(Point p)
{
    //nothing happens
}

bool MediocrePlayer::attackedBefore(const Point& p)
{
    for (int i = 0; i < m_attacks.size(); i++)
    {
        if (m_attacks.at(i).r == p.r && m_attacks.at(i).c == p.c)
            return true;
    }
    return false;
}

Point MediocrePlayer::recommendAttack()
{   
    for (;;)
    {
        while (m_state == 1)
        {
            Point p = game().randomPoint();
            if (!attackedBefore(p)) //test that the point hasn't been attacked before
                m_attacks.push_back(p);
            else
                continue;
            return p;
        }
        while (m_state == 2)
        {
            int r = m_lastAttack.hitP.r;
            int c = m_lastAttack.hitP.c;
            int d = randInt(2);
            if (d == 0)
                c = (m_lastAttack.hitP.c - 4) + randInt(9);
            else
                r = (m_lastAttack.hitP.r - 4) + randInt(9);
            Point p(r, c);
            if (game().isValid(p) && !attackedBefore(p)) //test the point's valid and it hasn't been attacked before
                m_attacks.push_back(p);
            else
                continue;
            return p;
        }
    }
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    m_lastAttack.p = p;
    m_lastAttack.validShot = validShot;
    m_lastAttack.shotHit = shotHit;
    m_lastAttack.shipDestroyed = shipDestroyed;
    m_lastAttack.shipId = shipId;
    
    if (m_state == 1)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
            m_state = 1;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and destroyed a ship
            m_state = 1;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_lastAttack.hitP = m_lastAttack.p;
            m_state = 2;
        }
    }
    else if (m_state == 2)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
            m_state = 2;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and destroyed a ship
            m_state = 2;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_state = 1;
            return;
        }
        
        int counter = 0; //counts how many empty cells in the cross are left
        for (int r = m_lastAttack.hitP.r - 4; r < m_lastAttack.hitP.r + 5; r++) //check if the entire cross has been attacked
        {
            Point p(r, m_lastAttack.hitP.c);
            if (game().isValid(p) && !attackedBefore(p)) //if the point hasn't been attacked
                counter++;
        }
        for (int c = m_lastAttack.hitP.c - 4; c < m_lastAttack.hitP.c + 5; c++)
        {
            Point p(m_lastAttack.hitP.r, c);
            if (game().isValid(p) && !attackedBefore(p)) //if the point hasn't been attacked
                counter++;
        }
        if (counter == 0)
            m_state = 1;
    }
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    bool placeShips(Board& b, int nShips);
    virtual void recordAttackByOpponent(Point p);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    bool attackedBefore(const Point& p);
private:
    vector<Point> m_attacks;
    struct lastAttack
    {
        Point p;
        Point hitP;
        bool validShot = false;
        bool shotHit = false;
        bool shipDestroyed = false;
        int shipId;
    };
    lastAttack m_lastAttack;
    int m_state = 1;
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
    : Player(nm, g)
{}

bool GoodPlayer::placeShips(Board& b, int shipId)
{
    if (shipId == game().nShips()) //base case - if all the ships have been placed
        return true;
    for (int r = 0; r != game().rows(); r++)
    {
        for (int c = 0; c != game().cols(); c++)
        {
            Point p(r, c);
            if (b.placeShip(p, shipId, VERTICAL))
                if (placeShips(b, shipId + 1))
                    return true;
                else
                    b.unplaceShip(p, shipId, VERTICAL);
            if (b.placeShip(p, shipId, HORIZONTAL))
                if (placeShips(b, shipId + 1))
                    return true;
                else
                    b.unplaceShip(p, shipId, HORIZONTAL);
        }
    }
    return false;
}

bool GoodPlayer::placeShips(Board& b)
{
    b.block();
    for (int i = 0; i < 50; i++)
    {
        if (placeShips(b, 0))
        {
            b.unblock();
            return true;
        }
    }
    b.unblock();
    return false;
}

void GoodPlayer::recordAttackByOpponent(Point p)
{

}

bool GoodPlayer::attackedBefore(const Point& p)
{
    for (int i = 0; i < m_attacks.size(); i++)
    {
        if (m_attacks.at(i).r == p.r && m_attacks.at(i).c == p.c)
            return true;
    }
    return false;
}

Point GoodPlayer::recommendAttack()
{
    for (;;)
    {
        while (m_state == 1)
        {
            Point p = game().randomPoint();
            if (!attackedBefore(p)) //test that the point hasn't been attacked before
                m_attacks.push_back(p);
            else
                continue;
            return p;
        }
        while (m_state == 2)
        {
            int r = m_lastAttack.hitP.r;
            int c = m_lastAttack.hitP.c;
            int d = randInt(2);
            if (d == 0)
                c = (m_lastAttack.hitP.c - 4) + randInt(9);
            else
                r = (m_lastAttack.hitP.r - 4) + randInt(9);
            Point p(r, c);
            if (game().isValid(p) && !attackedBefore(p)) //test the point's valid and it hasn't been attacked before
                m_attacks.push_back(p);
            else
                continue;
            return p;
        }
    }
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    m_lastAttack.p = p;
    m_lastAttack.validShot = validShot;
    m_lastAttack.shotHit = shotHit;
    m_lastAttack.shipDestroyed = shipDestroyed;
    m_lastAttack.shipId = shipId;

    if (m_state == 1)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
            m_state = 1;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and destroyed a ship
            m_state = 1;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_lastAttack.hitP = m_lastAttack.p;
            m_state = 2;
        }
    }
    else if (m_state == 2)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
            m_state = 2;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and destroyed a ship
            m_state = 2;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_state = 1;
            return;
        }

        int counter = 0; //counts how many empty cells in the cross are left
        for (int r = m_lastAttack.hitP.r - 4; r < m_lastAttack.hitP.r + 5; r++) //check if the entire cross has been attacked
        {
            Point p(r, m_lastAttack.hitP.c);
            if (game().isValid(p) && !attackedBefore(p)) //if the point hasn't been attacked
                counter++;
        }
        for (int c = m_lastAttack.hitP.c - 4; c < m_lastAttack.hitP.c + 5; c++)
        {
            Point p(m_lastAttack.hitP.r, c);
            if (game().isValid(p) && !attackedBefore(p)) //if the point hasn't been attacked
                counter++;
        }
        if (counter == 0)
            m_state = 1;
    }
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
