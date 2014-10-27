//
//  GameScene.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-9.
//
//

#include <iostream>
#include <fstream>
#include "GameScene.h"
#include "cocos2d.h"
#include "AppDelegate.h"
#include "Airship.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "Tools.h"
#include "DrawHelper.h"
#include "VisibleRect.h"
#include "pugixml/pugixml.hpp"
#include "SystemEventPool.h"
#include "Arsenal.h"
#include "GameElementsManager.h"
#include "GameOverScene.h"
#include "base/ccFPSImages.h"
#include "Triangle.h"
#include "typedefs.h"
#include "RoadSign.h"
USING_NS_CC;
using namespace std;
using namespace ui;

/*
 * revoked when start a new fight
 * filename: the filname of the xml layout file
 */
bool GameLayer::load_game_scene(std::string filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document(filename, doc);
    if (result) {
        /* initiate variables */
        dragging = false;
        pausing = false;
        slider->setPercent(50);
        
        /* load items */
        pugi::xml_node scene_node = doc.child("gamescene");
        
        Point point;
        
        /* set background */
        pugi::xml_node tmp_node = scene_node.child("background");
        this->_background->initWithFile(tmp_node.attribute("filename").value());
        this->_background->schedule(schedule_selector(BackgroundLayer::schedule_map), 0.2);
        
        /* set player */
        tmp_node = scene_node.child("player");
        _player = Player::getInstance().selected_airship;
        point = Tools::get_point_from_xml(tmp_node, GAME_SCENE_WIDTH, GAME_SCENE_HEIGHT);
        float rotation = Tools::get_rotation_from_xml(tmp_node);
        _player->reset_for_game();
        _player->setPosition(point);
        _player->setRotation(rotation);
        this->addChild(_player, GAME_ELEMENT_ZINDEX);
        _player->set_physics_body();
        _player->scheduleUpdate();
        for (auto dot : Path::getInstance().dots) {
            this->addChild(dot);
            dot->setVisible(false);
        }
        PLAYER_LEFTBOUND = winSize.width/2 - PLAYER_H_HAREA;
        PLAYER_RIGHTBOUND = winSize.width/2 + PLAYER_H_HAREA;
        PLAYER_TOPBOUND = winSize.height/2 + PLAYER_V_HAREA;
        PLAYER_BOTTOMBOUND = winSize.height/2 - PLAYER_V_HAREA;
        
        /* read enemies */
        tmp_node = scene_node.child("enemies");
        for (pugi::xml_node enemy_node = tmp_node.first_child(); enemy_node; enemy_node = enemy_node.next_sibling()) {
            Enemy* enemy = Enemy::create(enemy_node);
            point = Tools::get_point_from_xml(enemy_node, GAME_SCENE_WIDTH, GAME_SCENE_HEIGHT);
            rotation = Tools::get_rotation_from_xml(enemy_node);
            enemy->initiate_shipbase(enemy_node);
            enemy->move_type = enemy_node.attribute("move_type").as_int();
            enemy->setTag(ENEMY_TAG);
            enemy->setPosition(point);
            enemy->setRotation(rotation);
            enemy->current_angular_speed = 0;
            enemy->current_linear_speed = cocos2d::Point::ZERO;
            GameElementsManager::enemies.pushBack(enemy);
            enemy->scheduleUpdate();
            this->addChild(enemy, GAME_ELEMENT_ZINDEX);
            enemy->set_physics_body();
            for (auto &weapon : enemy->weapons) {
                if (weapon == nullptr) continue;
                this->addChild(weapon);
                weapon->schedule(schedule_selector(Weapon::find_player), weapon->data.projfreq);
            }
            
            Label *enemy_label = Label::createWithCharMap(texture, 12, 32, '.');
            this->addChild(enemy_label, 1);
            enemy_label->setVisible(false);
            GameElementsManager::enemy_label_map[enemy] = enemy_label;
        }
        
        /* load road signs */
        GameElementsManager::passed_sign_nums = 0;
        tmp_node = scene_node.child("roadsigns");
        for (pugi::xml_node sign_node = tmp_node.first_child(); sign_node; sign_node = sign_node.next_sibling()) {
            point = Tools::get_point_from_xml(sign_node);
            RoadSign * rs = RoadSign::create();
            rs->setPosition(point);
            auto _body = PhysicsBody::create();
            _body->addShape(PhysicsShapeCircle::create(RoadSign::RADIUS));
            if (nullptr != _body) {
                _body->setCollisionBitmask(0);
                _body->setContactTestBitmask(-1);
                rs->setPhysicsBody(_body);
            }
            GameElementsManager::signs.pushBack(rs);
            this->addChild(rs);
        }
        
        // if hit self, there should be no shield
        if (filename.length() > 10 &&
            filename.compare(filename.length() - 11, 11, "hitself.xml") == 0) {
            shield_used_time = SHIELD_TIME;
            shield_item->setVisible(false);
        } else {
            shield_used_time = 0;
            shield_item->setVisible(true);
        }
        
        /* contact listener, a helper for collision between airships and hit-test between projectiles and airships */
        contact_listener = EventListenerPhysicsContact::create();
        contact_listener->retain();
        contact_listener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
//        contact_listener->onContactPreSolve = [&](cocos2d::PhysicsContact& contact, PhysicsContactPreSolve &solve) -> bool {
//            printf("on contact presolve\n");
//            return true;
//        };
//        contact_listener->onContactPostSolve = [&](cocos2d::PhysicsContact& contact, const PhysicsContactPostSolve &solve) {
//            printf("postsolve: %f %f\n", solve.getFriction(), solve.getRestitution());
//        };
//        contact_listener->onContactSeperate = [&](cocos2d::PhysicsContact& contact) {
//            printf("on contact seperate\n");
//        };
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(contact_listener, this);
        
        /* backup player */
        GameElementsManager::copy_player(_player);
//        if (CCFileUtils::getInstance()->isFileExist("hitself.xml")) {
//            printf("crate success\n%s\n", CCFileUtils::getInstance()->getDataFromFile(CCFileUtils::getInstance()->getWritablePath()+"hitself.xml").getBytes());
//        } else {
//            printf("create fia\n");
//        }

        return true;
    } /* if (result) */ else {
 
        MessageBox(Tools::string_format("Load file %s error: %s", filename.c_str(), result.description()).c_str(), "Error");
        return false;
    }
}

