//
//  GameScene.h
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/22.
//
//

#ifndef __maji_kichi_kaishayamerutteyo__GameScene__
#define __maji_kichi_kaishayamerutteyo__GameScene__

#import "cocos2d.h"
using namespace cocos2d;

class NFNumberLayer;
class SquarePlace;

/*
 * ゲーム内のデータを司るクラス
    ここにアクセスすると、競馬の勝敗以外なら何でもわかる。
 */
class GameDatas{
    static GameDatas* instance;
    GameDatas();
    ~GameDatas(){}
    
    int m_deleteBlackNum;
public:
    static GameDatas* getInstance();
    static void removeInstance();

    static int MaxHorizon;
    static int MaxVertial;
    
    int Score;
    int TurnNum;
    int StageNum;
    
    void init();
    void start();
    void calcReversiScore(Vector<SquarePlace*> _list);
    void calcStackScore(int _num);
    void calcExplosionScore(Vector<SquarePlace*> _list);
    int calcClearBonus();
    int calcGameOverBonus();
    
    bool checkClear();
    bool checkLose();
    void endStage();

    void checkCanPutSquare();
};


/*
 *　言わずと知れたGameScene
 */
class GameState;
class GameScene : public cocos2d::Layer
{
    GameState* State;
//    bool m_isResult =false;
public:
    NFNumberLayer* ScoreNumLayer;
    NFNumberLayer* TurnNumLayer;
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void update(float delta);
    
    DrawNode* debug_draw;
    void drawTouchPoint(cocos2d::Touch *pTouch);
    
    void onClickSkill(cocos2d::Ref* _sender);
    void onClickReset(cocos2d::Ref* _sender);
    void onClickTitle(cocos2d::Ref* _sender);
    void createStage();
    
    CREATE_FUNC(GameScene);
    ~GameScene();
};

/*
 *  Game State
 */
class GameState : public cocos2d::Ref{
protected:
public:
    static GameScene* _gameScene;
//    GameState(){}
    ~GameState();
    virtual bool init();

    virtual bool begin() = 0;
    virtual bool excute(float delta) =0;
    virtual GameState* end()=0;
};

//ステージに入ったとき
class GameStateEnter : public GameState{
    cocos2d::Sprite* base;
    float m_timer;
public:
    virtual bool begin() override;
    virtual bool excute(float delta) override;
    virtual GameState* end() override;
    void onClick(cocos2d::Ref* _sender);
};

//ゲームプレイ中
class GameStatePlay : public GameState{
    
    int m_nowScore;
    int m_nowTurn;
    bool m_isClear;
public:
    virtual bool begin() override;
    virtual bool excute(float delta) override;
    virtual GameState* end() override;
};

//ゲームクリアリザルト
class GameStateResultClear : public GameState{
    bool isWait;
public:
    virtual bool begin() override;
    virtual bool excute(float delta) override;
    virtual GameState* end() override;
    
    void close(cocos2d::Ref* _sender);
};

//ゲームオーバーリザルト
class GameStateResultLose : public GameState{
    bool isWait;
public:
    virtual bool begin() override;
    virtual bool excute(float delta) override;
    virtual GameState* end() override;
    
    void close(cocos2d::Ref* _sender);
};

#endif /* defined(__maji_kichi_kaishayamerutteyo__GameScene__) */
