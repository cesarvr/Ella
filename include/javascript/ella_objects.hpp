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
    
    
    
    template <typename CallObject>
    class JNIWorker : public Nan::AsyncWorker {
    
    public:
        JNIWorker(
                  std::vector<CallObject *> _caller,
                  FunctionHandler _fn,
                  LibJNI::Object& _javaObject
                  ):
        AsyncWorker( _fn.GetCallback() ),
        caller( _caller ),
        fn( _fn ),
        javaObject( _javaObject ) {
            LookForReturnType(javaObject);
        };
        
        void HandleOKCallback () {
        };
        
        void execute() {
            call();
        };
        
        void call(){
            for(auto call: caller )
                if(returnType == call->Type())
                    call->Call(fn.GetName(), javaObject, fn.GetArguments());
        }
        
    private:
        void LookForReturnType(LibJNI::Object& _javaObject) {
            returnType = _javaObject.LookupMethod(fn.GetName(), fn.GetArguments());
        }
        
        std::vector<CallObject *> caller;
        std::string returnType;
        FunctionHandler fn;
        LibJNI::Object& javaObject;
    };
 
    
    
    
}

#endif /* ella_objects_hpp */
