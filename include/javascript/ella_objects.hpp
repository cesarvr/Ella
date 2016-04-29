//
//  ella_objects.hpp
//  nanProject
//
//  Created by Cesar Valdez on 22/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_objects_hpp
#define ella_objects_hpp

#include "nan.h"
#include "jvm_object.hpp"
#include "ella_utils.hpp"

#include <regex>
#include <map>

namespace  ella {
    
    
    using Algorithm =  LibJNI::BaseJavaValue* (*)(v8::Local<v8::Value>);
    
    class FunctionHandler {
    public:
        FunctionHandler(const Nan::FunctionCallbackInfo<v8::Value>& func);
        
        //accessors
        std::string GetName(){ return name; }
        
        int HashCode() {return hashcode; }
        
        Nan::Callback* GetCallback() {return callback; }
        
        void SetArguments(const Nan::FunctionCallbackInfo<v8::Value>& func,
                          std::initializer_list<Algorithm> functions );
        
        std::vector <LibJNI::BaseJavaValue*>& GetArguments(){ return args; }
        // ========
        
        
        template <typename F>
        void DetectAndGetCallback( const Nan::FunctionCallbackInfo<v8::Value>& func, F& fn) {
            callback = Utils::Search(func, fn);
        }
        
        
    private:
        std::string name;
        int hashcode;
        Nan::Callback* callback;
        std::vector <LibJNI::BaseJavaValue*> args;
    };
    
    
    
    template <typename Base>
    class InvocationList {
    public:
        
        std::map<std::string, Base* > callers;
        
        template <typename T>
        void Create() {
            auto caller = new T;
            
            callers[ caller->Type() ] = caller;
        }
        
        Base* operator()(std::string type){
            auto tmp = callers[type];
            if( tmp == nullptr )
                throw VMError{"No caller implemented for return type: " + type};
            
            return tmp;
        }
    };
    
    
    
    template <typename SupportedInvocation>
    class JNIWorker : public Nan::AsyncWorker {
    
    public:
        JNIWorker(
                  SupportedInvocation _supported,
                  FunctionHandler _fn,
                  std::shared_ptr<LibJNI::Object>& _javaObject
                  ):
        AsyncWorker( _fn.GetCallback() ),
        supported(_supported),
        fn( _fn ),
        javaObject( _javaObject ) {
            LookForReturnType(javaObject);
        };
        
        void HandleOKCallback () {
            v8::Local<v8::Value> argv[] = {
                retValue
            };
        
            
            callback->Call(1, argv);
        };
        
        
        /* Calling V8 here is illegal */
        void Execute() {
            retValue = call();
        };
        
        v8::Local<v8::Value> call(){
            return supported(returnType)->Call(fn.GetName(), javaObject, fn.GetArguments());
        }
        
        bool isAsync() {
            return callback == nullptr;
        }
        
    private:
        void LookForReturnType(std::shared_ptr<LibJNI::Object>& _javaObject) {
            auto method = _javaObject->LookupMethod(fn.GetName(), fn.GetArguments());
            returnType = method.GetReturnTypeInfo();
        }
        
        SupportedInvocation supported;
        std::string returnType;
        FunctionHandler fn;
        std::shared_ptr<LibJNI::Object>& javaObject;
        v8::Local<v8::Value> retValue;
    };
 
    
    
    
}

#endif /* ella_objects_hpp */
