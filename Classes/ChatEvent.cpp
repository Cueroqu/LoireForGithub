//
//  ChatEvent.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#include "ChatEvent.h"
#include "pugixml/pugixml.hpp"

ChatEvent::ChatEvent() {
    this->type = ChatEventType::VOICEOVER;
    this->content = "[No content]";
}

ChatEvent::ChatEvent(ChatEventType et, const std::string c) {
    this->type = et;
    this->content = c;
}

ChatEvent::ChatEvent(pugi::xml_node chat_node) {
    if (!strcasecmp(chat_node.attribute("type").value(), "left"))
        this->type = ChatEventType::LEFT;
    else if (!strcasecmp(chat_node.attribute("type").value(), "right"))
        this->type = ChatEventType::RIGHT;
    else if (!strcasecmp(chat_node.attribute("type").value(), "both"))
        this->type = ChatEventType::BOTH;
    else
        this->type = ChatEventType::VOICEOVER;
    this->content = chat_node.attribute("content").value();
}
