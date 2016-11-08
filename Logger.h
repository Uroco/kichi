#ifndef NinjaFlight_Logger_h
#define NinjaFlight_Logger_h

#include "cocos2d.h"
#include "NFLog.h" 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#ifdef DEBUG
// IOS DEBUG BUILD
#define NFLog(S,...) NFPrintLog(__FILE__,__LINE__,S,##__VA_ARGS__)

#else
// IOS RELEASE BUILD
#define NFLog //

#endif
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

// Android
//void NFLog(const char* pszFormat, ...);

#import "platform/android/jni/JniHelper.h"
#import <android/log.h>

#define  LOG_TAG    "Ninja"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#ifdef DEBUG
//#define NFLog LOGD
#define NFLog(S,...) NFPrintLog(__FILE__,__LINE__,S,##__VA_ARGS__)
#else
#define NFLog //
#endif

#endif

#endif
