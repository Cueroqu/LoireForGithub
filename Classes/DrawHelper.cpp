//
//  DrawHelper.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-16.
//
//

#include "DrawHelper.h"
#include "cocos2d.h"
#include "Tools.h"
USING_NS_CC;

void DrawHelper::drawSector(cocos2d::Point center, float radius, float orientation, float range_in_degree) {
    float halfAngle = range_in_degree / 2;
    float angle1 = CC_DEGREES_TO_RADIANS(Tools::modifyDegreeIntoRange(orientation - halfAngle, -180, 180));
    float angle2 = CC_DEGREES_TO_RADIANS(Tools::modifyDegreeIntoRange(orientation + halfAngle, -180, 180));
    if (fabsf(angle1 - angle2) > CC_RADIANS_TO_DEGREES(180)) Tools::fswap(angle1, angle2);
    
    Point p1 = center;
    p1.x = p1.x + radius * cosf(-angle1);
    p1.y = p1.y + radius * sinf(-angle1);
    
    Point p2 = center;
    p2.x = p2.x + radius * cosf(-angle2);
    p2.y = p2.y + radius * sinf(-angle2);
    
    Point mid = center;
    mid += Tools::offset_according_to_radius_and_degree(radius, orientation);
    
    glLineWidth(5.0f);
    DrawPrimitives::drawQuadBezier(p1, mid, p2, 50);
    DrawPrimitives::drawLine(center, p1);
    DrawPrimitives::drawLine(center, p2);
}
