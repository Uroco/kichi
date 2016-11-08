//
//  SquarePiece.cpp
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/18.
//
//

#import "SquarePlace.h"
#import "PieceDealer.h"
#import "FixedValue.h"
#import "GameScene.h"

SquareMenu* SquareMenu::create(){
    SquareMenu* menu = new SquareMenu();
    
    if (menu && menu->init())
    {
        menu->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(menu);
    }
    
    return menu;
}

bool SquareMenu::init(){
    if( Menu::init() ){
        m_startingPlace = NULL;
        m_isDragging = false;
        m_touchState = SquareMenu::TouchState_Tap;
        m_stackDirection = StackDirection_Max;
//        Vector<MenuItem*> items;
//        Menu::initWithArray(items);
        debug_draw = NULL;
        return true;
    }
    return false;
}

void SquareMenu::addSquare(SquarePlace *child){
    Menu::addChild(child);
    SquareList.insert(child->m_index, child);
    child->m_parent = this;
}

StackDirection SquareMenu::checkStackDirection(SquarePlace* square){
    if( m_stackList.size() < 1 ) return StackDirection_Max;
    
    auto it = m_stackList.end();
    SquarePlace* last = (SquarePlace*) *(--it);
    
    StackDirection direction = StackDirection_Max;
    
    if( square->m_index == last->m_index - MaxVertial ){
        direction = StackDirection_Vertex_Up;
    }
    else if( square->m_index == last->m_index + MaxVertial ){
        direction = StackDirection_Vertex_Down;
    }
    else if( square->m_index == last->m_index + 1 ){
        direction = StackDirection_Horizon_R;
    }
    else if( square->m_index == last->m_index - 1 ){
        direction = StackDirection_Horizon_L;
    }
    else if( square->m_index == last->m_index - MaxVertial + 1 ){
        direction = StackDirection_Slash_R_Up;
    }
    else if( square->m_index == last->m_index + MaxVertial - 1 ){
        direction = StackDirection_Slash_R_Down;
    }
    else if( square->m_index == last->m_index - MaxVertial - 1 ){
        direction = StackDirection_Slash_L_Up;
    }
    else if( square->m_index == last->m_index + MaxVertial + 1 ){
        direction = StackDirection_Slash_L_Down;
    }

    return direction;   //one direction.
}

bool SquareMenu::isPossibleDrag(SquarePlace *child){
    if(child->getPieceType() != ReversiPieceType_Max
       && m_startingPlace
       && m_startingPlace->getPieceType() != ReversiPieceType_Max
       && m_startingPlace->getPieceType() == child->getPieceType()
       && child->Piece
       && child->Piece->IsReversi)
    {
        if( m_stackList.size() < 2 ){
            if( m_stackDirection == StackDirection_Max
               || m_stackDirection == checkStackDirection(child)){
                return true;
            }
        }else{
            if(  m_stackDirection == checkStackDirection(child) ){
                return true;
            }
        }
    }
    return false;
}

void SquareMenu::stackDragSquare(SquarePlace *child){
//    child->m_isSelectedDrag = true;
    m_stackDirection = checkStackDirection(child);
    m_stackList.pushBack(child);
    CCLOG("stack up %d, Direct[%d]", child->m_index, m_stackDirection);
}

void SquareMenu::runStackSquare(SquarePlace *endSquare){
    if (m_touchState != TouchState_Drag) return;
    if( !m_isDragging ) return;
    
    
    bool canStack = false;
    if( m_stackList.size() > 2 ) //２個じゃ以上やないと重ねられへんで
    {
        canStack = true;

    }
    
    if( canStack ){
        GameDatas::getInstance()->calcStackScore((int)m_stackList.size());
        endSquare->setStack( calcStackCount(), m_stackDirection);
    }
    
    //統合されたコマをマスから消すやで！
    for (int i = 0; i < m_stackList.size(); i ++) {
        SquarePlace* square = m_stackList.at(i);
        if( canStack ){
            if( square != endSquare ) {
                square->deletePiece(true);
                square->m_count = 0;
                CCLOG("run stackup %d", square->m_index);
            }
        }
//        square->m_isSelectedDrag = false;
    }

    GameDatas::getInstance()->checkCanPutSquare();
}

