#import "CommonDefine.h"

#import "NFNumberLayer.h"

#define batchTagRight 1001
#define batchTagCenter 1002
#define batchTagLeft 1003
#define unitTag 1004
#define signTag 1005
#define decimalStartTag 1010

using namespace std;
using namespace cocos2d;

NFNumberLayer::NFNumberLayer() {
    color = Color3B::WHITE;
	opacity = 255;
	_isOpacityModifyRGB = false;
}

NFNumberLayer::~NFNumberLayer() {
}

NFNumberLayer* NFNumberLayer::create(int num, const char* fileName, int numWidth, int numHeight, int numInterval, int commaWidth, NumAlign numAlign, Point numAnchor, int minimumKeta, const char* batchName, int batchCenterWidth, const char* unitName, int unitWidth, const char* signName, int signWidth, int decimalNum, int decimalKeta, int pointWidth) {
	
	NFNumberLayer* numberLayer = new NFNumberLayer();
	numberLayer->autorelease();
	
	numberLayer->_num = num;
	numberLayer->fileName = fileName;
	numberLayer->numWidth = numWidth;
	numberLayer->numHeight = numHeight;
	numberLayer->numInterval = numInterval;
	numberLayer->commaWidth = commaWidth;
	numberLayer->numAlign = numAlign;
	numberLayer->numAnchor = numAnchor;
	numberLayer->minimumKeta = minimumKeta;
	numberLayer->batchName = batchName;
	numberLayer->batchCenterWidth = batchCenterWidth;
	numberLayer->unitName = unitName;
	numberLayer->unitWidth = unitWidth;
	numberLayer->signName = signName;
	numberLayer->signWidth = signWidth;
    numberLayer->decimalKeta = decimalKeta;
    numberLayer->pointWidth = pointWidth;
	
	numberLayer->setNum(num, decimalNum);
	
	return numberLayer;
}

bool NFNumberLayer::init(){
    Layer::init();
    return true;
}

int NFNumberLayer::getKeta(int num) {
	int keta = 0;
	do {
		keta++;
		num /= 10;
	} while(num > 0);

	if(keta < minimumKeta) {
		keta = minimumKeta;
	}

	return keta;
}

int NFNumberLayer::getNumWidth(int num) {
	int keta = getKeta(num);
	int width = (numWidth + numInterval) * keta + (commaWidth + numInterval) * ((keta - 1) / 3);
	return width;
}

int NFNumberLayer::getNum() {
	return _num;
}

void NFNumberLayer::setNum(int num){
    setNum(num, 0);
}

