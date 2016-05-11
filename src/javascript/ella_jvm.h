//
//  ella_jvm.h
//  nanProject
//
//  Created by Cesar Valdez on 21/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_jvm_hpp
#define ella_jvm_hpp

#include "nan.h"
#include "jvm_handler.h"
#include "values.h"
#include "ella_utils.h"


namespace  ella {
    


    // This is the class that defines an Asynchronous Worker, for now works in queue. 
    template <typename Fn>
    class JVM : public Nan::AsyncWorker {
    public:
        JVM(Nan::Callback* callback, JVMLoader& _vm, Fn& _fn ):
        AsyncWorker(callback),
        vm(_vm),
        fn(_fn) {};
        
       // this method is called by the Nan::AsyncQueue. this spawn a new thread so calling v8 from here is a SEGENV.
        void Execute(){
                if(!vm.isVMReady()) //only one vm for now.
                    vm.Start();
        };
        
        // everything is fine the this method get called.
        void HandleOKCallback () {
            auto object = Nan::New<v8::Object>();
            object->Set(Nan::New("new").ToLocalChecked(),
                        Nan::New<v8::FunctionTemplate>(fn)->GetFunction() );
            
            v8::Local<v8::Value> argv[] = {
                object
            };
            
            callback->Call(1, argv);
        };
        
        
        void HandleErrorCallback() {
            Nan::ThrowTypeError(Nan::New(error.errorMessage).ToLocalChecked());
        }
        
    private:
        JVMLoader &vm;
        Fn& fn;
        VMError error;
    };
    
    
}

#endif /* ella_jvm_hpp */