int SquareMenu::calcStackCount(){
    int cnt = 0;
    for (int i = 0; i < m_stackList.size(); i ++) {
        SquarePlace* square = m_stackList.at(i);
        if( square != NULL ) {
            cnt += square->m_count;
        }
    }
    return cnt;
}
void SquareMenu::drawTouchPoint(cocos2d::Touch *pTouch){
    if( !g_isDebug ) return;
    if( !debug_draw ){
        debug_draw = DrawNode::create();
        debug_draw->setPosition(0, 0);
        debug_draw->setAnchorPoint(Point(0.5, 0.5));
        this->getParent()->addChild(debug_draw, 100);
    }
    debug_draw->clear();
    debug_draw->drawDot(pTouch->getLocation(), 30, Color4F::RED);
}

bool SquareMenu::onTouchBegan(Touch* touch, Event* event)
{
    if (_state != Menu::State::WAITING || ! _visible || !_enabled)
    {
        return false;
    }
    
    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    drawTouchPoint(touch);
    
    m_startingPlace = NULL;
    
    _selectedItem = this->getItemForTouch(touch);
    if (_selectedItem)
    {
        _state = Menu::State::TRACKING_TOUCH;
        m_stackDirection = StackDirection_Max;
        
        SquarePlace* square = (SquarePlace*)_selectedItem;
        if(square->isTouchPiece(touch->getLocation())){  //コマをお触りしていたら
            CCLOG("START");
            m_startingPlace = (SquarePlace*)_selectedItem;  //始点を保存しておく
            stackDragSquare(m_startingPlace);
        }
        
        _selectedItem->selected();
        return true;
    }
    
    return false;
}

bool SquareMenu::isSelectedDrag(int index){
    if( m_stackList.size() < 1 ) return false;
    for (int i = 0; i < m_stackList.size(); i ++) {
        SquarePlace* square = m_stackList.at(i);
        if( square && square->m_index == index ){
            return true;
        }
    }
    return false;
}

void SquareMenu::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
    
    //ドラッグ決定やで(＾ω＾）
    if( m_touchState != TouchState_Drag ){
        m_touchState = TouchState_Drag;
        m_isDragging = true;
    }
    drawTouchPoint(touch);
    
    MenuItem *currentItem = this->getItemForTouch(touch);
    
    if( m_isDragging && currentItem){
        SquarePlace* square = (SquarePlace*)currentItem;
        if( !isSelectedDrag(square->m_index) )
        {
            //コマをお触りしていたら
            bool isTouch  = square->isTouchPiece(touch->getLocation());
            if( !isTouch ) isTouch = square->isTouchCenter(touch->getLocation());
            if( isTouch ){
                if( isPossibleDrag(square) ){
                    stackDragSquare(square);
                }else{
//                    for (int i = 0; i < m_stackList.size(); i ++) {
//                        SquarePlace* square = m_stackList.at(i);
////                        square->m_isSelectedDrag = false;
//                    }
                    m_isDragging = false;
                    CCLOG("STOP.");
                }
            }
        }
    }
    
    if (currentItem != _selectedItem)
    {
        
        if (_selectedItem)
        {
            _selectedItem->unselected();
        }
        _selectedItem = currentItem;
        if (_selectedItem)
        {
            _selectedItem->selected();
        }
    }
}

void SquareMenu::onTouchEnded(Touch* touch, Event* event)
{
    CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
    drawTouchPoint(touch);

    this->retain();
    if (_selectedItem)
    {
        _selectedItem->unselected();
        
        switch (m_touchState) {
            case TouchState_Tap:
                _selectedItem->activate();
                break;
                
            case TouchState_Drag:
                SquarePlace* square = (SquarePlace*)_selectedItem;
                if(square->isReversi() &&
                   square->isTouchPiece(touch->getLocation())){ //コマをお触りしていたら
                    runStackSquare((SquarePlace*)_selectedItem);
                    CCLOG("END");
                }
                break;
        }

    }
    _state = Menu::State::WAITING;
    this->release();
    
    m_touchState = TouchState_Tap;
    m_startingPlace = NULL;
    m_isDragging = false;
    m_stackList.clear();
    m_stackDirection = StackDirection_Max;
}

/*
 *  SquarePlace
 *      brief@ マス一個分のクラスざます
 */
SquarePlace::SquarePlace(){
    Piece = NULL;
//    isPiece = false;
//    m_pieceType = ReversiPieceType_Max;
    m_index = -1;
    m_count = 0;
    m_countLabel = NULL;
//    m_isSelectedDrag = false;
}

