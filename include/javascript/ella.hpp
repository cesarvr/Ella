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

#include <map>
#include <regex>


namespace ella {
    JVMLoader vm;

    
    using V8Args = const Nan::FunctionCallbackInfo<v8::Value>;
    
    
    void ClassLoader(V8Args& args) {
        std::cout << "boom" << std::endl;
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
            dirs = ella::Utils::GetArray<std::string>(args[0], ella::Utils::ObjectToString);
        }
        
        if( args[1]->IsBoolean())
            recursive = args[1]->ToBoolean()->Value();
        
        vm.SetClassPath( ClassPath::LocateLibraries(dirs, recursive) );
        
    }
        
    void GetClassPath(V8Args& args ){
        args.GetReturnValue().Set( Nan::New(vm.GetClassPath()).ToLocalChecked() );
    }
    
}

#endif /* js_vm_h */