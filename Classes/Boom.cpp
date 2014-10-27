//
//  Boom.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-5.
//
//

#include "Boom.h"
#include "Triangle.h"

Boom::Boom(cocos2d::Point _p) {
    position = _p;
    trigger_time = clock();
}

void Boom::draw_self() {
    Triangle t1, t2;
    t1.set_triangle(this->position, BOOM_SIZE, 0);
    t2.set_triangle(this->position, BOOM_SIZE, 180);
    cocos2d::Color4F color(1, 0.5, 0.5, 1);
    cocos2d::DrawPrimitives::drawSolidPoly(t1.points, 3, color);
    cocos2d::DrawPrimitives::drawSolidPoly(t2.points, 3, color);
}
