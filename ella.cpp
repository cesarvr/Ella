//
//  main.cpp
//  nanProject
//
//  Created by Cesar Valdez on 11/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//


#include <node.h>
#include <v8.h>


#include "java_class.hpp"
#include "js_vm.hpp"

using namespace v8;


JVMLoader vm;

using V8Arguments = const v8::FunctionCallbackInfo<Value>&;

void SetClassPath(V8Arguments args){
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    
    if(args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments.")));
    }
    
    if (!args[0]->IsString()) {
         isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "This funciton expect a String.")));
    }
    vm.SetClassPath( JSUtils::ObjectToString( args[0]->ToString() ));
}


void Start(V8Arguments args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    
    vm.SetClassPath("-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/");
    
    
    args.GetReturnValue().Set( String::NewFromUtf8(isolate, vm.Start().c_str() ));
}

void Init(Handle<Object> exports) {
    Isolate* isolate = Isolate::GetCurrent();
    
    exports->Set(String::NewFromUtf8(isolate, "start"),
                 FunctionTemplate::New(isolate, Start)->GetFunction());
    
    

}

NODE_MODULE(ella, Init)