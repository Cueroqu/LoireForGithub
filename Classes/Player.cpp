//
//  Player.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-16.
//
//

#include "Player.h"
#include "cocos2d.h"
#include "Tools.h"
USING_NS_CC;

Player::Player() {
    selected_airship = NULL;
    pugi::xml_document doc;
//    if (CCFileUtils::getInstance()->isFileExist("player.xml")) printf("yes fadsfskaf\n");
    pugi::xml_parse_result result = Tools::load_xml_document("player.xml", doc);
    if (result) {
        pugi::xml_node player_node = doc.child("player");
        person = Sprite::create(player_node.attribute("image").value());
        person->retain();
        load_idle_weapons(player_node.child("idleweapons"));
        AirshipData data;
        for (pugi::xml_node ship_node = player_node.child("airships").first_child(); ship_node; ship_node = ship_node.next_sibling())  {
            data = GET_TYPE_VALUE(Airship, ship_node, "airship");
            Airship* ship = Airship::create(data.image.c_str());
            ship->setTag(PLAYER_SHIP_TAG);
            ship->data = data;
            ship->get_shapes(data.name);
            ship->initiate_shipbase(ship_node);
            if (ship_node.attribute("selected").as_bool()) {
                selected_airship = ship;
            }
            airships.pushBack(ship);
        }
    } else {
        MessageBox("Read Player.xml failed", "Error");
    }
}

void Player::load_idle_weapons(pugi::xml_node node) {
    for (pugi::xml_node weapon_node = node.first_child(); weapon_node; weapon_node = weapon_node.next_sibling()) {
        Weapon * weapon = Weapon::createWithXmlNode(weapon_node);
        weapon->retain();
        idleweapons.push_back(weapon);
    }
}
