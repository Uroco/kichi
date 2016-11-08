//
//  FixedValue.h
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/19.
//
//

#ifndef __maji_kichi_kaishayamerutteyo__FixedValue__
#define __maji_kichi_kaishayamerutteyo__FixedValue__

#import <stdio.h>
#import "cocos2d.h"

#ifdef FIXED_VALUE_DEFINE
#define Extern
#else
#define Extern extern
#endif


Extern void initFixedValue();

//Extern cocos2d::Texture2D::TexParams texParamsRepeatVertical = {GL_LINEAR, GL_LINEAR, GL_ONE, GL_REPEAT};
//Extern cocos2d::Texture2D::TexParams texParamsRepeatHorizontal = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_ONE};
//Extern cocos2d::Texture2D::TexParams texParamsRepeatBoth = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
Extern bool g_isDebug;

class CCNodeUtil : public cocos2d::Node
{
    static CCNodeUtil* sharedInstance();
public:
    // removeFromParentAndCleanupするActionを作る
    static cocos2d::CallFuncN* createRemoveAction();
    void removeNode(cocos2d::Node* node);

};

#endif /* defined(__maji_kichi_kaishayamerutteyo__FixedValue__) */
