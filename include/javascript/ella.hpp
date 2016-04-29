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
        FunctionHandler fnHandler( args );
        
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
    
    
    void Start(V8Args& args ){
        
        if (!args[0]->IsFunction())
            Nan::ThrowTypeError("Callback required.");
        
        auto *jscallBack = new Nan::Callback(args[0].As<v8::Function>());
        
        auto vmInitWorker = new ella::JVM<decltype(ClassLoader)> (jscallBack, vm, ClassLoader);
        
        Nan::AsyncQueueWorker(vmInitWorker);
    }
    
    void SetClassPath(V8Args& args ){
        std::vector<std::string> dirs;
        bool recursive = false;
        
        if( args[0]->IsArray() ){
            auto list = Nan::New<v8::Array>()->Cast(*args[0]);
            dirs = ella::Utils::IterCollection< decltype(list), std::string>(list, ella::Utils::ObjectToString);
        
        }
        
        if( args[1]->IsBoolean() )
            recursive = args[1]->ToBoolean()->Value();
        
        vm.SetClassPath( ClassPath::LocateLibraries(dirs, recursive) );
    }
    
    void GetClassPath(V8Args& args ){
        args.GetReturnValue().Set( Nan::New(vm.GetClassPath()).ToLocalChecked() );
    }

}

#endif /* js_vm_h */