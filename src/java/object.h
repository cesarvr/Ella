//
//  jvm_reflect.h
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_object_hpp
#define jvm_object_hpp


#include <algorithm>
#include "reflect.h"
#include "jvm_global.h"
#include "values.h"
#include "jinvoke.h"
#include "jvm_handler.h"
#include "args.h"

using namespace LibJNI;
using namespace std;



// Object encapsulate the java object and a list of methods structures.
template <typename Broker>
class Object: HandleEnv {
private:
  
    string name;
    
    ObjectValue object;
    ObjectArray methodArray;
    Broker& service;
    
    void CreateObject(JVMLoader env, string classname, vector<BaseJavaValue *>& arguments);
    
public:
    Object(JVMLoader env, string className);
    Object(JVMLoader env, Broker& broker, string className);
   
    Object(JVMLoader env, string className, vector<BaseJavaValue *>& arguments);
    Object(JVMLoader env, Broker& broker, string className, vector<BaseJavaValue *>&& arguments);
    
    vector<string> MethodsNames() {
        return service.GetMethods(object);
    }
    
    // Get a qualified Java name.
    string GetName(){
        return object.GetType();
    }
    
    Method GetMethodDescriptor(string methodName, vector<BaseJavaValue* >&& args) {
        return service.MethodDescription(object, methodName, move(args) );
    }

    ObjectValue& GetObjectValue() { return object;}
    
    template <typename T>
    T Call(string methodName, vector<BaseJavaValue *>&& arguments) {
        
        T tmp;
        JEnv jni = Env();
        
        auto method = service.MethodDescription(object, methodName, move(arguments));
        
        auto javaValues = Arguments::GetValues(jni, arguments);
        
        if(!tmp.isCompatible( method.returnType  ))
            throw VMError{"Not supported type using " +tmp.GetType()+ " expected " +  method.returnType  };
        
        return Invoke::Call<T>(jni, object.Get(), method.method, (jvalue*)&javaValues[0]);
    }
};

#endif /* jvm_reflect_hpp */
