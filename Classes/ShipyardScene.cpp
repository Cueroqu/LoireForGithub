//
//  ShipyardScene.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-7.
//
//

#include "ShipyardScene.h"
#include "Player.h"
#include "Airship.h"
#include "AppDelegate.h"
#include "Tools.h"
#include "pugixml/pugixml.hpp"
#include "LaserBeam.h"
USING_NS_CC;

void ShipyardLayer::load_position_datas() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document("shipyard_config.xml", doc);
    if (result) {
        pugi::xml_node data_node = doc.first_child();
        left_ship_scale = data_node.attribute("left_ship_scale").as_float();
        item_ship_scale = data_node.attribute("item_ship_scale").as_float();
        MAX_ATTACK_VALUE = data_node.attribute("MAX_ATTACK_VALUE").as_float();
        STROKE_WIDTH = data_node.attribute("STROKE_WIDTH").as_int();
        ITEMBOX_SIZE = data_node.attribute("ITEMBOX_SIZE").as_int();
        LABEL_HEIGHT = data_node.attribute("LABEL_HEIGHT").as_int();
        LABEL_WIDTH = data_node.attribute("LABEL_WIDTH").as_int();
        LABEL_MARGIN = data_node.attribute("LABEL_MARGIN").as_int();
        MENUITEM_FONTSIZE = data_node.attribute("MENUITEM_FONTSIZE").as_int();
        ITEMLABEL_FONTSIZE = data_node.attribute("ITEMLABEL_FONTSIZE").as_int();
        LABEL_Y = LABEL_HEIGHT / 2+ ITEMBOX_SIZE;
        LABEL_LEFTMOST_X = LABEL_WIDTH / 2;
        BOXES_TOP_MARGIN = data_node.attribute("BOXES_TOP_MARGIN").as_int();
        
        /* shared datas */
        BOXES_TOP_Y = winSize.height - BOXES_TOP_MARGIN;
        BOXES_BOTTOM_Y = ITEMBOX_SIZE + LABEL_HEIGHT + LABEL_MARGIN + STROKE_WIDTH;
        BOXES_BOUNDARY = winSize.width / 2;
        SLOT_ORIGIN_RECT_HSIZE = data_node.attribute("SLOT_ORIGIN_RECT_HSIZE").as_int();
        SLOT_SELECTED_RECT_HSIZE = data_node.attribute("SLOT_SELECTED_RECT_HSIZE").as_int();
        
        
        /* only for left */
        LEFT_BOX_ORIGIN_X = data_node.attribute("LEFT_BOX_ORIGIN_X").as_int();
        LEFT_SHIP_OFFSET.x = LEFT_BOX_ORIGIN_X + ((BOXES_BOUNDARY - LEFT_BOX_ORIGIN_X) >> 1);
        LEFT_SHIP_OFFSET.y = BOXES_BOTTOM_Y + ((BOXES_TOP_Y - BOXES_BOTTOM_Y) >> 1);
        
        /* only for right */
        ATTACK_BAR_HEIGHT = data_node.attribute("ATTACK_BAR_HEIGHT").as_int();
        ATTACK_BAR_COL_MARGIN = data_node.attribute("ATTACK_BAR_COL_MARGIN").as_int();
        ATTACK_BAR_WIDTH = (winSize.width - BOXES_BOUNDARY) * data_node.attribute("ATTACK_BAR_WIDTH_RATIO").as_float();
        ATTACK_BAR_LEFT_POSITION.x = (winSize.width - BOXES_BOUNDARY) / 2 + BOXES_BOUNDARY - ATTACK_BAR_WIDTH / 2;
        ATTACK_BAR_LEFT_POSITION.y = BOXES_BOTTOM_Y + ATTACK_BAR_COL_MARGIN + ATTACK_BAR_HEIGHT / 2;
        RIGHT_SHIP_XOFFSET = BOXES_BOUNDARY + (winSize.width - BOXES_BOUNDARY) / 2;
        int tmp = 2 * ATTACK_BAR_COL_MARGIN + ATTACK_BAR_HEIGHT + BOXES_BOTTOM_Y;
        RIGHT_SHIP_YOFFSET = tmp + (BOXES_TOP_Y - tmp) / 2;

    } else {
        auto mb = CustomMessageBox::create();
        mb->setString("Load shipyard_config.xml Error");
        mb->onOkMenuItemPressed = [=](Ref * pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        };
        this->addChild(mb);
    }
}

