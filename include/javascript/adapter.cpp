//
//  adapter.cpp
//  nanProject
//
//  Created by Cesar Valdez on 03/03/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "adapter.hpp"

JavaValue JavaValueAdapter::toJavaValue(v8::Local<v8::Value>&& object) {
    
    if(object->IsNumber())
        return JavaValue((int)object->NumberValue()); //TODO: tricky casting to double lossing information here.
    
    if (object->IsString()) {
        auto param = ObjectToString( object->ToString() );
        return JavaValue(  param  );
    }

    return JavaValue();
};