void NFNumberLayer::setNum(int num, int decimalNum) {
	int positionX = -numWidth * numAnchor.x;
	if(numAlign == kNumAlignLeft) {
		positionX += getNumWidth(num) - (numWidth + numInterval);
		if(signWidth > 0) {
			positionX += (signWidth + numInterval);
		}
	} else if(numAlign == kNumAlignCenter) {
        float w = getNumWidth(num);
        if( !commaWidth ){
            w -= numInterval;
        }
		positionX += ( w - (numWidth + numInterval)) / 2;
        if (decimalKeta > 0) {
            positionX -= (pointWidth + decimalKeta * (numWidth + numInterval)) / 2;
        }
		if(unitWidth > 0) {
			positionX -= (unitWidth + numInterval) / 2;
		}
		if(signWidth > 0) {
			positionX += (signWidth + numInterval) / 2;
		}
	} else if(numAlign == kNumAlignRight){
        if (decimalKeta > 0) {
            positionX -= pointWidth + decimalKeta * (numWidth + numInterval);
        }
    }
	int tag = 0;
	
	// バッチ対応
	if(batchName != NULL) {
		// バッチ（右）
		Sprite* batchRight = (Sprite*)this->getChildByTag(batchTagRight);
		if(!batchRight) {
			char batchRightBuf[100];
			sprintf(batchRightBuf, "batch_%s_r.png", batchName);
			batchRight = Sprite::create(batchRightBuf);
			batchRight->setAnchorPoint(Point(0, 0.5));
			batchRight->setTag(batchTagRight);
			batchRight->setOpacity(opacity);
			this->addChild(batchRight);
		}
		// バッチ（中）
		Sprite* batchCenter = (Sprite*)this->getChildByTag(batchTagCenter);
		if(!batchCenter) {
			char batchCenerBuf[100];
			sprintf(batchCenerBuf, "batch_%s_c.png", batchName);
			batchCenter = Sprite::create(batchCenerBuf);
			batchCenter->setAnchorPoint(Point(0, 0.5));
			batchCenter->setTag(batchTagCenter);
			batchCenter->setOpacity(opacity);
			this->addChild(batchCenter);
		}
		// バッチ（左）
		Sprite* batchLeft = (Sprite*)this->getChildByTag(batchTagLeft);
		if(!batchLeft) {
			char batchLeftBuf[100];
			sprintf(batchLeftBuf, "batch_%s_l.png", batchName);
			batchLeft = Sprite::create(batchLeftBuf);
			batchLeft->setAnchorPoint(Point(1, 0.5));
			batchLeft->setTag(batchTagLeft);
			batchLeft->setOpacity(opacity);
			this->addChild(batchLeft);
		}
	}
	
	string commaFileNameString = "";
	if(commaWidth > 0) {
		commaFileNameString = string(fileName);
		commaFileNameString = commaFileNameString.substr(0, commaFileNameString.rfind("_01.png")) + "_comma.png";
	}
	
	int numTemp = num;
	int counter = 0;
	int numLeft = 0;
	int numRight = 0;
	do {
		if(counter > 0 && counter % 3 == 0 && commaWidth > 0) {
			positionX -= commaWidth + numInterval;
			tag++;
			
			Sprite* commaSprite = (Sprite*)this->getChildByTag(tag);
			if(!commaSprite) {
				commaSprite = Sprite::create(commaFileNameString.c_str());
				commaSprite->setAnchorPoint(Point(0, numAnchor.y));
				commaSprite->setTag(tag);
				commaSprite->setOpacity(opacity);
				this->addChild(commaSprite);
			}
			commaSprite->setPosition(Point(positionX, 0));
		}
		
		if(counter > 0) {
			positionX -= numWidth + numInterval;
		}
		tag++;
		
		Rect numRect = Rect(numWidth * (numTemp % 10), 0, numWidth, numHeight);
		Sprite* oneNumSprite = (Sprite*)this->getChildByTag(tag);
		if(this->getChildByTag(tag)) {
			oneNumSprite->setTextureRect(numRect);
		} else {
			oneNumSprite = Sprite::create(fileName, numRect);
			oneNumSprite->setAnchorPoint(Point(0, numAnchor.y));
			oneNumSprite->setTag(tag);
			oneNumSprite->setOpacity(opacity);
			this->addChild(oneNumSprite);
		}
		oneNumSprite->setPosition(Point(positionX, 0));
		
		// 位置を保持
		if(counter == 0) {
			numRight = positionX;
		}
		numLeft = positionX;
		
		numTemp /= 10;
		counter++;
	} while(numTemp > 0 || counter < minimumKeta);
    
    
    // 小数点以下対応
    if (decimalKeta > 0) {
        counter = 0;
        string pointFileNameString = "";
        pointFileNameString = string(fileName);
        pointFileNameString = pointFileNameString.substr(0, pointFileNameString.rfind("_01.png")) + "_point.png";

        tag = decimalStartTag;
        
        Sprite* pointSprite = (Sprite*)this->getChildByTag(tag);
        if(!pointSprite) {
            pointSprite = Sprite::create(pointFileNameString.c_str());
            pointSprite->setAnchorPoint(Point(0, numAnchor.y));
            pointSprite->setTag(tag);
            pointSprite->setOpacity(opacity);
            this->addChild(pointSprite);
        }
		numRight += numWidth + numInterval;
		pointSprite->setPosition(Point(numRight, 0));
		numRight += pointWidth + numInterval;
        
        int decimalNumTemp = decimalNum;
        do{
            int digit = (decimalNumTemp / pow(10, (decimalKeta - 1 - counter)));
            Rect numRect = Rect(numWidth * digit, 0, numWidth, numHeight);
            tag++;
            Sprite* oneNumSprite = (Sprite*)this->getChildByTag(tag);
            if(this->getChildByTag(tag)) {
                oneNumSprite->setTextureRect(numRect);
            } else {
                oneNumSprite = Sprite::create(fileName, numRect);
                oneNumSprite->setAnchorPoint(Point(0, numAnchor.y));
                oneNumSprite->setTag(tag);
                oneNumSprite->setOpacity(opacity);
                this->addChild(oneNumSprite);
            }
            oneNumSprite->setPosition(Point(numRight, 0));
            numRight += numWidth + numInterval;
            
            decimalNumTemp %= (int)pow(10, (decimalKeta - 1 - counter));
            counter++;
        } while(counter < decimalKeta);

    }

	// 符号対応
	if(signName) {
		Sprite* signSprite = (Sprite*)this->getChildByTag(signTag);
		if(!signSprite) {
			string signFileNameString = string(fileName);
			signFileNameString = signFileNameString.substr(0, signFileNameString.rfind("_01.png")) + "_" + signName + ".png";
			
			signSprite = Sprite::create(signFileNameString.c_str());
			signSprite->setAnchorPoint(Point(0, 0.5));
			signSprite->setTag(signTag);
			signSprite->setOpacity(opacity);
			this->addChild(signSprite);
		}
		
		int numHeightCenter = -numHeight * (numAnchor.y - 0.5);
		numLeft -= signWidth + numInterval;
		signSprite->setPosition(Point(numLeft, numHeightCenter));
	}
	
	// 単位対応
	if(unitName) {
		Sprite* unitSprite = (Sprite*)this->getChildByTag(unitTag);
		if(!unitSprite) {
			string unitFileNameString = string(fileName);
			unitFileNameString = unitFileNameString.substr(0, unitFileNameString.rfind("_01.png")) + "_" + unitName + ".png";
			
			unitSprite = Sprite::create(unitFileNameString.c_str());
			unitSprite->setAnchorPoint(Point(0, 0.5));
			unitSprite->setTag(unitTag);
			unitSprite->setOpacity(opacity);
			this->addChild(unitSprite);
		}
		
		int numHeightCenter = -numHeight * (numAnchor.y - 0.5);
		numRight += numWidth + numInterval;
		unitSprite->setPosition(Point(numRight, numHeightCenter));
	}
	
	// バッチ対応
	if(batchName != NULL) {
		int numLeftCenter = numLeft + numWidth * 0.5;
		int numRightCenter = numRight + numWidth * 0.5;
		int numHeightCenter = -numHeight * (numAnchor.y - 0.5);
		
		// バッチ（右）
		Sprite* batchRight = (Sprite*)this->getChildByTag(batchTagRight);
		batchRight->setPosition(Point(numRightCenter, numHeightCenter));
		// バッチ（中）
		Sprite* batchCenter = (Sprite*)this->getChildByTag(batchTagCenter);
		batchCenter->setPosition(Point(numLeftCenter, numHeightCenter));
		batchCenter->setScaleX((numRightCenter - numLeftCenter) / batchCenterWidth);
		// バッチ（左）
		Sprite* batchLeft = (Sprite*)this->getChildByTag(batchTagLeft);
		batchLeft->setPosition(Point(numLeftCenter, numHeightCenter));
	}
	
	// いらない要素を削除
	if(_num > num) {
        auto childrem = getChildren();
        auto it = childrem.begin();
        while (it != childrem.end()) {
            Sprite* child = dynamic_cast<Sprite*>((*it));
            if(child->getTag() > tag
               && child->getTag() != batchTagRight
               && child->getTag() != batchTagCenter
               && child->getTag() != batchTagLeft
               && child->getTag() != unitTag
               && child->getTag() != signTag) {
                child->removeFromParentAndCleanup(true);
            }
            it ++;
        }
        
//		Ref* _child;
//		CCARRAY_FOREACH_REVERSE(getChildren(), _child) {
//			Sprite* child = (Sprite*)_child;
//			if(child->getTag() > tag
//			   && child->getTag() != batchTagRight
//			   && child->getTag() != batchTagCenter
//			   && child->getTag() != batchTagLeft
//			   && child->getTag() != unitTag
//			   && child->getTag() != signTag) {
//				child->removeFromParentAndCleanup(true);
//			}
//		};
	}
	
	_num = num;
    _decimalNum = decimalNum;
}

void NFNumberLayer::setColor(const Color3B& color) {
	this->color = color;
}

const Color3B& NFNumberLayer::getColor() {
	return color;
}

GLubyte NFNumberLayer::getOpacity() {
	return opacity;
}

void NFNumberLayer::setOpacity(GLubyte opacity) {
	this->opacity = opacity;
//	Ref* _child;
    
    for (int i = 0; i < getChildrenCount(); i++) {
        Sprite* child = dynamic_cast<Sprite*>( getChildren().at(i));
        child->setOpacity(opacity);
    }

//	CCARRAY_FOREACH(getChildren(), _child) {
//		Sprite* child = dynamic_cast<Sprite*>(_child);
//		child->setOpacity(opacity);
//	}
}

void NFNumberLayer::setOpacityModifyRGB(bool bValue) {
	_isOpacityModifyRGB = bValue;
}

bool NFNumberLayer::isOpacityModifyRGB() {
	return _isOpacityModifyRGB;
}