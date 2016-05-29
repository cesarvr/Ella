//
//  ella_objects.cpp
//  nanProject
//
//  Created by Cesar Valdez on 22/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "ella_objects.h"

using namespace ella;


FunctionHandler::FunctionHandler(const Nan::FunctionCallbackInfo<v8::Value>& _func) {
    
    
    name = Utils::ObjectToString(_func.Callee()->GetName());
    hashcode = _func.This()->GetIdentityHash();
    

};


void FunctionHandler::SetArguments(const Nan::FunctionCallbackInfo<v8::Value>& func,
                                   std::initializer_list<Algorithm> functions) {
   
    //map each v8 arguemnt to an jvalue argument. 
    for(auto function : functions)
        Utils::SaveEach(func, args, function);
}










