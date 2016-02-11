//
//  jvm_global.h
//  LibJNI
//
//  Created by Cesar Valdez on 03/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_global_h
#define jvm_global_h


#include <iostream>
#ifdef __APPLE__
    #include <JavaVM/jni.h>
#else
    #include <jni.h>
#endif


#include <sstream>
#include <vector>

const std::string METHOD_CLASS("java/lang/reflect/Method");
const std::string JAVA_CLASS("java/lang/Class");
const std::string JAVA_STRING_CLASS("java.lang.String");


const std::string STRING_RETURN_TYPE("()Ljava/lang/String;");
const std::string CLASS_MOD_RETURN_TYPE("()Ljava/lang/Class;");
const std::string CLASS_ARRAY("()[Ljava/lang/Class;");

const std::string CLASS_DEFAULT_CTS("<init>");
const std::string VOID_RETURN("()V");

struct VMError {
    std::string errorMessage;
    int errorCode;
};

struct JavaObject{
    jclass member = nullptr;
    jmethodID constructor = nullptr;
    jobject object = nullptr;
    jobjectArray methods = nullptr;
};



#endif /* jvm_global_h */
