//
//  Airship.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-14.
//
//

#include "Airship.h"
#include "AppDelegate.h"
#include "Weapon.h"
#include "pugixml.hpp"
#include "GameScene.h"
#include "Tools.h"
#include "Projectile.h"
#include "Enemy.h"
#include "GameOverScene.h"
#include "GameElementsManager.h"
#include "typedefs.h"

USING_NS_CC;

Airship* Airship::create(const std::string &filename) {
    Airship* ret = new Airship();
    if (ret && ret->initWithFile(filename)) {
        ret->autorelease();
        ret->total_buff = 1;
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Airship::releaseElements() {
    releaseWeapons();
}

void Airship::set_percent(int value) {
    attack_buff = (float) value / 100.0 + 0.5;
    speed_buff = (float) (100.0 - value) / 100.0 + 0.5;
}

void Airship::reset_for_game() {
    //DEBUG
    Tools::get_hit_num = 0;
    
    this->setRotation(0);
    this->current_hp = this->total_hp;
    this->current_angular_speed = 0;
    this->current_linear_speed = Point::ZERO;
    this->set_percent(50);
    for (auto weapon : this->weapons) {
        if (weapon == nullptr) continue;
        weapon->reset();
        weapon->left_num = weapon->data.projnum;
    }
}

/* deprecated */
//void Airship::shoot_enemy(float t) {
//    Vector<Sprite *> projectiles_to_delete;
//    for (int i = 0; i < this->projectiles.size(); ++i) {
//        auto projectile = projectiles.at(i);
//        auto prect = Tools::get_sprite_rect(projectile);
//        bool monsterHit = false, final_projectile = false;
//        Vector<Enemy *> enemies_to_delete;
//        for (int j = 0; j < GameElementsManager::enemies.size(); ++j) {
//            auto target = (Sprite *)GameElementsManager::enemies.at(j);
//            auto erect = Tools::get_sprite_rect(target);
//            if (erect.containsPoint(projectile->getPosition())) {
//                if (!(((Projectile *)projectile)->should_damage_enemy((Enemy *)target))) continue;
//                monsterHit = true;
//                Enemy *enemy = (Enemy *) target;
//                enemy->current_hp -= ((Projectile *)projectile)->get_power();
//                if (enemy->current_hp <= 0) {
//                    enemies_to_delete.pushBack(enemy);
//                }
//                break;
//            }
//        }
//        
//        for (auto target : enemies_to_delete) {
//            target->remove_self_from_game();
//            if (GameElementsManager::enemies.size() == 0) {
//                ((AppDelegate *)Application::getInstance())->getGameScene()->getLayer()->quit_game();
//                final_projectile = true;
//            }
//        }
//        
//        if (monsterHit && !final_projectile) {
//            projectiles_to_delete.pushBack(projectile);
//        }
//        enemies_to_delete.clear();
//    }
//    
//    for (const auto &p : projectiles_to_delete) {
//        this->projectiles.eraseObject(p);
//        GameElementsManager::booms.push_back(Boom(p->getPosition()));
//        ((AppDelegate *)Application::getInstance())->getGameScene()->getLayer()->removeChild(p);
//    }
//    projectiles_to_delete.clear();
//}

void Airship::get_hit(cocos2d::Sprite *projectile) {
    Projectile * p = (Projectile *) projectile;
    //DEBUG
    ++Tools::get_hit_num;
    
    if (p->get_power() >= this->current_hp) {
        GET_GAME_LAYER->remove_game_elements();
        auto scene = GameOverScene::create();
        scene->getLayer()->getLabel()->setString("Game Over");
        Director::getInstance()->replaceScene(scene);
        return;
    }
    this->current_hp -= p->get_power();
}

void Airship::update(float t) { // check the projectiles hit enemies or not
//    shoot_enemy(t);
}

void Airship::movement(float t) {
//    move_type_1(t);
    move_type_2(t);
}

bool Airship::next_dot_is_closer() {
    return (this->commandIndex < Path::getInstance().size - 1) &&
    ((Path::getInstance().dots[this->commandIndex]->getPosition() - this->getPosition()).getLength() >
     (Path::getInstance().dots[this->commandIndex+1]->getPosition() - this->getPosition()).getLength());
}

void Airship::move_elements(cocos2d::Point target_direction) {
    Point relative = Point::ZERO, cur = this->getPosition();
    if ((target_direction.x < 0 &&
         cur.x < GET_GAME_LAYER->PLAYER_LEFTBOUND) ||
        (target_direction.x > 0 &&
         cur.x > GET_GAME_LAYER->PLAYER_RIGHTBOUND))
    {
        relative.x = -target_direction.x;
        target_direction.x = 0;
    }
    
    if ((target_direction.y < 0 &&
         cur.y < GET_GAME_LAYER->PLAYER_BOTTOMBOUND) ||
        (target_direction.y > 0 &&
         cur.y > GET_GAME_LAYER->PLAYER_TOPBOUND))
    {
        relative.y = -target_direction.y;
        target_direction.y = 0;
    }
    
    this->setPosition(cur + target_direction);
    move_others(relative);
}

void Airship::move_others(cocos2d::Point target_direction) {
    /* move enemy */
    for (auto sp : GameElementsManager::enemies) {
        ((Enemy *)sp)->move_delta_pool.push_back(target_direction);
        for (auto pr : ((Enemy *)sp)->projectiles) {
            pr->setPosition(pr->getPosition() + target_direction);
        }
    }
    
    /* move signs */
    for (auto sign : GameElementsManager::signs) {
        sign->setPosition(sign->getPosition() + target_direction);
    }
    
    /* move path */
    for (int i = 0; i < Path::getInstance().size; ++i) {
        auto sp = Path::getInstance().dots[i];
        sp->setPosition(sp->getPosition() + target_direction);
    }
    
    /* move projectiles */
    for (auto pr : projectiles) {
        pr->setPosition(pr->getPosition() + target_direction);
    }
    
    for (auto bg : GET_GAME_SCENE->getBackground()->bgs) {
        bg->setPosition(bg->getPosition() + target_direction);
    }
}

void Airship::move_type_2(float t) {
    if (this->commandIndex >= Path::getInstance().size) {
        current_angular_speed = 0;
//        printf("\n");
    }
    else {
        Point origin_target = Path::getInstance().dots[this->commandIndex]->getPosition();
        Point target_direction = origin_target - this->getPosition();
        
        /* fix airship orientation first */
        float cocos_angle = -CC_RADIANS_TO_DEGREES(target_direction.getAngle());
        float origin = Tools::modifyDegreeIntoRange(this->getRotation(), cocos_angle - 180, cocos_angle + 180);

        set_rotation_type_1(t, cocos_angle, origin);
        
        Point plane_direction = this->get_ship_direction();
        this->current_linear_speed = Tools::get_a_positive_projection_on_b(this->current_linear_speed, plane_direction);
        Point total_disp = Path::getInstance().back()->getPosition() - this->getPosition();
        float max_movespeed = MIN(this->get_move_speed() * Tools::get_positive_cosine(target_direction, plane_direction), sqrtf(2 * this->get_linear_acceleration() * total_disp.getLength()));
        float delta_speed = MIN(this->get_linear_acceleration() * t, max_movespeed - this->current_linear_speed.getLength());
        if (fabsf(delta_speed) < EPS) delta_speed = 0;
        this->current_linear_speed = this->current_linear_speed + Tools::a_direction_vector_with_length_b(plane_direction, delta_speed);
        if (target_direction.getLength() < (this->current_linear_speed * t).getLength()) {
            float speed = MAX(target_direction.getLength() / t, this->current_linear_speed.getLength() - t * this->get_linear_acceleration());
            this->current_linear_speed = Tools::a_direction_vector_with_length_b(this->current_linear_speed, speed);
        }
        
        if (this->commandIndex < Path::getInstance().size - 2 || this->getPosition().getDistance(Path::getInstance().dots[commandIndex]->getPosition()) > Path::DOTS_MIN_DISTANCE * 2)
        {
            target_direction = this->current_linear_speed * t;
        }
        else {
            target_direction = target_direction.getNormalized() * this->current_linear_speed.getLength() * t;
        }

        move_elements(target_direction);
        
        /* if finish current movement task, move on to next */
        /* strategy 1: in the next 3 points, if it's near enouch and has smaller turn angle, move command to that */
        float maxcosf = Tools::get_positive_cosine(Path::getInstance().dots[commandIndex]->getPosition() - this->getPosition(), plane_direction), tmp;
        int targetj = commandIndex;
        for (int i = 0, j = commandIndex+1; j < Path::getInstance().size && i < 3; ++i, ++j) {
            if (Path::getInstance().dots[j]->getPosition().getDistance(this->getPosition()) > this->get_move_speed()) continue;
            tmp = Tools::get_positive_cosine(Path::getInstance().dots[j]->getPosition() - this->getPosition(), plane_direction);
            if (tmp > maxcosf) {
                maxcosf = tmp;
                targetj = j;
            }
        }
        
        /* if above strategy find a better command */
        if (targetj != commandIndex) {
            commandIndex = targetj;
            return;
        }
        
        /* if next command has a smaller turn angle and near than data.movespeed */
        if (this->current_linear_speed.getLength() > EPS &&
            commandIndex+1 < Path::getInstance().size &&
            this->getPosition().getDistance(Path::getInstance().dots[commandIndex]->getPosition()) < this->get_move_speed() &&
            Tools::get_positive_cosine
            (Path::getInstance().dots[commandIndex + 1]->getPosition() - Path::getInstance().dots[commandIndex]->getPosition(),
             Path::getInstance().dots[commandIndex]->getPosition() - this->getPosition()) < 1 - EPS) {
                ++commandIndex;
                return;
        }
        
        /* if has move near enough to the command */
        if (this->getPosition().getDistance(Path::getInstance().dots[this->commandIndex]->getPosition()) < 5) {
            // || Tools::get_positive_cosine(origin_target - fake_position, Path::getInstance().dots[commandIndex]->getPosition() - fake_position) < 0) {
            ++this->commandIndex;
            if (Path::getInstance().size > 1 && this->commandIndex >= Path::getInstance().size) {
                ((AppDelegate *)Application::getInstance())->getGameScene()->getLayer()->removeDots();
                this->current_linear_speed = Point::ZERO;
                this->current_angular_speed = 0;
                this->commandIndex = 0;
            }
        }
    }
}

void Airship::move_type_1(float t) {
    if (this->commandIndex >= Path::getInstance().size) {
        current_angular_speed = 0;
    }
    else {
        Point total_disp = Path::getInstance().back()->getPosition() - this->getPosition();
        if (next_dot_is_closer()) {
            ++commandIndex;
        }
        Point disp = Path::getInstance().dots[this->commandIndex]->getPosition() - this->getPosition();
        this->current_linear_speed = Tools::get_a_positive_projection_on_b(this->current_linear_speed, disp);
        
        /* if the destination is so far away, just move at the max speed
         * if it's almost the end, the airship need to slow to stimulate inertia
         * formula: 
         * s = v_0 * t - 1/2 * a * t^2
         * and slowing down time should be: t = v_0 / a
         * so we substitute the t with above formula we have
         * current max speed: v_m = sqrt(2*a*s/3)
         */
        float max_movespeed = MIN(this->get_move_speed(), sqrtf(2 * this->get_linear_acceleration() * total_disp.getLength()));
        float delta_speed = MIN(this->get_linear_acceleration() * t, max_movespeed - this->current_linear_speed.getLength());
        if (fabsf(delta_speed) < EPS) delta_speed = 0;
        this->current_linear_speed = this->current_linear_speed + Tools::a_direction_vector_with_length_b(disp, delta_speed);
        disp = -this->current_linear_speed * t;
        
        move_others(disp);
        
        /* if finish current movement task, move on to next */
        if (this->getPosition().getDistance(Path::getInstance().dots[this->commandIndex]->getPosition()) < 2) {
            ++this->commandIndex;
            if (Path::getInstance().size > 1 && this->commandIndex >= Path::getInstance().size) {
                ((AppDelegate *)Application::getInstance())->getGameScene()->getLayer()->removeDots();
                this->current_linear_speed = Point::ZERO;
                this->current_angular_speed = 0;
                this->commandIndex = 0;
            }
        }
        /* fix to clockwise, -180 to 180 */
        float cocosAngle = -CC_RADIANS_TO_DEGREES(this->current_linear_speed.getAngle());
        cocosAngle = Tools::modifyDegreeIntoRange(cocosAngle, 0, 360);
        float origin = Tools::modifyDegreeIntoRange(this->getRotation(), cocosAngle-180, cocosAngle+180);
        set_rotation_type_1(t, cocosAngle, origin);
    }
}

void Airship::set_rotation_type_1(float t, float cocos_angle, float origin) {
    ROTATE_TYPE_1(t, cocos_angle, origin, this->current_angular_speed, this->get_rotate_speed(), this->get_rotate_acceleration());
}
