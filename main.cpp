//
//  main.cpp
//  nanProject
//
//  Created by Cesar Valdez on 11/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "ella.hpp"
#include "js_frontend.h"


void Init(v8::Local<v8::Object>  exports) {
    
    exports->Set(Nan::New("start").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(JSFrontEnd::Start)->GetFunction());
    
    exports->Set(Nan::New("setClassPath").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(JSFrontEnd::SetClassPath)->GetFunction());
}

NODE_MODULE(ella, Init)
