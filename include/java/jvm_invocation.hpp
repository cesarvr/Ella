//
//  jvm_type.h
//  LibJNI
//
//  Created by Cesar Valdez on 25/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_type_h
#define jvm_type_h

#include "jvm_global.hpp"
#include "utils.h"





template<typename T>
struct JNIType: HandleEnv {
    T value;
    
    JNIType(JEnv env):HandleEnv(env){};
    
    void SetValue(T val){
        value = val;
    }
    
    T GetValue(){
        return value;
    }
};

template<>
struct JNIType<jint>:HandleEnv {
    jint value;
    
    JNIType(JEnv env):HandleEnv(env){};
    
    void SetValue(jint val){
        value = (int)val;
    }
    
    int GetValue(){
        return value;
    }
};

template<>
struct JNIType<jstring>:HandleEnv {
   
    jstring value;
    
    JNIType(JEnv env):HandleEnv(env){};
    
    void SetValue(jobject val){
        value = (jstring)val;
    }
    
    std::string GetValue(){
        auto env = GetEnv();
        if(value == nullptr) return "Undefined";
        
        const char *str =env->GetStringUTFChars( (jstring)value , NULL );
        env->ReleaseStringUTFChars( (jstring)value ,str );
        std::string tmp = str;
        return tmp;
    };
    
};






/* 
    Make JNI call and capture possible exceptions.
 */


template <typename F, typename ...Args>
auto Wrapper(F && f, JEnv env,  Args &&... args) ->decltype(f(env.get(), args...)) {
    try {
        
        Utils::isNull(env.get());
        Utils::isNull(args...);
        
        return f(env.get(), args...);
    }catch(VMError& error){
        env->ExceptionDescribe();
        throw error;
    };
}




template <typename Fx>
class Functor : HandleEnv {

private:
    JEnv env;
    Fx func;
    
public:
    
    Functor( JEnv _env , Fx _func ): HandleEnv(_env), func(_func) {
       std::cout << "ctor: Functor" << std::endl;
    };
    
    template <typename ReturnType, typename... Args>
    JNIType<ReturnType>
    Call(Args... args) {
        
        auto env = GetEnv();
        
        JNIType<ReturnType> ret(env);
        
        try{
            
            auto value = func(env.get(), args...);
           
            ret.SetValue(value);
            return ret;
        }catch(VMError& error){
            env->ExceptionDescribe();
            throw error;
        };
    };
};




template <typename FN>
class Wrap: HandleEnv {
    
public:
    
    FN fn;
    
    Wrap(JEnv env, FN _fn):HandleEnv(env), fn(_fn){};
    
    template <typename... Args>
    auto call( Args... args) -> decltype(fn(args...)) {
        
        try{
         //   isNull(args...);
            Utils::isNull(args...);
            return fn(args...);
            
        }catch(VMError& error){
            GetEnv()->ExceptionDescribe();
            throw error;
        };
    }
};





#endif /* jvm_type_h */
