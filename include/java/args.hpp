//
//  args.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 20/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef args_hpp
#define args_hpp

#include "values.hpp"

class ArgumentTypeInfo {
public:
    void Set(std::vector<std::string> arguments);
    std::string operator[](int index) { return listTypes[index]; };
    unsigned long GetNumberOfArguments(){ return parametersNumber; };
    
private:
    std::vector<std::string> listTypes;
    unsigned long parametersNumber = 0;
};


class Arguments {
private:
    std::vector<LibJNI::BaseJavaValue *> arguments;
    
public:
    
    static std::vector<jvalue>
    Create( JEnv jenv ,
           std::vector<LibJNI::BaseJavaValue *> arguments );
};


class JavaMethod: HandleEnv {
private:
    std::string name;
    std::string returnTypeInfo;
    jmethodID javaMethodRef;
    ArgumentTypeInfo argsType;
    
public:
    
    /* Accessors */
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
