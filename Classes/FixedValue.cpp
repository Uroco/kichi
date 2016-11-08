//
//  FixedValue.cpp
//  maji-kichi-kaishayamerutteyo
//
//  Created by KUBO AYAMI on 2015/05/19.
//
//
#define FIXED_VALUE_DEFINE

#import "FixedValue.h"


void initFixedValue(){
    g_isDebug = false;
    
}


/*
 * AAAA BBBB CCCCC
 */
static CCNodeUtil* _sharedInstance = NULL;

CCNodeUtil* CCNodeUtil::sharedInstance() {
    if(_sharedInstance == NULL){
        _sharedInstance = new CCNodeUtil();
    }
    
    return _sharedInstance;
}

void CCNodeUtil::removeNode(cocos2d::Node* node) {
    node->removeFromParentAndCleanup(true);
}

cocos2d::CallFuncN* CCNodeUtil::createRemoveAction() {
    return cocos2d::CallFuncN::create(CCNodeUtil::sharedInstance(), callfuncN_selector(CCNodeUtil::removeNode));
}