SquarePlace* SquarePlace::create(int index){
    SquarePlace* square = new SquarePlace();
    square->init(index);
    square->setAnchorPoint(Point(0.5, 0.5));
    return square;
}

bool SquarePlace::init(int _index){
    MenuItem::init();
    
    Sprite* bg = Sprite::create("square.png");
    bg->setAnchorPoint(Point(0.5, 0.5));
    bg->setOpacity(100);
    this->initWithNormalSprite(bg, bg, bg, callfunc_selector(SquarePlace::onTap));
    this->autorelease();
    
    SquareSize= Size( bg->getTextureRect().getMaxX() , bg->getTextureRect().getMaxY());
    m_index = _index;
    m_parent = NULL;
    m_arrow = NULL;
    m_stackDirection = StackDirection_Max;
    debug_draw =NULL;
    m_canPut = NULL;
    
    if( g_isDebug ){
        char str[32];
        sprintf(str, "%02d", m_index);
        m_debugLabel = Label::createWithTTF(str, "fonts/arial.ttf", 24);
        m_debugLabel->setColor(cocos2d::Color3B::BLACK);
        m_debugLabel->setPosition(SquareSize.width / 2, SquareSize.height / 2);
        this->addChild(m_debugLabel);
    }
    return true;
}

void SquarePlace::createPiece(ReversiPieceType _type, bool _reversi){
    
    if( Piece ){
        if( Piece->IsReversi && Piece->Type == _type ) return;
        
        if( getPieceType() == ReversiPieceType_Black  ){
            deletePiece(true);  //新規
            m_count = 1;
        }else{
            deletePiece(false); //入れ替え
        }
    }else{
        m_count = 1;
    }
    Piece = ReversiPiece::create(_type, _reversi);
    Piece->setPosition(SquareSize.width / 2, SquareSize.height / 2);
    Piece->setAnchorPoint(Point(0.5, 0.5));
    this->addChild(Piece);
    
//    m_pieceType = _type;
}

void SquarePlace::onTap(){
    if( !Piece ){
        if( !m_canPut ) return; //置ける確定じゃないと置けない

        ReversiPieceType _type = PieceDealer::getInstance()->popoutDealType();
        createPiece(_type, true);
        checkAcross(getPieceType(), true);
        
        GameDatas::getInstance()->checkCanPutSquare();
    }else{
        if( m_count > 1 ){
            runExplode();
        }
    }
//    m_isSelectedDrag = false;
}



void SquarePlace::deletePiece(bool cleanup){
    if( Piece ){
        Piece->removeFromParent();
        Piece = NULL;
    }
//    getPieceType() = ReversiPieceType_Max;
    
    if( cleanup ){
        m_count = 0;
        setVisibleCount(false);
        setVisibleArrow(false);
    }
    
    if( debug_draw ){
        debug_draw->clear();
    }
}

void SquarePlace::setVisibleCount(bool st){
    if( m_countLabel != NULL ){
        m_countLabel->removeFromParent();
        m_countLabel = NULL;
    }
    
    if(st){
        char str[32];
        sprintf(str, "%02d", m_count);
        m_countLabel = Label::createWithTTF(str, "fonts/arial.ttf", 32);
        m_countLabel->setColor(cocos2d::Color3B::BLUE);
        m_countLabel->setPosition(SquareSize.width - 20, SquareSize.height - 20);
        this->addChild(m_countLabel, 5);
    }
}
void SquarePlace::setVisibleArrow(bool st){
    if( m_arrow != NULL ){
        m_arrow->removeFromParent();
        m_arrow = NULL;
    }
    
    if(st){
        if( !m_arrow ){
            m_arrow = Sprite::create("icon_arrow.png");
            m_arrow->setPosition(SquareSize.width*0.5, SquareSize.height*0.5);
            this->addChild(m_arrow, 5);
        }
        float angle = calcArrowAngle(m_stackDirection);
        m_arrow->setRotation(angle);
    }
}

void SquarePlace::setVisibleHere(bool st){
    if( m_canPut != NULL ){
        m_canPut->removeFromParent();
        m_canPut = NULL;
    }
    
    if(st){
        if( !m_canPut ){
            m_canPut = Sprite::create("icon_here.png");
            m_canPut->setPosition(SquareSize.width*0.5, SquareSize.height*0.5);
            this->addChild(m_canPut, 5);
        }
    }
}

