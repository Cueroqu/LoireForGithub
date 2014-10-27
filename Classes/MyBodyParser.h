//
//  MyBodyParser.h
//
//  Created by Jason Xu.
//
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "cocos2d.h"
USING_NS_CC;
#include "json/document.h"

class MyBodyParser {
    MyBodyParser(){}
    rapidjson::Document doc;
public:
    static MyBodyParser* getInstance();
    bool parseJsonFile(const std::string& pFile);
    bool parse(unsigned char* buffer, long length);
    void clearCache();
    PhysicsBody* bodyFormJson(Node* pNode, const std::string& name);
    std::vector<cocos2d::Vec2 * > get_shape_from_json(cocos2d::Node *pNode, const std::string & name, std::vector<int> & nums);
};