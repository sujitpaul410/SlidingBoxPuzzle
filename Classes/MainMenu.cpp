#include "MainMenu.h"
#include "Board.h"

USING_NS_CC;

#define audioInstance cocos2d::experimental::AudioEngine

bool MainMenu::isPlaying = false;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}

bool MainMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }
    //Resolution
    deviceResolution = Vec2(1152, 864);

    Director::getInstance()->getOpenGLView()->setFrameSize(deviceResolution.x / 1.25, deviceResolution.y / 1.25);
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(deviceResolution.x, deviceResolution.y, ResolutionPolicy::EXACT_FIT);
    auto const visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 const origin = Director::getInstance()->getVisibleOrigin();

    //Board
    Sprite* boardBackground = Sprite::create("Boards/Board_BG.png");
    boardBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(boardBackground, 100, 50);

    //Moves label
    auto movesLabel = Label::createWithTTF("Moves: 0", "fonts/Marker Felt.ttf", 50);
    if (movesLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        movesLabel->setPosition(Vec2(origin.x + movesLabel->getContentSize().width, origin.y + 40));

        // add the label as a child to this layer
        this->addChild(movesLabel, 34);
    }

    Board* board = Board::getInstance();
    board->initBoard(3, 3, "Boards/Board_1.png", this->getChildByTag(50), movesLabel);



    //MenuItems
    auto closeItem = MenuItemImage::create(
        "ui/exit.png",
        "ui/exit_pressed.png",
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

    playItem = MenuItemImage::create(
        "ui/PLAY.png",
        "ui/PLAY_pressed.png",
        CC_CALLBACK_1(MainMenu::menuPlayCallback, this));


    volumeItem = MenuItemImage::create(
        "ui/volume.png",
        "ui/mute.png",
        CC_CALLBACK_1(MainMenu::menuVolumeCallback, this));


    if (closeItem == nullptr || playItem == nullptr || volumeItem == nullptr)
    {
        problemLoading("UI sprites");
    }
    else
    {
        // create menu, it's an autorelease object
        auto menu = Menu::create(closeItem, NULL);
        menu->setPosition(Vec2(visibleSize.width / 2 + 200, visibleSize.height / 22));
        this->addChild(menu, 38);

        auto play = Menu::create(playItem, NULL);
        play->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 22));
        this->addChild(play, 39, "play");

        auto volume = Menu::create(volumeItem, NULL);
        volume->setPosition(Vec2(visibleSize.width / 2 - 200, visibleSize.height / 22));
        this->addChild(volume, 40, "volume");
    }


    //label
    auto label = Label::createWithTTF(":: Sliding Box Puzzle ::", "fonts/Marker Felt.ttf", 35);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 34);
    }

    // Background


    //version
    auto credits = Label::createWithTTF("By,\nSujit Paul", "fonts/Marker Felt.ttf", 20);
    if (credits == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        credits->setPosition(Vec2(1094, 37));
        credits->setVisible(true);

        // add the label as a child to this layer
        this->addChild(credits, 37);
    }

    //BGM
    playBGM();

    //update schedule
    this->scheduleUpdate();

    return true;
}

void MainMenu::update(float dt)
{
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
    clickID = audioInstance::play2d("sfx/click.mp3", false, 1.0f);

    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void MainMenu::menuPlayCallback(Ref* pSender)
{
    clickID = audioInstance::play2d("sfx/click.mp3", false, 1.0f);
    if (!isPlaying)
    {
        playItem->setNormalSpriteFrame(SpriteFrame::create("ui/LOCK.png", Rect(0, 0, 63, 66)));
    }
    else
    {
        playItem->setNormalSpriteFrame(SpriteFrame::create("ui/PLAY.png", Rect(0, 0, 63, 66)));
    }

}
void MainMenu::menuVolumeCallback(Ref* pSender)
{
    clickID = audioInstance::play2d("sfx/click.mp3", false, 1.0f);

    if (!isMuted)
    {
        volumeItem->setNormalSpriteFrame(SpriteFrame::create("ui/mute.png", Rect(0, 0, 50, 50)));
        stopBGM();
        isMuted = true;
    }
    else
    {
        volumeItem->setNormalSpriteFrame(SpriteFrame::create("ui/volume.png", Rect(0, 0, 50, 50)));
        playBGM();
        isMuted = false;
    }
}

void MainMenu::playBGM()
{
    bgmID = audioInstance::play2d("sfx/bgm.mp3", true, 0.1f);
}

void MainMenu::stopBGM()
{
    audioInstance::stop(bgmID);
}