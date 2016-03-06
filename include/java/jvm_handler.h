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
       
        int status = vm->GetEnv((void**)&env, JNI_VERSION_1_6);
       
        if (status == JNI_EDETACHED) {
            
            std::cout << "GetEnv: not attached" << std::endl;
            if (vm->AttachCurrentThread((void **) &env, NULL) != 0) {
                std::cout << "Failed to attach" << std::endl;
            }
            
        } else if (status == JNI_OK) {
            return env;
        } else if (status == JNI_EVERSION) {
            throw VMError{"GetEnv: version not supported"};
        }
        
        
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
