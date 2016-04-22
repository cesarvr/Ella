#ifndef jvm_loader_h
#define jvm_loader_h


#include "jvm_global.hpp"
#include "utils.h"

#ifdef _WIN32
using CreateJVM = jint(__stdcall*) (JavaVM **pvm, void **penv, void *args);
#else
using CreateJVM = jint(*) (JavaVM **pvm, void **penv, void *args);
#endif

class JVMLoader {
protected:
    std::shared_ptr<JavaVM> vm = nullptr;
    std::shared_ptr<JNIEnv> env= nullptr;
    std::string classPath;
    bool jvmStatus = false;
    CreateJVM create_vm;
    
public:
    JVMLoader();
    
    const std::shared_ptr<JNIEnv>& GetJNIEnviorment();
    void ReleaseThread();
    bool isVMReady();
    void SetClassPath(std::string _classPath);
    std::string GetClassPath();
    std::string VMStatus(int status);
    std::string Start();
};




class HandleEnv {
private:
    JVMLoader java;
    
public:
    HandleEnv(JVMLoader _java): java(_java){  };
    
    const JEnv& GetEnv(){
        Utils::isNull(java.GetJNIEnviorment());
        return java.GetJNIEnviorment();
    };
    
    const JVMLoader& GetLoader(){
        return java;
    };

    void Release() {
        java.ReleaseThread();
    };
};

#endif /* JVM_Loader */