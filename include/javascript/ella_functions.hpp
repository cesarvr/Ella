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
    std::cout << "GET_STRING" << std::endl;
    if (value->IsString()){
         std::cout << "VALUE:GET:STRING" << std::endl;
        v8::String::Utf8Value utf8_value(value);
        return  std::move( new LibJNI::Value<std::string>( *utf8_value ));
    }
    
    return nullptr;
}

JNIValue GetInteger(V8Value value) {
    if (value->IsInt32())
        return std::move( new LibJNI::Value<int> ( value->Int32Value() ) );
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
    virtual v8::Local<v8::Value> Call(std::string methodName,
                                      std::shared_ptr<LibJNI::Object> object,
                                      std::vector<LibJNI::BaseJavaValue *>)=0;
};


// java.lang.String Return type
struct StringCall: BaseCall {
    
    
    std::string Type() { return "java.lang.String"; };
    
    v8::Local<v8::Value> Call(std::string methodName,
                              std::shared_ptr<LibJNI::Object> object,
                              std::vector<LibJNI::BaseJavaValue *> args) {
        
        
        std::cout << "CALL_STRING" << methodName << std::endl;

        
        auto obj = object->Call<std::string>(methodName, args);
        
        std::cout << "CALL:RETURN'S: >" << obj.Get() << std::endl;

        return Nan::New( obj.Get() ).ToLocalChecked();
    };
};


// int Return type
struct IntCall: BaseCall {
    
    
    std::string Type() { return "int"; };
    
    v8::Local<v8::Value> Call(std::string methodName,
                              std::shared_ptr<LibJNI::Object> object,
                              std::vector<LibJNI::BaseJavaValue *> args) {
        
        auto obj = object->Call<int>(methodName, args);
        
        return Nan::New( obj.Get() );
    };
    
};










#endif /* ella_functions_h */
