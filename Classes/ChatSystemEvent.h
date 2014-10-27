//
//  ChatSystemEvent.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-25.
//
//

#ifndef __Loire__ChatSystemEvent__
#define __Loire__ChatSystemEvent__

#include <iostream>
#include "SystemEvent.h"
#include "cocos2d.h"
#include "pugixml/pugixml.hpp"

class ChatSystemEvent : public SystemEvent {
public:
    ChatSystemEvent();
    ChatSystemEvent(pugi::xml_node event_node);
    
    cocos2d::Sprite * left_sprite;
    cocos2d::Sprite * right_sprite;
};

#endif /* defined(__Loire__ChatSystemEvent__) */
