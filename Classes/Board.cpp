#include "Board.h"

Board* Board::instance = nullptr;

void Board::initBoard(int _numRow, int _numColumn, std::string _background, cocos2d::Node* _parent)
{
	rowSize = _numRow;
	columnSize = _numColumn;

	cocos2d::Vec2 pos = cocos2d::Vec2(188, 575);
	float rectPosX = 0;
	float rectPosY = 0;
	int blockNum = 1;

	for (int row = 0; row < rowSize; row++)
	{
		for (int col = 0; col < columnSize; col++)
		{
			cocos2d::Sprite* block = cocos2d::Sprite::create(_background, cocos2d::Rect(rectPosX, rectPosY, 364, 225));
			cocos2d::Label* blockLabel = cocos2d::Label::createWithTTF(std::to_string(blockNum), "fonts/Marker Felt.ttf", 35);
			blockLabel->enableOutline(cocos2d::Color4B(0, 0, 0, 255), 2);
			block->addChild(blockLabel);
			blockLabel->setPosition(cocos2d::Vec2(20, 200));
			_parent->addChild(block);
			blocks.push_back(block);
			block->setPosition(pos);
			blocksMap[blockNum] = blockNum;
			pos.x += 367;
			rectPosX += 364;
			blockNum++;
		}
		pos.x = 188;
		pos.y -= 228;
		rectPosX = 0;
		rectPosY += 225;
	}
	blocks.back()->setVisible(false);

	reSuffleBoard();
	//getRandomNumInRange(1, 9);
	while (!checkIfSolvable())
	{
		CCLOG("Re-Shuffling");
		blocksMap.clear();
		reSuffleBoard();
	}

	auto _mouseListener = cocos2d::EventListenerMouse::create();
	_mouseListener->onMouseUp = CC_CALLBACK_1(Board::onMouseEnded, this);
	_mouseListener->onMouseMove = CC_CALLBACK_1(Board::onMouseMove, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, _parent);

	//Test
	/*auto sp = cocos2d::Sprite::create("CloseNormal.png");
	_parent->addChild(sp, 101, "sp");
	sp->setPosition(cocos2d::Vec2(0, 0));*/
}

bool Board::checkIfCompleted()
{
	return false;
}

bool Board::checkIfSolvable()
{
	int countInversion = 0;
	int lastblockPos = -1;

	for (int indx = 0; indx < blocksMap.size(); indx++)
	{
		for (int laterIndx = indx + 1; laterIndx < blocksMap.size(); laterIndx++)
		{
			if (blocksMap[indx] > blocksMap[laterIndx])
			{
				countInversion++;
			}
		}
	}
	for (auto item : blocksMap)
	{
		if (item.second == (rowSize * rowSize))
		{
			lastblockPos = item.first;
			lastblockPos = (rowSize - lastblockPos);
		}
	}

	if (rowSize % 2 != 0 && countInversion % 2 == 0)
	{
		CCLOG("Solvable");
		return true;
	}
	else if (rowSize % 2 == 0)
	{
		if (lastblockPos % 2 == 0 && countInversion % 2 != 0)
		{
			CCLOG("Solvable");
			return true;
		}
		else if (lastblockPos % 2 != 0 && countInversion % 2 == 0)
		{
			CCLOG("Solvable");
			return true;
		}
		else
		{
			CCLOG("Not Solvable");
			return false;
		}
	}
	CCLOG("Not Solvable");
	return false;
}

int Board::getRandomNumInRange(int min, int max)
{
	//CCLOG("called randomNum");
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(min, max);

	for (int i = 0; i < 4; i++)
	{
		CCLOG("%d ", distrib(gen));
	}

	return distrib(gen);
}

void Board::reSuffleBoard()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, blocks.size());

	std::unordered_map<int, int> tempMap;
	int numSwapsRequired = ceil(blocks.size() / 2) + 1;
	//CCLOG("NumSwapsReq: %d", numSwapsRequired);

	for (int num = 0; num < numSwapsRequired; num++)
	{
		int pos1 = distrib(gen);
		int pos2 = distrib(gen);

		while (tempMap.find(pos1) != tempMap.end() || tempMap.find(pos2) != tempMap.end())
		{
			pos1 = distrib(gen);
			pos2 = distrib(gen);
		}

		//CCLOG("pos1: %d pos2: %d", pos1, pos2);
		tempMap[pos1] = pos2;
		tempMap[pos2] = pos1;
	}

	//CCLOG("TempMap contents:");
	for (auto i : tempMap)
	{
		//CCLOG("%d -> %d", i.first, i.second);
		blocksMap[i.first] = i.second;
	}
	tempMap.clear();
	/*CCLOG("blocksMap contents:");
	for (auto i : blocksMap)
	{
		CCLOG("%d -> %d", i.first, i.second);
	}*/

	std::unordered_map<int, int> visitedBlocks;
	for (auto i : blocksMap)
	{
		if (visitedBlocks.find(i.first) == visitedBlocks.end() && visitedBlocks.find(i.second) == visitedBlocks.end())
		{
			auto tmpPos = blocks[i.first - 1]->getPosition();
			blocks[i.first - 1]->setPosition(blocks[i.second - 1]->getPosition());
			blocks[i.second - 1]->setPosition(tmpPos);

			visitedBlocks[i.first]++;
			visitedBlocks[i.second]++;
		}
	}
	visitedBlocks.clear();
}

void Board::moveBlock()
{
}

void Board::highlightBlock()
{
}

void Board::showNumMoves()
{
}

void Board::onMouseEnded(cocos2d::Event* event)
{
	//CCLOG("Mouse click ended");
	cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;
	auto posConv = blocks[0]->getParent()->convertToNodeSpace(e->getLocationInView());

	for (auto blockItem : blocks)
	{
		if (blockItem->getBoundingBox().containsPoint(posConv))
		{
			blockItem->setVisible(false);
		}
	}
}

void Board::onMouseMove(cocos2d::Event* event)
{
	//CCLOG("Mouse moving");
	cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;

	auto posConv = blocks[0]->getParent()->convertToNodeSpace(e->getLocationInView());
	//blocks[0]->getParent()->getChildByName("sp")->setPosition(posConv);
	for (auto blockItem : blocks)
	{
		if (blockItem->getBoundingBox().containsPoint(posConv))
		{
			blockItem->setOpacity(100);
		}
		else
		{
			blockItem->setOpacity(255);
		}
	}
}



Board* Board::getInstance()
{
	if (!instance)
	{
		instance = new Board();
	}
	return instance;
}

Board::Board(int _numRow, int _numColumn)
{
	rowSize = _numRow;
	columnSize = _numColumn;
}
