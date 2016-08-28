//
//  server.cpp
//  LibJNI
//
//

#include "server.h"


template <typename T>
string Args(T args) {
    stringstream signature;
    
    if(args.size()<1)
        return "Void";
    
    for(auto arg: args)
        signature << arg->GetType();
    return signature.str();
}


Method Server::CreateMethod(JEnv& env, string name, jobject object, vector<BaseJavaValue* >& args) {
    Method method;
    
    method.method = Reflect::GetMethodReference(name, env, object);
    method.returnType = Reflect::GetReturnType(env, object);
    
    icache[CreateSignature(name, args)] = method;
    
    return method;
}

Method Server::GetMethodFromCache(JEnv &env, string name, vector<LibJNI::BaseJavaValue *> &args) {
    return icache[CreateSignature(name, args)];
}

vector<string>& Server::GetMethods(ObjectValue object){
    auto jni = Env();
    auto obj = GetMethodsNative(object);
    
    auto& list = names_cache[object.GetType()];
    
    if(list.empty())
        list = Reflect::GetMethodsNames(jni, obj);
    
    return list;
}

ObjectArray Server::GetMethodsNative(ObjectValue object) {
    auto jni = Env();
    auto classname = object.GetType();
    auto obj = mcache[classname];
    
    if(!obj.empty())
        return obj;
    
    auto clazz = GetClass(object);
    obj = Reflect::GetMethodsArray(jni, clazz);
    
    jni->NewGlobalRef(obj.Get());
            
    mcache[classname] = obj;
    return obj;
}

string Server::CreateSignature(string methodName, vector<BaseJavaValue*>& args) {
    stringstream signature;
    signature << methodName <<"@" << Args(args);
    return signature.str();
}

vector<Method> Server::MethodDesc(ObjectValue& object, string methodName){

    auto jni = Env();
    auto methodListPTR = GetMethodsNative(object);
    
    auto lookup = [&methodName](JEnv& env, jobject obj){
    
        Method m;
        auto name = Reflect::GetName(env, obj);
        cout << "looking for " << methodName  << "  == " << name << endl;
        if(methodName == name){
            m.returnType = Reflect::GetReturnType(env, obj);
            m.parameters = Reflect::GetParameters(env, obj);
        }
        
        return m;
    };
    
    return Utils::IterateJObjectArray<decltype(lookup), Method>(jni, (jobjectArray)methodListPTR.Get(), lookup);
}


Method Server::MethodDescription(ObjectValue object, string methodName, vector<BaseJavaValue* >&& args) {
    auto jni = Env();
    auto obj = GetMethodsNative(object);
    
    auto tmp = GetMethodFromCache(jni, methodName, args);

    
    if(tmp.method != nullptr)
        return tmp;
    
    jobject method;

    auto seek = [&method, &methodName, &args](JEnv& env, jobject object ){
        auto name = Reflect::GetName(env, object);
        
        if(name == methodName){
            
            bool valid = Utils::CheckParams<decltype(args)>
                            (args, Reflect::GetParameters(env, object) );
            
            if(valid){
                method = object;
                return true;
            }
        }
        return false;
    };

    auto found = Utils::Find<decltype(seek), jmethodID>(jni, obj.Get(), seek);
    
    if(!found)
        throw VMError{"Error method not found: " + methodName + ", with argument: " + Args(args)};
    
    return CreateMethod(jni, methodName, method, args);
}

ObjectValue Server::GetClass(LibJNI::ObjectValue& object) {

    auto type = object.GetType();
    auto clazz = class_cache[type];

    if(!clazz.isEmpty())
        return clazz;
    
    clazz = Reflect::GetClass(Env(), object);
    class_cache[type] = clazz;
    
    return clazz;
}


Server::~Server(){
    for(auto iterator = mcache.begin(); iterator != mcache.end(); iterator++)
        Env()->DeleteGlobalRef(iterator->second.Get());
}

