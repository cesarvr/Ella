//
//  java_invoke.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 14/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef java_invoke_hpp
#define java_invoke_hpp

#include "jvm_handler.h"
#include "utils.h"
#include "values.hpp"



template <typename Function, typename ...Args>
auto Wrapper(Function&& func, JEnv env, Args&&... args)
-> decltype(func(env.get(), args...))
{
    try {
        Utils::isNull(func);
        assert(env != nullptr);
        
        auto retValue = func(env.get(), args...);
        
        if (env->ExceptionCheck()){
            env->ExceptionDescribe();
            throw VMError{"JVM Exception thrown"};
        }
        
        return retValue;
    }catch(VMError& error){
        throw error;
    };
}



template <class JE, typename T>
struct Fn{
    //handle this JNI calls.

    static auto GetInterface(JE& env) ->decltype(env->functions->CallObjectMethodA) {
        return env->functions->CallObjectMethodA;
    }
};

template <class JE>
struct Fn<JE, jint > {
    
    static auto GetInterface(JE& env) ->decltype(env->functions->CallIntMethodA) {
        return env->functions->CallIntMethodA;
    }
};


struct Invoke: HandleEnv {
    
    Invoke(JVMLoader env): HandleEnv(env) {};
    
    template <typename T, typename... Args>
    LibJNI::Value<T> Call(Args... args) {
       
        auto env = GetEnv();
        auto I =  Fn<JEnv, T>::GetInterface(env);
        
        LibJNI::Value<T> retValue;
        using RetType = typename LibJNI::Value<T>::JType;
        
      //  Utils::isNull(args...);
        
        
        RetType ret = (RetType) Wrapper(I, env, args...);
        retValue.Set(env, ret);
        
        return retValue;
    }
};




#endif /* java_invoke_hpp */
