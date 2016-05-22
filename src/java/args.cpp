//
//  args.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 20/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "args.h"


std::vector<jvalue>
Arguments::GetValues( JEnv jenv ,
                  std::vector<LibJNI::BaseJavaValue *> arguments ) {
    
    std::vector<jvalue> values;
    
    for(auto arg: arguments )
        values.push_back( arg->GetJavaValue( jenv ) );
    
    return values;
}

std::string
Arguments::GetConstructorSignature( JEnv jenv ,
                        std::vector<LibJNI::BaseJavaValue *> arguments ) {
    std::stringstream signature;
    signature << "(";
    
    for(auto args: arguments){
        signature << Utils::normalizeClassName(args->GetJVMType());
    }
    signature << ")V";
    return signature.str();
}