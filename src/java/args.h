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



#endif /* args_hpp */
