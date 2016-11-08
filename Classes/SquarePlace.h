//
//  SquarePiece.h
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/18.
//
// マス目一個分クラス

#ifndef __maji_kichi_kaishayamerutteyo__SquarePiece__
#define __maji_kichi_kaishayamerutteyo__SquarePiece__

#import "cocos2d.h"
#import "ReversiPiece.h"

using namespace cocos2d;

enum StackDirection{
    StackDirection_Vertex_Up = 1,
    StackDirection_Vertex_Down,
    StackDirection_Horizon_R,
    StackDirection_Horizon_L,
    StackDirection_Slash_R_Up,
    StackDirection_Slash_R_Down,
    StackDirection_Slash_L_Up,
    StackDirection_Slash_L_Down,
    StackDirection_Max
};

const int Left = 1;
const int Right = 2;
const int UP = 3;
const int Down = 4;

/*
 * SquareMenu
    SquarePlace をこの中に入れると、オセロのマスとして動き出す。
 */
class SquarePlace;
class SquareMenu: public Menu{
    enum TouchState{
        TouchState_Tap,
        TouchState_Drag
    };
    
    TouchState m_touchState;
    StackDirection m_stackDirection;
    bool m_isDragging;
    SquarePlace* m_startingPlace;
    Vector<SquarePlace*> m_stackList;
    DrawNode* debug_draw;
    void drawTouchPoint(cocos2d::Touch *pTouch);
    
    virtual bool init();
    bool isPossibleDrag(SquarePlace *child);
    void stackDragSquare(SquarePlace *child);
    void runStackSquare(SquarePlace *endSquare);
    int calcStackCount();
    bool isSelectedDrag(int index);
    
    StackDirection checkStackDirection(SquarePlace* square);
public:
    static SquareMenu* create();
    void addSquare(SquarePlace* _square);
    
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchEnded(Touch* touch, Event* event) override;
    virtual void onTouchMoved(Touch* touch, Event* event) override;
    
    Vector<SquarePlace*> SquareList;
    int MaxHorizon;
    int MaxVertial;
};

/*
 * SquarePlace
 */

class SquarePlace : public MenuItemSprite{
    friend SquareMenu;
protected:
    int m_index;
    int m_count;
//    bool m_isSelectedDrag;
//    ReversiPieceType m_pieceType;
    StackDirection m_stackDirection;
    Size SquareSize;
    SquareMenu* m_parent;
    ReversiPiece* Piece;
    Sprite* m_arrow;
    Label* m_debugLabel;
    Label* m_countLabel;
    Sprite* m_canPut;
    
    Vector<SquarePlace*> m_replenishingList;    //補充してもらうマス
    
    void onTap();
    
    SquarePlace();
    
    bool isTouchPiece( Vec2 touchPoint);
    bool isTouchCenter( Vec2 touchPoint );
    void setVisibleCount(bool st);
    void setVisibleArrow(bool st);
    
    bool init(int _index);
    bool isBetweenColor( ReversiPieceType _type, int _xIncrease, int _yIncrease, bool _isSeed, Vector<SquarePlace*>* _reslist );
    void reversiPiece(Vector<SquarePlace*> _reslist);
    SquarePlace* getSquarePlaceFromSelf(int _xIncrease, int _yIncrease);
    bool checkNext(int _xIncrease, int _yIncrease);

    int isBorder();
    int isBorderHorizon();
    int isBorderVertex();
    bool isReversi();
    
    //stack
    void setStack(int cnt, StackDirection _direction);
    float calcArrowAngle(StackDirection _direction);
    
    //explode
    void runExplode();
    void checkExplode( int _xIncrease, int _yIncrease, bool _seed, Vector<SquarePlace*>* _reslist);
    void explode();
    void afterReplenishing();
    
    //fire beam effect
    void createExplosionAnimation();
    
    //reversi
    
    //debug
    DrawNode* debug_draw;
    void drawHitArea(cocos2d::Vec2 point, Rect rect);
public:
    virtual ~SquarePlace(){};
    static SquarePlace* create(int _index);
  
    void createPiece(ReversiPieceType _type, bool _reversi);
    void deletePiece(bool cleanup);
    bool checkAcross(ReversiPieceType _type, bool _runReversi);
    bool checkCanPut();   //隣接ますをチェックし、黒こま、手ゴマと隣接していたらtrueを返す
    
    ReversiPieceType getPieceType();
    int getStackNum();
    void setVisibleHere(bool st);  //ここに置けるてきな
    
    virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
};

#endif /* defined(__maji_kichi_kaishayamerutteyo__SquarePiece__) */
