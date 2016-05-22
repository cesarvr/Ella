//
//  ella_objects.h
//  nanProject
//
//  Created by Cesar Valdez on 22/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_objects_hpp
#define ella_objects_hpp

#include "nan.h"
#include "object.h"
#include "server.h"
#include "ella_utils.h"

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
        
        std::vector <LibJNI::BaseJavaValue*>&& GetArguments(){ return move(args); }
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
    
    
   


    // this class is an strategy pattern matcher, it match a JVM return type with a class(strategy) to handle that return type. 
    template <typename Base>
    class InvocationList {
    public:
        
        std::map<std::string, Base* > callers;
        
        template <typename T>
        void Create() {
            auto caller = new T;
            
            callers[ caller->Type() ] = caller;
        }
        
        bool ready(){
            return !callers.empty();
        }
        
        Base* operator()(std::string type){
            auto tmp = callers[type];
            if( tmp == nullptr )
                tmp = callers["java.lang.Object"]; //throw VMError{"No caller implemented for return type: " + type};
            
            return tmp;
        }
    };
    
    
    // This is the class that defines an Asynchronous Worker, for now works in queue. 
    
    template <typename SupportedInvocation>
    class JNIWorker : public Nan::AsyncWorker {
        
    public:
        JNIWorker(
                  SupportedInvocation _supported,
                  FunctionHandler _fn,
                  std::shared_ptr<Object<Server>>& _javaObject
                  ):
        AsyncWorker( _fn.GetCallback() ),
        supported(_supported),
        fn( _fn ),
        javaObject( _javaObject ) {
            LookForReturnType(javaObject);
        };
        
        void HandleOKCallback () {
            v8::Local<v8::Value> argv[] = {
                Get()
            };
            
            callback->Call(1, argv);
        };
        
        v8::Local<v8::Value> Get(){
            return supported(returnType)->Get();
        };
        
        // Calling V8 here is illegal 
        void Execute() {
            call();
        };
   
             
        JNIWorker& call(){
            try{
                std::cout << "name: " << fn.GetName() << std::endl;
                supported(returnType)->Call(fn.GetName(), javaObject, fn.GetArguments());
            }catch(VMError& error){
                throw error;
            }
    
            return *this;
        }
    
      // if the user provide a callback we transform this his call in an async call.    
        bool isAsync() {
            return callback != nullptr;
        }
        
    private:
        void LookForReturnType(std::shared_ptr<Object<Server>>& _javaObject) {
            
            auto method = _javaObject->GetMethodDescriptor(fn.GetName(), fn.GetArguments());
            returnType = method.returnType;
        }
        
        SupportedInvocation supported;
        std::string returnType;
        FunctionHandler fn;
        std::shared_ptr<Object<Server>>& javaObject;
        v8::Local<v8::Value> retValue;
    };
    
}

#endif /* ella_objects_hpp */
