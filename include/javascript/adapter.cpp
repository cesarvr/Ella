//
//  adapter.cpp
//  nanProject
//
//  Created by Cesar Valdez on 03/03/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "adapter.hpp"

JavaValue JavaValueAdapter::toJavaValue(v8::Local<v8::Value>&& object) {
    
    if(object->IsNumber()){
        std::cout << "params :) " <<  object->NumberValue() << std::endl;
        return JavaValue((int)object->NumberValue()); //TODO: tricky casting to double losing information here.
    }
    
    if (object->IsString()) {
        auto param = ObjectToString( object->ToString() );
        std::cout << "params :) " <<  param << std::endl;
        return JavaValue(  param  );
    }

    return JavaValue();
};