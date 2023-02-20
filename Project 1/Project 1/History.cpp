#include <iostream>
#include "History.h"
#include "globals.h"
using namespace std;

History::History(int nRows, int nCols)
	: m_rows(nRows), m_cols(nCols)
{ 

}
bool History::record(int r, int c)
{
    bool b = false;
    if (r <= m_rows && r >= 1 && c <= m_cols && c >= 1)
    {
        b = true;
        (m_record[r - 1][c - 1])++;
    }
	return(b);
}
void History::display() const
{
    // Position (row,col) in mesa coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    int hits;

    // Translate integer record grid into character grid
    
    for (r = 0; r < m_rows; r++)
        for (c = 0; c < m_cols; c++)
        {
            hits = m_record[r][c];
            if (hits < 27)
            {
                grid[r][c] = m_key[hits];
            }
            else if (hits >= 27)
            {
                grid[r][c] = 'Z';
            }
        }

    // Draw the history grid
    clearScreen();
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;    
}