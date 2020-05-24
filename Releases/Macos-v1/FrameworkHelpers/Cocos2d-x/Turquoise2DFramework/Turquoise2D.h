//
//  Turquoise2D.h
//  Turquoise2DProject
//
//  Created by Eray Zesen on 18/03/16.
//
//

#ifndef Turquoise2D_h
#define Turquoise2D_h

#include "cocos2d.h"
#include "json.hh"
#include "ObjectFactory.h"
#include <stdio.h>
#include "T2DVariant.h"

using namespace std;
using json = nlohmann::json;

class Turquoise2D{
public:
    Turquoise2D();
    static Turquoise2D* import(string scenePath,cocos2d::Layer *targetLayer, cocos2d::Scene *targetScene);
    template<class T> T GetObject(string objectName){
        cocos2d::Node* nObj=objects[objectName];
        if(nObj==NULL){
            string msg="'"+objectName +"' named object not found! ";
            CCLOGERROR(msg.c_str());
            return NULL;
        }
        T resultObject=dynamic_cast<T>(nObj);
        if(resultObject==NULL){
            string msg="Finded object isn't a/an '"+(string)typeid(T).name()+"' typed object! ";
            CCLOGERROR(msg.c_str());
            return NULL;
        }
        return resultObject;
    };
    cocos2d::Sprite* GetSprite(string spriteName);
    map<string, cocos2d::Sprite*> sprites;
    map<string, cocos2d::Node*> objects;
private:
    static cocos2d::Sprite* createSprite(json jsonObject);
    template<class T> static T createObject(json jsonObject);
    static void setPropertiesToGameItem(cocos2d::Node *item,json jsonObject);
    
};

#endif /* Turquoise2D_hpp */

