//
//  Tools.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-10.
//
//

#include "Tools.h"
#include "Reflections.h"
#include "cocos2d.h"
#include "VisibleRect.h"
#include "SystemEvent.h"
#include "SystemEventPool.h"
#include "ChatSystemEvent.h"
#include "pugixml/pugixml.hpp"
#include "typedefs.h"
USING_NS_CC;

//DEBUG
int Tools::get_hit_num;

Rect Tools::getTouchSquareByPosition(Sprite *sprite) {
    float size = MAX(sprite->getContentSize().width, sprite->getContentSize().height) + 80;
    return Rect(sprite->getPositionX() - size/2, sprite->getPositionY() - size/2, size, size);
}

void Tools::fswap(float &a, float &b) {
    float t = a;
    a = b;
    b = t;
}

void Tools::formatPoint(Point p) {
    printf("[x = %f, y = %f]\n", p.x, p.y);
}


float Tools::modifyDegreeIntoRange(float degree, float rangeLB_inclu, float rangeHB_exclu) {
    while (degree <= rangeLB_inclu) degree += 360;
    while (degree > rangeHB_exclu) degree -= 360;
    return degree;
}

Point Tools::offset_according_to_radius_and_degree(float linear, float degree) {
    float radian = CC_DEGREES_TO_RADIANS(-degree);
    float x = linear * cosf(radian);
    float y = linear * sinf(radian);
    return Point(x, y);
}

Point Tools::get_point_from_xml(pugi::xml_node node, float width, float height) {
    float x = node.attribute("x").as_float();
    float y = node.attribute("y").as_float();
    if (x <= 1) x = width * x;
    if (y <= 1) y = height * y;
    return Point(x, y);
}

bool Tools::to_left(cocos2d::Point p, cocos2d::Point q, cocos2d::Point k) {
    return area2(p, q, k) > 0;
}

float Tools::area2(cocos2d::Point p, cocos2d::Point q, cocos2d::Point k) {
    return  p.x * q.y - p.y * q.x   //p*q
          + q.x * k.y - q.y * k.x   //q*k
          + k.x * p.y - k.y * p.x;  //k*p, actually it equals to (q-p)*(k-p)
}

bool Tools::float_equals(float a, float b) {
    if (a > b+EPS) return false;
    if (a < b-EPS) return false;
    return true;
}

bool Tools::within_distance(cocos2d::Point p, cocos2d::Point q, float dis) {
    if (p.getDistance(q) < dis) return true;
    return false;
}

Rect Tools::get_sprite_rect(cocos2d::Sprite *sprite) {
    Rect ret(sprite->getPositionX() - sprite->getContentSize().width / 2,
                     sprite->getPositionY() - sprite->getContentSize().height / 2,
                     sprite->getContentSize().width,
                     sprite->getContentSize().height);
    return ret;
}

pugi::xml_parse_result Tools::load_xml_document(std::string filename, pugi::xml_document &doc) {
    auto content = CCFileUtils::getInstance()->getDataFromFile(filename);
    return doc.load_buffer(content.getBytes(), content.getSize());
}

int Tools::strincasecmp(const char *str1, const char *str2) {
    int ret = 0;
    while (!(ret = toupper(*(unsigned char *)str1) - toupper(*(unsigned char *)str2)) && *str2) ++str1, ++str2;
    if (ret < 0) return -1;
    if (ret > 0) return 1;
    return 0;
}

SystemEvent* Tools::systemevent_factory(pugi::xml_node event_node) {
    SystemEvent* ret;
    switch (Reflections::string_to_system_evnet_reflection.get_enum(event_node.attribute("type").value())) {
        case CHAT:
            ret = new ChatSystemEvent(event_node);
            break;
        default:
            ret = new SystemEvent(event_node);
            break;
    }
    return ret;
}

void Tools::load_game_events(std::string filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document(filename, doc);
    if (result) {
        pugi::xml_node script_node = doc.first_child();
        for (pugi::xml_node event_node = script_node.first_child(); event_node; event_node = event_node.next_sibling()) {
            SystemEvent * event = Tools::systemevent_factory(event_node);
            SystemEventPool::getInstance().event_pool.push_back(event);
        }
//        SystemEventPool::getInstance().start_event();
    }
}

std::string Tools::string_format(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2;
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    for (;;) {
        formatted.reset(new char[n]);
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else break;
    }
    return std::string(formatted.get());
}

Point Tools::get_a_positive_projection_on_b(cocos2d::Point a, cocos2d::Point b) {
    Point bn = b.getNormalized();
    Point ret = a.dot(bn) * bn;
    if (ret.x * bn.x < 0) ret.x = 0;
    if (ret.y * bn.y < 0) ret.y = 0;
    return ret;
}

Point Tools::a_direction_vector_with_length_b(cocos2d::Point a, float b) {
    a.x = a.getNormalized().x * b;
    a.y = a.getNormalized().y * b;
    return a;
}
