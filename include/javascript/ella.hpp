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
#include "adapter.hpp"
#include "queue_manager.hpp"

#include <map>
#include <regex>


namespace ella {
    
    JVMLoader vm;

    int count = 0;
    std::map<int, Object*> table;
    
    using NArguments = const Nan::FunctionCallbackInfo<v8::Value>;
    using NObject =  v8::Local<v8::Object>;
    
    struct MethodTopology {
        std::string method;
        int hashcode;
        std::vector<JavaValue> arguments;
        Nan::Callback *callback = nullptr;
    };

    struct AsynchronousWrapper : Nan::AsyncWorker {
    private:
        MethodTopology method;
        Object object;
        JavaValue returnValue;
        int count;
        
    public:
        
        AsynchronousWrapper(MethodTopology& method, Object object, Nan::Callback* callback, int _count):
        Nan::AsyncWorker(callback),
        method(method),
        object(object),
        returnValue(),
        count{ _count }
        {};
        
        
        void Execute(){
            try{
                returnValue = object.Call(method.method, method.arguments);
            }catch(VMError& error){
                std::cout << "Error in LibUV Land: "  << error.errorMessage << "Thread Number:" << std::endl;
            }
        }
        
        v8::Local<v8::Value> CallSync() {
            returnValue = object.Call(method.method, method.arguments);
            return getValue();
        }
        
        v8::Local<v8::Value> getValue(){
            
            v8::Local<v8::Value> argv[1];
            
            if(returnValue.IsString())
                argv[0] = Nan::New( returnValue.getStringValue() ).ToLocalChecked();
            
            if(returnValue.IsInteger())
                argv[0] = Nan::New( returnValue.getIntValue() );
            
            
            if (returnValue.IsArray()) {
                
                auto buf = returnValue.getArrayValue();
                auto len = buf.size();
             
                // just being fancy, this could be done using Nan::CopyBuffer not need of malloc.
                char *ptr = &buf[0];
                char *raw = (char*)malloc(len);
                
                for(unsigned long x=0; x<len; x++)
                    raw[x] = *ptr++;
                
                argv[0] = Nan::NewBuffer(raw, (int)len ).ToLocalChecked();
            }
            
            return argv[0];
        }
        
        void HandleOKCallback () {
            v8::Local<v8::Value> argv[] = {
                getValue()
            };
            
            //std::cout << "thread " << count << "finish" << std::endl;
            
            callback->Call(1, argv);
            object.ReleaseThread();
        }
    };
    
    MethodTopology GetMethodCall(std::string method, NArguments& args) {
        
        MethodTopology tmp;
        
        std::regex e ("function (.*)@(.*)\\(\\).+");
        std::cmatch cm;
        std::regex_match (method.c_str(),cm,e);
        
        if(cm.size() < 1) throw VMError{"Can't recognize the method signature. what!? this should never happen. " + std::to_string( cm.size() ) };
        
        auto len = args.Length();
        
        for (int cnt =0; cnt<len; cnt ++) {
            
            if( args[cnt]->IsFunction() ) {
                tmp.callback = new Nan::Callback(args[cnt].As<v8::Function>());
            }else{
                tmp.arguments.push_back( JavaValueAdapter::toJavaValue(args[cnt]) );
            }
        }
        
        tmp.method = cm[1];
        tmp.hashcode = std::stoi(cm[2]);
        
        return tmp;
    }
    
    
    void JSInterface(NArguments& args){
        //std::cout << "calling method" << std::endl;


        try{
            std::string funcCalle = ObjectToString( args.Callee() );
            
            MethodTopology mth = GetMethodCall(funcCalle, args);
            
            args.GetReturnValue().Set(Nan::New(funcCalle.c_str()).ToLocalChecked());
            
           
            count ++;
            if(mth.callback != 0x0) {
                
                Nan::AsyncQueueWorker( new AsynchronousWrapper( mth, *table[mth.hashcode], mth.callback, count  ) );
            }else{
                AsynchronousWrapper exec( mth, *table[mth.hashcode], mth.callback, count);
                args.GetReturnValue().Set( exec.CallSync() );
            }
            
        }catch(VMError& error){
            Nan::ThrowTypeError( error.errorMessage.c_str() );
        }
    }
    
    void LoadClass(NArguments& args ) {
        try {
                std::string classname = ObjectToString( args[0]->ToString() );
                
                Object *java_object =  new Object(vm, classname);
                
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
        
        }catch(VMError& e){
            Nan::ThrowTypeError( e.errorMessage.c_str() );
        }    
    }
    
    void Start(NArguments& args ){
        
        if (!args[0]->IsFunction())
            Nan::ThrowTypeError("Callback required.");
        
        auto *callBack = new Nan::Callback(args[0].As<v8::Function>());
        
        auto worker = []() {
            try{
                if(!vm.isVMReady()) {
                    
                   // vm.SetClassPath("-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/");
                    
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
        
        auto queueWorker = new AsyncWrap<decltype(worker), bool, decltype(whenFinish)>(callBack, worker, whenFinish);
        Nan::AsyncQueueWorker(queueWorker);
    }
    
    
    void GetClassPath(NArguments& args) {
        args.GetReturnValue().Set( Nan::New(vm.GetClassPath()).ToLocalChecked() );
    }
    
    void SetClassPath(NArguments& args){
 
        auto size = args.Length();
        
        std::string classpath;
        for(auto i =0; i<size; i++) {
            auto separator = (i<size-1)?":":"";
            classpath += ObjectToString( args[i]->ToString() ) +  separator;
        }
        
        vm.SetClassPath( "-Djava.class.path=.:" + classpath );
    }
}


#endif /* js_vm_h */
