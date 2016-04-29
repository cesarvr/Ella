//
//  jvm_reflect.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_object.hpp"


using namespace LibJNI;


std::string ReturnArrayOf(std::string className) {
    return "()[L" + className + ";";
};

std::string ReturnTypeOf(std::string className) {
    return "()L" + className + ";";
};

Object::Object(JVMLoader loader, std::string className):
HandleEnv(loader),
reflect(loader),
invoke(loader) {
    auto &env = GetEnv();
    name = className;
    
    auto member = Wrapper(env->functions->FindClass, GetEnv(), className.c_str() );
    
    
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


JavaMethod Object::FindFirstMethod( std::string methodName ) {
    for(auto& method: methods ){
        if (method.GetName() == methodName) {
            return method;
        }
    }
    
    throw VMError({ "Method not found: " + methodName });
} 

std::vector<JavaMethod> Object::FindMethod( std::string methodName ) {

    std::vector<JavaMethod> methodsList;
    for(auto& method: methods ){
        if (method.GetName() == methodName)
            methodsList.push_back(method);
    }
    
    if(methodsList.size() == 0)
        throw VMError({ "Method not found: " + methodName });

    return methodsList;
}



const std::vector<JavaMethod>& Object::GetMembers(){
    return methods;
};

std::string Object::GetClassName(){
    return name;
};


JavaMethod Object::LookupMethod(std::string methodName, std::vector<LibJNI::BaseJavaValue *>& arguments ) {
    
    auto methodCollection = FindMethod(methodName);
    

    auto validateArguments = [arguments](JavaMethod& method){
        int index = 0;
        
        if(arguments.size() != method.ArgumentsType().GetNumberOfArguments())
            return false;
        
        for(auto arg: arguments)
            if(arg->GetType() != method.ArgumentsType()[index++])
                return false;
        
        return true;
    };
    
    auto iter = std::find_if(methodCollection.begin(), methodCollection.end(), validateArguments);
    
    if(iter == methodCollection.end())
        throw VMError({ "Method found but arguments are incorrect for " + methodName });
    
    return *iter;
};




Reflect::Reflect(JVMLoader loader):
HandleEnv(loader),
invoke(loader)
{
  
    assert(GetEnv() != nullptr);
    
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
    
    auto methodsList = invoke.Call<jobject>(clazz, jmethodArray, nullptr);
    
    auto Fn = [this](JEnv env, jobject& object ) {
        JavaMethod javaMethod(GetLoader());
        
        javaMethod.SetName( GetName(METHOD_CLASS, object) );
       
        javaMethod.SetReturnTypeInfo( GetReturnType(object) );
        javaMethod.ArgumentsType().Set( GetParameters(object));
        
        auto methodId = Wrapper(GetEnv()->functions->FromReflectedMethod, GetEnv(), object );
        javaMethod.SetMethodByReference(methodId);
    
        return javaMethod;
    };
    
    return Utils::IterateJObjectArray< decltype(Fn), JavaMethod >( GetEnv(), (jobjectArray) methodsList.Get(), Fn );
}


std::string Reflect::GetName(std::string className, jobject object) {
    
    auto _tmp = GetMethod(className,
                          METHOD_GET_NAME,
                          ReturnTypeOf(JAVA_STRING_CLASS) );
    
    return invoke.Call<std::string>(object, _tmp, nullptr).Get();
}

std::string Reflect::ToString(std::string className, jobject object){
    
    auto _tmp = GetMethod(METHOD_CLASS,
                          METHOD_TOSTRING,
                          ReturnTypeOf(JAVA_STRING_CLASS) );
    
    return invoke.Call<std::string>(object, _tmp, nullptr).Get();
}

std::string Reflect::GetReturnType(jobject object){
    
    auto GetReflectionAPI = GetMethod(METHOD_CLASS,
                                      METHOD_RETURN_TYPE,
                                      ReturnTypeOf(JAVA_CLASS) );
    
    auto tmp = invoke.Call<jobject>(object, GetReflectionAPI, nullptr);
   
    return GetName(JAVA_CLASS, tmp.Get());
}

std::vector<std::string>
Reflect::GetParameters(jobject object) {
    
    auto GetParametersArray = GetMethod(METHOD_CLASS,
                                        METHOD_GET_PARAMETER,
                                        ReturnArrayOf(JAVA_CLASS) );
    
    auto array = invoke.Call<jobject>(object, GetParametersArray, nullptr);
    
    
    auto Fn = [this](JEnv env, jobject& object ) {
        return GetName(JAVA_CLASS, object);
    };
    
    return Utils::IterateJObjectArray< decltype(Fn), std::string >( GetEnv(), (jobjectArray) array.Get(), Fn );
}

/* Class < Object > <-- GetClass() */
void Reflect::SetClass(jobject object) {
    
    auto getClass = GetMethod(JAVA_CLASS, METHOD_GET_CLASS.c_str(), ReturnTypeOf(JAVA_CLASS) );
    clazz = invoke.Call<jobject>(object, getClass, nullptr).Get();
}

jobject Reflect::GetReflectClass() {
    return clazz;
}


