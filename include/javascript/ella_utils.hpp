//
//  ella_utils.hpp
//  nanProject
//
//  Created by Cesar Valdez on 21/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_utils_hpp
#define ella_utils_hpp

#include <iostream>
#include <vector>
#include "nan.h"

namespace ella {
    struct Utils {
        /* http://izs.me/v8-docs/process_8cc-example.html  */
        static std::string ObjectToString( v8::Local< v8::Value > value ) {
            v8::String::Utf8Value utf8_value(value);
            return std::string(*utf8_value);
        }
        
        template <typename T, typename Fn>
        static std::vector<T> GetArray( v8::Local<v8::Value> array, Fn& callback ) {
            
            std::vector<T> tmp;
            auto list = Nan::New<v8::Array>()->Cast(*array);
            auto size = list->Length();
            
            for(unsigned int i=0; i<size; i++)
                tmp.push_back(callback(list->Get(i)));
            
            
            return tmp;
        }
    };
}




#endif /* ella_utils_hpp */
