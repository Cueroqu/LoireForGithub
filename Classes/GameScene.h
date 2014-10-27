//
//  GameScene.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-9.
//
//

#ifndef __Loire__GameScene__
#define __Loire__GameScene__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include <vector>
#include "Airship.h"
#include "Enemy.h"
#include "pugixml.hpp"
#include "SystemEventPool.h"
#include "DialogLayer.h"
#include "BackgroundLayer.h"
#include "Projectile.h"
#include "ui/CocosGUI.h"

#define GET_SPRITE(shape) dynamic_cast<Sprite *>(shape->getBody()->getNode())
#define GET_SHIPBASE(shape) dynamic_cast<ShipBase *>(shape->getBody()->getNode())
#define GET_PROJECTILE(shape) dynamic_cast<Projectile *>(shape->getBody()->getNode())

#define LEFT_BOUND bounds[0]
#define TOP_BOUND bounds[1]
#define RIGHT_BOUND bounds[2]
#define BOTTOM_BOUND bounds[3]

#define TraverseGameELements(visit) \
{                                   \
    /* visit layer */               \
    this->visit();                  \
                                    \
    /* visit background */          \
    this->_background->visit();     \
                                    \
    /* visit player */              \
    if (_player != nullptr) {       \
        for (auto &weapon : _player->weapons) { \
            if(weapon == nullptr) continue; \
            weapon->visit();        \
        }                           \
        for (auto projectile : _player->projectiles) {  \
            projectile->visit();    \
        }                           \
        _player->visit();           \
    }                               \
                                    \
    /* visit enemy */               \
    for (auto enemy : GameElementsManager::enemies) {   \
        for (auto &weapon : ((Enemy *)enemy)->weapons) {    \
            if(weapon == nullptr) continue; \
            weapon->visit();        \
        }                           \
        for (auto projectile : ((Enemy *)enemy)->projectiles) { \
            projectile->visit();    \
        }                           \
        enemy->visit();             \
    }                               \
}

class GameLayer : public cocos2d::Layer
{
private:
    cocos2d::EventListenerPhysicsContact* contact_listener;
    cocos2d::EventListenerTouchOneByOne * listener;
    Airship* _player;
    CC_SYNTHESIZE(cocos2d::PhysicsWorld *, _physics_world, PhysicsWorld)
    CC_SYNTHESIZE(BackgroundLayer *, _background, Background);
    cocos2d::Texture2D *texture;
    DialogLayer *dialog_layer;
    ui::Slider * slider;
    bool dragging;
    bool hit_on_blank;
    bool pausing;
    bool has_enemy_in_screen;
    bool shield;
    int shield_cd;
    
public:
    const static int ENEMY_TAG = 2;
    constexpr static float SHIELD_LASTING_TIME = 5;
    constexpr static int SHIELD_CDTIME = 30;
    constexpr static int SHIELD_REFRESH_T = 1;
    /* the shield can only be used as many as SHIELD_TIME times */
    const static int SHIELD_TIME = 2;
    
    float bounds[4];    // left, top, right, bottom
    float GAME_SCENE_WIDTH;
    float GAME_SCENE_HEIGHT;
    float PLAYER_H_HAREA = 50; // player horizontal move area (half)
    float PLAYER_V_HAREA = 30; // player vertical move area (half)
    
    float PLAYER_LEFTBOUND;
    float PLAYER_RIGHTBOUND;
    float PLAYER_TOPBOUND;
    float PLAYER_BOTTOMBOUND;
    
    const float TRACE_BOUND_MARGIN = 20;
    const float STATUS_BAR_HEIGHT = 0;
        
    GameLayer() {};
    ~GameLayer() {};
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event* event);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
//    bool onConatctPreSolve(cocos2d::PhysicsContact& contact, PhysicsContactPreSolve &solve);
//    void onContactPostSolve(cocos2d::PhysicsContact& contact, const PhysicsContactPostSolve &solve);
//    void onContactSeperate(cocos2d::PhysicsContact& contact);
    void sliderEvent(cocos2d::Ref* pSender, ui::Slider::EventType type);
    void shield_call_back(cocos2d::Ref* pSender);
    void shield_finish_call_back(float t);
    void shield_cd_call_back(float t);
    
    cocos2d::Node *nodeUnderTouch(cocos2d::Touch *touch);    
    void fire(float t);
    
    void add_series_dots(cocos2d::Point target, cocos2d::Point offset);
    void addRouteDot(cocos2d::Point position);
    void removeDots();
    void remove_game_elements();
    void remove_booms(float t);
    inline Airship* getPlayer() {
        return _player;
    }
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 & transform, uint32_t flags) override;
    
    void abort_game_dialog(Ref *pSender);
    void pause_game();
    void resume_game();
    void quit_game();
    bool reset(std::string filename);
    
    bool touch_out_of_center(cocos2d::Point touch);
    bool load_game_scene(std::string filename);
    bool sprite_outof_screen(cocos2d::Sprite* sprite);
    void draw_sprite_trace(cocos2d::Sprite* sprite);
    
    inline float player_top_padding() {
        return TOP_BOUND - _player->getPositionY();
    }
    inline float player_bottom_padding() {
        return _player->getPositionY() - BOTTOM_BOUND;
    }
    inline float player_left_padding() {
        return _player->getPositionX() - LEFT_BOUND;
    }
    inline float player_right_padding() {
        return RIGHT_BOUND - _player->getPositionX();
    }
    
    virtual bool init() override;
    virtual void update(float t);
    CREATE_FUNC(GameLayer);
    
protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    cocos2d::CustomCommand _customCommand;
    MenuItemImage * shield_item;
    int shield_used_time;
};

class GameScene : public cocos2d::Scene
{
public:
    GameScene():_layer(NULL){};
    ~GameScene();
    bool init();
    CREATE_FUNC(GameScene);
    CC_SYNTHESIZE_READONLY(GameLayer *, _layer, Layer);
    CC_SYNTHESIZE_READONLY(BackgroundLayer *, _background, Background);
};

#endif /* defined(__Loire__GameScene__) */