void SquarePlace::drawHitArea(cocos2d::Vec2 point, Rect rect){
    if( !g_isDebug ) return;
    if( !debug_draw ){
        debug_draw = DrawNode::create();
        debug_draw->setAnchorPoint(Point(0.5, 0.5));
        debug_draw->setPosition(0, 0);
        this->addChild(debug_draw, 100);
    }
    debug_draw->clear();
    Point poss[] ={
        Point(rect.getMinX(), rect.getMinY()),
        Point(rect.getMinX(), rect.getMaxY()),
        Point(rect.getMaxX(), rect.getMaxY()),
        Point(rect.getMaxX(), rect.getMinY()),
    };
    debug_draw->drawPolygon(poss, 4, Color4F::YELLOW, 1, Color4F::GREEN);
    
    debug_draw->drawDot(point, 10, Color4F::MAGENTA);
}

bool SquarePlace::isTouchPiece(cocos2d::Vec2 touchPoint ){
    if( Piece ){
        Vec2 point = touchPoint;
        point = point - (getPosition() + getParent()->getPosition() ) + Piece->getPosition();
        Rect rect = Piece->getBoundingBox();
        drawHitArea(point, rect);
        if( rect.containsPoint(point) ){
            return true;
        }
    }
    return false;
}

bool SquarePlace::isTouchCenter(cocos2d::Vec2 touchPoint){
    Vec2 point = touchPoint;
    point = point - (getPosition() + getParent()->getPosition());
    Rect rect = Rect( 0, 0, SquareSize.width*0.5, SquareSize.height * 0.5);
    
    drawHitArea(point, rect);
    
    if( rect.containsPoint(point) ){
        CCLOG("CENTER!! %d", m_index);
        return true;
    }
    return false;
}

bool SquarePlace::checkAcross(ReversiPieceType _type, bool _runReversi){
    if( _type == ReversiPieceType_Max ) return false;
    int res = 0;
    Vector<SquarePlace*> _list;
    
    CCLOG("-----------BETWEEEn S");
    res += isBetweenColor(_type, 1, 0, true, &_list);    //右列を調べる
    CCLOG("R %d", res);
    res += isBetweenColor(_type, -1, 0, true, &_list);   //左列
    CCLOG("L %d", res);
    if( _runReversi )reversiPiece(_list);
    
    
    _list.clear();
    res += isBetweenColor(_type, 0, 1,true, &_list);     //縦下
    CCLOG("DOWN %d", res);
    res += isBetweenColor(_type, 0, -1, true, &_list);   //縦上
    CCLOG("UP %d", res);
    if( _runReversi )reversiPiece(_list);
    
    _list.clear();
    res += isBetweenColor(_type, 1, 1,true, &_list);    //斜め右下
    CCLOG("sR Down %d", res);
    res += isBetweenColor(_type, 1, -1,true, &_list);   //斜め右上
    CCLOG("sR Up %d", res);
    if( _runReversi )reversiPiece(_list);
    
    _list.clear();
    res += isBetweenColor(_type, -1, 1,true, &_list);  //斜め左下
    CCLOG("sL Down %d", res);
    res += isBetweenColor(_type, -1, -1, true, &_list);  //斜め左上
    CCLOG("sL Up %d", res);
    if( _runReversi )reversiPiece(_list);

     CCLOG("-----------BETWEEEn E");
    return res;
}

void SquarePlace::reversiPiece(Vector<SquarePlace *> _list){
    if( _list.size() < 3 ) return;   //3個以下はリバーシ不成立
    GameDatas::getInstance()->calcReversiScore(_list);
    for (int i = 0; i < _list.size(); i ++) {
        SquarePlace* square = _list.at(i);
        if( square != NULL ) {
            square->createPiece(getPieceType(), true);
        }
    }
}

