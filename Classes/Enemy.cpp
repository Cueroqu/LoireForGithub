//
//  Enemy.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-16.
//
//

#include "Enemy.h"
#include <iostream>
#include "cocos2d.h"
#include "Tools.h"
#include "pugixml/pugixml.hpp"
#include "Arsenal.h"
#include "Player.h"
#include "Projectile.h"
#include "AppDelegate.h"
#include "GameOverScene.h"
#include "GameElementsManager.h"
#include "typedefs.h"
#include "LaserBeam.h"
using namespace std;
USING_NS_CC;

Enemy* Enemy::create(pugi::xml_node node) {
    auto ret = new Enemy();
    AirshipData data = GET_TYPE_VALUE(Airship, node, "airship");
    if (ret && ret->initWithTexture(data.texture)) {
        ret->autorelease();
        ret->data = data;
        ret->move_type = 0;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

Enemy::~Enemy() {

}

void Enemy::rotate_type_1(float t, float cocos_angle, float origin) {
    ROTATE_TYPE_1(t, cocos_angle, origin, this->current_angular_speed, this->get_rotate_speed(), this->get_rotate_acceleration());
}

/* turret type */
void Enemy::move_type_1(float t) {
    Sprite *player = GET_GAME_LAYER->getPlayer();
    Point target_direction = player->getPosition() - this->getPosition();
    float cocos_angle = -CC_RADIANS_TO_DEGREES(target_direction.getAngle());
    float origin = Tools::modifyDegreeIntoRange(this->getRotation(), cocos_angle - 180, cocos_angle + 180);
    rotate_type_1(t, cocos_angle, origin);
}

/* airship type 2 */
void Enemy::move_type_2(float t) {
    Sprite *player = GET_GAME_LAYER->getPlayer();
    Point target_direction = player->getPosition() - this->getPosition();
    
    /* rotate */
    float cocos_angle = -CC_RADIANS_TO_DEGREES(target_direction.getAngle());
    float origin = Tools::modifyDegreeIntoRange(this->getRotation(), cocos_angle - 180, cocos_angle + 180);
    rotate_type_1(t, cocos_angle, origin);
    
    if (this->weapons.at(0) != nullptr) {
        float move_length = target_direction.getLength() - this->weapons.at(0)->data.linearrange * 0.6;
        if (move_length < 0) return;
        target_direction = Tools::a_direction_vector_with_length_b(target_direction, move_length);
        
        Point ship_direction = this->get_ship_direction();
        this->current_linear_speed = Tools::get_a_positive_projection_on_b(this->current_linear_speed, ship_direction);
        
        // check Airship::move_type_1 for details
        float max_movespeed = MIN(this->get_move_speed() * Tools::get_positive_cosine(target_direction, ship_direction), sqrtf(2 * this->get_linear_acceleration() * target_direction.getLength()));
        
        float delta_speed = MIN(this->get_linear_acceleration() * t, max_movespeed - this->current_linear_speed.getLength());
        if (fabsf(delta_speed) < EPS) {
            delta_speed = 0;
        }
        else {
            this->current_linear_speed = this->current_linear_speed + Tools::a_direction_vector_with_length_b(ship_direction, delta_speed);
        }
        
        if (target_direction.getLength() < (this->current_linear_speed * t).getLength()) {
            float speed = MAX(target_direction.getLength() / t, this->current_linear_speed.getLength() - t * this->get_linear_acceleration());
            this->current_linear_speed = Tools::a_direction_vector_with_length_b(this->current_linear_speed, speed);
        }
        target_direction = this->current_linear_speed * t;
        this->move_delta_pool.push_back(target_direction);
    }
}

void Enemy::movement(float t) {
    switch (move_type) {
        case 1:
            move_type_1(t);
            break;
        case 2:
            move_type_2(t);
            break;
        default:
            break;
    }
}

/* deprecated */
void Enemy::shoot_projectiles() {
    cocos2d::Vector<Sprite *> projectiles_to_delete;
    for (int i = 0; i < this->projectiles.size(); ++i) {
        auto projectile = projectiles.at(i);
        auto prect = Tools::get_sprite_rect(projectile);
        bool playerHit = false;
        auto target = Player::getInstance().selected_airship;
        auto trect = Tools::get_sprite_rect(target);
        if (prect.intersectsRect(trect)) {
            if (!(((Projectile *) projectile)->should_damage_enemy(target))) continue;
            playerHit = true;
            target->current_hp -= ((Projectile *)projectile)->get_power();
            if (target->current_hp == 0) {
                GET_GAME_LAYER->remove_game_elements();
                auto scene = GameOverScene::create();
                scene->getLayer()->getLabel()->setString("Game Over");
                Director::getInstance()->replaceScene(scene);
                break;
            }
        }
        
        if (playerHit) {
            projectiles_to_delete.pushBack(projectile);
        }
    }
    
    for (const auto &p : projectiles_to_delete) {
        this->projectiles.eraseObject(p);
        GameElementsManager::booms.push_back(Boom(p->getPosition()));
        ((AppDelegate *) Application::getInstance())->getGameScene()->getLayer()->removeChild(p);
    }
    projectiles_to_delete.clear();
}

/* enemy movement, including self-move and relatived movement to player */
void Enemy::update_position() {
    if (this->move_delta_pool.size() > 0) {
        cocos2d::Point p = this->move_delta_pool.at(0);
        for (int i = 1; i < this->move_delta_pool.size(); ++i)
            p += this->move_delta_pool.at(i);
        this->setPosition(this->getPosition() + p);
        this->move_delta_pool.clear();
    }
}

void Enemy::update(float t) {
    update_position();
//    shoot_projectiles();
}

void Enemy::get_hit(cocos2d::Sprite *projectile) {
    Projectile * p = (Projectile *) projectile;
    if (p->get_power() >= this->current_hp) {
        for (auto weapon : Player::getInstance().selected_airship->weapons) {
            if (weapon == nullptr) continue;
            if (weapon->current_target_sprite == this)
                weapon->current_target_sprite = nullptr;
        }
        for (auto proj : Player::getInstance().selected_airship->projectiles) {
            Trackings * t = GET_TRACKINGS(proj);
            if (t != nullptr && this == t->target) t->target = nullptr;
            LaserBeam * lb = GET_LASERBEAM(proj);
            if (lb != nullptr && this == lb->target) lb->target = nullptr;
        }
        this->remove_self_from_game();
    }
    else
        this->current_hp -= p->get_power();
    if (GameElementsManager::enemies.size() == 0) {
        GET_GAME_LAYER->quit_game();
    }
}

void Enemy::remove_self_from_game() {
    // removing projectiles must before weapons since the laser beam should be released before weapons.
    remove_projectiles_from_layer();
    for (auto &weapon : this->weapons) {
        if (weapon == nullptr) continue;
        //DEBUG
        printf("shooot num: %d\n", weapon->shoot_num);
        weapon->unschedule(schedule_selector(Weapon::find_enemy));
        ((AppDelegate *)Application::getInstance())->getGameScene()->getLayer()->removeChild(weapon);
        weapon->release();
    }
    if (this->getPhysicsBody()) {
        this->getPhysicsBody()->getWorld()->removeBody(this->getPhysicsBody());
        this->setPhysicsBody(nullptr);
    }
    
    GET_GAME_LAYER->removeChild(GameElementsManager::enemy_label_map[this]);
    
    this->unscheduleUpdate();
    GameElementsManager::enemies.eraseObject(this);
    GET_GAME_LAYER->removeChild(this);
}