void ShipyardLayer::add_menu_items() {
    auto weapons_item = MenuItemFont::create("Weapons", CC_CALLBACK_1(ShipyardLayer::weapons_item_call_back, this));
    weapons_item->setFontSizeObj(MENUITEM_FONTSIZE);
    weapons_item->setFontName(MENUITEM_FONTNAME);
    weapons_item->setPosition(Point(LABEL_LEFTMOST_X + LABEL_WIDTH, LABEL_Y));
    
    auto ships_item = MenuItemFont::create("Ships", CC_CALLBACK_1(ShipyardLayer::ships_item_call_back, this));
    ships_item->setFontSizeObj(MENUITEM_FONTSIZE);
    ships_item->setFontNameObj(MENUITEM_FONTNAME);
    ships_item->setPosition(Point(winSize.width - LABEL_LEFTMOST_X, LABEL_Y));
    
    auto back_item = MenuItemImage::create("back_button.png", "back_button.png", CC_CALLBACK_1(ShipyardLayer::back_item_call_back, this));
    back_item->setPosition(Point(1111,603));
//    auto back_item = MenuItemFont::create("Back", CC_CALLBACK_1(ShipyardLayer::back_item_call_back, this));
//    back_item->setFontSizeObj(MENUITEM_FONTSIZE);
//    back_item->setFontName(MENUITEM_FONTNAME);
//    back_item->setPosition(Point(LABEL_LEFTMOST_X, winSize.height - LABEL_HEIGHT / 2));
    
    auto menu = Menu::create(ships_item, weapons_item, back_item, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    auto attack_label = Label::createWithSystemFont("Attacks:", "Arial", 36);
    attack_label->setHorizontalAlignment(TextHAlignment::RIGHT);
    attack_label->setPosition(Point(ATTACK_BAR_LEFT_POSITION.x - attack_label->getContentSize().width/2 - 10, ATTACK_BAR_LEFT_POSITION.y));
    this->addChild(attack_label);
}

void ShipyardLayer::add_touch_listener() {
    contact_listener = EventListenerTouchOneByOne::create();
    contact_listener->retain();
    contact_listener->setSwallowTouches(true);
    contact_listener->onTouchBegan = CC_CALLBACK_2(ShipyardLayer::onTouchBegan, this);
    contact_listener->onTouchMoved = CC_CALLBACK_2(ShipyardLayer::onTouchMoved, this);
    contact_listener->onTouchEnded = CC_CALLBACK_2(ShipyardLayer::onTouchEnded, this);
}

bool ShipyardLayer::init() {
    if (Layer::init()) {
        load_position_datas();
        add_menu_items();
        add_touch_listener();
        
        // background image
        auto sprite = Sprite::create("shipyardbg.png");
        sprite->setPosition(Point(winSize.width/2,winSize.height/2));
        this->addChild(sprite, -2);

        return true;
    }
    return false;
}

/* when dragging a label at the item bar, actually a new label is created by this function */
cocos2d::Label * ShipyardLayer::copy_label(cocos2d::Label * src) {
    Label * ret = Label::createWithSystemFont(src->getString(), MENUITEM_FONTNAME, ITEMLABEL_FONTSIZE);
    ret->setColor(Color3B(255,255,255));
    selected_label_origin_position = src->getPosition();
    ret->setPosition(src->getPosition());
    this->addChild(ret);
    return ret;
}

/* remove the red square always be as the target of the weapons */
void ShipyardLayer::remove_idiots() {
    for (std::map<Weapon *, StupidEnemy *>::iterator it = abuse_map.begin(); it != abuse_map.end(); ++it) {
        if (it->second == nullptr) continue;
        it->second->unscheduleUpdate();
        this->removeChild(it->second);
    }
    abuse_map.clear();
}

void ShipyardLayer::bind_idiot(Weapon *w, StupidEnemy *se) {
    bool newone = false;
    if (se == nullptr) {
        newone = true;
        se = StupidEnemy::create();
    }
    se->abuser = w;
    abuse_map[w] = se;
    w->current_target_sprite = se;
    if (newone) {
        this->addChild(se);
        se->scheduleUpdate();
    }
    se->reset_direction_and_bound(w);
    //SET SE POSITION
    // done in the se update function
}

bool ShipyardLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    Point pos = touch->getLocation();
    if (pos.y < ITEMBOX_SIZE) {
        selected_box = pos.x / ITEMBOX_SIZE;
        /* check the contents in the item bar, ships or weapons */
        switch (status) {
            case SHIPS:
                if (selected_box < Player::getInstance().airships.size()) {
                    /* if changing the selected airship */
                    right_ship->remove_projectiles_from_layer();
                    right_ship->setVisible(false);
                    remove_idiots();
                    Player::getInstance().selected_airship =
                    Player::getInstance().airships.at(selected_box);
                    pre_set_right_ship(Player::getInstance().selected_airship);
                    add_right_ship_to_child();
                    get_left_ship_position();
                }
                break;
            case WEAPONS:
                if (selected_box < item_weapons.size() && selected_label == nullptr) {
                    selected_label = copy_label(item_weapons.at(selected_box));
                }
                break;
        }
    } else {
        /* if no item selected, reset the vars */
        in_a_slot(touch->getLocation());
        selected_box = -1;
        selected_label = nullptr;
    }
    return true;
}

void ShipyardLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (selected_label != nullptr) {
        selected_label->setPosition(touch->getLocation());
    }
    in_a_slot(touch->getLocation());
}

int ShipyardLayer::in_a_slot(cocos2d::Point position) {
    int find = -1;
    for (auto & slot : slots_buf) {
        if (slot.origin.x-SLOT_SELECTED_RECT_HSIZE < position.x &&
            slot.origin.y+SLOT_SELECTED_RECT_HSIZE > position.y &&
            slot.dest.x+SLOT_SELECTED_RECT_HSIZE > position.x &&
            slot.dest.y-SLOT_SELECTED_RECT_HSIZE < position.y) {
            find = slot.index;
            slot.selected = SLOT_SELECTED_RECT_HSIZE;
        } else {
            slot.selected = 0;
        }
    }
    return find;
}

void ShipyardLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (selected_label != nullptr) {
        /* slot id */
        int index;
        if ((index = in_a_slot(touch->getLocation())) >= 0) {
            auto weapon = right_ship->weapons[index];
            // bug fix (done) : se may be null here
            auto se = abuse_map[weapon];
            
            /* add weapon to idles */
            if (weapon != nullptr) {
                weapon->current_target_sprite = nullptr;
                //LLLBBB
                weapon->reset();
                weapon->unschedule(schedule_selector(Weapon::just_shoot));
                abuse_map[weapon] = nullptr;
                se->abuser = nullptr;
                this->removeChild(weapon);
                Player::getInstance().idleweapons.push_back(weapon);
            }
            /* install weapon to airship */
            if (selected_label->getString().compare("null")) {
                weapon = Player::getInstance().idleweapons.at(selected_box);
                weapon->set_self(right_ship, index);
                right_ship->weapons[index] = weapon;
                bind_idiot(weapon, se);
                //LLLBBB
                weapon->reset();
                weapon->schedule(schedule_selector(Weapon::just_shoot), weapon->data.projfreq);
                weapon->left_num = weapon->data.projnum;
                this->addChild(weapon);
                /* remove from the idle weapons */
                for (int i = 0; i < Player::getInstance().idleweapons.size(); ++i) {
                    if (Player::getInstance().idleweapons[i] == weapon) {
                        Player::getInstance().idleweapons.erase(Player::getInstance().idleweapons.begin()+i);
                        break;
                    }
                }
            } else {
                /* remove the weapon from the airship */
                right_ship->weapons[index] = nullptr;
                if (se != nullptr) {
                    for (auto p : right_ship->projectiles) {
                        Trackings * t = GET_TRACKINGS(p);
                        if (t != nullptr && t->target == se) {
                            t->target = nullptr;
                        }
                        LaserBeam * lb = GET_LASERBEAM(p);
                        if (lb != nullptr && lb->target == se) {
                            lb->target = nullptr;
                        }
                    }
                    se->abuser = nullptr;
                    se->unscheduleUpdate();
                    this->removeChild(se);
                }
            }
            get_attack_bar();
            get_item_weapons();
            this->removeChild(selected_label);
            selected_label = nullptr;
        } else {
            /* if the label is not dropped in a slot, release the memory */
            if (touch->getLocation().distance(selected_label_origin_position) > ITEMBOX_SIZE) {
                selected_label->runAction(Sequence::create(MoveTo::create(0.2, selected_label_origin_position), CallFuncN::create([&](Node *sender) {
                    this->removeChild(sender);
                    selected_label = nullptr;
                }), NULL));
            } else {
                this->removeChild(selected_label);
                selected_label = nullptr;
            }
        }
    }
    for (SlotRecord & s : slots_buf) s.selected = 0;
    selected_box = -1;
}

