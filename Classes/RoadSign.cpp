//
//  RoadSign.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-25.
//
//

#include "RoadSign.h"
#include "cocos2d.h"
USING_NS_CC;

void RoadSign::draw_self() {
    if (passed) {
        DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
    } else {
        DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
    }
    DrawPrimitives::drawSolidCircle(this->getPosition(), RADIUS, 0, 6);
}
