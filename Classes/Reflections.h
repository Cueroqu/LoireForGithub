//
//  Reflections.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-25.
//
//

#ifndef __Loire__Reflections__
#define __Loire__Reflections__

#include <iostream>
#include <string>
#include <map>
#include "SystemEventType.h"

#define String2SystemEventTypeMap std::map<std::string, SystemEventType>

class String2SystemEventReflection {
public:
    String2SystemEventTypeMap map;
    
    String2SystemEventReflection() {
        map = {
            {"CHAT", SystemEventType::CHAT},
            {"FIGHT", SystemEventType::FIGHT},
            {"BACK", SystemEventType::BACK},
            {"OVER", SystemEventType::OVER}
        };
    }
    
    inline SystemEventType get_enum(std::string type) {
        std::map<std::string, SystemEventType>::iterator it = map.find(type);
        if (it == map.end()) {
            return SystemEventType::INVALID;
        } else return it->second;
    }
};

class Reflections {
public:
    static String2SystemEventReflection string_to_system_evnet_reflection;
};

#endif /* defined(__Loire__Reflections__) */
