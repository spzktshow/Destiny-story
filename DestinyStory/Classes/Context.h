//
//  Context.h
//  DestinyStory
//
//  Created by RockLee on 14-8-26.
//
//

#ifndef DestinyStory_Context_h
#define DestinyStory_Context_h

#include "cocos2d.h"

static const cocos2d::Size designSize(960, 640);

/**笛卡尔 cocos***/
/**经典 classical***/
class CoordinateUtils : public cocos2d::Ref
{
public:
    static cocos2d::Point cocosToClassical(cocos2d::Point cocosPoint)
    {
        return cocos2d::Point(cocosPoint.x, designSize.height - cocosPoint.y);
    };
    static cocos2d::Point classicalToCocos(cocos2d::Point classicalPoint)
    {
        return cocos2d::Point(classicalPoint.x, designSize.height - classicalPoint.y);
    };
};

#endif
