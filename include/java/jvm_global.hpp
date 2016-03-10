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

#ifdef _WIN32
	#include <memory>
#endif

#ifdef __APPLE__
    #include <JavaVM/jni.h>
#else
    #include <jni.h>
#endif

#include <sstream>
#include <vector>


const std::string JAVA_METHOD_CLASS("java/lang/reflect/Method");
const std::string JAVA_STRING_CLASS("java/lang/String");

const std::string CLASS_DEFAULT_CTS("<init>");
const std::string VOID_RETURN("()V");

struct VMError {
    std::string errorMessage;
};


using JEnv  = const std::shared_ptr<JNIEnv>;





#endif /* jvm_global_h */
