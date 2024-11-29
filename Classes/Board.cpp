#include "Board.h"

Board* Board::instance = nullptr;

void Board::initBoard(int _numRow, int _numColumn, std::string _background, cocos2d::Node* _parent, cocos2d::Label* _movesLabel)
{
	rowSize = _numRow;
	columnSize = _numColumn;
	movesLabel = _movesLabel;

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
			block->addChild(blockLabel, 1, "text");
			blockLabel->setPosition(cocos2d::Vec2(20, 200));
			_parent->addChild(block, blockNum, std::to_string(blockNum));
			blocks.push_back(block);
			block->setPosition(pos);
			blocksPos.push_back(pos);
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

	resShuffleTillSolvable(_parent);

	auto _mouseListener = cocos2d::EventListenerMouse::create();
	_mouseListener->onMouseUp = CC_CALLBACK_1(Board::onMouseEnded, this);
	_mouseListener->onMouseMove = CC_CALLBACK_1(Board::onMouseMove, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, _parent);

	//Test
	/*auto sp = cocos2d::Sprite::create("CloseNormal.png");
	_parent->addChild(sp, 101, "sp");
	sp->setPosition(cocos2d::Vec2(0, 0));*/
}

bool Board::checkIfCompleted(cocos2d::Node* _parent)
{
	for (int i = 1; i <= rowSize * columnSize; i++)
	{
		if (_parent->getChildByName(std::to_string(i))->getPosition() != blocksPos[i - 1])
		{
			//CCLOG("Not yet Completed");
			return false;
		}
	}

	//CCLOG("Completed!");
	_parent->getChildByName(std::to_string(rowSize * columnSize))->setVisible(true);
	isSolved = true;
	return true;
}

