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
			pos.x += 367;
			rectPosX += 364;
			blockNum++;
		}
		pos.x = 188;
		pos.y -= 228;
		rectPosX = 0;
		rectPosY += 225;
	}

	auto _mouseListener = cocos2d::EventListenerMouse::create();
	_mouseListener->onMouseUp = CC_CALLBACK_1(Board::onMouseEnded, this);
	_mouseListener->onMouseMove = CC_CALLBACK_1(Board::onMouseMove, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, _parent);

	//Test
	auto sp = cocos2d::Sprite::create("CloseNormal.png");
	_parent->addChild(sp, 101, "sp");
	sp->setPosition(cocos2d::Vec2(0, 0));
}

bool Board::checkIfCompleted()
{
	return false;
}

bool Board::checkIfSolvable()
{
	return false;
}

void Board::reSuffleBoard()
{
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
	CCLOG("Mouse click ended");
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
	CCLOG("Mouse moving");
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
