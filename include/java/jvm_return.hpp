//
//  java_return.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 09/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef java_return_hpp
#define java_return_hpp

#include "jvm_handler.h"
#include "jvm_util.hpp"
#include "java_value.hpp"

class JavaReturn {
    
    std::string returnType;
  
public:
    JavaReturn(){};
    JavaReturn(std::string returnType);
   
    
    JavaValue GetValue(JVMLoader loader, jobject jniObject);
};

#endif /* java_return_hpp */