//挟んだかどうかをチェックする
//再起的に次のコマを呼び出し、falseかtrueが返るまで繰り返す。
bool SquarePlace::isBetweenColor(ReversiPieceType _type, int _xIncrease, int _yIncrease,bool _isSeed, Vector<SquarePlace*>* _reslist ){
    if( !m_parent ) return false;
    
    if (_type == getPieceType() && !_isSeed ) {
//        if( !Piece->IsReversi ){
//            this->createPiece(_type, true);
//        }
        if( !(*_reslist).contains(this)){
            (*_reslist).pushBack(this);
            CCLOG("between [SEED:%d] %d", _isSeed, m_index);
        }
        return true;    //同じカラーにたどり着いたら挟み成立
    }
    else{
        if (!_isSeed && getPieceType() == ReversiPieceType_Max) {
            return false;   //コマが未配置
        }
        
//        int idx = m_index + _xIncrease + (_yIncrease * m_parent->MaxHorizon);
//        if( idx >= m_parent->SquareList.size() || idx < 0 ){
//            return false;      //indexオーバー
//        }
        
        SquarePlace* square = getSquarePlaceFromSelf(_xIncrease, _yIncrease);//m_parent->SquareList.at(idx);
        if( !square ) return false;
        
        //thisが端っこだったら終わり
        int thisBorderHorizon = this->isBorderHorizon();
        int thisBorderVertex = this->isBorderVertex();
        if( thisBorderHorizon ){
            if( square->isBorderHorizon() && thisBorderHorizon != square->isBorderHorizon()){
                return false;
            }
        }
        if( thisBorderVertex ){
            if( square->isBorderVertex() && thisBorderVertex != square->isBorderVertex()){
                return false;
            }
        }
        
        bool flg = square->isBetweenColor(_type, _xIncrease, _yIncrease, false, _reslist);
        if( flg ){
            if( !(*_reslist).contains(this)){
                (*_reslist).pushBack(this);
                CCLOG("between [SEED:%d] %d", _isSeed, m_index);
            }
            
            if( _isSeed && (*_reslist).size() < 3 ){    //２個以下なら却下
                (*_reslist).clear();
                return false;
            }
        }
        
        return flg;
    }
    
    return false;
}
SquarePlace* SquarePlace::getSquarePlaceFromSelf(int _xIncrease, int _yIncrease){
    int idx = m_index + _xIncrease + (_yIncrease * m_parent->MaxHorizon);
    if( idx >= m_parent->SquareList.size() || idx < 0 ){
        return NULL;      //indexオーバー
    }
    
    return m_parent->SquareList.at(idx);
}

bool SquarePlace::checkNext(int _xIncrease, int _yIncrease){
    SquarePlace* square =NULL;
    square = getSquarePlaceFromSelf(_xIncrease, _yIncrease);
    if( square ){
        if (square->getPieceType() == ReversiPieceType_Black
            || square->isReversi()) {
            return true;
        }
    }
    return false;
}

//隣接ますをチェックし、自分がカラ、黒こま、手ゴマと隣接していたらtrueを返す
bool SquarePlace::checkCanPut(){
    if(getPieceType() != ReversiPieceType_Max) return false;
    
    if(checkNext(-1, 0)) return true;
    if(checkNext(1, 0)) return true;
    if(checkNext( 0, -1)) return true;
    if(checkNext( 0, 1)) return true;
    if(checkNext( -1, 1)) return true;
    if(checkNext( -1, -1)) return true;
    if(checkNext( 1, 1)) return true;
    if(checkNext( 1, -1)) return true;
    
    return false;
}

int SquarePlace::isBorder(){
    int ret = 0;
    ret = isBorderHorizon();
    ret = isBorderVertex();
    return ret;
}

int SquarePlace::isBorderHorizon(){
    if( m_index % m_parent->MaxHorizon == 0){
        return Left;
    }
    if(m_index % m_parent->MaxHorizon == (m_parent->MaxHorizon-1)){
        return Right;
    }
    return false;
}
int SquarePlace::isBorderVertex(){
    if( m_index % m_parent->MaxVertial == 0 ){
        return UP;
    }
    if( m_index % m_parent->MaxVertial == (m_parent->MaxVertial-1)){
        return Down;
    }
    return false;
}

ReversiPieceType SquarePlace::getPieceType(){
    if( !Piece ) return ReversiPieceType_Max;
    return Piece->Type;
}
bool SquarePlace::isReversi(){
    if( !Piece ) return false;
    return Piece->IsReversi;
}

void SquarePlace::setStack(int _cnt, StackDirection _direction){
    if( _direction != m_stackDirection ){
        m_stackDirection = _direction;
        setVisibleArrow(true);
    }
    
    m_count = _cnt;
    if( m_count > 1 )setVisibleCount(true);     //個数表示
}

