//
//  ella_functions.h
//  nanProject
//
//  Created by Cesar Valdez on 24/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_functions_h
#define ella_functions_h

#include <algorithm>
#include <memory.h>
#include "ella_jni_value.h"

using namespace ella;


using JNIValue = LibJNI::BaseJavaValue*;
using V8Value = v8::Local<v8::Value>;

// Algoritms for transformation from V8 Type to JVM Type.

JNIValue GetString(V8Value value) {
    if (value->IsString()){
        v8::String::Utf8Value utf8_value(value);
        return  std::move( new StringValue( *utf8_value ));
    }
    
    return nullptr;
}

JNIValue GetInteger(V8Value value) {
    if (value->IsInt32())
        return std::move( new IntValue ( value->Int32Value() ) );
    return nullptr;
}

JNIValue GetNumber(V8Value value) {
    
    if (value->IsNumber())
        return std::move( new NumbersValue ( value->NumberValue() ) );
    return nullptr;
}

Nan::Callback* GetFunctionCallback(V8Value value) {
    
    if(value->IsFunction())
        return new Nan::Callback(value.As<v8::Function>());
    
    return nullptr;
}



/*
 
 Handling different return types supported by Java
 
 Create a class that derivate from this base class to handle a new Java type.
 
 */

struct BaseCall {
    
    virtual std::string Type() =0;
    virtual void Call(std::string methodName,
                      std::shared_ptr<Object> object,
                      std::vector<LibJNI::BaseJavaValue *>)=0;
    
    virtual v8::Local<v8::Value> Get() = 0;
};



// This class try to solve the type inference in runtime, by executing the right code for the specific type.
template <typename ValueType>
class JSType: public BaseCall {
public:
    
    std::string Type(){
        return value.GetType();
    };
    
    void Call(std::string methodName,
              std::shared_ptr<Object> object,
              std::vector<LibJNI::BaseJavaValue *> args) {
        
        value = object->Call<ValueType>(methodName, args);
    };
    
protected:
    std::string type;
    ValueType value;
};


// java.lang.String Return type
class StringCall: public JSType<StringValue> {
public:
    StringCall(): JSType() {};
    
    v8::Local<v8::Value> Get() {
        return  Nan::New( value.Get() ).ToLocalChecked();
    }
};

// int Return type
class IntCall: public JSType<IntValue> {
public:
    IntCall(): JSType(){};
    
    v8::Local<v8::Value> Get() {
        return  Nan::New( value.Get() );
    }
};

// Native ByteArray
class ByteArrayCall: public JSType<ByteArrayValue> {
public:
    ByteArrayCall(): JSType() {};
    
    v8::Local<v8::Value> Get() {
        return Nan::CopyBuffer((char*)&value.Get()[0], (int)value.Get().size()).ToLocalChecked();
    }
};

// double Return type
class DoubleCall: public JSType<DoubleValue> {
public:
    DoubleCall(): JSType(){};
    
    v8::Local<v8::Value> Get() {
        return  Nan::New( value.Get() );
    }
};

// void Return type
struct VoidCall: public JSType<ObjectValue> {
public:
    VoidCall(): JSType(){};
    
    v8::Local<v8::Value> Get() {
        return  Nan::New( true );
    }
};










#endif /* ella_functions_h */
