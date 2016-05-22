#ifndef jvm_loader_h
#define jvm_loader_h

#include <algorithm>
#include "jvm_global.h"
#include "utils.h"

#ifdef _WIN32
using CreateJVM = jint(__stdcall*) (JavaVM **pvm, void **penv, void *args);
#else
using CreateJVM = jint(*) (JavaVM **pvm, void **penv, void *args);
#endif



  // Here we take care of the JVM.
 
  // From here we take care of initialization and state.



class JVMLoader {
protected:

    // vm: represents the virtual machine functionality.
    //  env: is the environment created by JVM, this variable are the one encapsulated almost all JNI API.

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



// This just try to handle the JEnv in an elegant way.
class HandleEnv {
private:
    JVMLoader java;
    
public:
    HandleEnv(JVMLoader _java): java(_java){  };
    
    const JEnv& Env(){
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
