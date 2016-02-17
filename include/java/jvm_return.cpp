//
//  java_return.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 09/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_return.hpp"

JavaReturn::JavaReturn(std::string returnType): returnType(returnType) {
    //:wstd::cout << "return: " << returnType << std::endl;
};


JavaValue JavaReturn::GetValue(JVMLoader loader, jobject jniObject) {

    try {
        if ( returnType == "class " + JAVA_STRING_CLASS ) {
            return JavaValue( Util::GetStringFromJVM( loader.GetJNIEnviorment(), (jstring) jniObject ) );
        }
        
        if (returnType == "class [B") {
            return JavaValue( Util::GetArrayFromJVM(loader.GetJNIEnviorment(), (jarray) jniObject) );
        }
        
    } catch (VMError& error) {
        loader.GetJNIEnviorment()->ExceptionDescribe();
        throw VMError{"In type deduction: \n" + error.errorMessage, 8};
    }
    
    
    return JavaValue();
}