//
//  ChatScene.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-22.
//
//

#ifndef __Loire__ChatScene__
#define __Loire__ChatScene__

#include <iostream>
#include <deque>
#include "ChatEvent.h"
#include "cocos2d.h"
#include "LabelLayer.h"
#include "NPCLayer.h"
#include "ChatSystemEvent.h"

enum ChatSceneState {NOTHING, LEFTIN, RIGHTIN, BOTHIN};

class ChatScene : public cocos2d::Scene {
public:
    ChatScene() : _labelLayer(NULL) {}
    ~ChatScene();
    CREATE_FUNC(ChatScene);
    virtual bool init() override;
    
    void start_event();
    void load_chat_events(ChatSystemEvent* event_node);
    void load_final_chat();
    void set_left(ChatEventType type);
    void set_right(ChatEventType type);
    void finish_chats();
    
    ChatSceneState state;
    std::deque<ChatEvent> chat_events;
    CC_SYNTHESIZE_READONLY(LabelLayer *, _labelLayer, LabelLayer);
    NPCLayer* npc_layers[2];    // 0 is left, 1 is right
};

#endif /* defined(__Loire__ChatScene__) */
