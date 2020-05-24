//
//  Turquoise2D.cpp
//  Turquoise2DProject
//
//  Created by Eray Zesen on 18/03/16.
//
//
#include "Turquoise2D.h"

Turquoise2D::Turquoise2D()
{
   //main func
}

Turquoise2D* Turquoise2D::import(string scenePath,cocos2d::Layer *targetLayer, cocos2d::Scene *targetScene){
    Turquoise2D *result=new Turquoise2D();
    
    cocos2d::Camera *camera=targetScene->getDefaultCamera();
    camera->setPosition(cocos2d::Vec2(0,0));
    
    //Getting file data
    string content=cocos2d::FileUtils::getInstance()->getStringFromFile(scenePath.c_str());
    
    auto document=json::parse(content.c_str());
    //Parsing all of game items
    //Sprites
    auto spriteArray=document["sprites"];
    for(json::size_type i=0;i<spriteArray.size();i++){
        json itemJsonObject=spriteArray.at(i);
        cocos2d::Sprite* nSprite=Turquoise2D::createSprite(itemJsonObject);
        Turquoise2D::setPropertiesToGameItem(nSprite, itemJsonObject);
        targetLayer->addChild(nSprite);
        //Setting tag property
        try{
            string itemTagStr=itemJsonObject["tag"];
            int itemTagNum=stoi(itemTagStr);
            nSprite->setTag(itemTagNum);
        }catch ( ... ) {
        }
        
        //Setting name property and adding sprites container
        string itemName=itemJsonObject["name"];
        result->sprites.insert({itemName,nSprite});
        
    }
    //Objects
    auto objectArray=document["objects"];
    for(json::size_type i=0;i<objectArray.size();i++){
        json itemJsonObject=objectArray.at(i);
        cocos2d::Node* nObject=Turquoise2D::createObject<cocos2d::Node*>(itemJsonObject);
        Turquoise2D::setPropertiesToGameItem(nObject, itemJsonObject);
        targetLayer->addChild(nObject);
        //Setting tag property
        try{
            string itemTagStr=itemJsonObject["tag"];
            int itemTagNum=stoi(itemTagStr);
            nObject->setTag(itemTagNum);
        }catch ( ... ) {
        }
        
        //Setting name property and adding objects container
        string itemName=itemJsonObject["name"];
        result->objects.insert({itemName,nObject});
    }
    
    return result;
}

cocos2d::Sprite* Turquoise2D::createSprite(json jsonObject){
    //properties
    string spriteType=jsonObject["spriteType"];
    string spritePath=jsonObject["spritePath"];
    string spriteFrameName=jsonObject["spriteFrameName"];
    bool spriteAntialias=jsonObject["spriteAntialias"];
    cocos2d::Sprite* gameItem;
    if(spriteType=="ImageFileBased"){
        gameItem=cocos2d::Sprite::create(spritePath);
    }else if(spriteType=="SpriteSheetBased"){
        auto spritecache = cocos2d::SpriteFrameCache::getInstance();
        spritecache->addSpriteFramesWithFile(spritePath);
        gameItem=cocos2d::Sprite::createWithSpriteFrameName(spriteFrameName);
    }
    if(spriteAntialias==false){
        gameItem->getTexture()->setAliasTexParameters();
    }

    gameItem->setAnchorPoint(cocos2d::Vec2(0.5f,0.5f));
    return gameItem;
}

template<class T>
T Turquoise2D::createObject(json jsonObject){
    //properties
    T gameItem;
    string objectClassPath=jsonObject["objectClassPath"];
    if(objectClassPath=="None"){
        gameItem=cocos2d::Node::create();
    }else{
        string objectType=jsonObject["objectType"];
        //Getting class parameters
        vector<string> strParameters=jsonObject["objectClassParameters"].get<vector<std::string>>();
    std:vector<T2DVariant> cpVariants;
        for(int i=0;i<strParameters.size();i++){
            cpVariants.push_back(T2DVariant(strParameters.at(i)));
        }
        gameItem=T2DObjectFactory::create(objectType,cpVariants);
    }
    return gameItem;
    
}
void Turquoise2D::setPropertiesToGameItem(cocos2d::Node *item,json jsonObject){
    //Main properties
    string name=jsonObject["name"];
    item->setName(name);
    string spritePath=jsonObject["spritePath"];
    int zOrder=jsonObject["zOrder"];
    cocos2d::Vec2 pos(jsonObject["position"][0].get<float>(),jsonObject["position"][1].get<float>());
    float rotation=jsonObject["rotation"];
    cocos2d::Vec2 scale(jsonObject["scale"][0].get<float>(),jsonObject["scale"][1].get<float>());
    // Physic Properties
    json physicsObj=jsonObject["physics"];
    if(physicsObj["enable"].get<bool>()==true){
        bool pDynamic=physicsObj["dynamic"];
        float pDensity=physicsObj["density"];
        float pFriction=physicsObj["friction"];
        float pRestitution=physicsObj["restitution"];
        float pMass=physicsObj["mass"];
        //Shapes
        json shapesArray=physicsObj["colliderShapes"];
        cocos2d::PhysicsBody *physicsBody=cocos2d::PhysicsBody::create();
        for(json::size_type i=0;i<shapesArray.size();i++){
            json shapeObj=shapesArray.at(i);
            string shapeType=shapeObj["type"];
            json sRectAr=shapeObj["rect"];
            cocos2d::Rect shapeRect(sRectAr[0].get<float>(),sRectAr[1].get<float>(),sRectAr[2].get<float>(),sRectAr[3].get<float>());
            if(shapeType=="box"){
                auto physicsShape=cocos2d::PhysicsShapeBox::create(shapeRect.size,cocos2d::PhysicsMaterial(pDensity,pRestitution,pFriction),shapeRect.origin);
                physicsBody->addShape(physicsShape);
            }else if(shapeType=="circle"){
                auto physicsShape=cocos2d::PhysicsShapeCircle::create(shapeRect.size.width/2,cocos2d::PhysicsMaterial(pDensity,pRestitution,pFriction),shapeRect.origin);
                physicsBody->addShape(physicsShape);
            }
        }
        //Adding Physics Body
        physicsBody->setDynamic(pDynamic);
        physicsBody->setMass(pMass);
        physicsBody->setContactTestBitmask(true);
        item->setPhysicsBody(physicsBody);
        item->runAction(cocos2d::RotateBy::create(0, rotation));
    }
    //Init sprite
    item->setRotation(rotation);
    item->setPosition(pos);
    item->setLocalZOrder(zOrder);
    item->setScale(scale.x, scale.y);

}



cocos2d::Sprite* Turquoise2D::GetSprite(string spriteName){
    cocos2d::Sprite* resultObject=sprites[spriteName];
    if(resultObject==NULL){
        string msg="'"+spriteName +"' named sprite not found! ";
        CCLOGERROR(msg.c_str());
        return NULL;
    }
    return resultObject;
}
