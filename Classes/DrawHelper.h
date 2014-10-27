//
//  DrawHelper.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-16.
//
//

#ifndef __Loire__DrawHelper__
#define __Loire__DrawHelper__

#include <iostream>
#include "cocos2d.h"

class DrawHelper {
public:
    static void drawSector(cocos2d::Point center, float radius, float orientation, float range_in_degree);
};

#endif /* defined(__Loire__DrawHelper__) */
