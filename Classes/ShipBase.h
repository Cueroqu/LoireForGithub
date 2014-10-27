//
//  ShipBase.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-18.
//
//

#ifndef __Loire__ShipBase__
#define __Loire__ShipBase__

#include <iostream>
#include "Weapon.h"
#include "cocos2d.h"
#include "pugixml.hpp"
#include "Arsenal.h"
#include "Tools.h"
#include "HitableObject.h"

class ShipBase : public HitableObject {
public:
    const float HPBARLENGTH = 80;
    const float HPBARHEIGHT = 6;
    
    void remove_projectiles_from_layer();
    virtual void initiate_shipbase(pugi::xml_node node);
    virtual void drawElements(bool draw_weapon = true);
    virtual void releaseElements();
    virtual void drawWeapons(bool ifdraw);
    virtual void releaseWeapons();
    virtual void draw_hp_bar();
    virtual void movement(float t);
    void set_physics_body();
    
    AirshipData data;
    std::vector<Weapon *> weapons;
    cocos2d::Vector<Sprite *> projectiles;
    cocos2d::Point current_linear_speed;
    
    inline cocos2d::Point get_ship_direction() {
        float pda_in_radians = CC_DEGREES_TO_RADIANS(-Tools::modifyDegreeIntoRange(this->getRotation()));
        return cocos2d::Point(cosf(pda_in_radians), sinf(pda_in_radians));
    }
    
    float speed_buff;
    float attack_buff;
    float total_buff;
    /* only for user's airship now */
    std::vector<cocos2d::Vec2 * > shapes;
    std::vector<int> nums;
    inline void get_shapes(const std::string & name) {
        MyBodyParser::getInstance()->parseJsonFile(this->data.json);
        shapes = MyBodyParser::getInstance()->get_shape_from_json(this, name, nums);
    }
    inline void release_shapes() {
        for (int i = 0; i < shapes.size(); ++i)
            delete [] shapes[i];
    }
    inline float get_rotate_speed() {
        return this->data.rotatespeed * this->speed_buff;
    };
    inline float get_rotate_acceleration() {
        return this->data.angular_acceleration * this->speed_buff;
    };
    float get_move_speed() {
        return this->data.movespeed * this->speed_buff;
    };
    float get_linear_acceleration() {
        return this->data.linear_acceleration * this->speed_buff;
    }


    float current_angular_speed;    // clockwise to positive
};

#endif /* defined(__Loire__ShipBase__) */
