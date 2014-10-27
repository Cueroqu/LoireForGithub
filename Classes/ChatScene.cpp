//
//  ChatScene.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-22.
//
//

#include "ChatScene.h"
#include "cocos2d.h"
#include "pugixml/pugixml.hpp"
#include "Tools.h"
#include "SystemEvent.h"
#include "SystemEventPool.h"
USING_NS_CC;

bool ChatScene::init() {
    if (Scene::init()) {
        this->state = ChatSceneState::NOTHING;
        
        _labelLayer = LabelLayer::create();
        _labelLayer->retain();
        this->addChild(_labelLayer, 0);
        
        this->npc_layers[0] = new NPCLayer();
        this->npc_layers[0]->retain();
        this->addChild(npc_layers[0], 0);
        
        this->npc_layers[1] = new NPCLayer();
        this->npc_layers[1]->retain();
        this->addChild(npc_layers[1], 0);
        
        return true;
    }
    return false;
}

ChatScene::~ChatScene() {

}

void ChatScene::load_chat_events(ChatSystemEvent* event_node) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document(event_node->related_filename, doc);
    if (result) {
        for (pugi::xml_node chat_node = doc.first_child(); chat_node; chat_node = chat_node.next_sibling()) {
            if (!strcasecmp(chat_node.name(), "chat")) {
                ChatEvent chat_event(chat_node);
                this->chat_events.push_back(chat_event);
            } else if (!strcasecmp(chat_node.name(), "event")) {
                SystemEvent * system_event = Tools::systemevent_factory(chat_node);
                SystemEventPool::getInstance().event_pool.push_front(system_event);
            } else {
                MessageBox(chat_node.name(), "ERROR CHAT NODE NAME");
            }
        }
        
        npc_layers[0]->sprite = event_node->left_sprite;
        npc_layers[0]->sprite->setPosition(cocos2d::Point(npc_layers[0]->sprite->getContentSize().width / 2, 300));

        npc_layers[1]->sprite = event_node->right_sprite;
        npc_layers[1]->sprite->setPosition(cocos2d::Point(winSize.width - npc_layers[1]->sprite->getContentSize().width/2, 300));
    }
}

void ChatScene::load_final_chat() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document("finalchat.xml", doc);
    if (result) {
        for (pugi::xml_node chat_node = doc.first_child(); chat_node; chat_node = chat_node.next_sibling()) {
            if (!strcasecmp(chat_node.name(), "chat")) {
                ChatEvent chat_event(chat_node);
                this->chat_events.push_back(chat_event);
            } else if (!strcasecmp(chat_node.name(), "event")) {
                SystemEvent * system_event = Tools::systemevent_factory(chat_node);
                SystemEventPool::getInstance().event_pool.push_front(system_event);
            } else {
                MessageBox(chat_node.name(), "ERROR CHAT NODE NAME");
            }
        }
    }
}

void ChatScene::set_left(ChatEventType type) {
    switch (this->state) {
        case LEFTIN:
        case BOTHIN:
            if (LEFT != type)
                this->removeChild(npc_layers[0]->sprite);
            break;
        case NOTHING:
        case RIGHTIN:
            if (LEFT == type)
                this->addChild(npc_layers[0]->sprite);
        default:
            break;
    }
}

void ChatScene::set_right(ChatEventType type) {
    switch (this->state) {
        case RIGHTIN:
        case BOTHIN:
            if (RIGHT != type)
                this->removeChild(npc_layers[1]->sprite);
            break;
        case LEFTIN:
        case NOTHING:
            if (RIGHT == type)
                this->addChild(npc_layers[1]->sprite);
        default:
            break;
    }
}

void ChatScene::finish_chats() {
    set_left(ChatEventType::VOICEOVER);
    set_right(ChatEventType::VOICEOVER);
    this->state = NOTHING;
}

void ChatScene::start_event() {
    if (this->chat_events.size() == 0) {
        finish_chats();
        SystemEventPool::getInstance().start_event();
        return ;
    }
    ChatEvent event = this->chat_events.front();
    this->chat_events.pop_front();
    this->_labelLayer->getLabel()->setString(event.content);
    switch (event.type) {
        case LEFT:
            set_left(ChatEventType::LEFT);
            set_right(ChatEventType::LEFT);
            this->state = LEFTIN;
            break;
        case RIGHT:
            set_left(ChatEventType::RIGHT);
            set_right(ChatEventType::RIGHT);
            this->state = RIGHTIN;
            break;
        case BOTH:
            set_left(ChatEventType::LEFT);
            set_right(ChatEventType::RIGHT);
            this->state = BOTHIN;
            break;
        case VOICEOVER:
            set_left(VOICEOVER);
            set_right(VOICEOVER);
            this->state = NOTHING;
        default:
            break;
    }
}