bool Board::checkIfSolvable()
{
	int countInversion = 0;
	int lastblockPos = -1;

	for (int indx = 0; indx < blocksMap.size(); indx++)
	{
		for (int laterIndx = indx + 1; laterIndx < blocksMap.size(); laterIndx++)
		{
			if (blocksMap[indx]<(rowSize*columnSize) && blocksMap[indx] > blocksMap[laterIndx])
			{
				//CCLOG("blocksMap[indx]: %d  blocksMap[laterIndx]: %d", blocksMap[indx], blocksMap[laterIndx]);
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

	//CCLOG("Count inversion: %d", countInversion);
	if (rowSize % 2 != 0 && countInversion % 2 == 0)
	{
		//CCLOG("Solvable");
		return true;
	}
	else if (rowSize % 2 == 0)
	{
		if (lastblockPos % 2 == 0 && countInversion % 2 != 0)
		{
			//CCLOG("Solvable");
			return true;
		}
		else if (lastblockPos % 2 != 0 && countInversion % 2 == 0)
		{
			//CCLOG("Solvable");
			return true;
		}
		else
		{
			//CCLOG("Not Solvable");
			return false;
		}
	}
	//CCLOG("Not Solvable");
	return false;
}

void Board::moveBlock(cocos2d::Sprite* blockItem, int lastblockPos, int clickedItemPos)
{
	auto tmpPos = blocks.back()->getPosition();
	blocks.back()->setPosition(blockItem->getPosition());
	blockItem->setPosition(tmpPos);

	//CCLOG("MAP swap-> %d %d", blocksMap[lastblockPos], blocksMap[clickedItemPos]);
	int tmpLoc = blocksMap[lastblockPos];
	blocksMap[lastblockPos] = blocksMap[clickedItemPos];
	blocksMap[clickedItemPos] = tmpLoc;
}

void Board::resetBoard(cocos2d::Node* _parent)
{
	blocksMap.clear();
	for (int i = 1; i <= rowSize * columnSize; i++)
	{
		blocksMap[i] = i;
		_parent->getChildByName(std::to_string(i))->setPosition(blocksPos[i-1]);
	}
	_parent->getChildByName(std::to_string(rowSize* columnSize))->setVisible(false);
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
		if (i.first!=0 && i.second!=0 && visitedBlocks.find(i.first) == visitedBlocks.end() && visitedBlocks.find(i.second) == visitedBlocks.end())
		{
			//CCLOG("Swap: %s->%s", (blocks[i.first - 1]->getName().data()), (blocks[i.second - 1]->getName().data()));
			auto tmpPos = blocks[i.first - 1]->getPosition();
			blocks[i.first - 1]->setPosition(blocks[i.second - 1]->getPosition());
			blocks[i.second - 1]->setPosition(tmpPos);

			visitedBlocks[i.first]++;
			visitedBlocks[i.second]++;
		}
	}
	visitedBlocks.clear();
}

void Board::showNumMoves()
{
	numMoves++;
	auto moves = std::to_string(numMoves);
	movesLabel->setString("Moves: " + moves);
}

void Board::resShuffleTillSolvable(cocos2d::Node* _parent)
{
	numMoves = 0;
	isSolved = false;
	movesLabel->setString("Moves: 0");

	resetBoard(_parent);
	reSuffleBoard();

	while (!checkIfSolvable())
	{
		//CCLOG("Re-Shuffling");
		resetBoard(_parent);
		reSuffleBoard();
	}
}

void Board::onMouseEnded(cocos2d::Event* event)
{
	//CCLOG("Mouse click ended");
	if (isSolved)
		return;

	cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;
	auto posConv = blocks[0]->getParent()->convertToNodeSpace(e->getLocationInView());

	int lastblockPos = -1;
	for (auto item : blocksMap)
	{
		if (item.second == stoi(blocks.back()->getName()))
		{
			lastblockPos = item.first;
		}
	}
	//CCLOG("Empty block is at pos: %d", lastblockPos);
	int _x = (lastblockPos-1) / rowSize;
	int _y = (lastblockPos-1) % rowSize;
	//CCLOG("2d index pos-> x: %d  y: %d", _x, _y);

	cocos2d::Vec2 posUp{ float(_x - 1), float(_y) };
	cocos2d::Vec2 posLeft{ float(_x), float(_y - 1) };
	cocos2d::Vec2 posRight{ float(_x), float(_y + 1) };
	cocos2d::Vec2 posDown{ float(_x + 1), float(_y) };

	/*CCLOG("posUpX: %d  posUpY: %d", (int)posUp.x, (int)posUp.y);
	CCLOG("posLeftX: %d  posLeftY: %d", (int)posLeft.x, (int)posLeft.y);
	CCLOG("posRightX: %d  posRightY: %d", (int)posRight.x, (int)posRight.y);
	CCLOG("posDownX: %d  posDownY: %d", (int)posDown.x, (int)posDown.y);*/

	/*if (posUp.x >= 0 && posUp.x < rowSize && posUp.y >= 0 && posUp.y < rowSize)
	{
		CCLOG("Up move is valid");
	}
	if (posLeft.x >= 0 && posLeft.x < rowSize && posLeft.y >= 0 && posLeft.y < rowSize)
	{
		CCLOG("Left move is valid");
	}
	if (posRight.x >= 0 && posRight.x < rowSize && posRight.y >= 0 && posRight.y < rowSize)
	{
		CCLOG("Right move is valid");
	}
	if (posDown.x >= 0 && posDown.x < rowSize && posDown.y >= 0 && posDown.y < rowSize)
	{
		CCLOG("Down move is valid");
	}*/

	for (auto blockItem : blocks)
	{
		if (blockItem->getBoundingBox().containsPoint(posConv))
		{
			//CCLOG(blockItem->getName().data());
			//blockItem->setVisible(false);

			int clickedItemPos = -1;
			for (auto item : blocksMap)
			{
				if (item.second == std::stoi(blockItem->getName()))
				{
					clickedItemPos = item.first;
				}
			}
			//CCLOG("Clicked item pos: %d", clickedItemPos);

			int _clickedX = (clickedItemPos - 1) / rowSize;
			int _clickedY = (clickedItemPos - 1) % rowSize;
			//CCLOG("ClickedBlockPosX: %d  ClickedBlockPosY: %d", _clickedX, _clickedY);

			cocos2d::Vec2 clickedPos{ float(_clickedX), float(_clickedY) };

			if (clickedPos == posUp)
			{
				//CCLOG("Moving Up");
				moveBlock(blockItem, lastblockPos, clickedItemPos);
				showNumMoves();
				if (checkIfCompleted(blockItem->getParent()))
				{
					blockItem->setColor(cocos2d::Color3B(255, 255, 255));
				}
				return;
			}
			else if (clickedPos == posDown)
			{
				//CCLOG("Moving Down");
				moveBlock(blockItem, lastblockPos, clickedItemPos);
				showNumMoves();
				if (checkIfCompleted(blockItem->getParent()))
				{
					blockItem->setColor(cocos2d::Color3B(255, 255, 255));
				}
				return;
			}
			if (clickedPos == posRight)
			{
				//CCLOG("Moving Right");
				moveBlock(blockItem, lastblockPos, clickedItemPos);
				showNumMoves();
				if (checkIfCompleted(blockItem->getParent()))
				{
					blockItem->setColor(cocos2d::Color3B(255, 255, 255));
				}
				return;
			}
			if (clickedPos == posLeft)
			{
				//CCLOG("Moving Left");
				moveBlock(blockItem, lastblockPos, clickedItemPos);
				showNumMoves();
				if (checkIfCompleted(blockItem->getParent()))
				{
					blockItem->setColor(cocos2d::Color3B(255, 255, 255));
				}
				return;
			}
		}
	}
}

void Board::onMouseMove(cocos2d::Event* event)
{
	//CCLOG("Mouse moving");
	if (isSolved)
		return;

	cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;

	auto posConv = blocks[0]->getParent()->convertToNodeSpace(e->getLocationInView());
	//blocks[0]->getParent()->getChildByName("sp")->setPosition(posConv);
	for (auto blockItem : blocks)
	{
		if (blockItem->getBoundingBox().containsPoint(posConv))
		{
			//blockItem->setOpacity(100);
			blockItem->setColor(cocos2d::Color3B(255, 255, 0));
		}
		else
		{
			//blockItem->setOpacity(255);
			blockItem->setColor(cocos2d::Color3B(255, 255, 255));
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
	numMoves = 0;
	isSolved = false;
}
