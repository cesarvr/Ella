//
//  ella_utils.h
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
        
        template <typename T>
        static bool isEmpty(T& t){
            return t == nullptr;
        }
        
        static bool isEmpty( std::string _val){
            return _val.empty();
        }
        
     
        
        /* http://izs.me/v8-docs/process_8cc-example.html  */
        static std::string ObjectToString( v8::Local< v8::Value > value ) {
           
            if(!value->IsString())
                Nan::ThrowTypeError("Error: String type expected.");

            v8::String::Utf8Value utf8_value(value);
            return std::string(*utf8_value);
        }
        
        static std::string GetClassName(v8::Local< v8::Value > value) {
            
            std::string classname = Utils::ObjectToString( value );
            std::replace( classname.begin(), classname.end(), '.', '/');
            
            return classname;
        }
        
        template <typename V8Collection, typename T, typename Fn>
        static std::vector<T> IterCollection(V8Collection collection, Fn& callback ) {
            
            std::vector<T> tmp;
            auto size = collection->Length();
            
            for(unsigned int i=0; i<size; i++){
                auto val = callback(collection->Get(i));
                if(!isEmpty(val))
                    tmp.push_back(val);
            }
            
            return tmp;
        }
        
        template <typename Collection, typename V8Collection, typename Fn>
        static void SaveEach(V8Collection& v8collection, Collection& collection, Fn& callback){
            auto len = v8collection.Length();
            
            for(auto i=0; i<len; i++){
               auto tmp = callback(v8collection[i]);
                if(!isEmpty(tmp))
                    collection.push_back(tmp);
            }
        }
        
        template <typename V8Collection, typename Fn>
        static auto
        Search(V8Collection& v8collection, Fn& callback) ->decltype(callback(v8collection[0])) {
            
            auto len = v8collection.Length();
            
            for(auto i=0; i<len; i++){
                auto tmp = callback(v8collection[i]);
                if(!isEmpty(tmp))
                    return tmp;
            }
            return nullptr;
        }
        
 
        
        
        template <typename Methods, typename Fn>
        static auto CreateJSObject(Methods& methods, int hashcode, Fn& callback ) ->decltype(Nan::New<v8::Object>()) {
            auto object = Nan::New<v8::Object>();
            
            for(auto method: methods) {
                auto fnCallback  = Nan::New<v8::FunctionTemplate>(callback);
                
                std::string name = method.GetName() + "@" + std::to_string(hashcode);
                
                fnCallback->SetClassName(Nan::New( name ).ToLocalChecked());
                
                object->Set(Nan::New( method.GetName() ).ToLocalChecked(),
                            fnCallback->GetFunction() );
            }
            
            return object;
        };
        
    };
    

    
    
}




#endif /* ella_utils_hpp */
