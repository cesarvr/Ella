//
//  jvm_reflect.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "object.h"


// Represent an object in java.
template <typename Broker>
Object<Broker>::Object(JVMLoader loader, std::string className)
: HandleEnv(loader), object(className), service(loader) {
    
    std::vector<BaseJavaValue*> _empty;
    
    CreateObject(loader, className, _empty);
};

template <typename Broker>
Object<Broker>::Object(JVMLoader loader, string className, vector<BaseJavaValue *>& arguments):
HandleEnv(loader),
object(className),
service(loader) {
    
    CreateObject(loader, className, arguments);
};

template <typename Broker>
Object<Broker>::Object(JVMLoader loader,  Broker& broker, std::string className)
: HandleEnv(loader), object(className), service(broker){
    std::vector<BaseJavaValue*> _empty;
    _empty.reserve(0);
    CreateObject(loader, className, _empty);
}

template <typename Broker>
Object<Broker>::Object(JVMLoader env, Broker& broker, string className, vector<BaseJavaValue *>&& arguments)
: HandleEnv(env), object(className), service(broker) {
    
    CreateObject(env, className, arguments);
}


template <typename Broker>
void Object<Broker>::CreateObject(JVMLoader env, std::string className, std::vector<BaseJavaValue *>& arguments) {
    
    jmethodID constructor;
    
    auto name = object.GetType();
    auto jni = Env();
    auto& findclazz = jni->functions->GetMethodID;
    
    auto member = Wrapper(jni->functions->FindClass, jni, name.c_str());
    
    if(arguments.empty())
        constructor = Wrapper(findclazz, jni, member,
                              CLASS_DEFAULT_CTS.c_str(), VOID_RETURN.c_str());
    else
        constructor = Wrapper(findclazz, jni, member,
                              CLASS_DEFAULT_CTS.c_str(), Arguments::GetConstructorSignature(jni,arguments).c_str());
    
    
    auto javaValues = Arguments::GetValues(jni, arguments);
    auto tmp = Wrapper( jni->functions->NewObjectA, jni, member, constructor, (jvalue*)&javaValues[0]);
    
    object.Set(tmp);
    methodArray = service.GetMethodsNative(object);
}
