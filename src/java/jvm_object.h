//
//  jvm_reflect.h
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_reflect_hpp
#define jvm_reflect_hpp


#include <algorithm>
#include "utils.h"
#include "values.h"
#include "jvm_handler.h"
#include "jinvoke.h"
#include "args.h"

using namespace LibJNI;
using ObjectInterface = jobject(*)(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args);
using IntegerInterface = jint(*)(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args);


const std::string JAVA_METHOD_CLASS("java/lang/reflect/Method");
const std::string JAVA_STRING_CLASS("java/lang/String");
const std::string CLASS_DEFAULT_CTS("<init>");
const std::string VOID_RETURN("()V");



// Reflectio class, this encapsulate all cool Java reflection functionality.
class Reflect: HandleEnv {
private:
    
    const std::string JAVA_CLASS = "java/lang/Class";
    const std::string METHOD_CLASS = "java/lang/reflect/Method";
    const std::string METHOD_CONSTRUCTOR = "java/lang/reflect/Constructor";
    
    const std::string METHOD_GET_CLASS = "getClass";
    const std::string METHOD_GET_METHODS = "getMethods";
    const std::string METHOD_RETURN_TYPE = "getReturnType";
    const std::string METHOD_TOSTRING = "toString";
    const std::string METHOD_GET_NAME = "getName";
    const std::string METHOD_GET_PARAMETER = "getParameterTypes";
    
    jobject clazz;
    Invoke invoke;
    
public:
    
    void SetClass(jobject object);
    jobject GetReflectClass();
    
    Reflect(JVMLoader env);
    
    jmethodID GetMethod( std::string className, std::string method, std::string returnType );
    
    std::vector<JavaMethod> GetMethodsDefinition();
    
    // reflects methods.
    std::string GetReturnType(jobject object);
    std::string GetName(std::string className, jobject object);
    std::string ToString(std::string className, jobject object);
    
    std::vector<std::string> GetParameters(jobject object);
};



// Object encapsulate the java object and a list of methods structures. 
class Object: HandleEnv {
private:
  
    std::string name;
    std::vector<JavaMethod> methods;
    
    Reflect reflect;
    Invoke invoke;
    
    ObjectValue object;
    
    void CreateObject(JVMLoader env, std::string classname, std::vector<BaseJavaValue *>& arguments);
    
public:
    Object(JVMLoader env, std::string className);
    Object(JVMLoader env, std::string className, std::vector<BaseJavaValue *>& arguments);
    
    JavaMethod FindFirstMethod( std::string methodName );
    std::vector<JavaMethod> FindMethod( std::string methodName );
    
    JavaMethod LookupMethod(std::string methodName, std::vector<BaseJavaValue *>& arguments );

    std::string GetName(){ return object.GetType(); }
    
    const std::vector<JavaMethod>& GetMembers();
    
    std::string GetClassName();
    
    void ReleaseThread(){ Release(); }
    
    
    
    template <typename T>
    T Call(std::string methodName) {
        std::vector<BaseJavaValue *> empty;
        return Call<T>(methodName, empty);
    }
    
    template <typename T>
    T Call(std::string methodName, std::vector<BaseJavaValue *>& arguments) {
        
        T tmp;
        
        auto method = LookupMethod(methodName, arguments);
        
        auto javaValues = Arguments::GetValues(GetEnv(), arguments);
        
        if(!tmp.isCompatible( method.GetReturnTypeInfo() ))
            throw VMError{"Not supported type using " +tmp.GetType()+ " expected " +  method.GetReturnTypeInfo()  };
        
        return invoke.Call<T>(object.Get(), method.GetMethodRef(), (jvalue*)&javaValues[0]);
    }
    
  
};

#endif /* jvm_reflect_hpp */
