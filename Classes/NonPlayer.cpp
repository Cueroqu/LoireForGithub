//
//  NonPlayer.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#include <iostream>
#include <map>
#include "pugixml/pugixml.hpp"
#include "NonPlayer.h"
#include "cocos2d.h"
#include "Tools.h"
#include "Player.h"
USING_NS_CC;

std::map<std::string, NonPlayer> NonPlayer::non_players;

void NonPlayer::load_npcs() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document("npcs.xml", doc);
    if (result) {
        pugi::xml_node npcs_node = doc.child("npcs");
        for (pugi::xml_node npc_node = npcs_node.first_child(); npc_node; npc_node = npc_node.next_sibling()) {
            NonPlayer tmp(npc_node.attribute("name").value(), npc_node.attribute("image").value());
            NonPlayer::non_players[tmp.name] = tmp;
        }
    }
    NonPlayer np("player", Player::getInstance().person);
    NonPlayer::non_players["player"] = np;
}

NonPlayer::NonPlayer(std::string _name, Sprite* _image) {
    name = _name;
    image = _image;
}

NonPlayer::NonPlayer(std::string _name, const char* filename) {
    name = _name;
    image = Sprite::create(filename);
    image->retain();
}

