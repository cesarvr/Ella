//
//  jvm_reflect.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_object.hpp"

std::string ReturnArrayOf(std::string className) {
    return "()[L" + className + ";";
};

std::string ReturnTypeOf(std::string className) {
    return "()L" + className + ";";
};

Object::Object(JVMLoader loader, std::string className):
HandleEnv(loader),
reflect(loader),
objectMethod(loader, loader.GetJNIEnviorment()->functions->CallObjectMethodA),
intMethod(loader, loader.GetJNIEnviorment()->functions->CallIntMethodA)
{
    
    auto env = GetEnv();
    name = className;
    
    auto member = Wrapper(env->functions->FindClass, env, className.c_str() );
    
    
    auto constructor = Wrapper(env->functions->GetMethodID,
                               env, member,
                               CLASS_DEFAULT_CTS.c_str(),
                               VOID_RETURN.c_str() );
    
    object = Wrapper(env->functions->NewObject,
                     env,
                     member,
                     constructor );
    
    reflect.SetClass(object);
    
    
    
    methods = reflect.GetMethodsDefinition();
};


JavaValue Object::Call(std::string methodName, std::vector<JavaValue> args){
    
    bool methodNotFound = true;
    
    
    auto env = GetEnv();
    
    
    JavaValue value;
    
    for(auto& method: methods ){
        
        if( methodName == method.name ){
            methodNotFound = false;
            
            
            if( method.returnType == JSTRING ){
                
                auto obj = objectMethod.Call<jstring>(object,
                                                      method.methodPTR,
                                                      method.arguments.GetArguments(env, args).get());
              
                value =  JavaValue(obj.GetValue());
                
            }else if( method.returnType == JINT ){
                
                
                
                auto obj = intMethod.Call<jint>(object,
                                                method.methodPTR,
                                                method.arguments.GetArguments(env, args).get());
                
                
                value = JavaValue(obj.GetValue());
                
            }else if( method.returnType == JBYTE_ARRAY ){
                
                auto obj = objectMethod.Call<jobject>(object,
                                                      method.methodPTR,
                                                      method.arguments.GetArguments(env, args).get());
                
                value = JavaValue(Utils::GetArrayFromJVM<char>(env, (jobjectArray)obj.GetValue()));
            }
        }
    }
    
    if (methodNotFound) throw VMError{"Method not found: " + methodName};
    
    return value;
};

const std::vector<JavaMethod>& Object::GetMembers(){
    return methods;
}

std::string Object::GetClassName(){
    return name;
};



Reflect::Reflect(JVMLoader loader):
HandleEnv(loader),
objectMethod(loader, loader.GetJNIEnviorment()->functions->CallObjectMethodA) {
    
};




jmethodID Reflect::GetMethod( std::string className, std::string method, std::string returnType) {
    auto env = GetEnv();
    
    auto member = Wrapper(env->functions->FindClass, env, className.c_str() );
    return Wrapper(env->functions->GetMethodID, env, member, method.c_str(), returnType.c_str() );
}

std::vector<JavaMethod>
Reflect::GetMethodsDefinition() {
    
    auto jmethodArray = GetMethod(JAVA_CLASS,
                                  METHOD_GET_METHODS,
                                  ReturnArrayOf(METHOD_CLASS));
    
    auto methodsList = objectMethod.Call<jobject>(clazz, jmethodArray, nullptr);
    
    auto env = GetEnv();
    
    auto Fn = [this](JEnv env, jobject& object ) {
        JavaMethod methd;
        
        methd.name = GetName(METHOD_CLASS, object);
        methd.returnType = GetReturnType(object);
        methd.arguments = JavaArguments( GetParameters(object) );
        methd.methodPTR = Wrapper(env->functions->FromReflectedMethod, env, object );
       
        return methd;
    };
    
    return Utils::IterateJObjectArray< decltype(Fn), JavaMethod >( GetEnv(), (jobjectArray) methodsList.GetValue(), Fn );
}


std::string Reflect::GetName(std::string className, jobject object) {
    
    auto _tmp = GetMethod(className,
                          METHOD_GET_NAME,
                          ReturnTypeOf(JAVA_STRING_CLASS) );
    
    return objectMethod.Call<jstring>(object, _tmp, nullptr).GetValue();
}

std::string Reflect::ToString(std::string className, jobject object){
    
    auto _tmp = GetMethod(METHOD_CLASS,
                          METHOD_TOSTRING,
                          ReturnTypeOf(JAVA_STRING_CLASS) );
    
    return objectMethod.Call<jstring>(object, _tmp, nullptr).GetValue();
}

std::string Reflect::GetReturnType(jobject object){
    
    auto GetReflectionAPI = GetMethod(METHOD_CLASS,
                                      METHOD_RETURN_TYPE,
                                      ReturnTypeOf(JAVA_CLASS) );
    
    auto tmp = objectMethod.Call<jobject>(object, GetReflectionAPI, nullptr);
   
    return GetName(JAVA_CLASS, tmp.GetValue());
}

std::vector<std::string>
Reflect::GetParameters(jobject object) {
    
    auto GetParametersArray = GetMethod(METHOD_CLASS,
                                        METHOD_GET_PARAMETER,
                                        ReturnArrayOf(JAVA_CLASS) );
    
    auto array = objectMethod.Call<jobject>(object, GetParametersArray, nullptr);
    
    
    auto Fn = [this](JEnv env, jobject& object ) {
        return GetName(JAVA_CLASS, object);
    };
    
    return Utils::IterateJObjectArray< decltype(Fn), std::string >( GetEnv(), (jobjectArray) array.GetValue(), Fn );
}

/* Class < Object > <-- GetClass() */
void Reflect::SetClass(jobject object) {
    
    auto getClass = GetMethod(JAVA_CLASS, METHOD_GET_CLASS.c_str(), ReturnTypeOf(JAVA_CLASS) );
    clazz = objectMethod.Call<jobject>(object, getClass, nullptr).GetValue();
}

jobject Reflect::GetReflectClass() {
    return clazz;
}


