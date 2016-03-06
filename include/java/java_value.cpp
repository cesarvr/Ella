//
//  jvm_type.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "java_value.hpp"


JavaValue::JavaValue(std::string value) {
    std::vector<char> tmp(value.begin(), value.end());
    bytes = tmp;
    type=JVMType::String;
};

JavaValue::JavaValue(float value) {
    data.f = value;
    type=JVMType::Float;
};

JavaValue::JavaValue(int value) {
    data.i = value;
    type=JVMType::Integer;
};

JavaValue::JavaValue(std::vector<char> data) {
    bytes = data;
    type=JVMType::ByteArray;
};

std::vector<char> JavaValue::getArrayValue(){
    if( !IsArray() ) throw VMError{ "Error: incompatible data. (expected Array)"};
    return bytes;
};

int JavaValue::getIntValue(){
    if( !IsInteger() ) throw VMError{ "Error: incompatible data. (expected Integer)"};
    return data.i;
};

float JavaValue::getFloatValue(){
    if( !IsFloat() ) throw VMError{ "Error: incompatible data. (expected Float)"};
    return data.f;
};

std::string JavaValue::getStringValue(){
    if( !IsString() ) throw VMError{ "Error: incompatible data. (expected String) "};
    std::string tmp(bytes.begin(), bytes.end());
    return tmp;
};
