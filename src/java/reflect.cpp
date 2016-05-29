//
//  jvm_reflect.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 21/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "reflect.h"


// Helper classes
std::string ReturnArrayOf(std::string className) {
    return std::move("()[L" + className + ";");
};

std::string ReturnTypeOf(std::string className) {
    return  std::move("()L" + className + ";");
};

jmethodID Reflect::GetMethod(JEnv& env, std::string className, std::string method,
                             std::string returnType) {

    auto member = Wrapper(env->functions->FindClass, env, className.c_str());
    return Wrapper(env->functions->GetMethodID, env, member, method.c_str(),
                   returnType.c_str());
}

jmethodID Reflect::GetMethodReference(string alias, JEnv& env, jobject object){
    return Wrapper(env->functions->FromReflectedMethod, env, object);
}


ObjectValue Reflect::GetClass(JEnv& env, LibJNI::ObjectValue &object) {
    auto getClass =
    GetMethod(env, JAVA_CLASS, METHOD_GET_CLASS.c_str(), ReturnTypeOf(JAVA_CLASS));
    return Invoke::Call<ObjectValue>(env, object.Get(), getClass, nullptr);
}


ObjectArray Reflect::GetMethodsArray(JEnv& env, ObjectValue clazz){
    auto methods =
    Reflect::GetMethod(env, JAVA_CLASS, METHOD_GET_METHODS, ReturnArrayOf(METHOD_CLASS));
    
    return Invoke::Call<ObjectArray>(env, clazz.Get(), methods, nullptr);
}

std::vector<string> Reflect::GetMethodsNames(JEnv& env, ObjectArray methods) {

    auto Fn = [](JEnv env, jobject &object) {
        return Reflect::GetName(env, object);
    };
    auto tmp = Utils::IterateJObjectArray<decltype(Fn), string>(env, (jobjectArray)methods.Get(), Fn);
    return std::move(tmp);
}

std::string Reflect::GetName(JEnv& env, jobject object) {
    
    return std::move(GetName(env, METHOD_CLASS, object));
}

std::string Reflect::GetName(JEnv& env, string methodName,  jobject object) {
    
    auto _tmp =
    Reflect::GetMethod(env, methodName, METHOD_GET_NAME, ReturnTypeOf(JAVA_STRING_CLASS));
    
    auto str = Invoke::Call<StringValue>(env, object, _tmp, nullptr).Get();
    return std::move(str);
}


std::string Reflect::ToString(JEnv& env, std::string className, jobject object) {
    
    auto _tmp =
    GetMethod(env, METHOD_CLASS, METHOD_TOSTRING, ReturnTypeOf(JAVA_STRING_CLASS));
    
    auto str = Invoke::Call<StringValue>(env, object, _tmp, nullptr).Get();
    return std::move(str);
}

std::string Reflect::GetReturnType(JEnv& env, jobject object) {
    
    auto GetReflectionAPI =
    GetMethod(env, METHOD_CLASS, METHOD_RETURN_TYPE, ReturnTypeOf(JAVA_CLASS));
    
    auto tmp = Invoke::Call<ObjectValue>(env, object, GetReflectionAPI, nullptr);
    
    return GetName(env,JAVA_CLASS, tmp.Get());
}

vector<string> Reflect::GetParameters(JEnv& env, jobject object) {
    
    auto GetParametersArray =
    GetMethod(env, METHOD_CLASS, METHOD_GET_PARAMETER, ReturnArrayOf(JAVA_CLASS));
    
    auto array = Invoke::Call<ObjectValue>(env, object, GetParametersArray, nullptr);
    
    auto Fn = [](JEnv env, jobject &object) {
        return GetName(env, JAVA_CLASS, object);
    };
    
    auto params =
    Utils::IterateJObjectArray<decltype(Fn), string>(env, (jobjectArray)array.Get(), Fn);
    
    return move(params);
}

