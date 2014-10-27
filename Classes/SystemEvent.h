//
//  SystemEvent.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#ifndef __Loire__SystemEvent__
#define __Loire__SystemEvent__

#include <iostream>
#include <string>
#include "pugixml/pugixml.hpp"
#include "SystemEventType.h"

class SystemEvent {
public:
    SystemEvent();
    SystemEvent(pugi::xml_node event_node);
    SystemEvent(SystemEventType _type, std::string filename);
    SystemEventType type;
    std::string related_filename;
};

#endif /* defined(__Loire__SystemEvent__) */
