//
//  SystemEvent.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#include <string>
#include "SystemEvent.h"
#include "pugixml/pugixml.hpp"
#include "Reflections.h"

SystemEvent::SystemEvent() {}

SystemEvent::SystemEvent(pugi::xml_node event_node) {
    this->type = Reflections::string_to_system_evnet_reflection.get_enum(event_node.attribute("type").value());
    this->related_filename = event_node.attribute("filename").value();
}

SystemEvent::SystemEvent(SystemEventType _type, std::string filename) {
    this->type = _type;
    this->related_filename = filename;
}
