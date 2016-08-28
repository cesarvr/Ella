//
//  server.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 21/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef server_hpp
#define server_hpp


#include <utility>
#include <map>
#include "reflect.h"

using namespace std;

// Server class, is the Broker class in charge of calling provide access and caching for Reflect class.

class Server {
public:
    Server(){};
    void SetJVM(JVMLoader loader){ jvm = loader; };

    //Get list of methods as strings is cheaper to get it this way than the native description of all methods.
    vector<string>& GetMethods(ObjectValue object);
    
    //Lazy-load and cache the JNI method.
    ObjectArray GetMethodsNative(ObjectValue object);

    //TODO
    vector<Method> MethodDesc( ObjectValue& object, string methodName );
    
    //Return the Method signature param and return type.
    Method MethodDescription( ObjectValue object, string methodName, vector<BaseJavaValue* >&& args );
    
    //Get native java.lang.Class instance from JNI.
    ObjectValue GetClass( LibJNI::ObjectValue& object );
    
    ~Server();
    
private:
    Method CreateMethod(JEnv& env, string name, jobject object, vector<BaseJavaValue* >& args);
    Method GetMethodFromCache(JEnv &env, string name, vector<LibJNI::BaseJavaValue *> &args);
    
    //Create an alias for the JNI method son can be use
    string CreateSignature( string methodName, vector<BaseJavaValue*>& args);
    
    const std::shared_ptr<JNIEnv>& Env(){return jvm.GetJNIEnviorment(); }
    
    map<string, ObjectArray> mcache;
    map<string, Method> icache;
    map<string, vector<string>> names_cache;
    map<string, ObjectValue> class_cache;
    
    JVMLoader jvm;
};


#endif /* server_hpp */
