//
//  js_frontend.h
//  nanProject
//
//  Created by Cesar Valdez on 12/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef js_frontend_h
#define js_frontend_h

#include "js_global.h"
#include "java_class.hpp"



JVMLoader loader;

class JSFrontEnd {
    
public:
    
    static void Invoke(V8Arguments& args ) {
        std::string fname = JSUtils::ObjectToString(args.Callee());
        args.GetReturnValue().Set(Nan::New(fname.c_str()).ToLocalChecked());
    }
    
    static void LoadClass(V8Arguments& args ) {
        try {
            std::string classname = JSUtils::ObjectToString( args[0]->ToString() );
            JavaClass javaObject = JavaClass::CreateObject(loader, classname);
            JSObject object = Nan::New<v8::Object>();
            
            auto methods = javaObject.GetMethods();
            
            for(const auto& method : methods){
               
                auto fnCallback  = Nan::New<v8::FunctionTemplate>(Invoke);
                v8::Handle<v8::String> fname = v8::String::NewFromUtf8(args.GetIsolate(), method.name.c_str());
               
                fnCallback->SetClassName(fname);
                object->Set(Nan::New(method.name).ToLocalChecked(),
                            fnCallback->GetFunction() );
            }
            
            args.GetReturnValue().Set(object);
            
        }catch(VMError& e){
            Nan::ThrowTypeError( e.errorMessage.c_str() );
        }
    };
    
    static void Start(V8Arguments& args ){
        
        if(args.Length() < 1) {
            Nan::ThrowTypeError("Wrong number of arguments. Expected a string with the classpath.");
        }
        
        if (!args[0]->IsFunction()) {
            Nan::ThrowTypeError("Callback required.");
        }
        
        try{
            JSObject object = Nan::New<v8::Object>();
            
            object->Set(Nan::New("New").ToLocalChecked(),
                  Nan::New<v8::FunctionTemplate>(LoadClass)->GetFunction() );

            v8::Local<v8::Function> callBack = args[0].As<v8::Function>();

            if(!loader.isVMReady()) {
            
                loader.SetClassPath("-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/");
            
                loader.Start();
            }
            
            const unsigned argc = 1;
            v8::Local<v8::Value> argv[argc] = { object };
            Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callBack, argc, argv);

        }catch(VMError& error){
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    };
    
    static void SetClassPath(V8Arguments& args){
        if(args.Length() < 1) {
            Nan::ThrowTypeError("Wrong number of arguments. Expected a string with the classpath.");
        }
        
        if (!args[0]->IsString()) {
            Nan::ThrowTypeError("This function expect a String.");
        }
        
        loader.SetClassPath("-Djava.class.path=" + JSUtils::ObjectToString( args[0]->ToString() ));
    };
};

#endif /* js_frontend_h */
