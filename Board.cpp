#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      const Game& m_game;
      char m_board[MAXROWS][MAXCOLS] = {};
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    clear();
}

void BoardImpl::clear()
{
    for (int r = 0; r != m_game.rows(); r++)
        for (int c = 0; c != m_game.cols(); c++)
            m_board[r][c] = '.';
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                m_board[r][c] = '#';
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            switch (m_board[r][c])
            {
            case '#':
                m_board[r][c] = '.';
                break;
            default:
                break;
            }
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (m_game.isValid(topOrLeft) && shipId >= 0 && shipId < m_game.nShips())
    {
        int length = m_game.shipLength(shipId);
        char symbol = m_game.shipSymbol(shipId);
        Point temp = topOrLeft;
        
        for (int i = 0; i < length; i++) //for loop ensures the ship fits
        {
            if (m_game.isValid(temp) && m_board[temp.r][temp.c] == '.')
                ;
            else
                return false;
            if (dir == HORIZONTAL)
                temp.c++;
            if (dir == VERTICAL)
                temp.r++;
        }
        for (int r = 0; r != m_game.rows(); r++) //nested for loops ensure ship hasn't been placed yet
            for (int c = 0; c != m_game.cols(); c++)
                if (m_board[r][c] == symbol)
                    return false;

        for (int i = 0; i < length; i++) //for loop places the ship
        {
            m_board[topOrLeft.r][topOrLeft.c] = symbol;
            if (dir == HORIZONTAL)
                topOrLeft.c++;
            if (dir == VERTICAL)
                topOrLeft.r++;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (m_game.isValid(topOrLeft) && shipId >= 0 && shipId < m_game.nShips())
    {
        int length = m_game.shipLength(shipId);
        char symbol = m_game.shipSymbol(shipId);
        Point temp = topOrLeft;

        for (int i = 0; i < length; i++) //for loop checks that the entire ship is present
        {
            if (m_game.isValid(temp) && m_board[temp.r][temp.c] == symbol)
                ;
            else
                return false;
            if (dir == HORIZONTAL)
                temp.c++;
            if (dir == VERTICAL)
                temp.r++;
        }

        for (int i = 0; i < length; i++) //for loop removes the ship
        {
            m_board[topOrLeft.r][topOrLeft.c] = '.';
            if (dir == HORIZONTAL)
                topOrLeft.c++;
            if (dir == VERTICAL)
                topOrLeft.r++;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void BoardImpl::display(bool shotsOnly) const
{
    // This compiles, but may not be correct
    cout << ' ' << ' ';
    for (int c = 0; c < m_game.cols(); c++)
    {
        cout << c;
    }
    cout << endl;
    for (int r = 0; r < m_game.rows(); r++)
    {
        cout << r << ' ';
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (shotsOnly)
            {
                char symbol = m_board[r][c];
                for (int s = 0; s != m_game.nShips(); s++)
                {
                    if (m_board[r][c] == m_game.shipSymbol(s))
                    {
                        symbol = '.';
                        break;
                    }
                }
                cout << symbol;
            }
            else
            {
                cout << m_board[r][c];
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if (m_game.isValid(p) && m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X')
    {
        if (m_board[p.r][p.c] == '.') //we have a miss
        {
            shotHit = false;
            shipDestroyed = false;
            m_board[p.r][p.c] = 'o';
        }
        else //we have a hit
        {
            shotHit = true;
            char symbol = m_board[p.r][p.c];
            m_board[p.r][p.c] = 'X';

            //determine the id of the ship hit
            int id = 0;
            for (; id < m_game.nShips(); id++)
            {
                if (m_game.shipSymbol(id) == symbol)
                {
                    shipId = id;
                    break;
                }
            }
            int length = m_game.shipLength(id);

            //find out if the ship was destroyed
            bool b = true;
            for (int r = 0; r != m_game.rows(); r++)
                for (int c = 0; c != m_game.cols(); c++)
                    if (m_board[r][c] == symbol)
                        b = false;
            shipDestroyed = b;
        }
        return true;
    }
    else 
    {
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
}

bool BoardImpl::allShipsDestroyed() const
{
    bool b = true;
    for (int r = 0; r != m_game.rows(); r++) //loops through each row, col, and ship
        for (int c = 0; c != m_game.cols(); c++)
            for(int s = 0; s!= m_game.nShips(); s++)
                if (m_board[r][c] == m_game.shipSymbol(s))
                    b = false;
    return b;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
