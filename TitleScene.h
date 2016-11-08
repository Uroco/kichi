//
//  TitleScene.h
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/28.
//
//

#ifndef __maji_kichi_kaishayamerutteyo__TitleScene__
#define __maji_kichi_kaishayamerutteyo__TitleScene__

#import "cocos2d.h"
class TitleScene : public cocos2d::Layer
{
    cocos2d::Sprite* logo;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(TitleScene);
    
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)override;
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)override;
    //    void start();
};

#endif /* defined(__maji_kichi_kaishayamerutteyo__TitleScene__) */
