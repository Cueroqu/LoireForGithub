//
//  BackgroundLayer.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-5.
//
//

#include "BackgroundLayer.h"
#include "VisibleRect.h"

void BackgroundLayer::initWithFile(const std::string & filename) {
    this->texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(filename);
    for (int i = 0; i < 9; ++i) {
        bgs[i] = cocos2d::Sprite::createWithTexture(texture);
        this->addChild(bgs[i], 0);
    }
    width = bgs[0]->getContentSize().width;
    half_width = width / 2;
    height = bgs[0]->getContentSize().height;
    half_height = height / 2;

    /*
     * 6 7 8
     * 3 4 5
     * 0 1 2
     */
    for (int i = 0; i < 9; ++i) {
        bgs[i]->setPosition((i%3)*width, (i/3)*height);
    }
}

void BackgroundLayer::schedule_row(float t) {
    cocos2d::Sprite * tmp;
    if (bgs[0]->getPositionX() - half_width > -tolerance) {
        /* rotate right */
        for (int i = 0; i < 3; ++i) {
            bgs[i*3+2]->setPosition(bgs[i*3]->getPosition() - cocos2d::Point(width, 0));
            tmp = bgs[i*3+2];
            bgs[i*3+2] = bgs[i*3+1];
            bgs[i*3+1] = bgs[i*3];
            bgs[i*3] = tmp;
        }
    } else if (bgs[2]->getPositionX() + half_width < winSize.width + tolerance) {
        /* rotate left */
        for (int i = 0; i < 3; ++i) {
            bgs[i*3]->setPosition(bgs[i*3+2]->getPosition() + cocos2d::Point(width, 0));
            tmp = bgs[i*3];
            bgs[i*3] = bgs[i*3+1];
            bgs[i*3+1] = bgs[i*3+2];
            bgs[i*3+2] = tmp;
        }
    }
}

void BackgroundLayer::schedule_col(float t) {
    cocos2d::Sprite * tmp;
    if (bgs[0]->getPositionY() - half_height > -tolerance) {
        /* rotate up */
        for (int i = 0; i < 3; ++i) {
            bgs[6+i]->setPosition(bgs[i]->getPosition() - cocos2d::Point(0, height));
            tmp = bgs[6+i];
            bgs[6+i] = bgs[3+i];
            bgs[3+i] = bgs[i];
            bgs[i] = tmp;
        }
    } else if (bgs[6]->getPositionY() + half_height < winSize.height + tolerance) {
        /* rotate down */
        for (int i = 0; i < 3; ++i) {
            bgs[i]->setPosition(bgs[6+i]->getPosition() + cocos2d::Point(0, height));
            tmp = bgs[i];
            bgs[i] = bgs[3+i];
            bgs[3+i] = bgs[6+i];
            bgs[6+i] = tmp;
        }
    }
}

void BackgroundLayer::schedule_map(float t) {
    schedule_row(t);
    schedule_col(t);
}
