//
//  jvm_global.h
//  LibJNI
//
//  Created by Cesar Valdez on 03/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_global_h
#define jvm_global_h

#ifdef _WIN32
	#include <memory>
#else
	#include <dirent.h>
	#include <unistd.h>
#endif

#ifdef __APPLE__
    #include <JavaVM/jni.h>
#else
    #include <jni.h>
#endif

#ifdef __linux__
  #include <memory>
  #include <dlfcn.h>
  #include <string>
#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

struct VMError {
    std::string errorMessage;
};


using JEnv  = const std::shared_ptr<JNIEnv>;





#endif /* jvm_global_h */
