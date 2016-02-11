//
//  jvm_argument.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_argument.hpp"


JavaArguments::JavaArguments(std::vector<std::string> arguments) {
    listOfArguments = arguments;
};

void JavaArguments::CheckInfo(){
    
    std::cout << "checking args: " << std::endl;
    for(std::string arg : listOfArguments) {
        std::cout << "->" << arg << std::endl;
    }
}


std::unique_ptr<jvalue[]>
JavaArguments::GetArguments(JVMLoader loader, std::vector<JavaValue>& values) const{
    
    if (values.size() < listOfArguments.size()) {
        std::stringstream msg;
        msg <<"Insuficient arguments expecting: " << listOfArguments.size() << " arguments, found: " << values.size();
        msg << "\n type [ ";
        for(const std::string& arg : listOfArguments)
            msg << arg << " ";
        
        msg << "] \n";
        
        throw VMError{ msg.str(), 5 };
    }
    
    int argsIndex = 0;
    

    std::unique_ptr<jvalue[]> jniArguments( new jvalue[ listOfArguments.size() ] );
    
    for(const std::string& arg : listOfArguments) {
        if (arg == JAVA_STRING_CLASS) {
            jniArguments[ argsIndex ].l = loader.GetJNIEnviorment()->NewStringUTF( values[ argsIndex ].getStringValue().c_str() );
            argsIndex++;
        }
    }
    
    return jniArguments;
};




