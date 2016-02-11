//
//  jvm_type.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_type_hpp
#define jvm_type_hpp

#include "jvm_global.hpp"


enum class JVMType { String, Integer, Float, ByteArray };


class JavaValue {
    JVMType type;
    std::vector<unsigned char> bytes;
    
public:
    
    JavaValue(){};
    JavaValue(std::string value);
    JavaValue(float value);
    JavaValue(int value);
    JavaValue(std::vector<unsigned char> data);
    
    
    bool IsString(){  return type==JVMType::String; };
    bool IsInteger(){ return type==JVMType::Integer; };
    bool IsFloat(){   return type==JVMType::Float; };
    bool IsArray(){   return type==JVMType::ByteArray; }
    bool IsUndefined() { return IsString() || IsInteger() || IsFloat() || IsArray();  };
    
    int getIntValue();
    
    float getFloatValue();
    
    std::vector<unsigned char> getArrayValue();
    
    std::string getStringValue();
    
};







#endif /* jvm_type_hpp */
