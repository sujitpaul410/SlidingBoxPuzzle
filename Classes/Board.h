#ifndef __Board_H__
#define __Board_H__

#include "cocos2d.h"
#include <string>
#include <vector>

class Board
{
private:
	static Board* instance;
	int rowSize;
	int columnSize;
	bool isSolved;
	std::vector<cocos2d::Sprite*> blocks;

	Board(int _numRow = 3, int _numColumn = 3);

public:
	void initBoard(int _numRow, int _numColumn, std::string background, cocos2d::Node* parent);
	bool checkIfCompleted();
	bool checkIfSolvable();
	void reSuffleBoard();
	void moveBlock();
	void highlightBlock();
	void showNumMoves();
	virtual void onMouseEnded(cocos2d::Event* event);
	void Board::onMouseMove(cocos2d::Event* event);

	static Board* getInstance();
};

#endif