//
//  Path.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-15.
//
//

#ifndef __Loire__Path__
#define __Loire__Path__

#include <iostream>
#include "cocos2d.h"
#include "Tools.h"

class Path
{
public:
    static const int MAX_POINTS = 50;
    constexpr static const float DOTS_MIN_DISTANCE = 20.0f;
    
private:
    Path();
    Path(Path const&);
    void operator=(Path const&);
    
public:
    int size;
    cocos2d::Sprite* dots[MAX_POINTS];
    
    static Path& getInstance() {
        static Path _path;
        return _path;
    }
    
    static void Release() {
        Path& _path = Path::getInstance();
        for (int i = 0; i < Path::MAX_POINTS; ++i) {
            _path.dots[i]->release();
        }
    }
    
    bool dotsInSize();
    cocos2d::Sprite* addPathDot(const cocos2d::Point &position);
    cocos2d::Sprite* back();
    void clear_dots();
    
};


#endif /* defined(__Loire__Path__) */
