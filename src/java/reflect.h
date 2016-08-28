//
//  jvm_reflect.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 21/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_reflect_hpp
#define jvm_reflect_hpp

#include <algorithm>
#include "utils.h"
#include "jvm_handler.h"
#include "jinvoke.h"
#include "args.h"

using namespace std;




// Reflect class contains the reflection mechanism, that allow the API to
// instrospect the require Java object
// garther information about the methods the user wants to execute.

const string JAVA_CLASS = "java/lang/Class";
const string METHOD_CLASS = "java/lang/reflect/Method";
const string METHOD_CONSTRUCTOR = "java/lang/reflect/Constructor";

const string METHOD_GET_CLASS = "getClass";
const string METHOD_GET_METHODS = "getMethods";
const string METHOD_RETURN_TYPE = "getReturnType";
const string METHOD_TOSTRING = "toString";
const string METHOD_GET_NAME = "getName";
const string METHOD_GET_PARAMETER = "getParameterTypes";


class Reflect {
    
public:
    
    static jmethodID GetMethod(JEnv& env, string className, string method, string returnType);
    static ObjectArray GetMethodsArray(JEnv& env, ObjectValue clazz);
    static ObjectValue GetClass(JEnv& env, ObjectValue& object);
    static std::vector<string> GetMethodsNames(JEnv& env, ObjectArray methods);
    static jmethodID GetMethodReference(string alias, JEnv& env, jobject object);
    
    
    // reflects methods.
    static string GetReturnType(JEnv& env, jobject object);
    static string GetName(JEnv& env, jobject object);
    static string GetName(JEnv& env, string methodName,  jobject object);
    static string ToString(JEnv& env, string className, jobject object);
    
    static std::vector<string> GetParameters(JEnv& env, jobject object);
};


#endif /* jvm_reflect_hpp */