bool GameLayer::onContactBegin(cocos2d::PhysicsContact &contact) {
    // seems useless
    int proj_num = 0;
    
    /* check if an airship collides with a road sign */
    RoadSign * rs;
    rs = GET_ROADSIGN(contact.getShapeA()->getBody()->getNode());
    if (rs == nullptr) {
        rs = GET_ROADSIGN(contact.getShapeB()->getBody()->getNode());
    }
    if (rs != nullptr) {
        if (!rs->passed) {
            rs->passed = true;
            GameElementsManager::passed_sign_nums++;
            if (GameElementsManager::passed_sign_nums == GameElementsManager::signs.size()) {
                quit_game();
            }
        }
        return false;
    }
    
    /* if not road sign, check whether a projectile */
    Projectile * nodea;
    ShipBase * nodeb;
    Airship * nodeb_again;
    nodea = GET_PROJECTILE(contact.getShapeA());
    if (nodea == NULL) {
        nodea = GET_PROJECTILE(contact.getShapeB());
        if (nodea == NULL) return true;
        else {
            ++proj_num;
            nodeb = GET_SHIPBASE(contact.getShapeA());
            nodeb_again = GET_AIRSHIP(contact.getShapeA()->getBody()->getNode());
            if (nodeb == NULL) return false;
        }
    } else {
        ++proj_num;
        nodeb = GET_SHIPBASE(contact.getShapeB());
        nodeb_again = GET_AIRSHIP(contact.getShapeB()->getBody()->getNode());
        if (nodeb == NULL) return false;
    }
    
    if (nodea->getTag() == nodeb->getTag()) return false;
    GameElementsManager::booms.push_back(Boom(contact.getContactData()->points[0]));
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion.caf");
    nodea->getOwner()->projectiles.eraseObject(nodea);
    this->removeChild(nodea);
    /* only the player may have a shield */
    if (!shield || (nodeb_again == nullptr)) {
        nodeb->get_hit(nodea);
    }
    return false;
}

void GameLayer::pause_game() {
    TraverseGameELements(pause);
    pausing = true;
}

void GameLayer::resume_game() {
    TraverseGameELements(resume);
    pausing = false;
}

