#include "Game.h"
#include "Player.h"
#include "globals.h"
#include "Board.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

int main()
{
    Game g(10, 10);
    assert(g.rows() == 10 && g.cols() == 10);
    assert(!g.isValid(Point(10, 11)) && g.isValid(Point(1, 1)));
    addStandardShips(g);
    assert(g.nShips() == 5);
    assert(g.shipLength(1) == 4);
    assert(g.shipSymbol(1) == 'B');
    assert(g.shipName(1) == "battleship");

    Board b(g);
    Point p1(1, 2);
    Point p2(9, 9);
    Point p3(0, 2);
    Point p4(3, 2);
    Point p5(1, 3);
    bool b1 = false;
    bool b2 = false;
    int i = 0;

    b.block();
    b.unblock();

    assert(b.placeShip(p2, 1, HORIZONTAL) == false);
    assert(b.placeShip(p1, 1, HORIZONTAL) == true);
    assert(b.placeShip(p3, 0, VERTICAL) == false);
    assert(b.placeShip(p4, 0, VERTICAL) == true);

    /*assert(b.unplaceShip(p5, 1, HORIZONTAL) == false);
    assert(b.unplaceShip(p1, 2, HORIZONTAL) == false);
    assert(b.unplaceShip(p1, 1, VERTICAL) == false);
    assert(b.unplaceShip(p1, 1, HORIZONTAL) == true);
    assert(b.unplaceShip(p2, 0, VERTICAL) == false);
    assert(b.unplaceShip(p4, 0, VERTICAL) == true);*/
    assert(b.allShipsDestroyed() == false);

    assert(b.attack(p2, b1, b2, i) == true && b1 == false && b2 == false);
    assert(b.attack(p2, b1, b2, i) == false && b1 == false && b2 == false);
    assert(b.attack(p4, b1, b2, i) == true && b1 == true && b2 == false && i == 0);
    p4.r++;
    assert(b.attack(p4, b1, b2, i) == true && b1 == true && b2 == false);
    p4.r++;
    assert(b.attack(p4, b1, b2, i) == true && b1 == true && b2 == false);
    p4.r++;
    assert(b.attack(p4, b1, b2, i) == true && b1 == true && b2 == false);
    p4.r++;
    assert(b.attack(p4, b1, b2, i) == true && b1 == true && b2 == true);
    p4.r++;
    assert(b.attack(p4, b1, b2, i) == true && b1 == false && b2 == false);
    
    assert(b.allShipsDestroyed() == false);

    assert(b.attack(p1, b1, b2, i) == true && b1 == true && b2 == false && i == 1);
    p1.c++;
    assert(b.attack(p1, b1, b2, i) == true && b1 == true && b2 == false && i == 1);
    p1.c++;

    assert(b.attack(p1, b1, b2, i) == true && b1 == true && b2 == false && i == 1);
    p1.c++;
    assert(b.attack(p1, b1, b2, i) == true && b1 == true && b2 == true && i == 1);
    p1.c++;
    
    assert(b.allShipsDestroyed() == true);

    Game g2(10, 10);
    addStandardShips(g2);
    Board b22(g2);
    Player* mediocre = createPlayer("mediocre", "zaid", g2);
    b22.display(false);
    mediocre->placeShips(b22);
    b22.display(false);

    delete mediocre;

    //b.display(true);
    //b.display(false);

    /*const int NTRIALS = 10;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a mediocre and an awful player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        int nMediocreWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("awful", "Awful Audrey", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p1;
            delete p2;
        }
        cout << "The mediocre player won " << nMediocreWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }*/
}
