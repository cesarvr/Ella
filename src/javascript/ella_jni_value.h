//
//  ella_jni_value.h
//  nanProject
//
//  Created by Cesar Valdez on 10/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef ella_jni_value_h
#define ella_jni_value_h

#include <algorithm>
#include <vector>
#include "values.h"
#include <math.h>

// Class to graceful degrade to float - double and vice-versa.
struct NumbersValue : public Value<jdouble, double> {
    
    NumbersValue(): Value("double") {}
    NumbersValue(double x): Value("double", x) {
       if(x == floor(x))
           type = "int";
       else
           type = "double";
    }
    
    
    jvalue GetJavaValue(JEnv& env) {
        if(type == "double")
            jniValue.d = value;
        
        if(type == "int")
            jniValue.i = value;
        return jniValue;
    }
};



#endif /* ella_jni_value_h */