void GameLayer::abort_game_dialog(Ref *pSender) {
    pause_game();
    dialog_layer = DialogLayer::create();
    dialog_layer->setString("Are you sure to abort the game, all the process will be aborted.");
    dialog_layer->onOkMenuItemPressed = [&](Ref *pSender) {
        GET_GAME_LAYER->remove_game_elements();
        GET_GAME_LAYER->removeChild(dialog_layer);
        SystemEventPool::getInstance().clear_event_pool();
        Director::getInstance()->replaceScene(((AppDelegate *)Application::getInstance())->getMainScene());
    };
    dialog_layer->onCancelMenuItemPressed = [&](Ref *pSender) {
        GET_GAME_LAYER->resume_game();
        dialog_layer->removeFromParentAndCleanup(false);
    };
    this->addChild(dialog_layer);
}

void GameLayer::sliderEvent(cocos2d::Ref *pSender, ui::Slider::EventType type) {
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        _player->set_percent(slider->getPercent());
    }
}

bool GameLayer::touch_out_of_center(cocos2d::Point touch) {
    if (touch.x < LEFT_BOUND) return true;
    if (touch.x > RIGHT_BOUND) return true;
    if (touch.y < BOTTOM_BOUND) return true;
    if (touch.y > TOP_BOUND) return true;
    return false;
}

void GameLayer::shield_call_back(cocos2d::Ref *pSender) {
    if (shield_cd > 0) return;
    shield = true;
    this->scheduleOnce(schedule_selector(GameLayer::shield_finish_call_back), SHIELD_LASTING_TIME);
    ++shield_used_time;
    /* 
     * if the shield has been used such many times
     * set it unvisible so it can't be used anymore
     */
    if (shield_used_time >= SHIELD_TIME) {
        shield_item->setVisible(false);
        return;
    }
    shield_cd = SHIELD_CDTIME;
    this->schedule(schedule_selector(GameLayer::shield_cd_call_back), 1);
}

void GameLayer::shield_cd_call_back(float t) {
    if ((--shield_cd) <= 0) {
        shield_cd = 0;
        this->unschedule(schedule_selector(GameLayer::shield_cd_call_back));
    }
}

void GameLayer::shield_finish_call_back(float t) {
    shield = false;
}

