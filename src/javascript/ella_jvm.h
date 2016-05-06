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
    

    template <typename Fn>
    class JVM : public Nan::AsyncWorker {
    public:
        JVM(Nan::Callback* callback, JVMLoader& _vm, Fn& _fn ):
        AsyncWorker(callback),
        vm(_vm),
        fn(_fn) {};
        
        
        void Execute(){
            try{
                if(!vm.isVMReady()) //only one vm for now.
                    vm.Start();
                
            }catch(VMError& _error){
                error = _error;
            }
        };
        
        void HandleOKCallback () {
            auto object = Nan::New<v8::Object>();
            object->Set(Nan::New("New").ToLocalChecked(),
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
