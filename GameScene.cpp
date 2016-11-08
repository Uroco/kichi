//
//  GameScene.cpp
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/22.
//
//

#import "GameScene.h"
#import "SquarePlace.h"
#import "PieceDealer.h"
#import <stdlib.h>
#import "FixedValue.h"
#import "NFNumberLayer.h"
#include <vector>
#import "CommonDefine.h"

#import "GameResultLayer.h"
using namespace cocos2d;

GameDatas* GameDatas::instance = NULL;
int GameDatas::MaxHorizon = 6;
int GameDatas::MaxVertial = 6;

GameDatas* GameDatas::getInstance(){
    if( instance == NULL ){
        instance = new GameDatas();
    }
    return instance;
}

void GameDatas::removeInstance(){
    delete instance;
}

GameDatas::GameDatas(){
//    Score = 0;
    init();
}

void GameDatas::init(){
    StageNum = 0;
    start();
}

void GameDatas::start(){
    Score = 0;
    m_deleteBlackNum = 0;
    TurnNum = 20;
}

void GameDatas::calcReversiScore(Vector<SquarePlace*> _list){
    int _addScore = 0;
    for( int i = 0 ; i < _list.size() ; i++){
        SquarePlace* _square = _list.at(i);
        if( _square->getPieceType() == ReversiPieceType_Black ){
            _addScore += 200;
            m_deleteBlackNum ++;
        }else if( _square->getPieceType() != ReversiPieceType_Max ){
            _addScore += 20;
        }
    }
    Score += _addScore;
}

void GameDatas::calcStackScore(int _num){
    Score +=( _num * 20 );
}

void GameDatas::calcExplosionScore(Vector<SquarePlace *> _list){
    int _addScore = 0;
    for( int i = 0 ; i < _list.size() ; i++){
        SquarePlace* _square = _list.at(i);
        if( _square->getPieceType() == ReversiPieceType_Black ){
            _addScore += 100;
            m_deleteBlackNum ++;
        }else if( _square->getPieceType() == ReversiPieceType_Max ){
            _addScore += 10;
        }
    }
    Score += _addScore;
}

int GameDatas::calcClearBonus(){
    Vector<SquareMenu*> _squareMenuList = PieceDealer::getInstance()->getMenus();
    int allStackNum = 0;
    int ret = 0;
    std::vector<int> colorlist(3);
    
    for (int i = 0; i < _squareMenuList.size(); i ++) {
        SquareMenu* menu = _squareMenuList.at(i);
        for( int i = 0 ; i < menu->SquareList.size() ; i++){
            SquarePlace* _square = menu->SquareList.at(i);
            colorlist[(int)_square->getPieceType()-ReversiPieceType_Red] ++;
            
            if( _square->getStackNum() > 1 ){   //stack bonus
                allStackNum += _square->getStackNum();
            }
        }
    }
    
    //color bonus
    for (int i = 0; i < colorlist.size(); i++) {
        int val = ( colorlist[i]/( MaxHorizon*MaxVertial ) )*100*1.2;
        ret += val;
    }
    
    
    ret += allStackNum * 20;
    
    return ret;
}

int GameDatas::calcGameOverBonus(){
    int ret = m_deleteBlackNum * 100;
    
    return ret;
}

bool GameDatas::checkClear(){
    Vector<SquareMenu*> _squareMenuList = PieceDealer::getInstance()->getMenus();
    for (int i = 0; i < _squareMenuList.size(); i ++) {
        SquareMenu* menu = _squareMenuList.at(i);
        for( int i = 0 ; i < menu->SquareList.size() ; i++){
            SquarePlace* _square = menu->SquareList.at(i);
            if( _square->getPieceType() == ReversiPieceType_Black ){
                return false;
            }
        }
    }
    return true;
}

bool GameDatas::checkLose(){
    if( TurnNum < 1 ){
        return true;
    }
    return false;
}

void GameDatas::endStage(){
    StageNum ++;
}

