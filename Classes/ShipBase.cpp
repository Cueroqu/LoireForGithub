//
//  ShipBase.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-18.
//
//

#include "ShipBase.h"
#include "cocos2d.h"
#include "Tools.h"
#include "MyBodyParser.h"
#include "AppDelegate.h"
#include "LaserBeam.h"
USING_NS_CC;

void ShipBase::drawElements(bool draw_weapon) {
    draw_hp_bar();
    drawWeapons(draw_weapon);
}

void ShipBase::drawWeapons(bool ifdraw) {
    for (Weapon* w: this->weapons) {
        if (w == nullptr) continue;
        w->draw_self(ifdraw);
    }
}

void ShipBase::initiate_shipbase(pugi::xml_node node) {
    this->total_hp = this->current_hp = node.attribute("hp").as_int();
    this->speed_buff = this->attack_buff = 1;
    int i = 0;
    this->weapons.resize(this->data.weapon_slotnumber, nullptr);
    for (pugi::xml_node weapon_node = node.child("Weapons").first_child(); weapon_node && i < this->data.weapon_slotnumber; weapon_node = weapon_node.next_sibling(), ++i) {
        Weapon* weapon = Weapon::createWithXmlNode(weapon_node);
        /* NOTE!!! : in the xml file, weapon slot id should start from 1, for 0 is the mark for nonexistence */
        weapon->slot_id = weapon_node.attribute("id").as_int();
        if (!weapon->slot_id) weapon->slot_id = i;
        else --weapon->slot_id;
        if (weapons[weapon->slot_id] != nullptr) continue;
        weapon->retain();
        weapon->set_self(this, weapon->slot_id);
        weapons[weapon->slot_id] = weapon;
    }
}

void ShipBase::draw_hp_bar() {
    Point hp_bar_position = this->getPosition();
    hp_bar_position.y = hp_bar_position.y + this->getContentSize().height / 2 + 20;
    glLineWidth(ShipBase::HPBARHEIGHT);
    Point leftcenter(hp_bar_position.x - ShipBase::HPBARLENGTH / 2, hp_bar_position.y);
    Point rightcenter(hp_bar_position.x + ShipBase::HPBARLENGTH / 2, hp_bar_position.y);
    
    DrawPrimitives::setDrawColor4B(255, 255, 255, 255);
    DrawPrimitives::drawRect(leftcenter+Point(-HPBARHEIGHT/2,HPBARHEIGHT/2), rightcenter+Point(HPBARHEIGHT/2,-HPBARHEIGHT/2));
    
    DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
    DrawPrimitives::drawLine(leftcenter,
                             Point(hp_bar_position.x + ShipBase::HPBARLENGTH * (MAX(this->current_hp, 0) / this->total_hp - 0.5f), hp_bar_position.y));
}

void ShipBase::remove_projectiles_from_layer() {
    Trackings * t;
    LaserBeam * lb;
    for (auto p : projectiles) {
        t = GET_TRACKINGS(p);
        if (t != nullptr) t->unscheduleUpdate();
        else {
            lb = GET_LASERBEAM(p);
            if (lb != nullptr) {
                //LLLBBB
                lb->unschedule_update_without_removeself();
            }
        }
        p->getParent()->removeChild(p);
    }
    projectiles.clear();
}

void ShipBase::releaseElements() {
    releaseWeapons();
}

void ShipBase::releaseWeapons() {
    for (auto w : this->weapons) {
        if (w)
            w->release();
    }
    this->weapons.clear();
}

void ShipBase::movement(float t) {
}

void ShipBase::set_physics_body() {
    MyBodyParser::getInstance()->parseJsonFile(this->data.json);
    auto _body = MyBodyParser::getInstance()->bodyFormJson(this, this->data.name);
    if (_body != nullptr) {
        _body->setContactTestBitmask(-1);
        this->setPhysicsBody(_body);
    }
    else {
        MessageBox(Tools::string_format("%s, get json file error", this->data.name.c_str()).c_str(), "Error");
    }
}
