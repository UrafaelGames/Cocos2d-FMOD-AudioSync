#include "GameScene.h"

USING_NS_CC;

cocos2d::Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!cocos2d::Scene::init()) {
        return false;
    }

    beatDetector = new BeatDetector();
    if (!beatDetector->init("C:/projectscpp/FMODTest/Resources/audio/TMM43 - Ultimate Destruction.mp3", true)) {  
        CCLOG("Error: sound no load!");
        delete beatDetector;
        beatDetector = nullptr;
        return false;
    }

    beatDetector->setVolume(1.f);   

    sprite = cocos2d::Sprite::create("HelloWorld.png");
    sprite->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
    this->addChild(sprite);

    scheduleUpdate();
    return true;
}

void GameScene::update(float dt)
{
    if (!beatDetector) return;

    beatDetector->update();

    float intensity = beatDetector->getBeatIntensity();
    float scaleFactor = 0.6f + (intensity * 0.14f);  
    sprite->setScale(scaleFactor);
}

GameScene::~GameScene()
{
    if (beatDetector) {
        delete beatDetector;
        beatDetector = nullptr;
    }
}
