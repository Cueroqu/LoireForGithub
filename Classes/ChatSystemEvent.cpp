//
//  ChatSystemEvent.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-25.
//
//

#include "ChatSystemEvent.h"
#include "NonPlayer.h"

ChatSystemEvent::ChatSystemEvent() {}

ChatSystemEvent::ChatSystemEvent(pugi::xml_node event_node) {
    this->type = SystemEventType::CHAT;
    this->related_filename = event_node.attribute("filename").value();
    this->left_sprite = NonPlayer::non_players[event_node.attribute("left").value()].image;
    this->right_sprite = NonPlayer::non_players[event_node.attribute("right").value()].image;
}
