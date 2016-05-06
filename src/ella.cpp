//
//  main.cpp
//  nanProject
//
//  Created by Cesar Valdez on 11/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "ella.h"


void Init(v8::Local<v8::Object>  exports) {
    
    exports->Set(Nan::New("start").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(ella::Start)->GetFunction());
    
    exports->Set(Nan::New("setClassPath").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(ella::SetClassPath)->GetFunction());
    
    exports->Set(Nan::New("getClassPath").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(ella::GetClassPath)->GetFunction() );
}

NODE_MODULE(ella, Init)