bool GameLayer::init() {
    if (Layer::init()) {

        bounds[0] = 100;  // left bound
        bounds[1] = winSize.height - STATUS_BAR_HEIGHT; // top
        bounds[2] = winSize.width;  // right
        bounds[3] = 0;  // bottom
        GAME_SCENE_WIDTH = RIGHT_BOUND - LEFT_BOUND;
        GAME_SCENE_HEIGHT = TOP_BOUND - BOTTOM_BOUND;

        _player = nullptr;
        
        /* read digits data for distance label*/
        unsigned char *data = cc_fps_images_png;
        ssize_t data_length = cc_fps_images_len();
        Image* image = new Image();
        if (!image->initWithImageData(data, data_length)) {
            MessageBox("Fails: init fps_image when starting game", "Error");
            quit_game();
        }
        texture = Director::getInstance()->getTextureCache()->addImage(image, "distant_images");
        CC_SAFE_RELEASE(image);
        
        /* the abort item at the right bottom in the battle field*/
        auto abort_item = MenuItemImage::create(
                                                "CloseNormal.png",
                                                "CloseSelected.png",
                                                CC_CALLBACK_1(GameLayer::abort_game_dialog, this));
        abort_item->setScale(1.5);
        abort_item->setPosition(Point(VisibleOrigin.x + VisibleSize.width - abort_item->getContentSize().width / 2, VisibleOrigin.y + abort_item->getContentSize().height / 2));
        
        /* so annoying that every 'button' should be included in a menu */
        shield_item = MenuItemImage::create("tech1.png", "tech1.png", CC_CALLBACK_1(GameLayer::shield_call_back, this));
        shield_item->setPosition(Point(winSize.width - shield_item->getContentSize().width/2-5, 500));
        auto menu = Menu::create(abort_item, shield_item, NULL);
        menu->setPosition(Point::ZERO);
        this->addChild(menu, GAME_CONTROL_ZINDEX);

        /* buff slider at the left */
        slider = Slider::create();
        slider->loadBarTexture("sliderTrack.png");
        slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
        slider->loadProgressBarTexture("sliderProgress.png");
        slider->setPosition(Point(LEFT_BOUND/2, winSize.height / 2));
        slider->setRotation(90);
        slider->addEventListener(CC_CALLBACK_2(GameLayer::sliderEvent, this));
        this->addChild(slider,9999);
        
        /* the labels for the buff slider */
        Label * upper = Label::createWithSystemFont("Speed\nBuff", "Arial", 30);
        upper->setPosition(LEFT_BOUND/2, winSize.height - 40);
        upper->setHorizontalAlignment(TextHAlignment::CENTER);
        this->addChild(upper, GAME_CONTROL_ZINDEX);
        Label * bottom = Label::createWithSystemFont("Attack\nBuff", "Arial", 30);
        bottom->setPosition(LEFT_BOUND /2 , 40);
        bottom->setHorizontalAlignment(TextHAlignment::CENTER);
        this->addChild(bottom, GAME_CONTROL_ZINDEX);
        
        /* wait for cocos2dx v3.2, since previous version has the bug that a sprite can not rotateTo or By if set physics body *
         * update: though not in use of these codes, but the touch functions work fine now.
         //bind physicsbody to sprite
         * MyBodyParser::getInstance()->parseJsonFile("plane.json");
         * auto _body = MyBodyParser::getInstance()->bodyFormJson(_player, "plane");
         * if (_body != nullptr) {
         *     _body->setDynamic(false);
         *     _body->setCollisionBitmask(0x000000);
         *       _player->setPhysicsBody(_body);
         * }
         */
        
        // add touch listener
        listener = EventListenerTouchOneByOne::create();
        listener->retain();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [&](cocos2d::Touch *touch, cocos2d::Event* event) {
            Vec2 touchpoint = this->convertTouchToNodeSpace(touch);
            Rect rect = Tools::getTouchSquareByPosition(_player);
            /* if hit within the rect containning the _player airship */
            dragging = rect.containsPoint(touchpoint);
            
            hit_on_blank = true && !this->touch_out_of_center(touchpoint);
            for (auto enemy : GameElementsManager::enemies) {
                if (Tools::get_sprite_rect(enemy).containsPoint(touchpoint)) {
                    hit_on_blank = false;
                    break;
                }
            }
            if (!dragging && !hit_on_blank) return false;
            
            if (dragging) {
                this->removeDots();
                
                /* wait for cocos2dx v3.2, since previous version has the bug that a sprite can not rotateTo or By if set physics body *
                 *
                 *  auto current_node = nodeUnderTouch(touch);
                 *  if (current_node == _player)
                 *      printf("contact!\n");
                 *  else return false;
                 */
                
                this->_player->commandIndex = 1;
                this->_player->touch_offset = this->_player->getPosition() - touch->getLocation();
                this->addRouteDot(this->_player->getPosition());
                
            } else {
                assert(hit_on_blank);
                this->removeDots();
                this->_player->commandIndex = 1;
                this->_player->touch_offset = Point::ZERO;
                this->addRouteDot(_player->getPosition());
                this->add_series_dots(touch->getLocation(), this->_player->touch_offset);
//                dragging = true;
            }
            return true;
        };
        listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
        listener->onTouchEnded = [&](cocos2d::Touch *touch, cocos2d::Event* event) {
            dragging = false;
        };
        return true;
    }
    return false;
}

/* 
 * deprecated
 * original for accurate touch within the player airship
 * but the airship is so small
 * too much requirements on the hit accuracy leads to lots of miss touch when playing on the phone
 */
Node *GameLayer::nodeUnderTouch(Touch *touch) {
    Node *node = nullptr;
    //translate the touch to layer coordinate
    auto location = this->convertTouchToNodeSpace(touch);
    
    //retrive all physics shapes under the touch location through cocos2d-x
    auto scene = ((AppDelegate *)Application::getInstance())->getGameScene();
    auto arr = scene->getPhysicsWorld()->getShapes(location);
    
    //iterate the shapes, find our sprite
    for (auto& obj : arr) {
        if (obj->getBody()->getNode() == this->_player) {
            node = obj->getBody()->getNode();
            break;
        }
    }
    return node;
}

/*
 * used when:
 * 1. when manipulating the airship by translation
 * a fast moving of the finger leads to a big interval between to 2 adjacent dots,
 * 2. when wanna move the airship by touch;
 * this function works to fill enough dots between a big interval
 */
