//
//  ella_objects.cpp
//  nanProject
//
//  Created by Cesar Valdez on 22/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "ella_objects.hpp"

using namespace ella;

FunctionHandler::FunctionHandler(const Nan::FunctionCallbackInfo<v8::Value>& _func) {
    
    auto fname = Utils::ObjectToString(_func.Callee());
    std::regex e ("function (.*)@(.*)\\(\\).+");
    std::cmatch cm;
    std::regex_match (fname.c_str(),cm,e);
    
    name = cm[1];
    hashcode = std::stoi(cm[2]);
};


void FunctionHandler::SetArguments(const Nan::FunctionCallbackInfo<v8::Value>& func,
                                   std::initializer_list<Algorithm> functions) {
    
    for(auto function : functions)
        Utils::SaveEach(func, args, function);
}










