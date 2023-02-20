#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <algorithm>
#include <queue>
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
            {
                if (placeShips(b, shipId + 1))
                    return true;
                else
                    b.unplaceShip(p, shipId, VERTICAL);
            }
            if (b.placeShip(p, shipId, HORIZONTAL))
            {
                if (placeShips(b, shipId + 1))
                    return true;
                else
                    b.unplaceShip(p, shipId, HORIZONTAL);
            }
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
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
            m_state = 2;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and destroyed a ship
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
    bool attackedBefore(const Point& p, bool misses = false) const;
    bool spaceShips(Board& b) const;
    vector<Point> recommendation() const;
private:
    vector<Point> m_attacks;
    vector<Point> m_misses;
    struct lastAttack
    {
        Point p;
        bool validShot = false;
        bool shotHit = false;
        bool shipDestroyed = false;
        int shipId;
    };
    struct hitInfo
    {
        Point hitP;
        Direction hitDir;
        bool downOrRight = true;
        int hitLen = 0;
        int hitId;
    };
    lastAttack m_lastAttack;
    queue<hitInfo> m_hitInfo;
    vector<int> m_shipsLeft;
    int m_state = 1;
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
    : Player(nm, g)
{
    for (int i = 0; i < game().nShips(); i++)
    {
        m_shipsLeft.push_back(game().shipLength(i));
    }
}

bool GoodPlayer::spaceShips(Board& b) const
{
    queue<Point> shipCoords;

    int maxShipLength = 0;
    for (int i = 0; i < game().nShips(); i++)
    {
        if (game().shipLength(i) > maxShipLength)
            maxShipLength = game().shipLength(i);
    }

    for (int i = 0; i < game().nShips(); i++)
    {
        Point p(0, 0);
        Direction dir = VERTICAL;
        if (i == game().nShips() - 1)
        {
            p.r = maxShipLength - 1;
            p.c = game().cols() - game().shipLength(i);
            dir = HORIZONTAL;
        }
        else if (i % 2 == 0)
        {
            p.r = 0;
            p.c = 1 + (i/2 * 5);
        }
        else
        {
            p.r = game().rows() - game().shipLength(i);
            p.c = 2 + ((i - 1)/2 * 5);
        }

        if (b.placeShip(p, i, dir))
        {
            shipCoords.push(p);
        }
        else
        {
            for (int j = 0; j < i; j++)
            {
                b.unplaceShip(shipCoords.front(), j, VERTICAL);
                shipCoords.pop();
            }
            return false;
        }
    }
    return true;
}

bool GoodPlayer::placeShips(Board& b, int shipId)
{
    if (shipId == game().nShips()) //base case - if all the ships have been placed
        return true;
    int i = randInt(4);
    switch (i)
    {
    case 0:
        for (int r = 0; r != game().rows(); r++)
        {
            for (int c = 0; c != game().cols(); c++)
            {
                Point p(r, c);
                if (b.placeShip(p, shipId, VERTICAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, VERTICAL);
                }
                if (b.placeShip(p, shipId, HORIZONTAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, HORIZONTAL);
                }
            }
        }
        break;
    case 1:
        for (int r = 0; r != game().rows(); r++)
        {
            for (int c = game().cols() - 1; c != -1; c--)
            {
                Point p(r, c);
                if (b.placeShip(p, shipId, VERTICAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, VERTICAL);
                }
                if (b.placeShip(p, shipId, HORIZONTAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, HORIZONTAL);
                }
            }
        }
        break;
    case 2:
        for (int r = game().rows() - 1; r != -1; r--)
        {
            for (int c = 0; c != game().cols(); c++)
            {
                Point p(r, c);
                if (b.placeShip(p, shipId, VERTICAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, VERTICAL);
                }
                if (b.placeShip(p, shipId, HORIZONTAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, HORIZONTAL);
                }
            }
        }
        break;
    case 3:
        for (int r = game().rows(); r != -1; r--)
        {
            for (int c = game().cols(); c != -1; c--)
            {
                Point p(r, c);
                if (b.placeShip(p, shipId, VERTICAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, VERTICAL);
                }
                if (b.placeShip(p, shipId, HORIZONTAL))
                {
                    if (placeShips(b, shipId + 1))
                        return true;
                    else
                        b.unplaceShip(p, shipId, HORIZONTAL);
                }
            }
        }
        break;
    default:
        break;
    }
    return false;
}

bool GoodPlayer::placeShips(Board& b)
{
    if (spaceShips(b))
        return true;
    else
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
}

void GoodPlayer::recordAttackByOpponent(Point p)
{

}

