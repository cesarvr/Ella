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
#include "jvm_method.h"

using JEnv = const std::shared_ptr<JNIEnv>;


class JNITypeBase {
private:
    JEnv env;
    
public:
    JNITypeBase(JEnv env): env(env){};
    
    const JEnv& GetEnv(){
        return env;
    };
};

template<typename T>
struct JNIType: JNITypeBase {
    T value;
    
    JNIType(JEnv env):JNITypeBase(env){};
    
    void SetValue(T val){
        value = val;
    }
    
    int GetValue(){
        return value;
    }
};

template<>
struct JNIType<jint>:JNITypeBase {
    jint value;
    
    JNIType(JEnv env):JNITypeBase(env){};
    
    void SetValue(jint val){
        value = (int)val;
    }
    
    int GetValue(){
        return value;
    }
};

template<>
struct JNIType<jstring>:JNITypeBase {
   
    jstring value;
    
    JNIType(JEnv env):JNITypeBase(env){};
    
    void SetValue(jobject val){
        value = (jstring)val;
    }
    
    std::string GetValue(){
        auto env = GetEnv();
        const char *str =env->GetStringUTFChars( (jstring)value , NULL );
        env->ReleaseStringUTFChars( (jstring)value ,str );
        std::string tmp = str;
        return tmp;
    };
    
};

template <typename T>
class Functor {
private:
    JEnv env;
    
public:
    
    using X = T (*)(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);
    X ptr;
    
    Functor( JEnv& _env , X func ):env(_env), ptr(func) {
       
    };
    
    JNIType<T>
    Call(jobject obj, JavaMethod& javaMethod, std::vector<JavaValue> arg = {}) {
        
        JNIType<T> ret(env);
        
        try{
            auto value = ptr( env.get(),
                              obj,
                              javaMethod.methodPTR,
                              javaMethod.arguments.GetArguments(env, arg).get() );
           
            ret.SetValue(value);
            return ret;
        }catch(VMError& error){
            throw error;
        };
    };
};




#endif /* jvm_type_h */
