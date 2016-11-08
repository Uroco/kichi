#include "cocos2d.h"

//#import "Double.h"
//#import "CCBoolean.h"

using namespace cocos2d;

typedef enum {
	kNumAlignRight,
	kNumAlignCenter,
	kNumAlignLeft
} NumAlign;

class NFNumberLayer : public Layer
{
	int _num;
	const char* fileName;
	int numWidth;
	int numHeight;
	int numInterval;
	int commaWidth;
	NumAlign numAlign;
	Point numAnchor;
	int minimumKeta;
	const char* batchName;
	int batchCenterWidth;
	const char* unitName;
	int unitWidth;
	const char* signName;
	int signWidth;
    int _decimalNum;
    int decimalKeta;
    int pointWidth;
	
	Color3B color;
	GLubyte opacity;
	bool _isOpacityModifyRGB;
	
	int getKeta(int num);
    
    virtual bool init() override;
public:
	NFNumberLayer();
	~NFNumberLayer();
	static NFNumberLayer* create(int num, const char* fileName, int numWidth, int numHeight, int numInterval, int commaWidth, NumAlign numAlign, Point numAnchor, int minimumKeta = 1, const char* batchName = NULL, int batchCenterWidth = 0, const char* unitName = NULL, int unitWidth = 0, const char* signName = NULL, int signWidth = 0, int decimalNum = 0, int decimalKeta = 0, int pointWidth = 0);
	
	void setNum(int num);
    void setNum(int num, int decimalNum);
	int getNumWidth(int num);
	int getNum();
	
    void setColor(const Color3B& color);
	const Color3B& getColor(void);
    GLubyte getOpacity(void);
	void setOpacity(GLubyte opacity);
    void setOpacityModifyRGB(bool bValue);
    bool isOpacityModifyRGB(void);
};