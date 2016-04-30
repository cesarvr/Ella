//
//  js_vm.h
//  nanProject
//
//  Created by Cesar Valdez on 11/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef js_vm_h
#define js_vm_h



#include "nan.h"

#include "jvm_handler.h"
#include "jvm_object.hpp"
#include "classpath.hpp"


#include "ella_utils.hpp"
#include "ella_jvm.hpp"
#include "ella_objects.hpp"
#include "ella_functions.hpp"


#include <map>
#include <regex>


namespace ella {
    JVMLoader vm;
    std::map< int, std::shared_ptr<LibJNI::Object> > objectsMap;
    
    
    using V8Args = const Nan::FunctionCallbackInfo<v8::Value>;
    
    
    void MakeCallToJNI(V8Args& args) {
        
        try {
            FunctionHandler fnHandler( args );
            
            
            //functions dedicated to transform from v8 -> LibJNI::BaseJavaValue in ella_functions.
            fnHandler.SetArguments(args, {GetString, GetInteger});
            fnHandler.DetectAndGetCallback(args, GetFunctionCallback);
            
            
            //supported class are declared in ella_functions.
            InvocationList<BaseCall> supportedInvocations;
            
            supportedInvocations.Create<StringCall>();
            supportedInvocations.Create<IntCall>();
            
            
            auto jniWorker =  new JNIWorker<InvocationList<BaseCall>>(supportedInvocations,
                                                                      fnHandler,
                                                                      objectsMap[fnHandler.HashCode()]);
            
            
            
            if(!jniWorker->isAsync()){
                std::cout << "[sync]" <<std::endl;
                args.GetReturnValue().Set( jniWorker->call() );
            }else{
                std::cout << "[async]" <<std::endl;
                Nan::AsyncQueueWorker(jniWorker);
            }
            
        }catch(VMError& error) {
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    }
    
    
    void ClassLoader(V8Args& args) {
        
        try{
            
            auto classname = Utils::GetClassName(args[0]);
            
            std::shared_ptr<LibJNI::Object> clazz(new LibJNI::Object(vm, classname));
            
            auto methods  = clazz->GetMembers();
            auto hashcode = clazz->Call<int>("hashCode").Get();
            
            auto jsObject = Utils::CreateJSObject<decltype(methods), decltype(MakeCallToJNI)>(methods,hashcode, MakeCallToJNI);
            
            objectsMap[hashcode] = clazz;
            
            args.GetReturnValue().Set(jsObject);
            
        }catch(VMError& error){
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    }
    
    
    /*
     Start (callback)
     ==================================
     
     Start a new JVM instance.
     
     callback: a function where to callback when instance has finish startup.
     
     */
    
    
    void Start(V8Args& args ){
        
        if (!args[0]->IsFunction())
            Nan::ThrowTypeError("Callback required.");
        
        auto *jscallBack = new Nan::Callback(args[0].As<v8::Function>());
        
        auto vmInitWorker = new ella::JVM<decltype(ClassLoader)> (jscallBack, vm, ClassLoader);
        
        Nan::AsyncQueueWorker(vmInitWorker);
    }
    
    
    /*
     SetClassPath (Array, isRecursive)
     ==================================
     
     Array -> a list of paths where to find your [ .jars .class ].
     isRecursive -> true look recursively, false it wont.
     
     */
    
    void SetClassPath(V8Args& args ){
        
        try{
            std::vector<std::string> dirs;
            bool recursive = false;
            
            if( args[0]->IsArray() ){
                auto list = Nan::New<v8::Array>()->Cast(*args[0]);
                dirs = ella::Utils::IterCollection< decltype(list), std::string>(list, ella::Utils::ObjectToString);
            }
            
            if( args[1]->IsBoolean() )
                recursive = args[1]->ToBoolean()->Value();
            
            vm.SetClassPath( ClassPath::LocateLibraries(dirs, recursive) );
            
        }catch(VMError& error){
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
        
    }
    
    void GetClassPath(V8Args& args ){
        args.GetReturnValue().Set( Nan::New(vm.GetClassPath()).ToLocalChecked() );
    }
    
}

#endif /* js_vm_h */
