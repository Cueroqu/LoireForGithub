//
//  VisibleRect.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-13.
//
//

#ifndef __Loire__VisibleRect__
#define __Loire__VisibleRect__

#include <iostream>
#include "cocos2d.h"

#define VisibleSize Director::getInstance()->getVisibleSize()
#define VisibleOrigin Director::getInstance()->getVisibleOrigin()
#define winSize ((cocos2d::Director::getInstance())->getWinSize())
#define screen_center Point(winSize.width/2, winSize.height/2)

class VisibleRect
{
public:
    static cocos2d::Rect getVisibleRect();
    static cocos2d::Point left();
    static cocos2d::Point right();
    static cocos2d::Point top();
    static cocos2d::Point bottom();
    static cocos2d::Point center();
    static cocos2d::Point leftTop();
    static cocos2d::Point rightTop();
    static cocos2d::Point leftBottom();
    static cocos2d::Point rightBottom();
private:
    static void lazyInit();
    static cocos2d::Rect s_visibleRect;
};


#endif /* defined(__Loire__VisibleRect__) */
