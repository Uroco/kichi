//
//  TitleScene.cpp
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/28.
//
//

#include "TitleScene.h"
#import "GameScene.h"
USING_NS_CC;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TitleScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TitleScene::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto sprite = Sprite::create("sam1.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);
    
    logo = Sprite::create("logo_tapstart.png");
    logo->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(logo, 0);
    
    logo->setAnchorPoint(Point(0.5, 1));
    logo->runAction(RepeatForever::create(Sequence::create(RotateTo::create(2.0f, 15.0f),
                                                           RotateTo::create(4.0f, -30.0f),
                                                                            NULL)));
    
    
    return true;
}
bool TitleScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if( pTouch->getID() != 0 ){
        return false;
    }
    return true;
}

void TitleScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto sprite = Sprite::create("sam2.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);
    
    logo->removeFromParentAndCleanup(true);
    
    Scene* next = GameScene::createScene();
    float duration = 2.0f;
    Scene* pScene = TransitionFade::create(duration, next, Color3B::WHITE);
    if( pScene ){
        Director::getInstance()->replaceScene(pScene);
    }
}