float SquarePlace::calcArrowAngle(StackDirection _direction){
    switch (_direction) {
        case StackDirection_Vertex_Up:
        case StackDirection_Vertex_Down:
            return -90.0f;
            
        case StackDirection_Horizon_R:
        case StackDirection_Horizon_L:
            return 0.0f;
            
        case StackDirection_Slash_R_Up:
        case StackDirection_Slash_R_Down:
            return -45.0f;
            
        case StackDirection_Slash_L_Up:
        case StackDirection_Slash_L_Down:
            return 45.0f;
        default:
            break;
    }
    return 0;
}

void SquarePlace::runExplode(){
    CCLOG("--explode--");
//    Vector<SquarePlace*> _list;
    m_replenishingList.clear();
    
    switch (m_stackDirection) {
        case StackDirection_Vertex_Up:
        case StackDirection_Vertex_Down:
            checkExplode(0, -1, true, &m_replenishingList);
            checkExplode(0, 1, true, &m_replenishingList);
            break;
            
        case StackDirection_Horizon_R:
        case StackDirection_Horizon_L:
            checkExplode(-1, 0, true, &m_replenishingList);
            checkExplode(1, 0, true, &m_replenishingList);
            break;
            
        case StackDirection_Slash_R_Up:
        case StackDirection_Slash_R_Down:
            checkExplode(1, -1, true, &m_replenishingList);
            checkExplode(-1, 1, true, &m_replenishingList);
            break;
            
        case StackDirection_Slash_L_Up:
        case StackDirection_Slash_L_Down:
            checkExplode(-1, -1, true, &m_replenishingList);
            checkExplode(1, 1, true, &m_replenishingList);
            break;
            
        default:
            break;
    }
    GameDatas::getInstance()->calcExplosionScore(m_replenishingList);
    explode();
    
//    PieceDealer::getInstance()->replenishing(_list, m_parent);
    this->runAction(
                    Sequence::create(DelayTime::create(0.5f),
                                     CallFunc::create([this](){
                        PieceDealer::getInstance()->replenishing(m_replenishingList);
                    })
                                     , NULL)
    );
    
    CCLOG("--explode end--");
}

void SquarePlace::explode(){
    for( int i = 0 ; i < m_replenishingList.size() ; i++){
        SquarePlace* _square = m_replenishingList.at(i);
        _square->deletePiece(true);
        _square->createExplosionAnimation();
    }
}


void SquarePlace::checkExplode(int _xIncrease, int _yIncrease, bool _seed, Vector<SquarePlace*>* _reslist){
    if( !m_parent ) return;
    
//    deletePiece(true);
//    (*_reslist).pushBack(this);
//    createExplosionAnimation();
    if( !(*_reslist).contains(this)){
        (*_reslist).pushBack(this);
        CCLOG("explode! %d", m_index);
    }
    
    int idx = m_index + _xIncrease + (_yIncrease * m_parent->MaxHorizon);
    if( idx >= m_parent->SquareList.size() || idx < 0 ){
        return;      //indexオーバー
    }
    
    SquarePlace* square = m_parent->SquareList.at(idx);
//    if( !_seed
//       &&(( this->isBorderHorizon() && this->isBorderHorizon() != square->isBorderHorizon() )
//          || ( this->isBorderVertex() && this->isBorderVertex() != square->isBorderVertex()))){
//           return;    //thisが端っこだったら終わり
//       }
    
    //thisが端っこだったら終わり
    int thisBorderHorizon = this->isBorderHorizon();
    int thisBorderVertex = this->isBorderVertex();
    if( thisBorderHorizon ){
        if( square->isBorderHorizon() && thisBorderHorizon != square->isBorderHorizon()){
            return;
        }
    }
    if( thisBorderVertex ){
        if( square->isBorderVertex() && thisBorderVertex != square->isBorderVertex()){
            return;
        }
    }
    
    square->checkExplode(_xIncrease, _yIncrease, false, _reslist);
    
    
    return;
}

void SquarePlace::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags){
}

int SquarePlace::getStackNum(){
    return m_count;
}

