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

	Board(int _numRow = 3, int _numColumn = 3);

	void onMouseEnded(cocos2d::Event* event);
	void Board::onMouseMove(cocos2d::Event* event);

	void moveBlock();
	void highlightBlock();
	int getRandomNumInRange(int min, int max);
	void resetBoard(cocos2d::Node* _parent);

public:
	void initBoard(int _numRow, int _numColumn, std::string background, cocos2d::Node* parent);
	bool checkIfCompleted();
	bool checkIfSolvable();
	void reSuffleBoard();
	void showNumMoves();

	static Board* getInstance();
};

#endif