void GameLayer::add_series_dots(cocos2d::Point target, cocos2d::Point offset) {
    Point modified = target + offset;
    if (Path::getInstance().dotsInSize() && !Tools::within_distance(modified, Path::getInstance().back()->getPosition(), Path::DOTS_MIN_DISTANCE)) {
        Point direction = modified - Path::getInstance().back()->getPosition();
        int complement = direction.getLength() / Path::DOTS_MIN_DISTANCE - 1;
        if (complement > 0) {
            Point interval = direction.getNormalized() * direction.getLength() / complement;
            for (int i = 0; i < complement && Path::getInstance().dotsInSize(); ++i) { 
                this->addRouteDot(Path::getInstance().back()->getPosition() + interval);
            }
        }
        if (Path::getInstance().dotsInSize()) {
            this->addRouteDot(target + offset);
        }
    }
}

void GameLayer::addRouteDot(Point position) {
    auto dot = Path::getInstance().addPathDot(position);
    dot->setVisible(true);
}

void GameLayer::removeDots() {
    for (int i = 0; i < Path::getInstance().size; ++i) {
        Path::getInstance().dots[i]->setVisible(false);
    }
    Path::getInstance().clear_dots();
}

void GameLayer::remove_booms(float t) {
    clock_t current = clock();
    Boom tmp;
    while (!GameElementsManager::booms.empty()) {
        tmp = GameElementsManager::booms.front();
        if (float(current - tmp.trigger_time) / CLOCKS_PER_SEC < Boom::BOOM_TIME) break;
        GameElementsManager::booms.pop_front();
    }
}

void GameLayer::onTouchMoved(Touch *touch, Event *event) {
    if (dragging) {
        this->add_series_dots(touch->getLocation(), this->_player->touch_offset);
    }
}