bool GoodPlayer::attackedBefore(const Point& p, bool misses) const
{
    if (misses)
    {
        for (int i = 0; i < m_misses.size(); i++)
        {
            if (m_misses.at(i).r == p.r && m_misses.at(i).c == p.c)
                return true;
        }
    }
    else
    {
        for (int i = 0; i < m_attacks.size(); i++)
        {

            if (m_attacks.at(i).r == p.r && m_attacks.at(i).c == p.c)
                return true;
        }
    }
    return false;
}

vector<Point> GoodPlayer::recommendation() const
{
    //create a grid giving the number of times each ship could be located at that point
    vector<vector<int>> grid(game().rows());
    int maxCount = 0;
    Point recommend = game().randomPoint(); //creates a random point by default in the top half
    recommend.r -= randInt(recommend.r + 1);
    for (int r = 0; r < game().rows(); r++)
    {
        grid.at(r).resize(game().cols());
    }
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            for (int s = 0; s < m_shipsLeft.size(); s++)
            {
                int length = m_shipsLeft.at(s);
                Point p(r, c);
                if (attackedBefore(p))
                    break;
                for (int j = 0; j < length; j++)
                {
                    Point q(r + j, c);
                    if (!game().isValid(q) || attackedBefore(q, true))
                        break;
                    if (j == length - 1) //these also increase the prob of surrounding available points
                    {
                        grid.at(p.r).at(p.c)++;
                    }
                }
                for (int k = 0; k < length; k++)
                {
                    Point q(r, c + k);
                    if (!game().isValid(q) || attackedBefore(q, true))
                        break;
                    if (k == length - 1)
                    {
                        grid.at(p.r).at(p.c)++;
                    }
                }
                for (int j = 0; j < length; j++)
                {
                    Point q(r - j, c);
                    if (!game().isValid(q) || attackedBefore(q, true))
                        break;
                    if (j == length - 1)
                    {
                        grid.at(p.r).at(p.c)++;
                    }
                }
                for (int k = 0; k < length; k++)
                {
                    Point q(r, c - k);
                    if (!game().isValid(q) || attackedBefore(q, true))
                        break;
                    if (k == length - 1)
                    {
                        grid.at(p.r).at(p.c)++;
                    }
                }
            }
            //cout << grid.at(r).at(c) << " ";
            if (grid.at(r).at(c) > maxCount)
            {
                maxCount = grid.at(r).at(c);
                recommend.r = r;
                recommend.c = c;
            }
        }
        //cout << endl;
    }
    //create a vector of points with the highest count and randomly return one of them
    vector<Point> recommendations;
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if (grid.at(r).at(c) == maxCount)
            {
                Point p(r, c);
                recommendations.push_back(p);
            }
        }
    }
    return recommendations;
}

