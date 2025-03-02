#pragma once

#include "cocos2d.h"
#include "BeatDetector.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual ~GameScene();
    virtual bool init();
    void update(float dt) override;
    CREATE_FUNC(GameScene);

private:
    BeatDetector* beatDetector;
    cocos2d::Sprite* sprite;
};
