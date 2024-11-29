#ifndef __Board_H__
#define __Board_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <random>

class Board
{
private:
	static Board* instance;
	int rowSize;
	int columnSize;
	bool isSolved;
	std::vector<cocos2d::Sprite*> blocks;
	std::unordered_map<int, int> blocksMap;
	std::vector<cocos2d::Vec2> blocksPos;
	int numMoves;
	cocos2d::Label* movesLabel;

	Board(int _numRow = 3, int _numColumn = 3);

	void onMouseEnded(cocos2d::Event* event);
	void Board::onMouseMove(cocos2d::Event* event);

	void moveBlock(cocos2d::Sprite* blockItem, int lastblockPos, int clickedItemPos);
	void resetBoard(cocos2d::Node* _parent);
	void reSuffleBoard();

public:
	void initBoard(int _numRow, int _numColumn, std::string background, cocos2d::Node* parent, cocos2d::Label* movesLabel);
	bool checkIfCompleted();
	bool checkIfSolvable();
	void showNumMoves();
	void resShuffleTillSolvable(cocos2d::Node* parent);

	static Board* getInstance();
};

#endif