void GameLayer::draw(Renderer *renderer, const Mat4 & transform, uint32_t flags) {
    Layer::draw(renderer, transform, flags);
    if (!pausing) {
        _customCommand.init(GAME_ELEMENT_ZINDEX);
        _customCommand.func = CC_CALLBACK_0(GameLayer::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
}

bool GameLayer::sprite_outof_screen(cocos2d::Sprite *sprite) {
    return (sprite->getPositionY() - sprite->getContentSize().height/2 > TOP_BOUND) ||
           (sprite->getPositionY() + sprite->getContentSize().height/2 < BOTTOM_BOUND) ||
           (sprite->getPositionX() - sprite->getContentSize().width/2 > RIGHT_BOUND) ||
           (sprite->getPositionX() + sprite->getContentSize().width/2 < LEFT_BOUND);
}

/* draw a small arrow to show the direction from the enemy to the player when the enemy is out of the screen */
void GameLayer::draw_sprite_trace(cocos2d::Sprite *sprite) {
    Point direction = (sprite->getPosition() - _player->getPosition()).getNormalized();
    Point position;
    if (Tools::float_equals(direction.x, 0)) { // equals to zero
        position.x = 0;
        if (direction.y > 0) {
            position.y = TOP_BOUND - TRACE_BOUND_MARGIN;
        } else {
            position.y = BOTTOM_BOUND + TRACE_BOUND_MARGIN;
        }
    } else {
        float k = direction.y / direction.x;
        float cross_x = (direction.x > 0) ? player_right_padding() - TRACE_BOUND_MARGIN : -player_left_padding() + TRACE_BOUND_MARGIN;
        float cross_y = cross_x * k;
        if (cross_y > (player_top_padding() - TRACE_BOUND_MARGIN)) {
            cross_y = player_top_padding() - TRACE_BOUND_MARGIN;
            cross_x = cross_y / k;
        } else if (cross_y < -player_bottom_padding() + TRACE_BOUND_MARGIN) {
            cross_y = TRACE_BOUND_MARGIN - player_bottom_padding();
            cross_x = cross_y / k;
        }
        position.x = cross_x;
        position.y = cross_y;
        position += _player->getPosition();
    }
    
    /* i.e. the arrow */
    Triangle trace_mark;
    trace_mark.set_triangle(position, 8, direction.getAngle(), false);
    DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
    glLineWidth(5);
    DrawPrimitives::drawSolidPoly(trace_mark.points, 3, Color4F(1,0,0,1));
    
    /* set the label showing the distance from the enemy to the player */
    Label *tmp = GameElementsManager::enemy_label_map[sprite];
    tmp->setVisible(true);
    
    char buffer[30];
    sprintf(buffer, "%.2f", (sprite->getPosition() - _player->getPosition()).getLength());
    tmp->setString(buffer);
    float offset = tmp->getContentSize().width / 2 + 20;
    if (position.x > _player->getPositionX() * 1.5) {
        tmp->setPosition(position - Point(offset, 0));
    } else {
        tmp->setPosition(position + Point(offset, 0));
    }
}

void GameLayer::onDraw(const Mat4 &transform, uint32_t flags) {
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    /* draw the enemy related elements */
    has_enemy_in_screen = false;
    for (auto enemy : GameElementsManager::enemies) {
        if (sprite_outof_screen(enemy)) {
            draw_sprite_trace(enemy);
        } else {
            ((Enemy *)enemy)->drawElements(false);
            has_enemy_in_screen = true;
            GameElementsManager::enemy_label_map[enemy]->setVisible(false);
        }
    }
    
    /* draw the player related enements */
    if (_player) {
        _player->drawElements(has_enemy_in_screen);
        if (shield) {
            DrawPrimitives::setDrawColor4B(0, 223, 58, 255);
            DrawPrimitives::drawCircle(_player->getPosition(), MAX(_player->getContentSize().width, _player->getContentSize().height)/2, 0, 16, false, 1, 1);
            DrawPrimitives::setDrawColor4B(0, 128, 29, 32);
            DrawPrimitives::drawSolidCircle(_player->getPosition(), MAX(_player->getContentSize().width, _player->getContentSize().height)/2, 0, 16, 1, 1);
        }
        if (shield_cd) {
            DrawPrimitives::setDrawColor4B(0, 0, 0, 192);
            DrawPrimitives::drawSolidCircle(shield_item->getPosition(), (float)((float) shield_cd / (float)SHIELD_CDTIME) * shield_item->getContentSize().width/2, 0, 16, 1, 1);
        }
    }
    
    
    /* draw signs */
    for (auto sign : GameElementsManager::signs) {
        ((RoadSign *)sign)->draw_self();
    }
    
    /* draw current dest dot */
    DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
    if (Path::getInstance().size > 0)
        DrawPrimitives::drawCircle(Path::getInstance().dots[_player->commandIndex]->getPosition(), 5, 360, 50, false, 1, 1);
    
    /* draw left bar */
//    DrawPrimitives::drawSolidRect(Point(0, winSize.height), Point(LEFT_BOUND, 0), Color4F(0.5, 0.3, 0.02, 0.6));
    
    /* draw status bar */
    DrawPrimitives::drawSolidRect(Point(0, winSize.height), Point(winSize.width, winSize.height - STATUS_BAR_HEIGHT), Color4F(0,1,1,1));
    
    CHECK_GL_ERROR_DEBUG();
    
    DrawPrimitives::setDrawColor4B(255, 50, 50, 255);
    DrawPrimitives::setPointSize(5);
    for (Boom &boom : GameElementsManager::booms) {
        boom.draw_self();
    }
    // TIP:
    // If you are going to use always thde same color or width, you don't
    // need to call it before every draw
    //
    // Remember: OpenGL is a state-machine.
    
    // draw big point in the center
//    DrawPrimitives::setPointSize(64);
//    DrawPrimitives::setDrawColor4B(0,0,255,128);
//    DrawPrimitives::drawPoint( VisibleRect::center() );
    
//    CHECK_GL_ERROR_DEBUG();
//    
//    // open yellow poly
//    DrawPrimitives::setDrawColor4B(255, 255, 0, 255);
//    glLineWidth(10);
//    Point vertices[] = { Point(0,0), Point(50,50), Point(100,50), Point(100,100), Point(50,100) };
//    DrawPrimitives::drawPoly( vertices, 5, false);
    
//    CHECK_GL_ERROR_DEBUG();
//    
//    // filled poly
//    glLineWidth(1);
//    Point filledVertices[] = { Point(0,120), Point(50,120), Point(50,170), Point(25,200), Point(0,170) };
//    DrawPrimitives::drawSolidPoly(filledVertices, 5, Color4F(0.5f, 0.5f, 1, 1 ) );
    
    
    // closed purble poly
//    DrawPrimitives::setDrawColor4B(255, 0, 255, 255);
//    glLineWidth(2);
//    Point vertices2[] = { Point(30,130), Point(30,230), Point(50,200) };
//    DrawPrimitives::drawPoly( vertices2, 3, true);
//    
//    // draw a green circle with 50 segments with line to center
//    glLineWidth(2);
//    DrawPrimitives::setDrawColor4B(0, 255, 255, 255);
//    DrawPrimitives::drawCircle( VisibleRect::center(), 50, CC_DEGREES_TO_RADIANS(0), 50, true);
//    
//    CHECK_GL_ERROR_DEBUG();
//    
//    //draw a solid polygon
//    Point vertices3[] = {Point(60,160), Point(70,190), Point(100,190), Point(90,160)};
//    DrawPrimitives::drawSolidPoly( vertices3, 4, Color4F(1,1,0,1) );
    
    // restore original values
//    CHECK_GL_ERROR_DEBUG();
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

/* invoked before load the scene into the screen */
bool GameLayer::reset(std::string filename) {
    if (GameLayer::load_game_scene(filename)) {
        this->scheduleUpdate();
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        for (auto &weapon : _player->weapons) {
            if (weapon == nullptr) continue;
            this->addChild(weapon);
            weapon->schedule(schedule_selector(Weapon::find_enemy), weapon->data.projfreq);
        }
        shield = false;
        shield_cd = 0;
        return true;
    }
    else return false;
}

void GameLayer::update(float t) {
    _player->movement(t);
    for (auto e : GameElementsManager::enemies)
        ((Enemy *)e)->movement(t);
    remove_booms(t);
}

void GameLayer::remove_game_elements() {
    //DEBUG
    printf("get hit time: %d\n", Tools::get_hit_num);
    
    /* remove path */
    this->removeDots();
    
    /* remove enemy labels */
    GameElementsManager::enemy_label_map.clear();
    
    while (GameElementsManager::enemies.size() > 0) {
        ((Enemy *)GameElementsManager::enemies.at(0))->remove_self_from_game();
    }
    
    for (int i = 0; i < GameElementsManager::signs.size(); ++i) {
        this->removeChild(GameElementsManager::signs.at(i));
    }
    GameElementsManager::signs.clear();
    
    /* remove player airship */
    if (_player) {
        /* remove projectiles */
        for (auto & weapon : _player->weapons) {
            if (weapon == nullptr) continue;
            weapon->stop_working = true;
        }
        _player->remove_projectiles_from_layer();
        for (auto &weapon : _player->weapons) {
            if (weapon == nullptr) continue;
            weapon->unschedule(schedule_selector(Weapon::find_enemy));
            weapon->stop_working = false;
            this->removeChild(weapon);
        }
        _player->unscheduleUpdate();
        _player->setPhysicsBody(nullptr);
        this->removeChild(_player);
        _player = nullptr;
    }

    // bug: it seems doesn't work as I want.
    _physics_world->removeAllBodies();
    
    _eventDispatcher->removeEventListener(listener);
    listener->release();
    _eventDispatcher->removeEventListener(contact_listener);
    contact_listener->release();
    
    /* stop update */
    this->unscheduleUpdate();
}

void GameLayer::quit_game() {
    /* fix bug: it seems there's memory leak */
    this->remove_game_elements();
    auto scene = GameOverScene::create();
    scene->getLayer()->getLabel()->setString("You Win");
    Director::getInstance()->replaceScene(scene);
}

bool GameScene::init() {
    if (Scene::initWithPhysics()) {
        this->getPhysicsWorld()->setGravity(Vec2(0, 0));
        this->_layer = GameLayer::create();
        this->_layer->retain();
        _layer->setPhysicsWorld(this->getPhysicsWorld());
        this->addChild(_layer, 1);
        
        //enable debug draw
//        this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
        
        this->_background = BackgroundLayer::create();
        this->_background->retain();
        this->_layer->setBackground(_background);
        this->addChild(_background, 0);
        
        return true;
    }
    return false;
}

GameScene::~GameScene() {
    if (_layer) {
        this->removeChild(_layer);
        _layer->release();
        _layer = NULL;
    }
    if (_background) {
        _background->removeAllChildren();
        _background->release();
        _background = NULL;
    }
}
