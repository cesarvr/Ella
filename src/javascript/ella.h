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

//LibJNI
#include "jvm_handler.h"
#include "object.cpp"
#include "object.h"
#include "server.h"
#include "classpath.h"

#include "ella_utils.h"
#include "ella_jvm.h"
#include "ella_objects.h"
#include "ella_types.h"

#include <map>
#include <regex>


namespace ella {
    using namespace LibJNI;
    JVMLoader vm;
    std::map< int, std::shared_ptr< Object<Server> > > objectsMap;
    Server server;
    
    using V8Args = const Nan::FunctionCallbackInfo<v8::Value>;
    
    //supported class are declared in ella_functions.
    InvocationList<BaseCall> supportedInvocations;
   
    
    // initializing the Server & Call strategies.
    void Initialize(){
        
        if(!supportedInvocations.ready()){
            cout << "vm->" << endl;
            // call flavors.
            supportedInvocations.Create<StringCall>();
            supportedInvocations.Create<IntCall>();
            supportedInvocations.Create<DoubleCall>();
            supportedInvocations.Create<VoidCall>();
            supportedInvocations.Create<ByteArrayCall>();
        }
    }
    
    
   //here we extract the functions body, arguments and callback and proceed to call the JVM.
    void MakeCallToJNI(V8Args& args) {
        
        try {
            FunctionHandler fnHandler( args );
       
            //functions dedicated to transform from v8 -> LibJNI::BaseJavaValue in  [ella_types.h].
            fnHandler.SetArguments(args, {GetString, GetNumber});
            fnHandler.DetectAndGetCallback(args, GetFunctionCallback);
            
            auto jniWorker =  new JNIWorker<InvocationList<BaseCall>>(supportedInvocations,
                                                                      fnHandler,
                                                                      objectsMap[fnHandler.HashCode()]);
            
            if(!jniWorker->isAsync()){
                args.GetReturnValue().Set( jniWorker->call().Get() );
                delete jniWorker;
            }else{
                Nan::AsyncQueueWorker(jniWorker);
            }
            
        }catch(VMError& error) {
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    }
    
   
   // Class loader, instanciation and cache in dictionary using the hashcode as key.  
    void ClassLoader(V8Args& args) {
        
        try{
            server.SetJVM(vm);
            auto classname = Utils::GetClassName(args[0]);
            
            std::shared_ptr<Object<Server>> clazz(new Object<Server>(vm,server, classname));
            
            auto methods  = clazz->MethodsNames();
            auto hashcode = clazz->Call<IntValue>("hashCode",{}).Get();
            
            auto jsObject = Utils::CreateJSObject<decltype(methods), decltype(MakeCallToJNI)>(methods,hashcode, MakeCallToJNI);
            
            objectsMap[hashcode] = clazz;
            
            args.GetReturnValue().Set(jsObject);
            
        }catch(VMError& error){
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    }
    
    
    
    // Start (callback)
    // ==================================
     
    // Start a new JVM instance.
     
    // callback: a function where to callback when instance has finish startup.
    
    void Start(V8Args& args ){
        
        Initialize();

        if (!args[0]->IsFunction())
            Nan::ThrowTypeError("Callback required.");
        
        auto *jscallBack = new Nan::Callback(args[0].As<v8::Function>());
        
        auto vmInitWorker = new ella::JVM<decltype(ClassLoader)> (jscallBack, vm, ClassLoader);
        
        Nan::AsyncQueueWorker(vmInitWorker);
    }
    
    
    
    // SetClassPath (Array, isRecursive)
    // ==================================
     
     // Array -> a list of paths where to find your [ .jars .class ].
     // isRecursive -> true look recursively, false it wont.
    
    
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
            
            if(!dirs.empty())
                vm.SetClassPath( ClassPath::LocateLibraries(dirs, recursive) );
            else
                throw VMError{"Array needed: SetClassPath(Array, boolean)"};
            
        }catch(VMError& error){
            Nan::ThrowError( error.errorMessage.c_str() );
        }
        
    }
    
   
    void GetClassPath(V8Args& args ){
        args.GetReturnValue().Set( Nan::New(vm.GetClassPath()).ToLocalChecked() );
    }
    
}




#endif /* js_vm_h */
