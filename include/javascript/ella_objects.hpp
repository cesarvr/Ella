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
#include "ella_functions.hpp"
#include <regex>
#include <map>

namespace  ella {
    
    using Algorithm = JNIValue (*)(V8Value value);
    
    class FunctionHandler {
    public:
        FunctionHandler(const Nan::FunctionCallbackInfo<v8::Value>& func);
        
        //accessors
        std::string GetName(){ return name; }
        int HashCode() {return hashcode; }
        Nan::Callback* GetCallback() {return callback; }
        
        void GetArguments(const Nan::FunctionCallbackInfo<v8::Value>& func,
                          std::initializer_list<Algorithm> functions );
        
        template <typename F>
        void GetCallback( const Nan::FunctionCallbackInfo<v8::Value>& func, F& fn) {
            callback = Utils::Search(func, fn);
        }
        
        
    private:
        Nan::Callback* callback;
        std::string name;
        int hashcode;
        std::vector <LibJNI::BaseJavaValue *> args;
    };
    
    class JNIWorker : public Nan::AsyncWorker {
    
    public:
        JNIWorker(Nan::Callback *callback,
                  std::vector<BaseCall *> _caller,
                  FunctionHandler _fn,
                  std::string _returnType,
                  std::shared_ptr<LibJNI::Object> _javaObject
                  ):
        AsyncWorker(callback),
        caller(_caller),
        returnType(_returnType),
        fn(_fn),
        javaObject(_javaObject) {};
        
        void execute();
        void call();
        void HandleOKCallback();
        
    private:
        std::vector<BaseCall *> caller;
        std::string returnType;
        FunctionHandler fn;
        std::shared_ptr<LibJNI::Object> javaObject;
        std::vector<LibJNI::BaseJavaValue *> args;
        
    };
 
    
    
    
}

#endif /* ella_objects_hpp */
