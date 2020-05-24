#ifndef ObjectFactory_h
#define ObjectFactory_h



#include "stdio.h"
#include "T2DVariant.h"
#include "cocos2d.h"

using namespace std;

class ObjectFactory
{
public:
    cocos2d::Node* newNode;
    ObjectFactory(){
        
    }
    static cocos2d::Node* create(string classType,vector<T2DVariant> parameters){
        cocos2d::Node* object;
        
        return object;
    }
    
private:
    
};


#endif //End of ObjectFactory.h
