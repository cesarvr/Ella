//
//  args.h
//  LibJNI
//
//  Created by Cesar Valdez on 20/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef args_hpp
#define args_hpp

#include "values.h"


 
// This defines list the arguments/types and the size.
 


class ArgumentTypeInfo {
public:
    void Set(std::vector<std::string> arguments);
    std::string operator[](int index) { return listTypes[index]; };
    unsigned long GetNumberOfArguments(){ return parametersNumber; };
    
private:
    std::vector<std::string> listTypes;
    unsigned long parametersNumber = 0;
};



// Take care of the transformation between (C++)Data to (JNI) jvalue
class Arguments {
private:
    std::vector<LibJNI::BaseJavaValue *> arguments;
    
public:
    
    static std::vector<jvalue>
    GetValues( JEnv jenv ,
           std::vector<LibJNI::BaseJavaValue *> arguments );
    
    static std::string GetConstructorSignature( JEnv jenv ,
                            std::vector<LibJNI::BaseJavaValue *> arguments );
};




 
//    Method structure.

class JavaMethod: HandleEnv {
private:
    std::string name;
    std::string returnTypeInfo;
    jmethodID javaMethodRef;
    ArgumentTypeInfo argsType;
    
public:
    
    void SetName(std::string _name){ name = _name; };
    std::string GetName(){ return name; };
    
    void SetReturnTypeInfo(std::string _returnTypeInfo){ returnTypeInfo = _returnTypeInfo; };
    std::string GetReturnTypeInfo(){ return returnTypeInfo; };
    
    void SetMethodByReference(jmethodID& methodId){ javaMethodRef = methodId; };
    jmethodID GetMethodRef() { return javaMethodRef; };
    
    ArgumentTypeInfo& ArgumentsType(){ return argsType; };
    
    JavaMethod(JVMLoader loader): HandleEnv(loader) {};
};

#endif /* args_hpp */
