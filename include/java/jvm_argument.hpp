 //
//  jvm_argument.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_argument_hpp
#define jvm_argument_hpp

#include "jvm_global.hpp"
#include "java_value.hpp"



const std::string JSTRING = "java.lang.String";
const std::string JBYTE = "byte";
const std::string JINT = "int";
const std::string JBYTE_ARRAY = "[B";


class JavaArguments {
    
private:
    std::vector<std::string> listOfArguments;

    
public:
    JavaArguments(std::vector<std::string> arguments);
    JavaArguments(){};
    
    bool IsVoid() { return listOfArguments.size() == 0; };
    
    std::unique_ptr<jvalue[]> GetArguments( JEnv env, std::vector<JavaValue>& values );
    
    void CheckInfo();
    //jvalue *GetArguments();
};





#endif /* jvm_argument_hpp */
