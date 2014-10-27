//
//  GameElementsManager.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-28.
//
//

#include <fstream>
#include "GameElementsManager.h"
#include "Airship.h"
USING_NS_CC;
using namespace std;

cocos2d::Vector<cocos2d::Sprite *> GameElementsManager::enemies;
std::map<cocos2d::Sprite*, cocos2d::Label*> GameElementsManager::enemy_label_map;
std::deque<Boom> GameElementsManager::booms;
cocos2d::Vector<cocos2d::Sprite *> GameElementsManager::signs;
int GameElementsManager::passed_sign_nums;

void GameElementsManager::copy_player(Airship * ship) {
    // airship type and hp
    string filepath = CCFileUtils::getInstance()->getWritablePath() + "hitself.xml";
    FILE *fp = fopen(filepath.c_str(), "w");
    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "<gamescene>\n");
    fprintf(fp, "<background filename=\"bgstar.jpg\" x=\"480\" y=\"320\"/>\n");
    fprintf(fp, "<player x=\"0.5\" y=\"0.5\" />\n");
    fprintf(fp, "<enemies>\n");
    fprintf(fp, "<enemy airship=\"%d\" x=\"0.2\" y=\"0.5\" hp=\"%f\" rotation=\"0\" move_type=\"2\">\n", (int)ship->data.type, ship->total_hp);
    fprintf(fp, "<Weapons>\n");
    for (int i = 0; i < ship->weapons.size(); ++i) {
        if (ship->weapons[i] == nullptr) continue;
        fprintf(fp, "<weapon type=\"%d\" projtype=\"%d\" id=\"%d\" />\n", ship->weapons[i]->data.type, ship->weapons[i]->projectile_data.type, ship->weapons[i]->slot_id+1);
    }
    fprintf(fp, "</Weapons>\n");
    fprintf(fp, "</enemy>\n");
    fprintf(fp, "</enemies>\n");
    fprintf(fp, "</gamescene>\0");
    fclose(fp);
    
    /*
     <?xml version="1.0"?>
     <gamescene>
     <background filename="bgstar.jpg" x="480" y="320"/>
     <player x="0.5" y="0.5" />
     <enemies>
     <enemy airship="2" x="0.2" y="0.5" hp="500" rotation="0">
     </enemy>
     </enemies>
     <obstacles>
     </obstacles>
     </gamescene>
     */
    
    // weapons
}
