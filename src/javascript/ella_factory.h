//
//  ella_factory.h
//  nanProject
//
//  Created by Cesar Valdez on 29/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_factory_h
#define ella_factory_h

class ObjectBinding {
public:
    
    ObjectBinding() {}
    
    template <typename Fn, typename T>
    v8::Local<v8::Object> CreateFunction(T& object, Fn& callback ){
        
        auto isolate = Nan::GetCurrentContext()->GetIsolate();
        auto methods  = object->MethodsNames();
        auto classname = object->GetName();
        
        auto tmpl = objectTmpl[classname];
        
        if(!tmpl.IsEmpty())
            return tmpl.Get(isolate)->NewInstance();
        
        auto jsobject = Nan::New<v8::ObjectTemplate>();
        
        for(auto method: methods) {
            auto fn = Nan::New<v8::FunctionTemplate>( callback );
            fn->GetFunction()->SetName( Nan::New(method).ToLocalChecked() );
            jsobject->Set( Nan::New( method ).ToLocalChecked(), fn->GetFunction() );
        }
        
        objectTmpl[classname].Set(isolate, jsobject);
        return jsobject->NewInstance();
    }
    
private:
    map<string, v8::Eternal<v8::ObjectTemplate>> objectTmpl;
};



#endif /* ella_factory_h */
