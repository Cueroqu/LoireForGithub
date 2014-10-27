//
//  Tools.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-10.
//
//

#ifndef __Loire__Tools__
#define __Loire__Tools__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "pugixml.hpp"
#include "SystemEvent.h"
#include "VisibleRect.h"

class Tools {
public:
    //DEBUG
    static int get_hit_num;
    
    static cocos2d::Rect getTouchSquareByPosition(cocos2d::Sprite *spriete);
    static void fswap(float &a, float &b);
    static void formatPoint(cocos2d::Point p);
    static float modifyDegreeIntoRange(float degree, float rangeLB_inclu = -180, float rangeHB_exclu = 180);
    static cocos2d::Point offset_according_to_radius_and_degree(float linear, float degree);
    static cocos2d::Point get_point_from_xml(pugi::xml_node node, float width = winSize.width, float height = winSize.height);
    static inline float get_rotation_from_xml(pugi::xml_node node) {
        return node.attribute("rotation").as_float();
    }
    static float area2(cocos2d::Point p, cocos2d::Point q, cocos2d::Point k);
    static bool to_left(cocos2d::Point p, cocos2d::Point q, cocos2d::Point k);
    static bool within_distance(cocos2d::Point p, cocos2d::Point q, float dis);
    static bool float_equals(float a, float b);
    static cocos2d::Rect get_sprite_rect(cocos2d::Sprite *sprite);
    static pugi::xml_parse_result load_xml_document(std::string filename, pugi::xml_document &doc);
    static int strincasecmp(const char* str1, const char* str2);
    static SystemEvent* systemevent_factory(pugi::xml_node event_node);
    static void load_game_events(std::string filename);
    static std::string string_format(const std::string fmt_str, ...);
    static cocos2d::Point get_a_positive_projection_on_b(cocos2d::Point a, cocos2d::Point b);
    inline static float get_positive_cosine(cocos2d::Point a, cocos2d::Point b) {
        return MAX(cosf(a.getAngle(b)), 0);
    }
    static cocos2d::Point a_direction_vector_with_length_b(cocos2d::Point a, float b);
};

#endif /* defined(__Loire__Tools__) */
