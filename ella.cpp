//
//  main.cpp
//  nanProject
//
//  Created by Cesar Valdez on 11/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//


#include <nan.h>


#include <java_class.hpp>
#include <js_vm.hpp>


JVMLoader vm;

using V8Arguments = const Nan::FunctionCallbackInfo<v8::Value>&;

void SetClassPath(V8Arguments args){
   

    if(args.Length() < 1) {
       Nan::ThrowTypeError("Wrong number of arguments.");
    }
    
    if (!args[0]->IsString()) {
         Nan::ThrowTypeError("This funciton expect a String.");
    }
    vm.SetClassPath( JSUtils::ObjectToString( args[0]->ToString() ));
}


void Start(V8Arguments args) {
    
    
    vm.SetClassPath("-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/");
    
    
    args.GetReturnValue().Set( Nan::New( vm.Start() ).ToLocalChecked());
}

void Init(v8::Local<v8::Object>  exports) {
    
    exports->Set(Nan::New("start").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Start)->GetFunction());
}

NODE_MODULE(ella, Init)