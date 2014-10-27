//
//  Weapon.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-14.
//
//

#include "Weapon.h"
#include "cocos2d.h"
#include "Tools.h"
#include "DrawHelper.h"
#include "Player.h"
#include "Airship.h"
#include "Enemy.h"
#include "GameScene.h"
#include "AppDelegate.h"
#include "Projectile.h"
#include "Trackings.h"
#include "GameElementsManager.h"
#include "typedefs.h"
#include "LaserBeam.h"
USING_NS_CC;

//Weapon* Weapon::create() {
//    auto ret = new Weapon();
//    if (ret) {
//        ret->autorelease();
//        ret->current_target = Point::ZERO;
//        ret->_owner = nullptr;
//        return ret;
//    }
//    CC_SAFE_DELETE(ret);
//    return nullptr;
//}

Weapon* Weapon::createWithXmlNode(pugi::xml_node node) {
    static int cnt_id = 0;
    auto ret = new Weapon();
    if (ret) {
        ret->autorelease();
        ret->weapon_id = ++cnt_id;
        ret->current_target = Point::ZERO;
        ret->current_target_sprite = nullptr;
        ret->data = GET_TYPE_VALUE(Weapon, node, "type");
        ret->left_num = ret->data.projnum;
        ret->_owner = nullptr;
        ret->has_target_within_distance = false;
        ret->projectile_data = GET_TYPE_VALUE(Projectile, node, "projtype");
        ret->laser_beam = nullptr;
        ret->stop_working = false;
//        if (Tools::float_equals(ret->projectile_data.speed, Arsenal::LASER_BEAM_SPEED)) {
//            ret->laser_beam = LaserBeam::createWithTexture(ret->projectile_data.texture);
//            ret->laser_beam->max_distance = ret->data.projlinear;
//            ret->laser_beam->retain();
//        }
        ret->shoot_num = 0;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

Weapon::~Weapon() {

}

void Weapon::set_self(ShipBase *ship, int slot_id) {
    this->setOwner(ship);
    this->slot_id = slot_id;
    this->left_num = this->data.projnum;
    this->angle_offset = ship->data.weapon_slots[slot_id].angleoffset;
    this->linear_offset = ship->data.weapon_slots[slot_id].linearoffset;
}

float Weapon::get_orientation() {
    return Tools::modifyDegreeIntoRange(this->getOwner()->getRotation() + this->angle_offset);
}

void Weapon::draw_self(bool ifdraw) {
    Point player_position = ((Sprite *)this->getOwner())->getPosition();
    this->orientation = get_orientation();
    this->position = player_position + Tools::offset_according_to_radius_and_degree(this->linear_offset, this->orientation);
    if (this->laser_beam != nullptr) {
        this->laser_beam->setPosition(this->position);
    }
    if (!ifdraw) return;
    if (has_target_within_distance) {
        DrawPrimitives::setDrawColor4B(128, 20, 20, 64);
    } else {
        DrawPrimitives::setDrawColor4B(128, 128, 128, 8);
    }
    DrawHelper::drawSector(this->position, this->data.linearrange, this->orientation, this->data.anglerange);
    if (left_num <= 0) {
        DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
        DrawPrimitives::drawSolidCircle(this->position, 10, 0, 6);
    }
}

bool Weapon::ensure_target(Point weapon_position, float weapon_orientation, Sprite * target) {
    if (Tools::within_distance(weapon_position, target->getPosition(), this->data.linearrange + target->getContentSize().width / 2)) {
        Point delta = target->getPosition() - weapon_position;
        float tmp = CC_RADIANS_TO_DEGREES(delta.getAngle());
        tmp = Tools::modifyDegreeIntoRange(-tmp);
        weapon_orientation = Tools::modifyDegreeIntoRange(weapon_orientation, tmp-180, tmp+180);
        if (fabsf(tmp-weapon_orientation) < this->data.anglerange/2) {
            has_target_within_distance = true;
            current_target_sprite = target;
            current_target = target->getPosition();
            fireAt(weapon_position, delta.getNormalized());
            return true;
        }
    }
    return false;
}

void Weapon::shoot_up(cocos2d::Point weapon_position, float weapon_orientation) {
    Point direction = (current_target_sprite ? current_target_sprite->getPosition() : current_target) - weapon_position;
    float angle = Tools::modifyDegreeIntoRange(-CC_RADIANS_TO_DEGREES(direction.getAngle()), weapon_orientation-180, weapon_orientation+180);
    float signd = (angle - weapon_orientation > 0) ? 1 : -1;
    if (fabsf(angle - weapon_orientation) > this->data.anglerange/2) {
        angle = weapon_orientation + this->data.anglerange / 2 * signd;
    }
    fireAt(weapon_position, Tools::offset_according_to_radius_and_degree(1.0f, angle));
}

void Weapon::find_enemy(float t) {
    if (left_num <= 0) return;
    if (stop_working) return;
    Vector<Sprite *> enemies = GameElementsManager::enemies;
    float weapon_orientation = this->get_orientation();
    Point weapon_position = ((Sprite *)this->getOwner())->getPosition() + Tools::offset_according_to_radius_and_degree(this->linear_offset, weapon_orientation);

    for (auto enemy : enemies) {
        if (ensure_target(weapon_position, weapon_orientation, enemy)) {
            return;
        }
    }
    
    has_target_within_distance = false;
    
    /* if the left_num isn't full, shoot up */
    if (left_num < this->data.projnum) {
        shoot_up(weapon_position, weapon_orientation);
    }
}

void Weapon::find_player(float t) {
    if (left_num <= 0) return;
    float weapon_orientation = this->get_orientation();
    Point weapon_position = ((Sprite *)this->getOwner())->getPosition() + Tools::offset_according_to_radius_and_degree(this->linear_offset, weapon_orientation);
    has_target_within_distance = false;
    if (!ensure_target(weapon_position, weapon_orientation, GET_GAME_LAYER->getPlayer()) && left_num < this->data.projnum) {
        shoot_up(weapon_position, weapon_orientation);
    }
}

void Weapon::laser_beam_fire(cocos2d::Node * parent, cocos2d::Point weapon_position, cocos2d::Point direction, bool reset_target) {
    if (laser_beam != nullptr) {
        return;
    }
    laser_beam = LaserBeam::createWithTexture(this->projectile_data.texture);
    time(&laser_beam->start_time);
    this->laser_beam->max_distance = this->data.projlinear;
    laser_beam->owner_weapon = this;
    set_projectile_info(parent, laser_beam, weapon_position, direction, false);
    laser_beam->target = current_target_sprite;
    laser_beam->scheduleUpdate();
}

void Weapon::just_shoot(float t) {
    
    if (left_num <= 0) return;
    float weapon_orientation = this->get_orientation();
    Point weapon_position = ((Sprite *)this->getOwner())->getPosition() + Tools::offset_according_to_radius_and_degree(this->linear_offset, weapon_orientation);
    Point direction = Tools::offset_according_to_radius_and_degree(1.0f, weapon_orientation);
    if (current_target_sprite != nullptr) {
        direction = (current_target_sprite->getPosition() - weapon_position).getNormalized();
    }
    
    if (this->projectile_data.track && current_target_sprite != nullptr) {
        shoot_tracks(GET_SHIPYARD_LAER, weapon_position, direction, false);
    } else if (data.parallel > 1) {
        cannons_shoot(GET_SHIPYARD_LAER, weapon_position, direction, false);
    } else if (Tools::float_equals(projectile_data.speed, Arsenal::LASER_BEAM_SPEED)) {
        laser_beam_fire(GET_SHIPYARD_LAER, weapon_position, direction, false);
    }
    else {
        go_straight(weapon_position, direction, GET_SHIPYARD_LAER, false);
    }
}

void Weapon::go_straight(cocos2d::Point weapon_position, cocos2d::Point direction, Node * parent_node, bool reset_target) {
    auto projectile = Projectile::createWithTexture(this->projectile_data.texture);
    set_projectile_info(parent_node, projectile, weapon_position, direction);
    float range = data.projlinear - projectile->getContentSize().width / 2;
    Point delta(direction.x * range, direction.y * range);
    assert(projectile_data.speed > EPS);
    float duration = data.projlinear / projectile_data.speed;
    if (this->projectile_data.linear_acceleration > EPS) {
        projectile->total_distance = this->data.projlinear;
        projectile->current_distance = 0;
        projectile->scheduleUpdate();
    } else {
        ((Sprite *)projectile)->runAction(Sequence::create(MoveBy::create(duration, delta),
                                                           CallFuncN::create([&](Node *sender) {
            sender->getParent()->removeChild(sender);
            this->getOwner()->projectiles.eraseObject((Sprite *)sender);
        }), NULL));
    }
    check_proj_left(reset_target);
}

void Weapon::shoot_tracks(cocos2d::Node * parent, cocos2d::Point weapon_position, cocos2d::Point direction, bool reset_target) {
    auto trackings = Trackings::createWithTexture(this->projectile_data.texture);
    set_projectile_info(parent, trackings, weapon_position, direction);
    trackings->total_distance = this->data.projlinear;
    trackings->target = current_target_sprite;
    trackings->scheduleUpdate();
    check_proj_left(reset_target);
}

void Weapon::cannons_shoot(cocos2d::Node *parent, cocos2d::Point weapon_position, cocos2d::Point direction, bool reset_target) {
    float step_rotation = this->get_orientation()+90;
    Point step_point = Tools::offset_according_to_radius_and_degree(data.interval, step_rotation);
    Point start_position = weapon_position + (float)(data.parallel - 1.) / 2. * step_point;
    step_point = -step_point;
    for (int i = 0; i < data.parallel; ++i) {
        go_straight(start_position+i*step_point, direction, parent, reset_target);
    }
}

void Weapon::set_projectile_info(cocos2d::Node * parent, Projectile * proj, cocos2d::Point weapon_position, cocos2d::Point direction, bool needbody) {
    proj->setPosition(weapon_position);
    if (needbody) {
        float width = proj->getContentSize().width;
        float height = proj->getContentSize().height;
        auto _body = PhysicsBody::create();
        _body->addShape(PhysicsShapeBox::create(Size(width, height)));
        if (_body != nullptr) {
            _body->setCollisionBitmask(0);
            _body->setContactTestBitmask(-1);
            proj->setPhysicsBody(_body);
        }
    }
    proj->setOwner(this->_owner);
    proj->setTag(this->_owner->getTag());
    proj->setRotation(-CC_RADIANS_TO_DEGREES(direction.getAngle()));
    proj->data = this->projectile_data;
    parent->addChild(proj, -1);
    this->_owner->projectiles.pushBack(proj);
}

void Weapon::fireAt(Point weapon_position, Point direction) {
    if (this->projectile_data.track) {
        shoot_tracks(GET_GAME_LAYER, weapon_position, direction);
    } else if (this->data.parallel > 1) {
        cannons_shoot(GET_GAME_LAYER, weapon_position, direction);
    } else if (Tools::float_equals(this->projectile_data.speed, Arsenal::LASER_BEAM_SPEED)) {
        laser_beam_fire(GET_GAME_LAYER, weapon_position, direction);
    }
    else {
        go_straight(weapon_position, direction, GET_GAME_LAYER);
    }
}

void Weapon::check_proj_left(bool reset_target) {
    //DEBUG
    ++shoot_num;
    
    --left_num;
    if (left_num <= 0) {
        if (reset_target) {
            this->schedule(schedule_selector(Weapon::resupply), 0, 0, this->data.cdtime);
        } else {
            this->schedule(schedule_selector(Weapon::resupply_without_reset_target), 0, 0, this->data.cdtime);
        }
    }
}

void Weapon::resupply_without_reset_target(float t) {
    left_num = this->data.projnum;
}

void Weapon::resupply(float t) {
    left_num = this->data.projnum;
    current_target_sprite = nullptr;
}

void Weapon::reset() {
    if (Tools::float_equals(this->projectile_data.speed, Arsenal::LASER_BEAM_SPEED)) {
        if (nullptr != laser_beam) {
            this->laser_beam->unschedule_update();
        }
    }
}
