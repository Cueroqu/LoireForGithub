//
//  ShipyardScene.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-7.
//
//

#ifndef __Loire__ShipyardScene__
#define __Loire__ShipyardScene__

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "cocos2d.h"
#include "Airship.h"
#include "VisibleRect.h"
#include "RightTrapezoid.h"
#include "StupidEnemy.h"

#define MENUITEM_FONTNAME "Helvetica"
#define set_vector_visible(vector, visible) {   \
    for (auto i : vector) {                     \
        i->setVisible(visible);                 \
    }                                           \
}


enum LABEL_STATUS {SHIPS, WEAPONS};

struct SlotRecord {
    cocos2d::Point origin;
    cocos2d::Point dest;
    int selected;
    int index;
    SlotRecord(){};
    SlotRecord(cocos2d::Point o, cocos2d::Point d, int i) : origin(o), dest(d), selected(false), index(i) {};
};

class Weapon;

class ShipyardLayer : public cocos2d::Layer {
private:
    float left_ship_scale;
    float item_ship_scale;
    float MAX_ATTACK_VALUE;
    int STROKE_WIDTH;
    int ITEMBOX_SIZE;
    int LABEL_HEIGHT;
    int LABEL_WIDTH;
    int LABEL_MARGIN;
    int MENUITEM_FONTSIZE;
    int ITEMLABEL_FONTSIZE;
    int LABEL_Y;
    int LABEL_LEFTMOST_X;
    int BOXES_TOP_MARGIN;
    
    int LEFT_BOX_ORIGIN_X;
    int BOXES_TOP_Y;
    int BOXES_BOTTOM_Y;
    cocos2d::Point LEFT_SHIP_OFFSET;
    /* the boundary of left and right boxes */
    int BOXES_BOUNDARY;
    
    int ATTACK_BAR_HEIGHT;
    int ATTACK_BAR_COL_MARGIN;
    int ATTACK_BAR_WIDTH;
    cocos2d::Point ATTACK_BAR_LEFT_POSITION;
    RightTrapezoid attack_bar;
    int RIGHT_SHIP_XOFFSET;
    int RIGHT_SHIP_YOFFSET;
    int SLOT_ORIGIN_RECT_HSIZE;
    int SLOT_SELECTED_RECT_HSIZE;
    
    LABEL_STATUS status;
    
    cocos2d::EventListenerTouchOneByOne * contact_listener;
    cocos2d::CustomCommand _customCommand;
    cocos2d::Vector<cocos2d::Sprite * > item_ships;
    cocos2d::Vector<cocos2d::Label * > item_weapons;
    std::vector<cocos2d::Vec2 *> left_points_buf;
    std::vector<SlotRecord> slots_buf;
    std::set<Airship *> selected_ships_buf;
    std::map<Weapon *, StupidEnemy *> abuse_map;
    int selected_box = -1;
    cocos2d::Label * selected_label;
    cocos2d::Point selected_label_origin_position;
    Airship * right_ship;
public:
    static const int SETUP_WEAPON_LAYER_INDEX = 10;
    ShipyardLayer() {}
    ~ShipyardLayer() {
        release_buf();
    }
    virtual bool init() override;
    virtual void draw (cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags) override;
    void draw_setup_weapons_layer(const cocos2d::Mat4 &transform, uint32_t flags);
    void release_buf() {
        for (auto v : left_points_buf)
            delete [] v;
        left_points_buf.clear();
    }
    std::string weapon_to_string(Weapon * weapon);
    void load_position_datas();
    void remove_idiots();
    void bind_idiot(Weapon * w, StupidEnemy * se);
    void get_attack_bar();
    void add_menu_items();
    void add_touch_listener();
    void ships_item_call_back(cocos2d::Ref * pSender);
    void weapons_item_call_back(cocos2d::Ref * pSender);
    void back_item_call_back(cocos2d::Ref * pSender);
    void get_left_ship_position();
    void get_item_ships();
    void get_item_weapons();
    void add_right_ship_to_child();
    void pre_set_right_ship(Airship * ship);
    void release_item_weapons();
    void release_memory();
    void remove_right_ship_projectiles();
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);
    cocos2d::Label * copy_label(cocos2d::Label * src);
    int in_a_slot(cocos2d::Point position);
    void reset();
    CREATE_FUNC(ShipyardLayer);
};

class ShipyardScene : public cocos2d::Scene {
    CC_SYNTHESIZE_READONLY(ShipyardLayer *, _layer, Layer);
    ShipyardScene() : _layer(NULL){};
    ~ShipyardScene();
    bool init();
    CREATE_FUNC(ShipyardScene);
};

#endif /* defined(__Loire__ShipyardScene__) */
