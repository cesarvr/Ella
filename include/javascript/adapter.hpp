//
//  adapter.hpp
//  nanProject
//
//  Created by Cesar Valdez on 03/03/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef adapter_hpp
#define adapter_hpp

#include "java_value.hpp"
#include <nan.h>

/* http://izs.me/v8-docs/process_8cc-example.html  */
static std::string ObjectToString( v8::Local< v8::Value > value ) {
    
    v8::String::Utf8Value utf8_value(value);
    return std::string(*utf8_value);
}

class JavaValueAdapter : JavaValue {
    
    
public:
    static JavaValue toJavaValue(v8::Local<v8::Value>&& object);

    static auto handleReturn(JavaValue& jValue) ->decltype( Nan::New(jValue.getStringValue()) ) {
        
        return Nan::New(jValue.getStringValue());
    }
    
};


#endif /* adapter_hpp */
