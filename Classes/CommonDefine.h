//
//  CommonDefine.h
//  NinjaFlight
//
//  Created by Watanabe Takuya on 2012/12/06.
//
//

#ifndef NinjaFlight_CommonDefine_h
#define NinjaFlight_CommonDefine_h

#import "cocos2d.h"

// リリースビルドを作る時はコメントアウトする
// 本当にリリースする時は、iOS版はReleaseビルドをしてプログラムを最適化する
#ifdef NO_DEBUG
#else

#define DEBUG

#endif

#ifdef DEBUG
#define COCOS2D_DEBUG 1
#else
#define NDEBUG
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define FONT_W3 "HiraKakuProN-W3"
#define FONT_W6 "HiraKakuProN-W6"
#else
#define FONT_W3 "DroidSans"
#define FONT_W6 "DroidSans"
#endif

#import "Logger.h"

const cocos2d::Texture2D::TexParams texParamsRepeatVertical = {GL_LINEAR, GL_LINEAR, GL_ONE, GL_REPEAT};
const cocos2d::Texture2D::TexParams texParamsRepeatHorizontal = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_ONE};
const cocos2d::Texture2D::TexParams texParamsRepeatBoth = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};



#endif
