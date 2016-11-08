//
//  PieceDealer.h
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/18.
//
// ピースを配布するクラス

#ifndef __maji_kichi_kaishayamerutteyo__PieceDealer__
#define __maji_kichi_kaishayamerutteyo__PieceDealer__

#import "cocos2d.h"
#import "ReversiPiece.h"
#import <vector>
#import "SquarePlace.h"

using namespace cocos2d;

class PieceDealer : public cocos2d::Node{
    
    const int MaxDealNum = 5;
    
    Layer* m_dispLayer;
    std::vector<ReversiPieceType> m_dealList;
    
    PieceDealer();
    void dispDealPiese();
    int lotteryType();
    Vector<SquareMenu*> m_squareMenuList;
    
    //deal
    void lotteryAndDealPiece(SquarePlace* _square);
    void shiftPiece();
    
    ReversiPiece* createPiece(ReversiPieceType _type, int _no);
public:
    static PieceDealer* getInstance();
    static void removeInstance();
    
    //IN BOARD
    void initBoardPiece();
    
    //NEXT
    void dispDealer( Node* parent);
    ReversiPieceType popoutDealType();
    ReversiPieceType getNextDealType();
    
    Vector<SquareMenu*> getMenus();
    void setMenus(SquareMenu* _menu);
    
    //explode
    void replenishing(Vector<SquarePlace*> _reslist);
    
    //delete
    void deleteAllMenus();
};


#endif /* defined(__maji_kichi_kaishayamerutteyo__PieceDealer__) */
