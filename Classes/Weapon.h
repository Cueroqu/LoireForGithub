//
//  Weapon.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-14.
//
//

#ifndef __Loire__Weapon__
#define __Loire__Weapon__

#include <iostream>
#include "cocos2d.h"
#include "Arsenal.h"
#include "pugixml/pugixml.hpp"

class ShipBase;
class Enemy;
class Projectile;
class LaserBeam;

class Weapon : public cocos2d::Sprite {
private:
    bool has_target_within_distance;
    virtual const cocos2d::Vec2& getPosition() const override {return cocos2d::Point::ZERO;} ;
    virtual void setPosition(const cocos2d::Vec2 & position) override {} ;
    virtual float getRotation() const override { return 0.;};
    virtual void setRotation(float rotation) override {};
public:
//    static Weapon* create();
    static Weapon* createWithXmlNode(pugi::xml_node node);

    //DEBUG:
    int shoot_num;

    ~Weapon();
    void draw_self(bool ifdraw);
    void fireAt(cocos2d::Point weapon_position, cocos2d::Point direction);
    void set_projectile_info(cocos2d::Node * parent, Projectile * proj, cocos2d::Point weapon_position, cocos2d::Point direction, bool needbody = true);
    void go_straight(cocos2d::Point weapon_position, cocos2d::Point direction, cocos2d::Node * parent_node, bool reset_target = true);
    void shoot_tracks(cocos2d::Node * parent, cocos2d::Point weapon_position, cocos2d::Point direction, bool reset_target = true);
    void cannons_shoot(cocos2d::Node * parent, cocos2d::Point weapon_position, cocos2d::Point direction, bool reset_target = true);
    void laser_beam_fire(cocos2d::Node * parent, cocos2d::Point weapon_position, cocos2d::Point direction, bool reset_target = true);
    void just_shoot(float t);
    void find_enemy(float t);
    void find_player(float t);
    bool ensure_target(cocos2d::Point weapon_position, float weapon_orientation, cocos2d::Sprite * target);
    void shoot_up(cocos2d::Point weapon_position, float weapon_orientation);
    void check_proj_left(bool reset_target = true);
    void resupply(float t);
    void resupply_without_reset_target(float t);
    void set_self(ShipBase * ship, int slot_id);
    void reset();
    float get_orientation();

    float angle_offset;
    float linear_offset;
    WeaponData data;
    int slot_id;
    int left_num;
    int weapon_id;
    bool stop_working;
    ProjectileData projectile_data;
    cocos2d::Point position;
    cocos2d::Point current_target;
    cocos2d::Sprite * current_target_sprite;
    LaserBeam *laser_beam = nullptr;
    float orientation;
    CC_SYNTHESIZE(ShipBase *, _owner, Owner);
};

#endif /* defined(__Loire__Weapon__) */