void GameDatas::checkCanPutSquare(){
    bool canPut = false;
   
    Vector<SquareMenu*> _squareMenuList = PieceDealer::getInstance()->getMenus();
    ReversiPieceType type = PieceDealer::getInstance()->getNextDealType();
    
    for (int i = 0; i < _squareMenuList.size(); i ++) {
        SquareMenu* menu = _squareMenuList.at(i);
        for( int i = 0 ; i < menu->SquareList.size() ; i++){
            SquarePlace* _square = menu->SquareList.at(i);
            if( _square->getPieceType() == ReversiPieceType_Max ){
                int cnt = _square->checkAcross(type, false);
                if( cnt > 0 ){
                    _square->setVisibleHere(true);
                    canPut = true;
                    continue;
                }
            }
            _square->setVisibleHere(false);
        }
    }
    
    if( !canPut ){
        for (int i = 0; i < _squareMenuList.size(); i ++) {
            SquareMenu* menu = _squareMenuList.at(i);
            for( int i = 0 ; i < menu->SquareList.size() ; i++){
                SquarePlace* _square = menu->SquareList.at(i);
                if( _square->checkCanPut() ){
                   _square->setVisibleHere(true);
                    canPut = true;
                }else{
                    _square->setVisibleHere(false);
                }
            }
        }
    }
    
    if( !canPut ){      //破棄
        PieceDealer::getInstance()->popoutDealType();
    }
}

/*
 * Game Scene
 */

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene::~GameScene(){
    State->release();
    PieceDealer::removeInstance();
    GameState::_gameScene = NULL;
}

