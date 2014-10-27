//
//  Projectile.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-18.
//
//

#include "Projectile.h"
#include "cocos2d.h"
#include "Enemy.h"
USING_NS_CC;

Projectile::Projectile() {
    
}

Projectile::~Projectile() {
    _enemiesHit.clear();
}

float Projectile::get_power() {
    return this->data.power * this->_owner->attack_buff;
}

Projectile* Projectile::create() {
    auto ret = new Projectile();
    if (ret && ret->initWithFile("Projectile2.png")) {
        ret->autorelease();
        ret->_owner = NULL;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

Projectile * Projectile::createWithTexture(cocos2d::Texture2D *texture) {
    auto ret = new Projectile();
    if (ret && ret->initWithTexture(texture)) {
        ret->autorelease();
        ret->_owner = NULL;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

void Projectile::update(float t) {
    float delta_speed = MIN(this->get_linear_acceleration() * t, this->data.speed - this->current_linear_speed.getLength());
    Point direction = this->get_proj_direction();
    if (Tools::float_equals(delta_speed, 0)) {
        delta_speed = 0;
    } else {
        this->current_linear_speed = this->current_linear_speed + direction * delta_speed;
    }
    direction = this->current_linear_speed * t;
    this->setPosition(this->getPosition() + direction);
    this->current_distance += direction.getLength();
    if (this->current_distance >= total_distance) {
        this->_owner->projectiles.eraseObject(this);
        this->unscheduleUpdate();
        this->getParent()->removeChild(this);
    }
}

bool Projectile::should_damage_enemy(ShipBase *enemy) {
    if (_enemiesHit.contains(enemy)) {
        return false;
    } else {
        _enemiesHit.pushBack(enemy);
        return true;
    }
    return true;
}
