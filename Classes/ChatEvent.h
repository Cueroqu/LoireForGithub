//
//  ChatEvent.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#ifndef __Loire__ChatEvent__
#define __Loire__ChatEvent__

#include <iostream>
#include "pugixml/pugixml.hpp"

enum ChatEventType {VOICEOVER, LEFT, RIGHT, BOTH};

class ChatEvent {
public:
    ChatEventType type;
    std::string content;
    
    ChatEvent ();
    ChatEvent (pugi::xml_node chat_node);
    ChatEvent (ChatEventType et, const std::string c);
};

#endif /* defined(__Loire__ChatEvent__) */
