//
//  LaserBeam.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-20.
//
//

#include "LaserBeam.h"
#include "cocos2d.h"
#include "Enemy.h"
#include "Airship.h"
#include "AppDelegate.h"

USING_NS_CC;

LaserBeam * LaserBeam::createWithTexture(cocos2d::Texture2D *texture) {
    auto ret = new LaserBeam();
    if (ret && ret->initWithTexture(texture) ) {
        ret->autorelease();
        ret->_owner = nullptr;
        ret->target = nullptr;
        ret->working = false;
        ret->max_distance = 5;
        ret->setVisible(false);
        ret->owner_weapon = nullptr;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

void LaserBeam::unschedule_update() {
    if (working) {
        working = false;
        owner_weapon->left_num = 0;
        owner_weapon->scheduleOnce(schedule_selector(Weapon::resupply_without_reset_target), owner_weapon->data.cdtime);
        remove_elements();
        this->owner_weapon->laser_beam = nullptr;
        this->unscheduleUpdate();
        this->_owner->projectiles.eraseObject(this);
        this->getParent()->removeChild(this);
    }
}

void LaserBeam::unschedule_update_without_removeself() {
    if (working) {
        working = false;
        remove_elements();
        this->owner_weapon->laser_beam = nullptr;
        this->unscheduleUpdate();
    }
}

//void LaserBeam::reset() {
//    set_elements_visble(false, ue);
//    this->unschedule_update();
//}

void LaserBeam::remove_elements() {
    for (int i = 0; i < beams.size(); ++i) {
        beams[i]->getParent()->removeChild(beams[i]);
    }
    beams.clear();
}

void LaserBeam::set_elements_visble(bool visible) {
    for (Sprite * s : this->beams) {
        s->setVisible(visible);
    }
}

void LaserBeam::update(float t) {
    if (target == nullptr) {
        this->unschedule_update();
        return;
    }
    working = true;
    Point dest = target->getPosition();
    Point src = this->owner_weapon->position;
    Point dir = dest - src;
    time_t end;
    time(&end);
    if (difftime(end, this->start_time) > (float)this->owner_weapon->data.projnum - 0.1) {
        this->target = nullptr;
        this->unschedule_update();
        return;
    }
    if (dir.getLength() > this->max_distance) {
        this->unschedule_update();
        return;
    }
    
    float len = this->getContentSize().width;
    int num = ceilf(dir.getLength() / len), i;
    float finallen = dir.getLength() - (num-1)*len;
    dir = dir.getNormalized();
    if (num > beams.size()) {
        int size = (int)beams.size();
        for (i = 0; i < num - size; ++i) {
            auto b = Sprite::createWithTexture(this->data.texture);
            beams.push_back(b);
            this->getParent()->addChild(b);
        }
    } else {
        for (i = num; i < beams.size(); ++i) beams[i]->setVisible(false);
    }
    for (int i = 0; i < num-1; ++i) {
        beams[i]->setScale(1);
        beams[i]->setVisible(true);
        beams[i]->setRotation(-CC_RADIANS_TO_DEGREES(dir.getAngle() ));
        beams[i]->setPosition(src+dir*(0.5+i)*len);
    }
    beams[num-1]->setVisible(true);
    beams[num-1]->setRotation(-CC_RADIANS_TO_DEGREES(dir.getAngle()));
    beams[num-1]->setPosition(src+dir*((num-1)*len+finallen*0.5));
    beams[num-1]->setScale(finallen/len, 1);

    Enemy * enemy = GET_ENEMY(target);
    if (enemy != nullptr) {
        enemy->get_hit(this);
    } else {
        Airship * airship = GET_AIRSHIP(target);
        if (airship != nullptr && !GET_GAME_LAYER->shield) {
            airship->get_hit(this);
        }
    }
}
