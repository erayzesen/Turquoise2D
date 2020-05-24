//
//  T2DVariant.h
//  SampleProject
//
//  Created by Eray Zesen on 01/10/16.
//
//

#ifndef T2DVariant_h
#define T2DVariant_h

#include <stdio.h>

struct T2DVariant{
    
    std::string val;
    
    T2DVariant(std::string value){
        val=value;
    }
    std::string toString(){
        return val;
    }
    float toFloat(){
        try{
            return std::stof(val);
        }catch(...){
            return 0;
        }
    }
    
    int toInt(){
        try{
            return std::stoi(val);
        }catch(...){
            return 0;
        }
    }
    
    bool toBoolean(){
        if(val=="true" || val=="1"){
            return true;
        }else if(val=="false" || val=="0"){
            return false;
        }else{
            return false;
        }
    }
    
};


#endif /* T2DVariant_h */
