//
//  SystemEventPool.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#ifndef __Loire__SystemEventPool__
#define __Loire__SystemEventPool__

#include <iostream>
#include <deque>
#include "SystemEvent.h"

class SystemEventPool {
private:
    SystemEventPool();
    SystemEventPool(SystemEventPool const&);
    void operator=(SystemEventPool const&);

public:
    static SystemEventPool& getInstance() {
        static SystemEventPool _pool;
        return _pool;
    }
    static void Release() {
    }
    void start_event();
    void skip_event();
    void start_event(SystemEvent* event);
    void clear_event_pool();
    std::deque<SystemEvent *> event_pool;
    SystemEvent * current_event;
};

#endif /* defined(__Loire__SystemEventPool__) */
