//
//  java_invoke.h
//  LibJNI
//
//  Created by Cesar Valdez on 14/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef java_invoke_hpp
#define java_invoke_hpp

#include "jvm_handler.h"
#include "utils.h"
#include "values.h"


using namespace LibJNI;



//    Wraps the call with:
  //      - Execption handling.
  //      - Nullpointer checking.
  //      - Check VM status.

template <typename Function, typename ...Args>
auto Wrapper(Function&& func, JEnv env, Args&&... args)
-> decltype(func(env.get(), args...))
{
    try {
        
        assert(func != nullptr);
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


// Return the right function to call the JVM depending on the template parameter.
template <class JE, typename T>
struct Fn{
    //handle this JNI calls.

    static auto GetInterface(JE& env) ->decltype(env->functions->CallObjectMethodA) {
        return env->functions->CallObjectMethodA;
    }
};

template <class JE>
struct Fn<JE, IntValue > {
    
    static auto GetInterface(JE& env) ->decltype(env->functions->CallIntMethodA) {
        return env->functions->CallIntMethodA;
    }
};


template <class JE>
struct Fn<JE, FloatValue > {
    
    static auto GetInterface(JE& env) ->decltype(env->functions->CallFloatMethodA) {
        return env->functions->CallFloatMethodA;
    }
};


template <class JE>
struct Fn<JE, jbyte > {
    
    using ReturnType = jbyte;
    
    static auto GetInterface(JE& env) ->decltype(env->functions->CallByteMethodA) {
        return env->functions->CallByteMethodA;
    }
};



// This take charge of the invocation.
struct Invoke: HandleEnv {
    
    Invoke(JVMLoader env): HandleEnv(env) {};
    
    
    
   
     
       // Make the call to the JVM with the right method, then it use the method Set with the following
       // signature Set(JEN, value) and place the value inside the object, the object can be any object that
       // that agreed with that method, this make this class flexible for other value implementation.
     
    template <typename T, typename... Args>
    T Call(Args... args) {
       
        auto env = GetEnv();
        auto I =  Fn<JEnv, T>::GetInterface(env);
        
        T value;
        
        auto ret = Wrapper(I, env, args...);
        
        value.Set(env, ret);
        
        return value;
    }
};




#endif /* java_invoke_hpp */