//void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
//    CCLOG("uh");
//}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    initFixedValue();
    scheduleUpdate();
    
    auto sprite = Sprite::create("bg_game_01.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);

    auto skillmenu = Menu::create();
    skillmenu->setPosition(Vec2::ZERO);
    this->addChild(skillmenu, 1);
    for (int i = 0; i < 3; i ++) {
        char normalname[32];
        char pushname[32];
        sprintf(normalname, "btn_skil_%02d_00.png", i+1);
        sprintf(pushname, "btn_skil_%02d_01.png", i+1);
        auto skillbtn = MenuItemImage::create(
                                               normalname,
                                               pushname,
                                              this,
                                              menu_selector(GameScene::onClickSkill));
        Value val = Value(i+1);
        skillbtn->setUserData(&val);
        skillbtn->setPosition(Vec2(120 + (skillbtn->getContentSize().width + 20)*i,
                                    160));
        
        skillmenu->addChild(skillbtn);
    }
    
    auto uimenu = Menu::create();
    uimenu->setPosition(Vec2::ZERO);
    this->addChild(uimenu, 1);
    
    auto resetbtn = MenuItemImage::create(
                                          "btn_reset_00.png",
                                          "btn_reset_01.png",
                                          CC_CALLBACK_1(GameScene::onClickReset, this));
    resetbtn->setPosition(Vec2( 240, 60));
    uimenu->addChild(resetbtn);
    
    auto titlebtn = MenuItemImage::create("btn_go_title_00.png",
                                          "btn_go_title_01.png",
                                          CC_CALLBACK_1(GameScene::onClickTitle, this));
    titlebtn->setPosition(Vec2( 360, 60));
    uimenu->addChild(titlebtn);
    
    
    auto label = Label::createWithTTF("Good Luck.", "fonts/arial.ttf", 32);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
    auto label2 = Label::createWithTTF("SCORE.", "fonts/arial.ttf", 32);
    label2->setPosition(Vec2( 100,
                            origin.y + visibleSize.height - 50));
    label2->setColor(Color3B::BLACK);
    this->addChild(label2, 1);
    ScoreNumLayer = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignLeft, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    ScoreNumLayer->setPosition(Point( 100, origin.y + visibleSize.height - 100));
    this->addChild(ScoreNumLayer);
    
    auto label3 = Label::createWithTTF("TURN.", "fonts/arial.ttf", 32);
    label3->setPosition(Vec2(origin.x + visibleSize.width - 150,
                            origin.y + visibleSize.height - 50));
    label3->setColor(Color3B::BLACK);
    this->addChild(label3, 1);
    TurnNumLayer = NFNumberLayer::create(0, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignRight, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    TurnNumLayer->setPosition(Point(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 100));
    this->addChild(TurnNumLayer);
    
//    m_nowScore = GameDatas::getInstance()->Score;

    
    srand(time(0));
    
//    createStage();
    GameState::_gameScene = this;
    State = new GameStateEnter();
    State->begin();
    
    return true;
}

void GameScene::onClickSkill(cocos2d::Ref *_sender){
    auto val = (Value*)((Node*)_sender)->getUserData();
    CCLOG("push skill %d", val->asInt());
}

void GameScene::onClickReset(cocos2d::Ref* _sender){
    State->end();
    
    createStage();
    State = new GameStatePlay();
    State->begin();
}

#import "TitleScene.h"
void GameScene::onClickTitle(cocos2d::Ref* _sender){
    Scene* next = TitleScene::createScene();
    float duration = 2.0f;
    Scene* pScene = TransitionFade::create(duration, next, Color3B(233, 242, 201));
    if( pScene ){
        Director::getInstance()->replaceScene(pScene);
    }
}

void GameScene::drawTouchPoint(cocos2d::Touch *pTouch){
    debug_draw->clear();
    debug_draw->drawDot(pTouch->getLocation(), 50, Color4F::RED);
}

void GameScene::update(float delta){
    if( !State->excute(delta) ){
        State = State->end();
        State->begin();
    }
}

void GameScene::createStage(){
    PieceDealer::getInstance()->deleteAllMenus();
    GameDatas::getInstance()->start();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    SquareMenu* squareParent = SquareMenu::create();
    squareParent->setPosition(Vec2::ZERO);
    this->addChild(squareParent, 10);
    
    squareParent->MaxHorizon = GameDatas::getInstance()->MaxHorizon;
    squareParent->MaxVertial = GameDatas::getInstance()->MaxVertial;
    
    PieceDealer::getInstance()->setMenus(squareParent);
    PieceDealer::getInstance()->dispDealer(this);
    
    int index = 0;
    for( int y = 0 ; y < 6 ; y ++ ){
        for( int x = 0 ; x < 6 ; x ++ ){
            auto item = SquarePlace::create(index);
            item->setPosition(Point( 64 + (102 * x),
                                    visibleSize.height - 200 - (102 * y)));
            squareParent->addSquare(item);
            index ++;
        }
    }
    PieceDealer::getInstance()->initBoardPiece();
    GameDatas::getInstance()->checkCanPutSquare();
    
    ScoreNumLayer->setNum(GameDatas::getInstance()->Score);
    TurnNumLayer->setNum(GameDatas::getInstance()->TurnNum);
}

//STATE

GameScene* GameState::_gameScene = NULL;
bool GameState::init(){
    this->autorelease();
    return true;
}
GameState::~GameState(){
//    _gameScene->release();
//    _gameScene = NULL;
}

/*
 * GameStateEnter
 */
bool GameStateEnter::begin(){
    m_timer = 0;
    GameDatas::getInstance()->start();
    
    base = Sprite::create();
    _gameScene->addChild(base, 100);
    
    Menu* menu = Menu::create();
    base->addChild(menu);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* verticalFrame = Sprite::create("bg_pattern_rule.png");
    verticalFrame->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    verticalFrame->getTexture()->setTexParameters(&texParamsRepeatBoth);
    verticalFrame->setAnchorPoint(Point(0, 0));
    verticalFrame->setPosition(Point(0, 0));
    
    auto btn = MenuItemSprite::create(verticalFrame, verticalFrame);
    btn->setCallback(CC_CALLBACK_1(GameStateEnter::onClick, this));
    menu->addChild(btn);
    
//    base->addChild(verticalFrame, 100);
    
    auto label = Label::createWithTTF("STAGE", "fonts/arial.ttf", 32);
    label->setPosition(Point(200, visibleSize.height*0.5 + 100));
    label->setColor(Color3B::BLACK);
    base->addChild(label, 100);
    
    NFNumberLayer* _scoreNum = NFNumberLayer::create(GameDatas::getInstance()->StageNum+1, "num_distance_l_01.png", 70.0, 88.0, -16, 44, kNumAlignLeft, Point(0.5, 0.5), 1, NULL, 0, NULL, 0);
    _scoreNum->setPosition(Point(visibleSize.width*0.5 + 80, visibleSize.height*0.5 + 100));
    base->addChild(_scoreNum, 100);
    
    char ruletext[1024];
    sprintf(ruletext,"黒のコマを%dターン以内に全て消そう！", GameDatas::getInstance()->TurnNum);
    auto label1 = Label::createWithTTF(ruletext, "fonts/nicomoji-plus.ttf", 32);
    label1->setPosition(Point(visibleSize.width*0.5, visibleSize.height*0.5 - 80));
    label1->setColor(Color3B::BLACK);
    base->addChild(label1, 100);
    
    _gameScene->createStage();
    
//    base->runAction(Sequence::create(DelayTime::create(1.0f),
//                                     FadeOut::create(3.0f),
//                                     NULL));
    
    return true;
}

bool GameStateEnter::excute(float delta){
    if( m_timer > 3.0f ){
        return false;
    }
    m_timer += delta;
    return true;
}

GameState* GameStateEnter::end(){
    base->removeFromParent();
    
    GameStatePlay* state = new GameStatePlay();
    delete this;
    return state;
}

void GameStateEnter::onClick(cocos2d::Ref *_sender){
    m_timer = 10;
}


/*
 * GameStatePlay
 */
bool GameStatePlay::begin(){
    
    //create stage
    
    m_nowScore = GameDatas::getInstance()->Score;
    _gameScene->ScoreNumLayer->setNum(m_nowScore);
    
    m_nowTurn = GameDatas::getInstance()->TurnNum;
    _gameScene->TurnNumLayer->setNum(m_nowTurn);
    
    m_isClear =false;
    return true;
}

bool GameStatePlay::excute(float delta){
    if( m_nowScore != GameDatas::getInstance()->Score ){
        m_nowScore = GameDatas::getInstance()->Score;
        _gameScene->ScoreNumLayer->setNum(m_nowScore);
    }
    if( m_nowTurn != GameDatas::getInstance()->TurnNum ){
        m_nowTurn = GameDatas::getInstance()->TurnNum;
        _gameScene->TurnNumLayer->setNum(m_nowTurn);
    }
    
    if( GameDatas::getInstance()->checkClear() ){
        m_isClear = true;
        return false;
    }
    
    if( GameDatas::getInstance()->checkLose() ){
        return false;
    }
    return true;
}

GameState* GameStatePlay::end(){
    GameState* state;
    if( m_isClear ){
        state = new GameStateResultClear();
    }else{
        state = new GameStateResultLose();
    }
    
    delete this;
    return state;
}

/*
 *  CLEAR
 */
bool GameStateResultClear::begin(){
    isWait = true;

    GameResultLayer* layer = GameResultLayer::create(true);
    _gameScene->addChild(layer, 50);
    layer->setCloseListener(this, menu_selector(GameStateResultClear::close));
    
    return true;
}

bool GameStateResultClear::excute(float delta){
    return isWait;
}

GameState* GameStateResultClear::end(){
    GameStateEnter* state = new GameStateEnter();
    delete this;
    return state;
}

void GameStateResultClear::close(cocos2d::Ref *_sender){
    GameDatas::getInstance()->endStage();
    isWait = false;
}

/*
 *  GAME OVER
 */
bool GameStateResultLose::begin(){
    isWait = true;
    
    GameResultLayer* layer = GameResultLayer::create(false);
    _gameScene->addChild(layer, 50);
    layer->setCloseListener(this, menu_selector(GameStateResultClear::close));
    
    return true;
}

bool GameStateResultLose::excute(float delta){
    return isWait;
}

GameState* GameStateResultLose::end(){
    _gameScene->createStage();  //仕切り直し
    GameStatePlay* state = new GameStatePlay();
    delete this;
    return state;
}

void GameStateResultLose::close(cocos2d::Ref *_sender){
    isWait = false;
}