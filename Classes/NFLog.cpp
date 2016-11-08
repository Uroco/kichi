//
//  NFLog.m
//  NinjaFlight
//


#import "NFLog.h"

//#include <string.h>
#import "Logger.h"

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#import <objc/runtime.h>
//#import <Foundation/Foundation.h>
//#import <UIKit/UIKit.h>
//#endif

void NFPrintLog(const char *filename, int nLine, const char * pszFormat, ...)
{
    
    char szBuf[maxLogLength];
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, maxLogLength, pszFormat, ap);
    va_end(ap);
    char* filenameBuf = strrchr(filename,'/')+1;
    
    
    CCLOG("[FILE:%s l:%d] %s",filenameBuf, nLine, szBuf);

//    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        NSString* str = [NSString stringWithUTF8String:szBuf];
//        NSLog(@"[FILE:%s l:%d] %@",filenameBuf, nLine, str);
//    CCLog("[FILE:%s l:%d]",filenameBuf, nLine, );

//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        char logbuf[maxLogLength];
//        sprintf( logbuf, "[FILE:%s l:%d] %s",filenameBuf, nLine, szBuf );
//        LOGD(logbuf);
//    #endif
}

//void NFDictionaryPrint(cocos2d::Dictionary* _dic){
//    Array* array =_dic->allKeys();
//    Object* access = NULL;
//    CCARRAY_FOREACH(array, access){
//        CCString* key = (CCString*)access;
//        CCObject* obj = _dic->objectForKey(key->getCString());
//        CCInteger* value = (CCInteger*)obj;
//        NFLog("{""%s"":%d}",key->getCString(), value->getValue());
//    }
//}