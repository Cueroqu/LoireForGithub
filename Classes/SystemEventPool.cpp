//
//  SystemEventPool.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#include "AppDelegate.h"
#include "SystemEventPool.h"
#include "SystemEvent.h"
#include "ChatSystemEvent.h"
USING_NS_CC;

SystemEventPool::SystemEventPool() {
    
}

void SystemEventPool::start_event() {
    if (event_pool.empty()) return;
    SystemEvent* event = event_pool.front();
    event_pool.pop_front();
    start_event(event);
}

void SystemEventPool::skip_event() {
    if (event_pool.empty()) return;
    SystemEvent* event = event_pool.front();
    if (event->type == SystemEventType::OVER) return;
    event_pool.pop_front();
    delete event;
}

void SystemEventPool::start_event(SystemEvent* event) {
    current_event = nullptr;
    switch (event->type) {
        case SystemEventType::CHAT :
            ((AppDelegate *)Application::getInstance())->getChatScene()->load_chat_events((ChatSystemEvent *) event);
            delete event;
            ((AppDelegate *)Application::getInstance())->getChatScene()->start_event();
            Director::getInstance()->replaceScene(((AppDelegate *)Application::getInstance())->getChatScene());
            break;
        case SystemEventType::FIGHT :
            ((AppDelegate *) Application::getInstance())->restartGame(event->related_filename);
            current_event = event;
            break;
        case SystemEventType::BACK:
            ((AppDelegate *) Application::getInstance())->launchGame();
            delete event;
            break;
        case SystemEventType::OVER : {
            ((AppDelegate *)Application::getInstance())->getChatScene()->load_final_chat();
            SystemEvent * fevent = new SystemEvent(SystemEventType::FIGHT, CCFileUtils::getInstance()->getWritablePath()+"hitself.xml");
            event_pool.push_back(fevent);
            event_pool.push_back(event);
            ((AppDelegate *)Application::getInstance())->getChatScene()->start_event();
            Director::getInstance()->replaceScene(((AppDelegate *)Application::getInstance())->getChatScene());
            break;
        }
        default: {
            delete event;
            break;
        }
    }
}

void SystemEventPool::clear_event_pool() {
    for (auto event : event_pool) {
        delete event;
    }
    event_pool.clear();
}
