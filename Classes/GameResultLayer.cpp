//
//  GameResultLayer.cpp
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/27.
//
//

#import "GameResultLayer.h"
#import "NFNumberLayer.h"
#import "GameScene.h"
#import "FixedValue.h"
#import "CommonDefine.h"
using namespace cocos2d;

GameResultLayer* GameResultLayer::create(bool isClear)
{
    auto layer = new GameResultLayer();
    layer->autorelease();
    layer->isClear = isClear;
    layer->init();
    
    return layer;
}

bool GameResultLayer::init(){
    Layer::init();
    _callback = NULL;

//    LayerGradient* variable_grad_bg_000000_000000 = LayerGradient::create(Color4B(0x00, 0x00, 0x00, 255), Color4B(0x00, 0x00, 0x00, 255));
//    variable_grad_bg_000000_000000->setContentSize(Size(640.0, 960.0 + (visibleSize.height - 960)));
//    variable_grad_bg_000000_000000->setAnchorPoint(Point(0.0, 0.0));
//    variable_grad_bg_000000_000000->setPosition(Point(0.0, 0.0));
//    variable_grad_bg_000000_000000->setOpacity(153.4);
//    this->addChild(variable_grad_bg_000000_000000);
    

    if( isClear ){
        initClear();
    }else{
        initGameOver();
    }
    
    
    
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameResultLayer::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameResultLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    
    return true;
}

void GameResultLayer::initClear(){
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* verticalFrame = Sprite::create("bg_pattern_clear.png");
    verticalFrame->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    verticalFrame->getTexture()->setTexParameters(&texParamsRepeatBoth);
    verticalFrame->setAnchorPoint(Point(0, 0));
    verticalFrame->setPosition(Point(0, 0));
    this->addChild(verticalFrame);
    
    Sprite* logo = Sprite::create("logo_clear.png");
    logo->setPosition(Point( visibleSize.width * 0.5, visibleSize.height - 200 ));
    this->addChild(logo);
    
    auto label = Label::createWithTTF("SCORE", "fonts/arial.ttf", 32);
    label->setPosition(Point(200, visibleSize.height*0.5 + 100));
    label->setColor(Color3B::BLACK);
    this->addChild(label, 1);
    
    NFNumberLayer* _scoreNum = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignRight, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    _scoreNum->setPosition(Point(visibleSize.width - 120, visibleSize.height*0.5 + 100));
    this->addChild(_scoreNum);
    
    auto label2 = Label::createWithTTF("BONUS", "fonts/arial.ttf", 32);
    label2->setPosition(Point(200, visibleSize.height*0.5));
    label2->setColor(Color3B::BLACK);
    this->addChild(label2, 1);
    
    NFNumberLayer* _scoreNum2 = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignRight, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    _scoreNum2->setPosition(Point(visibleSize.width - 120, visibleSize.height*0.5));
    this->addChild(_scoreNum2);
    
    auto label3 = Label::createWithTTF("TOTAL", "fonts/arial.ttf", 32);
    label3->setPosition(Point(200, visibleSize.height*0.5 - 150));
    label3->setColor(Color3B::BLACK);
    this->addChild(label3, 1);
    
    NFNumberLayer* _scoreNum3 = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignRight, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    _scoreNum3->setPosition(Point(visibleSize.width - 120, visibleSize.height*0.5 - 150));
    this->addChild(_scoreNum3);
    
    int bonus = GameDatas::getInstance()->calcClearBonus();
    int score = GameDatas::getInstance()->Score;
    _scoreNum->setNum(score);
    _scoreNum2->setNum(bonus);
    _scoreNum3->setNum(score + bonus);
}

void GameResultLayer::initGameOver(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* verticalFrame = Sprite::create("bg_pattern_gameover.png");
    verticalFrame->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    verticalFrame->getTexture()->setTexParameters(&texParamsRepeatBoth);
    verticalFrame->setAnchorPoint(Point(0, 0));
    verticalFrame->setPosition(Point(0, 0));
    this->addChild(verticalFrame);
    
    Sprite* logo = Sprite::create("logo_gameover.png");
    logo->setPosition(Point( visibleSize.width * 0.5, visibleSize.height - 200 ));
    this->addChild(logo);
    
    auto label = Label::createWithTTF("SCORE", "fonts/arial.ttf", 32);
    label->setPosition(Point(200, visibleSize.height*0.5 + 100));
    label->setColor(Color3B::BLACK);
    this->addChild(label, 1);
    
    NFNumberLayer* _scoreNum = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignRight, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    _scoreNum->setPosition(Point(visibleSize.width - 120, visibleSize.height*0.5 + 100));
    this->addChild(_scoreNum);
    
    auto label2 = Label::createWithTTF("BONUS", "fonts/arial.ttf", 32);
    label2->setPosition(Point(200, visibleSize.height*0.5));
    label2->setColor(Color3B::BLACK);
    this->addChild(label2, 1);
    
    NFNumberLayer* _scoreNum2 = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignRight, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    _scoreNum2->setPosition(Point(visibleSize.width - 120, visibleSize.height*0.5));
    this->addChild(_scoreNum2);
    
    auto label3 = Label::createWithTTF("TOTAL", "fonts/arial.ttf", 32);
    label3->setPosition(Point(200, visibleSize.height*0.5 - 150));
    label3->setColor(Color3B::BLACK);
    this->addChild(label3, 1);
    
    NFNumberLayer* _scoreNum3 = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignRight, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    _scoreNum3->setPosition(Point(visibleSize.width - 120, visibleSize.height*0.5 - 150));
    this->addChild(_scoreNum3);
    
    int bonus = GameDatas::getInstance()->calcGameOverBonus();
    int score = GameDatas::getInstance()->Score;
    _scoreNum->setNum(score);
    _scoreNum2->setNum(bonus);
    _scoreNum3->setNum(score + bonus);
}

bool GameResultLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if( pTouch->getID() != 0 ){
        return false;
    }
    return true;
}

//void GameResultLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
//    //    if( g_isDebug ){
//    //        drawTouchPoint(pTouch);
//    //    }
//}

void GameResultLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    //    if( g_isDebug ){
    //        drawTouchPoint(pTouch);
    //    }
    
    if( _callback ){
        _callback(this);
    }
    removeFromParentAndCleanup(true);
}

void GameResultLayer::setCloseListener(cocos2d::Ref *target, SEL_MenuHandler selector){
    _callback = std::bind(selector,target, std::placeholders::_1);
}