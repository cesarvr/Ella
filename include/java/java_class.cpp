//
//  jvm_object.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 04/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "java_class.hpp"


JavaClass::JavaClass(std::string _className,
                     jobject _classObject,
                     JVMLoader _loader ):   name(_className),
classObject(_classObject),
loader(_loader)
{};

JavaClass::JavaClass(){};

JavaClass JavaClass::CreateObject(JVMLoader loader, std::string className) {
    
    try {
        
        auto classObject = Field::ObjectCreate(loader, className);
        
        
        /* < Class * > --> GetClass() */
        jobject ClassObj = Field::GetterUsingString<jobject>(loader,
                                                             JAVA_CLASS,
                                                             "getClass",
                                                             Util::ReturnTypeOf( JAVA_CLASS ) ,
                                                             classObject);
        
        /* Methods[]  <- Class.getMethods  */
        auto methods = Field::GetterUsingString<jobjectArray>(loader,
                                                              JAVA_CLASS,
                                                              "getMethods",
                                                              Util::ReturnArrayOf( METHOD_CLASS ),
                                                              ClassObj );
        JavaClass clazz(className, classObject, loader);
        clazz.SetMethods(methods);
        
        return clazz;
    }catch( VMError& error ) {
        JavaClass empty;
        return empty;
    }
};


void JavaClass::SetMethods(jobjectArray methods) {
    
    if (methods == nullptr)
        throw VMError{ "Unable to access any method" ,  3 };
    
    
    using Interface = JavaMethod (*)( JVMLoader loader, jobject& reflectObject );
    
    auto Fn = [](JVMLoader loader, jobject& object ) {
        JavaMethod m;
        
        m.name = Field::GetName(loader, object);
        std::cout << "->" << m.name << std::endl;
        m.returnType = JavaReturn( Field::GetReturnType(loader, object) );
        m.arguments =  JavaArguments( Field::GetParameters(loader, object) );
        m.methodPTR = loader.GetJNIEnviorment()->FromReflectedMethod(object);
        
        m.arguments.CheckInfo();
        
        return m;
    };
    
    Methods = Util::IterateJObjectArray< Interface, JavaMethod >(loader, methods, Fn );
};

JavaMethod JavaClass::LookupMethod(std::string name) {
    
    for (const JavaMethod& method: Methods) {
        if (method.name == name) {
            return method;
        }
    }
    
    throw VMError{"Method not found.", 0};
};