void ShipyardLayer::draw (cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags) {
    Layer::draw(renderer, transform, flags);
    _customCommand.init(SETUP_WEAPON_LAYER_INDEX);
    _customCommand.func = CC_CALLBACK_0(ShipyardLayer::draw_setup_weapons_layer, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void ShipyardLayer::draw_setup_weapons_layer(const cocos2d::Mat4 &transform, uint32_t flags) {
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    /* shared variables */
    int i;
    
    /* draw left ship */
    DrawPrimitives::setDrawColor4B(56, 223, 237, 255);
    Airship * airship = Player::getInstance().selected_airship;
    glLineWidth(STROKE_WIDTH);
    for (i = 0; i < airship->shapes.size(); ++i) {
        DrawPrimitives::drawPoly(left_points_buf[i], airship->nums[i], true);
    }
    DrawPrimitives::setDrawColor4B(255, 255, 255, 255);
    for (auto r : slots_buf) {
        DrawPrimitives::drawRect(r.origin+Point(-r.selected,r.selected), r.dest+Point(r.selected,-r.selected));
    }
    
    /* draw item bar */
    switch (status) {
        case SHIPS:
            int sindex;
            for (i = 0; i < Player::getInstance().airships.size(); ++i) {
                DrawPrimitives::drawRect(Point(i * ITEMBOX_SIZE, ITEMBOX_SIZE), Point((i+1)*ITEMBOX_SIZE, 0));
                if (Player::getInstance().airships.at(i) == airship) {
                    sindex = i;
                }
            }
            DrawPrimitives::setDrawColor4B(128, 0, 0, 255);
            DrawPrimitives::drawRect(Point(sindex * ITEMBOX_SIZE + STROKE_WIDTH, ITEMBOX_SIZE - STROKE_WIDTH),
                                          Point((sindex+1)*ITEMBOX_SIZE - STROKE_WIDTH, STROKE_WIDTH));
            DrawPrimitives::drawSolidRect(Point(Player::getInstance().airships.size() * ITEMBOX_SIZE + STROKE_WIDTH, ITEMBOX_SIZE), Point(winSize.width, 0), Color4F(0.5,0.5,0.5,0.5));
            break;
        case WEAPONS:
            for (i = 0; i < item_weapons.size(); ++i) {
                DrawPrimitives::drawRect(Point(i * ITEMBOX_SIZE, ITEMBOX_SIZE), Point((i+1)*ITEMBOX_SIZE, 0));
            }
            DrawPrimitives::drawSolidRect(Point(item_weapons.size() * ITEMBOX_SIZE + STROKE_WIDTH, ITEMBOX_SIZE), Point(winSize.width, 0), Color4F(0.5,0.5,0.5,0.5));
            break;
    }
    
    /* draw right ship */
    if (right_ship) {
        right_ship->drawElements();
        for (auto w : right_ship->weapons) {
            if (w == nullptr) continue;
            if (!w->projectile_data.track) continue;
            
        }
        DrawPrimitives::drawSolidPoly(attack_bar.points, 4, Color4F(0.8, 0.1, 0.1, 1));
        DrawPrimitives::setDrawColor4B(240, 240, 240, 255);
        DrawPrimitives::drawLine(attack_bar.points[3]+Point(0,-3), attack_bar.points[3]+Point(ATTACK_BAR_WIDTH,-3));
    }
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void ShipyardLayer::ships_item_call_back(cocos2d::Ref *pSender) {
    status = SHIPS;
    set_vector_visible(item_ships, true);
    set_vector_visible(item_weapons, false);
}

void ShipyardLayer::weapons_item_call_back(cocos2d::Ref *pSender) {
    status = WEAPONS;
    set_vector_visible(item_ships, false);
    set_vector_visible(item_weapons, true);
}

void ShipyardLayer::release_item_weapons() {
    for (auto a : item_weapons) {
        this->removeChild(a);
    }
    item_weapons.clear();
}

void ShipyardLayer::release_memory() {
    for (auto a : item_ships) {
        this->removeChild(a);
    }
    item_ships.clear();
    for (auto a : selected_ships_buf) {
        this->removeChild(a);
    }
    remove_idiots();
    remove_right_ship_projectiles();
    release_item_weapons();
    selected_ships_buf.clear();
    release_buf();
}

void ShipyardLayer::back_item_call_back(cocos2d::Ref *pSender) {
    release_memory();
    _eventDispatcher->removeEventListener(contact_listener);
    Director::getInstance()->replaceScene(((AppDelegate *)Application::getInstance())->getGameManageInterfaceScene());
}

void ShipyardLayer::remove_right_ship_projectiles() {
    if (right_ship) {
        right_ship->remove_projectiles_from_layer();
        for (auto &weapon : right_ship->weapons) {
            if (weapon == nullptr) continue;
            weapon->unschedule(schedule_selector(Weapon::just_shoot));
            this->removeChild(weapon);
        }
    }
}

void ShipyardLayer::get_left_ship_position() {
    int i, j;
    Airship * airship = Player::getInstance().selected_airship;
    release_buf();
    for (i = 0; i < airship->shapes.size(); ++i) {
        Vec2* tmp = new Vec2[airship->nums[i]];
        for (j = 0; j < airship->nums[i]; ++j) {
            tmp[j].x = airship->shapes[i][j].x * left_ship_scale + LEFT_SHIP_OFFSET.x;
            tmp[j].y = airship->shapes[i][j].y * left_ship_scale + LEFT_SHIP_OFFSET.y;
        }
        left_points_buf.push_back(tmp);
    }
    
    /* weapon slot buf */
    slots_buf.clear();
    Point position;
    WeaponSlot slot;
    for (i = 0; i < airship->data.weapon_slots.size(); ++i) {
        slot = airship->data.weapon_slots.at(i);
        position = LEFT_SHIP_OFFSET +
        Tools::offset_according_to_radius_and_degree(slot.linearoffset, Tools::modifyDegreeIntoRange(slot.angleoffset)) * left_ship_scale;
        Rect rect(position.x, position.y, SLOT_ORIGIN_RECT_HSIZE * 2 , SLOT_ORIGIN_RECT_HSIZE * 2);
        slots_buf.push_back(SlotRecord(Point(position.x-SLOT_ORIGIN_RECT_HSIZE, position.y+SLOT_ORIGIN_RECT_HSIZE), Point(position.x+SLOT_ORIGIN_RECT_HSIZE, position.y-SLOT_ORIGIN_RECT_HSIZE), i));
    }
}

void ShipyardLayer::get_item_ships() {
    for (int i = 0; i < Player::getInstance().airships.size(); ++i) {
        Sprite * sprite = Sprite::create(Player::getInstance().airships.at(i)->data.image);
        sprite->setScale(item_ship_scale, item_ship_scale);
        sprite->setPosition(Point(i * ITEMBOX_SIZE + ITEMBOX_SIZE / 2, ITEMBOX_SIZE / 2));
        item_ships.pushBack(sprite);
        this->addChild(sprite, SETUP_WEAPON_LAYER_INDEX + 5);
        sprite->setVisible(false);
    }
}

std::string ShipyardLayer::weapon_to_string(Weapon *weapon) {
    return Tools::string_format("projectile = %d\nlinear range = %.1f\nangle range=%.1f", weapon->projectile_data.type, weapon->data.linearrange, weapon->data.anglerange);
}

void ShipyardLayer::get_item_weapons() {
    release_item_weapons();
    for (int i = 0; i < Player::getInstance().idleweapons.size(); ++i) {
        auto weapon = Player::getInstance().idleweapons.at(i);
        Label * label = Label::createWithSystemFont(weapon_to_string(weapon), MENUITEM_FONTNAME, ITEMLABEL_FONTSIZE);
        label->setColor(Color3B(255,255,255));
        label->setPosition(Point(i*ITEMBOX_SIZE + ITEMBOX_SIZE / 2, ITEMBOX_SIZE / 2));
        item_weapons.pushBack(label);
        this->addChild(label, SETUP_WEAPON_LAYER_INDEX + 5);
    }
    Label * label = Label::createWithSystemFont("null", MENUITEM_FONTNAME, ITEMLABEL_FONTSIZE);
    label->setColor(Color3B(255,255,255));
    label->setPosition(Point(Player::getInstance().idleweapons.size()*ITEMBOX_SIZE + ITEMBOX_SIZE / 2, ITEMBOX_SIZE / 2));
    item_weapons.pushBack(label);
    this->addChild(label, SETUP_WEAPON_LAYER_INDEX + 5);
}

void ShipyardLayer::pre_set_right_ship(Airship * ship) {
    if (right_ship != ship) {
        remove_right_ship_projectiles();
        right_ship = ship;
        get_attack_bar();
        for (auto &weapon : right_ship->weapons) {
            if (weapon == nullptr) continue;
            weapon->left_num = weapon->data.projnum;
            //LLLBBB
            weapon->reset();
            this->addChild(weapon);
            /* add idiot */
            bind_idiot(weapon, nullptr);
            
            weapon->schedule(schedule_selector(Weapon::just_shoot), weapon->data.projfreq);
        }
    }
    right_ship->setRotation(0);
    right_ship->setPosition(Point(RIGHT_SHIP_XOFFSET, RIGHT_SHIP_YOFFSET));
}

void ShipyardLayer::add_right_ship_to_child() {
    if (selected_ships_buf.find(right_ship) == selected_ships_buf.end()) {
        right_ship->current_hp = right_ship->total_hp;
        selected_ships_buf.insert(right_ship);
        right_ship->setVisible(true);
        this->addChild(right_ship);
    } else {
        right_ship->setVisible(true);
    }
}

/* recompute the length of the attack bar */
void ShipyardLayer::get_attack_bar() {
    float width = 0;
    for (auto w : right_ship->weapons) {
        if (w == nullptr) continue;
        width += w->projectile_data.power;
    }
    width = width / MAX_ATTACK_VALUE * ATTACK_BAR_WIDTH;
    attack_bar.set_trapezoid(ATTACK_BAR_LEFT_POSITION, width, ATTACK_BAR_HEIGHT);
}

void ShipyardLayer::reset() {
    static bool firsttime = true;
    if (firsttime) {
        MessageBox("Please drag the bottom weapons to the left airship to replace the weapon. Drag the 'null' instead to uninstall the weapon.", "Important");
        firsttime = false;
    }
    get_left_ship_position();
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contact_listener, this);
    get_item_ships();
    get_item_weapons();
    /* add right ship */
    right_ship = nullptr;
    pre_set_right_ship(Player::getInstance().selected_airship);
    add_right_ship_to_child();
    selected_label = nullptr;
    selected_box = -1;
    status = WEAPONS;
}

bool ShipyardScene::init() {
    if (Scene::init()) {
        this->_layer = ShipyardLayer::create();
        this->addChild(_layer, 1);
        return true;
    }
    return false;
}

ShipyardScene::~ShipyardScene() {
    if (_layer) {
        this->removeChild(_layer);
        _layer = NULL;
    }
}
