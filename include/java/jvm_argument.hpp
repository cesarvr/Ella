//
//  jvm_argument.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_argument_hpp
#define jvm_argument_hpp

#include "jvm_handler.h"
#include "jvm_global.hpp"
#include "java_value.hpp"

class JavaArguments {
    std::vector<std::string> listOfArguments;
    
public:
    JavaArguments(std::vector<std::string> arguments);
    JavaArguments(){};
    
    bool IsVoid() { return listOfArguments.size() == 0; };
    
    std::unique_ptr<jvalue[]>
    GetArguments(JVMLoader loader,  std::vector<JavaValue>& values ) const;
    
    void CheckInfo();
    //jvalue *GetArguments();
};


#endif /* jvm_argument_hpp */
