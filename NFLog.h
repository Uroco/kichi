//
//  NFLog.h
//  NinjaFlight
//


#ifndef __NF_LOG_H__
#define __NF_LOG_H__

#include "platform/CCPlatformMacros.h"
#import "cocos2d.h"
static const int maxLogLength = 256*1024;


void NFPrintLog(const char *filename, int nLine,const char * pszFormat, ...);

//void NFDictionaryPrint(cocos2d::Dictionary* _dic);
#endif
