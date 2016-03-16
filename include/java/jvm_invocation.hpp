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
    
    JNIType(JVMLoader env):HandleEnv(env){};
    
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
    
    JNIType(JVMLoader env):HandleEnv(env){};
    
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
    
    JNIType(JVMLoader env):HandleEnv(env){};
    
    void SetValue(jobject val){
        value = (jstring)val;
    }
    
    std::string GetValue(){
        auto env = GetEnv();
        if(value == nullptr) return "Undefined";

        const char *str =env->GetStringUTFChars( (jstring)value , NULL );
        std::string tmp{str};
        env->ReleaseStringUTFChars( (jstring)value ,str );
        
        return tmp;
    };
    
};






/*
 Make JNI call and capture possible exceptions.
 */


template <typename F, typename ...Args>
auto Wrapper(F && f, JEnv env,  Args &&... args) ->decltype(f(env.get(), args...)) {
    try {
        Utils::isNull(f);
        Utils::isNull(env);
        
        auto x = f(env.get(), args...);
        
        if (env->ExceptionCheck()){
            throw VMError{"error in JVM: \n"};
            env->ExceptionDescribe();
        }
        
        return x;
    }catch(VMError& error){
        env->ExceptionDescribe();
        throw error;
    };
}

/*
 Class to wrap JNI Call###Method.
 */

template <typename Fx>
class Functor : HandleEnv {
    
private:
    JEnv env;
    Fx func;
    
public:
    
    Functor( JVMLoader _env , Fx _func ): HandleEnv(_env), func(_func) { };
    
    template <typename ReturnType, typename... Args>
    JNIType<ReturnType>
    Call(Args... args) {
        auto env = GetEnv();
        JNIType<ReturnType> ret(GetLoader());
        
        auto value = Wrapper(func, env, args...);
        ret.SetValue(value);
        
        return ret;
    };
};

#endif /* jvm_type_h */
