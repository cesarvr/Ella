//
//  js_vm.h
//  nanProject
//
//  Created by Cesar Valdez on 11/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef js_vm_h
#define js_vm_h



#include <nan.h>
#include "jvm_handler.h"
#include "jvm_object.hpp"
#include "adapter.hpp"
#include "queue_manager.hpp"

#include <map>
#include <regex>


namespace ella {
    
    JVMLoader vm;
    
    std::map<int, Object*> table;
    
    using NArguments = const Nan::FunctionCallbackInfo<v8::Value>;
    using NObject =  v8::Local<v8::Object>;
    
    
    
    struct MethodTopology {
        std::string method;
        std::string hashcode;
    };
    
    
    
    
    MethodTopology GetMethodCall(std::string method) {
        std::regex e ("function (.*)@(.*)\\(\\).+");
        std::cmatch cm;
        std::regex_match (method.c_str(),cm,e);
        
        //std::cout << "matchs: " << cm.size() << std::endl;
        if(cm.size() < 1) throw VMError{"Can't recognize the method signature. what!?, " + std::to_string( cm.size() ) };
        return MethodTopology{cm[1], cm[2]};
    }
    
    
    
    
    
    
    void JSInterface(NArguments& args){
        
        try{
            std::string funcCalle = ObjectToString( args.Callee() );
            
            MethodTopology mth = GetMethodCall(funcCalle);
            
            args.GetReturnValue().Set(Nan::New(funcCalle.c_str()).ToLocalChecked());
            
            auto len = args.Length();
            
            std::vector<JavaValue> arguments;
            arguments.reserve(len);
            
            for (int cnt =0; cnt<len; cnt ++) {
                arguments.push_back( JavaValueAdapter::toJavaValue(args[cnt]) );
            }
            
            int hash = std::stoi(mth.hashcode);
            
            JavaValue value = table[hash]->Call(mth.method, arguments);
            
            if(value.IsString())
                args.GetReturnValue().Set( Nan::New( value.getStringValue() ).ToLocalChecked() );
            
            if(value.IsInteger())
                args.GetReturnValue().Set( Nan::New( value.getIntValue() ) );
            
            
            if (value.IsArray()) {
                
                auto buf = value.getArrayValue();
                uint32_t len = (uint32_t)buf.size();
                
                // just being fancy, this could be done using Nan::CopyBuffer not need of malloc.
                char *ptr = &buf[0];
                char *raw = (char*)malloc(len);
                
                for(int x=0; x<len; x++)
                    raw[x] = *ptr++;
                
                
                args.GetReturnValue().Set( Nan::NewBuffer(raw, len ).ToLocalChecked() );
            }
            
        }catch(VMError& error){
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    }
    
    


    void LoadClass(NArguments& args ) {
        try {
            
            if(vm.isVMReady()){
                
                std::cout << "HOllly Cow  vm ->" << vm.GetJNIEnviorment() << std::endl;
                
                std::string classname = ObjectToString( args[0]->ToString() );
                
                Object *java_object =  new Object (vm.GetJNIEnviorment(), "pdf/P2HService");
                
                NObject object = Nan::New<v8::Object>();
                
                auto methods = java_object->GetMembers();
                auto hashcode = java_object->Call("hashCode", {});
                
                for(const auto& method : methods) {
                    auto fnCallback  = Nan::New<v8::FunctionTemplate>(JSInterface);
                    
                    std::string methodName = method.name + "@" + std::to_string(hashcode.getIntValue());
                    v8::Handle<v8::String> fname = v8::String::NewFromUtf8(args.GetIsolate(), methodName.c_str());
                    
                    table[hashcode.getIntValue()] = java_object;
                    
                    
                    fnCallback->SetClassName(fname);
                    object->Set(Nan::New(method.name).ToLocalChecked(),
                                fnCallback->GetFunction() );
                }
                
                args.GetReturnValue().Set(object);
         
            }else{
                Nan::ThrowTypeError( "JVM not ready." );
            }
            
        }catch(VMError& e){
            Nan::ThrowTypeError( e.errorMessage.c_str() );
        }
    };
    
    
    void LoadClassXX(NArguments& args ) {
        
        std::cout << "HOllly Cow  vm ->"  << std::endl;
        
        try{
            std::cout << "HOllly Cow  vm ->" << vm.GetJNIEnviorment() << std::endl;
            
            LoadClass(args);
            
        }catch(VMError& error){
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    }
    
    
    
    void Start(NArguments& args ){
        
        if(args.Length() < 1)
            Nan::ThrowTypeError("Wrong number of arguments. Expected a string with the classpath.");
        
        
        if (!args[0]->IsFunction())
            Nan::ThrowTypeError("Callback required.");
        
 //       NObject object = Nan::New<v8::Object>();
   //     object->Set(Nan::New("New").ToLocalChecked(),
     //               Nan::New<v8::FunctionTemplate>(LoadClassXX)->GetFunction() );

        
        auto *callBack = new Nan::Callback(args[0].As<v8::Function>());
        
        auto worker = []() {
            
            try{
                
                std::cout << "starting vm" << std::endl;
                if(!vm.isVMReady()) {
                    
                    vm.SetClassPath("-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/");
                    
                    vm.Start();
                }
                
                return true;
                
            }catch(VMError& error){
                Nan::ThrowTypeError( error.errorMessage.c_str() );
            }
            
            return false;
        };
        
        auto whenFinish = [](Nan::Callback* callback,  bool x){
            if(x) {
                

                NObject object = Nan::New<v8::Object>();
                object->Set(Nan::New("New").ToLocalChecked(),
                            Nan::New<v8::FunctionTemplate>(LoadClass)->GetFunction() );

                
                v8::Local<v8::Value> argv[] = {
                    object
                };
                
                callback->Call(1, argv);
            }
        };
        
        auto queueWorker = new AsyncWrap<decltype(worker), decltype(whenFinish)>(callBack, worker, whenFinish);
        Nan::AsyncQueueWorker(queueWorker);
        
        
        
        /*
         const unsigned argc = 1;
         v8::Local<v8::Value> argv[argc] = { object };
         Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callBack, argc, argv);
         */
        
    };
    
    void SetClassPath(NArguments& args){
        if(args.Length() < 1) {
            Nan::ThrowTypeError("Wrong number of arguments. Expected a string with the classpath.");
        }
        
        if (!args[0]->IsString()) {
            Nan::ThrowTypeError("This function expect a String.");
        }
        
        vm.SetClassPath("-Djava.class.path=" + ObjectToString( args[0]->ToString() ));
    };
    
    
}


#endif /* js_vm_h */
