//
//  Arsenal.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-26.
//
//

#include "Arsenal.h"
#include "pugixml.hpp"
#include "Tools.h"
#include "typedefs.h"

std::map<AirshipType, AirshipData> Arsenal::AirshipMap;
std::map<WeaponType, WeaponData> Arsenal::WeaponMap;
std::map<ProjectileType, ProjectileData> Arsenal::ProjectileMap;
std::map<TechmodType, TechmodFunction> Arsenal::TechmodMap;

void Arsenal::load_types() {
    load_airship_types();
    load_weapon_types();
    load_projectile_types();
}

void Arsenal::load_airship_types() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document("AirshipType.xml", doc);
    if (result) {
        for (pugi::xml_node airship_node = doc.first_child().first_child(); airship_node; airship_node = airship_node.next_sibling()) {
            AirshipData data;
            data.type = (AirshipType)airship_node.attribute("type").as_int();
            data.image = airship_node.attribute("image").value();
            data.texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(data.image);
            data.name = data.image.substr(0, data.image.find("."));
            data.json = data.name + ".json";
            data.movespeed = airship_node.attribute("movespeed").as_float();
            data.rotatespeed = airship_node.attribute("rotatespeed").as_float();
            data.linear_acceleration = airship_node.attribute("linearacc").as_float();
            data.angular_acceleration = airship_node.attribute("angacc").as_float();
            data.weapon_slotnumber = airship_node.attribute("weaponslotnum").as_int();
            data.techmodnum = airship_node.attribute("techmodnum").as_int();
            for (pugi::xml_node slot_node = airship_node.child("weaponslots").first_child(); slot_node; slot_node = slot_node.next_sibling()) {
                WeaponSlot slot_data;
                slot_data.angleoffset = slot_node.attribute("angleoffset").as_float();
                slot_data.linearoffset = slot_node.attribute("linearoffset").as_float();
                data.weapon_slots.push_back(slot_data);
            }
            AirshipMap[data.type] = data;
        }
    }
}

void Arsenal::load_weapon_types() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document("WeaponType.xml", doc);
    if (result) {
        for (pugi::xml_node weapon_node = doc.first_child().first_child(); weapon_node; weapon_node = weapon_node.next_sibling()) {
            WeaponData data;
            data.type = (WeaponType)weapon_node.attribute("type").as_int();
            data.anglerange = weapon_node.attribute("anglerange").as_float();
            data.linearrange = weapon_node.attribute("linearrange").as_float();
            data.cdtime = weapon_node.attribute("cdtime").as_float();
            data.projfreq = MAX(weapon_node.attribute("projfreq").as_float(), WeaponData::MIN_PROJFREQ);
            data.projnum = weapon_node.attribute("projnum").as_int();
            data.parallel = weapon_node.attribute("parallel").as_int();
            data.interval = weapon_node.attribute("interval").as_float();
            data.projlinear = weapon_node.attribute("projlinear").as_float();
            if (Tools::float_equals(data.projlinear, 0))
                data.projlinear = 1;
            data.projlinear *= data.linearrange;
            WeaponMap[data.type] = data;
        }
    }
}

void Arsenal::load_projectile_types() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document("ProjectileType.xml", doc);
    if (result) {
        for (pugi::xml_node proj_node = doc.first_child().first_child(); proj_node; proj_node = proj_node.next_sibling()) {
            ProjectileData data;
            data.type = (ProjectileType)proj_node.attribute("type").as_int();
            data.image = proj_node.attribute("image").value();
            data.texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(data.image);
            data.power = proj_node.attribute("power").as_int();
            data.speed = proj_node.attribute("speed").as_float();
            data.track = proj_node.attribute("track").as_bool();
            data.rotate = proj_node.attribute("rotate").as_float();
            data.linear_acceleration = proj_node.attribute("linearacc").as_float();
//            if (data.linear_acceleration < EPS) data.linear_acceleration = MAX_LINEAR_ACCR;
            data.angular_acceleratoin = proj_node.attribute("angacc").as_float();
            if (data.angular_acceleratoin < EPS) data.angular_acceleratoin = MAX_ANGULAR_ACCR;
            ProjectileMap[data.type] = data;
        }
    }
}

void Arsenal::load_techmod_types() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document("TechmodType.xml", doc);
    if (result) {
        for (pugi::xml_node tech_node = doc.first_child().first_child(); tech_node; tech_node = tech_node.next_sibling()) {
            TechmodData data;
            data.type = (TechmodType)tech_node.attribute("type").as_int();
            switch (data.type) {
                case RESPRING:
                    TechmodMap[data.type] = Arsenal::respring;
                    break;
                default:
                    break;
            }
        }
    } else {
        cocos2d::MessageBox(Tools::string_format("Load TechmodType.xml error: %s",result.description()).c_str(), "Error");
    }
}

void Arsenal::respring(cocos2d::Sprite *pSender) {
    // I DON'T KNOW HOW TO GET SHIPBASE VALUE HERE
}
