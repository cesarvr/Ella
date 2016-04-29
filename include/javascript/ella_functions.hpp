//
//  ella_functions.h
//  nanProject
//
//  Created by Cesar Valdez on 24/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_functions_h
#define ella_functions_h

using namespace ella;


using JNIValue = LibJNI::BaseJavaValue*;
using V8Value = v8::Local<v8::Value>;

// Algoritms for transformation from V8 Type to JVM Type.

JNIValue GetString(V8Value value) {
    if (value->IsString())
        return  std::move( new LibJNI::Value<std::string>(ella::Utils::ObjectToString( value )) );
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
        
        auto obj = object->Call<std::string>(methodName, args);
        
        return Nan::New( obj.Get() ).ToLocalChecked();
    };
};


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
