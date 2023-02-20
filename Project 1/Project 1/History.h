#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"

class Mesa;
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_rows;
	int m_cols;
	int m_record[MAXROWS][MAXCOLS] = { }; //creates a grid of 0s
	char m_key[27] = {'.', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', //the index of each char indicates the int it corresponds to
					  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
					  'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
};

#endif // !HISTORY_H


