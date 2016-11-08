//
//  PieceDealer.cpp
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/18.
//
//

#import "PieceDealer.h"
#import "SquarePlace.h"
#import "GameScene.h"
#import "FixedValue.h"
static PieceDealer* instance;

PieceDealer* PieceDealer::getInstance(){
    if( instance == NULL ){
        instance = new PieceDealer();
    }
    return instance;
}

void PieceDealer::removeInstance(){
    if( instance->m_dispLayer ){
        instance->m_dispLayer->removeFromParent();
    }
    delete instance;
    instance = NULL;
}

PieceDealer::PieceDealer(){
    m_dispLayer = NULL;
    
    for (int i = 0; i < MaxDealNum; i ++) {
        int type = lotteryType();
        m_dealList.push_back((ReversiPieceType)type);
    }
}

int PieceDealer::lotteryType(){
    int type = rand()% 3;
//    type = MAX(type, ReversiPieceType_Black+1);
//    type = MIN(type, ReversiPieceType_Max-1);
    switch (type) {
        case 0:return ReversiPieceType_Red;
        case 1:return ReversiPieceType_Blue;
        case 2:return ReversiPieceType_Yellow;

    }
    
    return ReversiPieceType_Red;
}


void PieceDealer::dispDealer(cocos2d::Node *parent){
    if( m_dispLayer ){
        m_dispLayer->removeFromParent();
    }
    m_dispLayer = Layer::create();
    parent->addChild(m_dispLayer);
    
    dispDealPiese();
}

ReversiPieceType PieceDealer::popoutDealType(){
    ReversiPieceType _out = m_dealList.front();
    
    Vector<ReversiPieceType>::iterator it = m_dealList.begin();
    m_dealList.erase(it);
    
    int type = lotteryType();
    m_dealList.push_back((ReversiPieceType)type);
    
    ReversiPiece* piece = createPiece((ReversiPieceType)type, (int)m_dealList.size());
    m_dispLayer->addChild(piece);
    
    shiftPiece();
//    shiftPiece();
    
//    dispDealPiese();
    GameDatas::getInstance()->TurnNum --;
    
    return _out;
}

ReversiPieceType PieceDealer::getNextDealType(){
    ReversiPieceType _out = m_dealList.front();
    return _out;
}

Vector<SquareMenu*> PieceDealer::getMenus(){
    return m_squareMenuList;
}
void PieceDealer::setMenus(SquareMenu *_menu){
    m_squareMenuList.pushBack(_menu);
}

const int Tag_Const= 1;

//下に流れるピース予告生成やで
void PieceDealer::dispDealPiese(){
    if( !m_dispLayer ) return;

    m_dispLayer->removeAllChildren();
    
    Sprite* icon = Sprite::create("icon_piece_info.png");
    icon->setPosition(150, 290);
    m_dispLayer->addChild(icon, 2, Tag_Const);
    
    for (int i = 0; i < MaxDealNum; i ++) {
        ReversiPieceType _out = m_dealList[i];
         ReversiPiece* piece = createPiece(_out, i);
//        ReversiPiece* piece = ReversiPiece::create(_out, true);
//        if( i == 0 ){
//            piece->setPosition( 280, 300);  //目分量や！
//            piece->setScale(1.8);
//        }else{
//            piece->setPosition( 300 + (i*100) , 300);
//        }
        m_dispLayer->addChild(piece);
    }
}

void PieceDealer::shiftPiece(){
    auto childrem = m_dispLayer->getChildren();
    auto it = childrem.begin();
    int index = 0;
    float time = 0.5;
    while (it != childrem.end()) {
        ReversiPiece* child = dynamic_cast<ReversiPiece*>((*it));
        if( child ){
            child->stopAllActions();
            if( index == 0 ){
                child->runAction(Sequence::create(
                                                  ScaleTo::create(time, 0),
                                                  CCNodeUtil::createRemoveAction(),
                                                  NULL));
            }if( index == 1 ){
                child->runAction(Spawn::create(
                                               MoveTo::create(time, Vec2(280,300)),
                                               ScaleTo::create(time, 1.8),
                                               NULL));
            }else{
                child->runAction(MoveTo::create(time, Vec2(300 + ( (index-1) * 100 ), 300)));
            }
            index ++;
        }
        
        it ++;
    }
    
}

ReversiPiece* PieceDealer::createPiece(ReversiPieceType _type, int _no){
    ReversiPiece* piece = ReversiPiece::create(_type, true);
    if( _no == 0 ){
        piece->setPosition( 280, 300);  //目分量や！
        piece->setScale(1.8);
    }else{
        piece->setPosition( 300 + ( _no * 100 ) , 300);
    }
    return piece;
}

void PieceDealer::initBoardPiece(){
    //black
    for (int i = 0; i < m_squareMenuList.size(); i ++) {
        int blackNum = 6;
        SquareMenu* menu = m_squareMenuList.at(i);
        for( int i = 0 ; i < blackNum ; i++){
            int idx = rand()% ( menu->MaxHorizon * menu->MaxVertial -1);
            menu->SquareList.at(idx)->createPiece(ReversiPieceType_Black, false);
        }
    }
    
    //all
    for (int i = 0; i < m_squareMenuList.size(); i ++) {
        SquareMenu* menu = m_squareMenuList.at(i);
        for( int i = 0 ; i < menu->SquareList.size() ; i++){
            SquarePlace* _square = menu->SquareList.at(i);
            lotteryAndDealPiece(_square);
        }
    }
}

void PieceDealer::lotteryAndDealPiece(SquarePlace *_square){
    if( _square->getPieceType() != ReversiPieceType_Max ) return;
    
    int isColor = rand() % 2;   //50%は空白
    if( isColor ){
        int type = lotteryType();   //残り半分色抽選
        _square->createPiece((ReversiPieceType)type, false);
    }
}

void PieceDealer::replenishing(Vector<SquarePlace *> _reslist){
    for( int i = 0 ; i < _reslist.size() ; i++){
        SquarePlace* _square = _reslist.at(i);
        lotteryAndDealPiece(_square);
    }
    GameDatas::getInstance()->checkCanPutSquare();
}

void PieceDealer::deleteAllMenus(){
    for (int i = 0; i < m_squareMenuList.size(); i ++) {
        SquareMenu* menu = m_squareMenuList.at(i);
        menu->removeFromParentAndCleanup(true);
    }
    m_squareMenuList.clear();
}
