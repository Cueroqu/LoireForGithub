//
//  Trackings.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-12.
//
//

#include "Trackings.h"
#include "AppDelegate.h"
#include "Arsenal.h"
#include "typedefs.h"

USING_NS_CC;

Trackings * Trackings::createWithTexture(Texture2D * texture) {
    auto ret = new Trackings();
    if (ret && ret->initWithTexture(texture)) {
        ret->autorelease();
        ret->_owner = NULL;
        ret->target = nullptr;
        ret->current_distance = 0;
        ret->current_angular_speed = 0;
        ret->current_linear_speed = Point::ZERO;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

void Trackings::update(float t) {
    if (target == nullptr) {
        current_distance = total_distance + 1;
    } else {
        Point target_direction = target->getPosition() - this->getPosition();
        
        /* rotate */
        float cocos_angle = -CC_RADIANS_TO_DEGREES(target_direction.getAngle());
        float origin = Tools::modifyDegreeIntoRange(this->getRotation(), cocos_angle-180, cocos_angle+180);
        ROTATE_TYPE_1(t, cocos_angle, origin, this->current_angular_speed, this->get_rotate_speed(), this->get_rotate_acceleration());
        
        Point proj_direction = this->get_proj_direction();
        this->current_linear_speed = Tools::get_a_positive_projection_on_b(this->current_linear_speed, proj_direction);
        float max_movespeed = this->get_move_speed() * Tools::get_positive_cosine(target_direction, proj_direction);
        float delta_speed = MIN(this->get_linear_acceleration() * t, max_movespeed - this->current_linear_speed.getLength());
        if (fabsf(delta_speed) < EPS) {
            delta_speed = 0;
        } else {
            this->current_linear_speed = this->current_linear_speed + Tools::a_direction_vector_with_length_b(proj_direction, delta_speed);
        }
        target_direction = this->current_linear_speed * t;
        this->setPosition(this->getPosition()+target_direction);
        current_distance += target_direction.getLength();
    }

    
    if (current_distance >= total_distance) {
        this->_owner->projectiles.eraseObject(this);
        this->unscheduleUpdate();
        this->getParent()->removeChild(this);
    }
}
