//
//  ReversiPiece.h
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/18.
//
//

#ifndef __maji_kichi_kaishayamerutteyo__ReversiPiece__
#define __maji_kichi_kaishayamerutteyo__ReversiPiece__

#import "cocos2d.h"

typedef enum{
    ReversiPieceType_Black =1,
    ReversiPieceType_Red = 2,
    ReversiPieceType_Yellow,
    ReversiPieceType_Blue,
    ReversiPieceType_Max
}ReversiPieceType;

class ReversiPiece : public cocos2d::Sprite{

public:
    static ReversiPiece* create(ReversiPieceType _type, bool _reversi);
    void setPieseImage(ReversiPieceType _type, bool _reversi);
    
    ReversiPieceType Type;
    bool    IsReversi;
};

#endif /* defined(__maji_kichi_kaishayamerutteyo__ReversiPiece__) */
