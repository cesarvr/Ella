#ifndef jvm_loader_h
#define jvm_loader_h


#include "jvm_global.hpp"

#ifdef _WIN32
using CreateJVM = jint(__stdcall*) (JavaVM **pvm, void **penv, void *args);
#else
using CreateJVM = jint(*) (JavaVM **pvm, void **penv, void *args);
#endif

class JVMLoader {
protected:
    JavaVM *vm = nullptr;
    std::shared_ptr<JNIEnv> env= nullptr;
    std::string classPath;
    bool isJvmStarted = false;
    CreateJVM create_vm;
    
public:
    JVMLoader();
    const std::shared_ptr<JNIEnv>& GetJNIEnviorment() {
        if(env == nullptr || env == 0x0) throw VMError{"JVM: has not been initialize. "};
       // vm->GetEnv((void**)&env, JNI_VERSION_1_6);
       // std::cout << "getting an JEnv for this thread" << std::endl;
        
       // if(env == nullptr || env == 0x0) throw VMError{"JVM: has not been initialize. "};
        return env;
    };
    
    
    
    
    bool isVMReady();
    void SetClassPath(std::string _classPath);
    std::string GetPath() { return classPath; };
    
    std::string VMStatus(int status);
    std::string Start();
    
    JVMLoader& operator=( JVMLoader &loader )
    {
        env = loader.GetJNIEnviorment();
        classPath = loader.GetPath();
        return *this;  // Assignment operator returns left side.
    }
};

#endif /* JVM_Loader */
