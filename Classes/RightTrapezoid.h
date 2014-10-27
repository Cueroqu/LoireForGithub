//
//  Trapezoid.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-11.
//
//

#ifndef __Loire__RightTrapezoid__
#define __Loire__RightTrapezoid__

#include <iostream>
#include "cocos2d.h"

/* NO! Now it's just a rectangle
 * a right trapezoid like this
 *
 *     |-----------------------------------
 *     |                                 /
 *     |                                /
 *     |                               /
 *     |------------------------------/
 *
 * and the angle is 45 in degree
 *
 */

class RightTrapezoid {
public:
    RightTrapezoid();
    void set_trapezoid(cocos2d::Point left_center, float width, float height);
    cocos2d::Point points[4];   //left top, right top, right bottom, left bottom
};

#endif /* defined(__Loire__Trapezoid__) */
