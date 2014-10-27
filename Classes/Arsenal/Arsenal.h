//
//  Arsenal.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-26.
//
//

#ifndef __Loire__Arsenal__
#define __Loire__Arsenal__

#include <iostream>
#include <map>
#include <vector>
#include "AirshipType.h"
#include "WeaponType.h"
#include "ProjectileType.h"
#include "TechmodType.h"
#include "cocos2d.h"

#define GET_TYPE_VALUE(TYPENAME, node, value) Arsenal::TYPENAME##Map[((TYPENAME##Type)node.attribute(value).as_int())]

/*
 * cas : current angular speed
 * grs : get rotate speed
 * gra : get rotate acceleration
 */

#define ROTATE_TYPE_1(t, cocos_angle, origin, cas, grs, gra) {  \
    if (fabsf(cocos_angle - origin) > 5) {                      \
        float delta = MIN(t * grs, fabsf(cocos_angle - origin));\
        if (cocos_angle < origin) delta = -delta;               \
        float signd = delta / fabsf(delta);                     \
        if (delta * cas < -EPS) {                               \
            cas = -signd * (fabsf(cas)-t*gra);                  \
            if (fabsf(cas) > grs) {                             \
                cas = grs * signd;                              \
            }                                                   \
        } else {                                                \
            cas = MIN(grs, fabsf(cas) + t*gra) * signd;         \
        }                                                       \
        delta = t*cas;                                          \
        this->setRotation(this->getRotation()+delta);           \
    } else if (cas != 0) {                                      \
        cas = 0;                                                \
    }                                                           \
}

typedef void (*TechmodFunction)(cocos2d::Sprite * pSender);

struct WeaponSlot {
    float angleoffset;
    float linearoffset;
};

struct AirshipData {
    AirshipType type;
    std::string image;
    std::string name;
    std::string json;
    float movespeed;
    float rotatespeed;
    float linear_acceleration;
    float angular_acceleration;
    int weapon_slotnumber;
    int techmodnum;
    std::vector<WeaponSlot> weapon_slots;
    cocos2d::Texture2D * texture;
};

struct WeaponData {
    constexpr static float const MIN_PROJFREQ = 0.2f;
    WeaponType type;
    float anglerange;
    float linearrange;
    float projlinear;
    float cdtime;
    float projfreq; //projectile frequence
    int projnum;
    int parallel;
    float interval;
};

struct ProjectileData {
    ProjectileType type;
    std::string image;
    cocos2d::Texture2D * texture;
    float power;
    float speed;
    float rotate;
    float linear_acceleration;
    float angular_acceleratoin;
    bool track;
};

struct TechmodData {
    TechmodType type;
    std::function<void (cocos2d::Sprite * pSender)> onTechTriggered;
};

class Arsenal {
public:
    static constexpr float LASER_BEAM_SPEED = 2048;
    static constexpr float MAX_LINEAR_ACCR = 500;
    static constexpr float MAX_ANGULAR_ACCR = 100;
    static std::map<AirshipType, AirshipData> AirshipMap;
    static std::map<WeaponType, WeaponData> WeaponMap;
    static std::map<ProjectileType, ProjectileData> ProjectileMap;
    static std::map<TechmodType, TechmodFunction> TechmodMap;
    static void load_types();
    static void load_airship_types();
    static void load_weapon_types();
    static void load_projectile_types();
    static void load_techmod_types();
    static void respring(cocos2d::Sprite * pSender);
};

#endif /* defined(__Loire__Arsenal__) */
