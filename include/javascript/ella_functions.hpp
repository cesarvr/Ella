//
//  ella_functions.h
//  nanProject
//
//  Created by Cesar Valdez on 24/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_functions_h
#define ella_functions_h

#include <memory.h>

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
   
    if (value->IsInt32()){
        std::cout << "n: " << value->Int32Value() << std::endl;
        return std::move( new IntValue ( value->Int32Value() ) );
    }
    
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


template <typename ValueType>
struct JSType: public BaseCall {
    
    JSType(std::string _type){
        type = _type;
    };
    
    std::string Type(){
        return type;
    };
    
    void Call(std::string methodName,
              std::shared_ptr<Object> object,
              std::vector<LibJNI::BaseJavaValue *> args) {
        
        value = object->Call<ValueType>(methodName, args);
    };
    
private:
    std::string type;
    ValueType value;
};




// java.lang.String Return type
struct StringCall: JSType<StringValue> {
    
    StringCall(): JSType("java.lang.String") {};
    
    v8::Local<v8::Value> Get() {
        return  Nan::New( value.Get() ).ToLocalChecked();
    }
    
private:
    StringValue value;
};


// int Return type
struct IntCall: JSType<IntValue> {
    
    IntCall(): JSType("int") {};
    
    v8::Local<v8::Value> Get() {
        return  Nan::New( value.Get() );
    }
    
private:
    IntValue value;
};



// int Return type
struct VoidCall: JSType<JObject> {
    
    VoidCall(): JSType("void") {};
    
    v8::Local<v8::Value> Get() {
        return  Nan::New( true );
    }
};










#endif /* ella_functions_h */