void SquarePlace::createExplosionAnimation(){
    
    Sprite* eff_explosion3 = Sprite::create("eff_explosion3.png");
    eff_explosion3->setAnchorPoint(Point(0.5, 0.5));
    eff_explosion3->setPosition(Point(50.0, 60.0));
    eff_explosion3->setScale(0.5);
    eff_explosion3->setOpacity(0.0);
    this->addChild(eff_explosion3);
    
    Sprite* eff_explosion4 = Sprite::create("eff_explosion4.png");
    eff_explosion4->setAnchorPoint(Point(0.5, 0.5));
    eff_explosion4->setPosition(Point(50.0, 60.0));
    eff_explosion4->setScale(2.0);
    eff_explosion4->setOpacity(0.0);
    this->addChild(eff_explosion4);
    
    Sprite* eff_explosion1 = Sprite::create("eff_explosion1.png");
    eff_explosion1->setAnchorPoint(Point(0.5, 0.5));
    eff_explosion1->setPosition(Point(50.0, 60.0));
    eff_explosion1->setScale(0.6);
    this->addChild(eff_explosion1);
    
    Sprite* eff_explosion2 = Sprite::create("eff_explosion2.png");
    eff_explosion2->setAnchorPoint(Point(0.5, 0.0471));
    eff_explosion2->setPosition(Point(50.0, 60.0));
    eff_explosion2->setScaleX(0.15);
    eff_explosion2->setScaleY(0.5);
    this->addChild(eff_explosion2);
    
    eff_explosion3->runAction
    (CCSpawn::create
     (CCSequence::create
      (CCDelayTime::create(2.0 / 30.0),
       CCScaleTo::create(4.0 / 30.0, 2.3),
       CCScaleTo::create(3.0 / 30.0, 2.0),
       CCDelayTime::create(1.0 / 30.0),
       CCNodeUtil::createRemoveAction(),
       NULL),
      CCSequence::create
      (CCDelayTime::create(2.0 / 30.0),
       CCFadeTo::create(0.0 / 30.0, 255.0),
       CCDelayTime::create(4.0 / 30.0),
       CCFadeTo::create(3.0 / 30.0, 50.8),
       CCDelayTime::create(1.0 / 30.0),
       CCFadeTo::create(0.0 / 30.0, 0.0),
       NULL),
      NULL));
    eff_explosion4->runAction
    (CCSpawn::create
     (CCSequence::create
      (CCDelayTime::create(4.0 / 30.0),
       CCScaleTo::create(8.0 / 30.0, 4.0),
       CCNodeUtil::createRemoveAction(),
       NULL),
      CCSequence::create
      (CCDelayTime::create(4.0 / 30.0),
       CCFadeTo::create(0.0 / 30.0, 255.0),
       CCFadeTo::create(8.0 / 30.0, 0.0),
       NULL),
      NULL));
    eff_explosion1->runAction
    (CCSpawn::create
     (CCSequence::create
      (CCMoveBy::create(4.0 / 30.0, Point(-0.05, 0.0)),
       CCDelayTime::create(3.0 / 30.0),
       CCMoveBy::create(5.0 / 30.0, Point(0.05, 0.0)),
       CCNodeUtil::createRemoveAction(),
       NULL),
      CCSequence::create
      (CCScaleTo::create(4.0 / 30.0, 2.0),
       CCScaleTo::create(3.0 / 30.0, 1.8),
       CCScaleTo::create(5.0 / 30.0, 1.6),
       NULL),
      CCSequence::create
      (CCDelayTime::create(4.0 / 30.0),
       CCRotateBy::create(3.0 / 30.0, 0.0),
       CCRotateBy::create(5.0 / 30.0, 0.0),
       NULL),
      CCSequence::create
      (CCDelayTime::create(7.0 / 30.0),
       CCFadeTo::create(5.0 / 30.0, 0.0),
       NULL),
      NULL));
    eff_explosion2->runAction
    (CCSpawn::create
     (CCSequence::create
      (CCScaleTo::create(3.0 / 30.0, 1.4, 2.0),
       CCScaleTo::create(4.0 / 30.0, 2.0, 1.8),
       CCScaleTo::create(4.0 / 30.0, 1.6, 0.6),
       CCDelayTime::create(1.0 / 30.0),
       CCNodeUtil::createRemoveAction(),
       NULL),
      CCSequence::create
      (CCDelayTime::create(3.0 / 30.0),
       CCFadeTo::create(4.0 / 30.0, 127.5),
       CCFadeTo::create(4.0 / 30.0, 0.0),
       CCDelayTime::create(1.0 / 30.0),
       NULL),
      NULL));
}
