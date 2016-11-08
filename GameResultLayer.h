//
//  GameResultLayer.h
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/27.
//
//

#ifndef __maji_kichi_kaishayamerutteyo__GameResultLayer__
#define __maji_kichi_kaishayamerutteyo__GameResultLayer__

#import "cocos2d.h"

class GameResultLayer : public cocos2d::Layer{
    bool isClear;
    void initClear();
    void initGameOver();
    
    cocos2d::ccMenuCallback _callback;
public:
    static GameResultLayer* create(bool isClear);
    virtual bool init();
    void setCloseListener(cocos2d::Ref *target, cocos2d::SEL_MenuHandler selector);
    
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)override;
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)override;
};


#endif /* defined(__maji_kichi_kaishayamerutteyo__GameResultLayer__) */
