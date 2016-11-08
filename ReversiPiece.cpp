
//
//  ReversiPiece.cpp
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/18.
//
//

#import "ReversiPiece.h"

using namespace cocos2d;
ReversiPiece* ReversiPiece::create(ReversiPieceType _type, bool _reversi){
    ReversiPiece* piece = new ReversiPiece();
    piece->autorelease();
    piece->setPieseImage(_type, _reversi);
    piece->Type = _type;
    piece->IsReversi = _reversi;
    return piece;
}

void ReversiPiece::setPieseImage(ReversiPieceType _type, bool _reversi){
    Sprite::init();
    char filename[32];
    sprintf(filename, "piece_%02d_%02d.png", (int)_type, (int)_reversi);
    
//    setNormalImage(bg);
//    initWithCallback(this, callfunc_selector(ReversiPiece::onPush));
//    switch (_type) {
//        case Black: filename = ".png"; break;
//        case Red: filename = ".png"; break;
//        case Yellow: filename = ".png"; break;
//        case Blue: filename = ".png"; break;
//        default:
//            return;
//    }
  
//    this->set
    this->setTexture(filename);
}
