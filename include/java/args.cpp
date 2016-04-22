//
//  args.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 20/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "args.hpp"

void ArgumentTypeInfo::Set(std::vector<std::string> argsTypeList) {
    listTypes = argsTypeList;
    parametersNumber = listTypes.size();
}


std::vector<jvalue>
Arguments::Create( JEnv jenv ,
                  std::vector<LibJNI::BaseJavaValue *> arguments ) {
    
    std::vector<jvalue> values;
    
    for(auto arg: arguments )
        values.push_back( arg->GetJavaValue( jenv ) );
    
    return values;
}