Point GoodPlayer::recommendAttack()
{
    for (;;)
    {
        while (m_state == 1)
        {
            //PROBABILITY METHOD
            vector<Point> recommendations = recommendation();
            for (;;)
            {
                Point p = recommendations.at(randInt(recommendations.size()));
                if (!attackedBefore(p)) //test that the point hasn't been attacked before
                {
                    m_attacks.push_back(p);
                    return p;
                }
            }
            
            /*//CHECKERBOARD METHOD
            for (int count = 0; count < 100; count++)
            {
                int r, c;
                int i = randInt(2);
                if (i == 0) //even
                {
                    r = 2 * randInt(game().rows() / 4); //ensures the top half is hit
                    c = 2 * randInt(game().cols() / 2);
                }
                else //odd
                {
                    r = 2 * randInt(game().rows() / 4) + 1;
                    c = 2 * randInt(game().cols() / 2) + 1;
                }
                //r -= 2 * randInt(r / 2); //increases liklihood of attacking a cell in the upper half
                Point p(r, c);
                if (!attackedBefore(p)) //test that the point hasn't been attacked before
                {
                    m_attacks.push_back(p);
                    return p;
                }                
            }
            for (;;)
            {
                Point p = game().randomPoint(); //revert to random points if shots in between needed
                if (!attackedBefore(p)) //test that the point hasn't been attacked before
                {
                    m_attacks.push_back(p);
                    return p;
                }
            }
            
            //RANDOM POINT METHOD
            Point p = game().randomPoint();
            p.r -= randInt(p.r + 1); //increases liklihood of attacking a cell in the upper half
            if (!attackedBefore(p)) //test that the point hasn't been attacked before
                m_attacks.push_back(p);
            else
                continue;
            return p;*/
        }
        while (m_state == 2) //figure out the direction of the ship hit
        {
            int r = m_hitInfo.front().hitP.r;
            int c = m_hitInfo.front().hitP.c;
            int counter = 0; //if all four cells directly touching the center of attack have been attacked, revert to state 1
            for (int i = -1; i < 2; i += 2)
            {
                Point p(m_hitInfo.front().hitP.r, m_hitInfo.front().hitP.c + i);
                if (game().isValid(p) && !attackedBefore(p))
                    counter++;
            }
            for (int i = -1; i < 2; i += 2)
            {
                Point p(m_hitInfo.front().hitP.r + i, m_hitInfo.front().hitP.c);
                if (game().isValid(p) && !attackedBefore(p))
                    counter++;
            }
            if (counter == 0)
            {
                m_hitInfo.pop();
                m_state = 1;
                break;
            }
                   
            /*int d = randInt(2);
            if (d == 0)
            {
                c = (m_hitInfo.front().hitP.c - 1) + randInt(3);
                m_hitInfo.front().hitDir = HORIZONTAL;
            }
            else
            {
                r = (m_hitInfo.front().hitP.r - 1) + randInt(3);
                m_hitInfo.front().hitDir = VERTICAL;
            }
            if (r < m_hitInfo.front().hitP.r || c < m_hitInfo.front().hitP.c)
                m_hitInfo.front().downOrRight = false;
            else
                m_hitInfo.front().downOrRight = true;*/

            Point p(r, c);
            int direction[4] = { };
            for (int s = 0; s < m_shipsLeft.size(); s++)
            {
                int length = m_shipsLeft.at(s);
                for (int j = 1; j < length; j++)
                {
                    Point q(r + j, c);
                    if (!game().isValid(q) || attackedBefore(q))
                        break;
                    if (j == length - 1)
                        direction[0]++; //down
                }
                for (int k = 1; k < length; k++)
                {
                    Point q(r, c + k);
                    if (!game().isValid(q) || attackedBefore(q))
                        break;
                    if (k == length - 1)
                        direction[1]++; //right
                }
                for (int j = 1; j < length; j++)
                {
                    Point q(r - j, c);
                    if (!game().isValid(q) || attackedBefore(q))
                        break;
                    if (j == length - 1)
                        direction[2]++; //up
                }
                for (int k = 1; k < length; k++)
                {
                    Point q(r, c - k);
                    if (!game().isValid(q) || attackedBefore(q, true))
                        break;
                    if (k == length - 1)
                        direction[3]++; //left
                }
            }
            int maxCount = 1;
            int index = randInt(4);
            for (int i = 0; i < 4; i++)
                if (direction[i] > maxCount)
                {
                    maxCount = direction[i];
                    index = i;
                }
            switch (index)
            {
            case 0:
                p.r++;
                m_hitInfo.front().hitDir = VERTICAL;
                m_hitInfo.front().downOrRight = true;
                break;
            case 1:
                p.c++;
                m_hitInfo.front().hitDir = HORIZONTAL;
                m_hitInfo.front().downOrRight = true;
                break;
            case 2:
                p.r--;
                m_hitInfo.front().hitDir = VERTICAL;
                m_hitInfo.front().downOrRight = false;
                break;
            case 3:
                p.c--;
                m_hitInfo.front().hitDir = HORIZONTAL;
                m_hitInfo.front().downOrRight = false;
                break;
            default:
                break;
            }
            if (game().isValid(p) && !attackedBefore(p)) //test the point's valid and it hasn't been attacked before
            {
                m_attacks.push_back(p);
                return p;
            }

            for (int i = 0; i < 10; i++)
            {
                int index = randInt(4);
                switch (index)
                {
                case 0:
                    p.r++;
                    m_hitInfo.front().hitDir = VERTICAL;
                    m_hitInfo.front().downOrRight = true;
                    break;
                case 1:
                    p.c++;
                    m_hitInfo.front().hitDir = HORIZONTAL;
                    m_hitInfo.front().downOrRight = true;
                    break;
                case 2:
                    p.r--;
                    m_hitInfo.front().hitDir = VERTICAL;
                    m_hitInfo.front().downOrRight = false;
                    break;
                case 3:
                    p.c--;
                    m_hitInfo.front().hitDir = HORIZONTAL;
                    m_hitInfo.front().downOrRight = false;
                    break;
                default:
                    break;
                }
                if (game().isValid(p) && !attackedBefore(p)) //test the point's valid and it hasn't been attacked before
                    m_attacks.push_back(p);
                else
                    continue;
                return p;
            }
            m_state = 1;
        }
        while (m_state == 4) //destroy the ship from one side
        {
            int r = m_hitInfo.front().hitP.r;
            int c = m_hitInfo.front().hitP.c;
            int i = 1;
            Point p(r, c);
            if (m_hitInfo.front().downOrRight == true)
                i = 1;
            else
                i = -1;
            for (int n = 1; n < 5; n++)
            {
                if (m_hitInfo.front().hitDir == HORIZONTAL)
                    p.c = m_hitInfo.front().hitP.c + (i * n);
                else
                    p.r = m_hitInfo.front().hitP.r + (i * n);
                if (attackedBefore(p, true)) //test the point's valid and it hasn't been attacked before
                {
                    break;
                }
                else if (game().isValid(p) && !attackedBefore(p)) //test the point's valid and it hasn't been attacked before
                {
                    m_attacks.push_back(p);
                    return p;
                }
            }
            m_state = 3; //if the player dead ends they should go to state 3
            if (m_hitInfo.front().downOrRight)
                m_hitInfo.front().downOrRight = false;
            else
                m_hitInfo.front().downOrRight = true;
        }
        while (m_state == 3) //destroy the ship from one side
        {
            int r = m_hitInfo.front().hitP.r;
            int c = m_hitInfo.front().hitP.c;
            int i = 1;
            Point p(r, c);
            if (m_hitInfo.front().downOrRight == true)
                i = 1;
            else
                i = -1;
            for (int n = 1; n < 5; n++)
            {
                if (m_hitInfo.front().hitDir == HORIZONTAL)
                    p.c = m_hitInfo.front().hitP.c + (i * n);
                else
                    p.r = m_hitInfo.front().hitP.r + (i * n);
                if (attackedBefore(p, true)) //test the point's valid and it hasn't been attacked before
                {
                    break;
                }
                if (game().isValid(p) && !attackedBefore(p)) //test the point's valid and it hasn't been attacked before
                {
                    m_attacks.push_back(p);
                    return p;
                }
            }
            m_state = 2; //if the player dead ends they should revert to state 2
        }
    }
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    Point lastLastAttack = m_lastAttack.p;

    m_lastAttack.p = p;
    m_lastAttack.validShot = validShot;
    m_lastAttack.shotHit = shotHit;
    m_lastAttack.shipDestroyed = shipDestroyed;
    m_lastAttack.shipId = shipId;

    hitInfo h;
    h.hitP = p;
    h.hitId = shipId;


    if (m_lastAttack.validShot && !m_lastAttack.shotHit)
        m_misses.push_back(p);

    if (m_state == 1)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
            m_state = 1;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and destroyed a ship
        {
            m_state = 1;
            int length = game().shipLength(shipId);
            vector<int>::iterator i = find(m_shipsLeft.begin(), m_shipsLeft.end(), length);
            m_shipsLeft.erase(i);
        }
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_hitInfo.push(h);
            m_state = 2;
        }
    }
    else if (m_state == 2)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
            m_state = 2;
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_state = 4;
        }
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and destroyed a ship
        {
            if(!m_hitInfo.empty())
                m_hitInfo.pop();
            if (!m_hitInfo.empty())
                m_state = 2;
            else
                m_state = 1;
            int length = game().shipLength(shipId);
            vector<int>::iterator i = find(m_shipsLeft.begin(), m_shipsLeft.end(), length);
            m_shipsLeft.erase(i);
        }
    }
    else if (m_state == 3)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
        {
            m_state = 2;
        }
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_state = 3;
        }
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and and destroyed a ship
        {
            if (!m_hitInfo.empty())
                m_hitInfo.pop();
            if (!m_hitInfo.empty())
                m_state = 2;
            else
                m_state = 1;
            int length = game().shipLength(shipId);
            vector<int>::iterator i = find(m_shipsLeft.begin(), m_shipsLeft.end(), length);
            m_shipsLeft.erase(i);
        }
    }
    else if (m_state == 4)
    {
        if (m_lastAttack.validShot && !m_lastAttack.shotHit) //if the attack was valid but missed
        {
            m_state = 3;
            if (m_hitInfo.front().downOrRight == true) //needs to be flipped because the missed shot represents where the ship is NOT
                m_hitInfo.front().downOrRight = false;
            else
                m_hitInfo.front().downOrRight = true;            //h.hitP = lastLastAttack;
            //m_hitInfo.push(h); //TEST TO SEE IF THIS HELPS
        }
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && !m_lastAttack.shipDestroyed) //if the attack was valid and hit but didn't destroy a ship
        {
            m_state = 4;
        }
        else if (m_lastAttack.validShot && m_lastAttack.shotHit && m_lastAttack.shipDestroyed) //if the attack was valid and and destroyed a ship
        {
            if (!m_hitInfo.empty())
                m_hitInfo.pop();
            if (!m_hitInfo.empty())
                m_state = 2; 
            else
                m_state = 1;
            int length = game().shipLength(shipId);
            vector<int>::iterator i = find(m_shipsLeft.begin(), m_shipsLeft.end(), length);
            m_shipsLeft.erase(i);
        }
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
