//
//  StupidEnemy.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-13.
//
//

#include "StupidEnemy.h"
#include "Arsenal.h"
#include "typedefs.h"

StupidEnemy * StupidEnemy::create() {
    auto ret = new StupidEnemy();
    AirshipData data = Arsenal::AirshipMap[AirshipType::STUPID];
    if (ret && ret->initWithTexture(data.texture)) {
        ret->autorelease();
        ret->data = data;
        ret->horizontal_direction = ret->vertical_direction = 0;
        ret->abuser_angle_range = ret->abuser_linear_range;
        ret->stupid_center = Point::ZERO;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

void StupidEnemy::update(float t) {
    if (abuser_linear_range != abuser->data.linearrange ||
        abuser_angle_range != abuser->data.anglerange ||
        abuser_origin_position != abuser->position) {
        abuser_linear_range = abuser->data.linearrange;
        abuser_angle_range = abuser->data.anglerange;
        abuser_origin_position = abuser->position;
        stupid_center = abuser->position + Tools::offset_according_to_radius_and_degree(abuser->data.linearrange, Tools::modifyDegreeIntoRange(abuser->get_orientation(), -180, 180));
    }
    Point direction(horizontal_direction, vertical_direction);
    direction *= this->data.movespeed;
    float delta = horizontal_direction * data.movespeed * t + vertical_direction * data.movespeed * t;
    current_offset += delta;
    direction = Point(horizontal_direction * horizontal_direction * current_offset, vertical_direction * vertical_direction * current_offset);
    this->setPosition(stupid_center+direction);
    this->setRotation((Tools::float_equals(vertical_direction, 0)?1:-vertical_direction)*90-horizontal_direction*90);
    if (fabsf(current_offset) >= MAX(x_offset, y_offset)) {
        if (!Tools::float_equals(horizontal_direction, 0))
            horizontal_direction = -horizontal_direction;
        if (!Tools::float_equals(vertical_direction, 0))
            vertical_direction = -vertical_direction;
    }
}

void StupidEnemy::reset_direction_and_bound(Weapon * w) {
    if (Tools::float_equals(cosf(CC_DEGREES_TO_RADIANS(-w->get_orientation())), 0)) {
        horizontal_direction = 1;
    }
    else {
        horizontal_direction = 0;
    }
    if (Tools::float_equals(sinf(CC_DEGREES_TO_RADIANS(-w->get_orientation())), 0)) {
        vertical_direction = 1;
    }
    else {
        vertical_direction = 0;
    }
    
    float offset = this->abuser->data.linearrange * sinf(CC_DEGREES_TO_RADIANS(this->abuser->data.anglerange/2));
    
    // NOTE: only consider the condition of v_d * h_d = 0
    if (horizontal_direction) {
        x_offset = offset;
        y_offset = 0;
    } else if (vertical_direction) {
        x_offset = 0;
        y_offset = offset;
    }
    current_offset = 0;
}
