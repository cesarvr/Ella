//
//  JSUtils.hpp
//  nanProject
//
//  Created by Cesar Valdez on 11/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef JSUtils_hpp
#define JSUtils_hpp

#include <node.h>
#include <v8.h>
#include <iostream.h>
class JSUtils {
    
public:
    
    /* http://izs.me/v8-docs/process_8cc-example.html  */
    
    static std::string ObjectToString( v8::Local< v8::Value > value ) {
        v8::String::Utf8Value utf8_value(value);
        return std::string(*utf8_value);
    }
    
};


#endif /* JSUtils_hpp */
