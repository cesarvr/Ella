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
    std::shared_ptr<JavaVM> vm = nullptr;
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
            if (vm->AttachCurrentThread((void **) &env, NULL) != 0) {
                std::cout << "GetJNIEnviorment-> Failed to attach" << std::endl;
            }
            
        } else if (status == JNI_OK)
            return env;
         else if (status == JNI_EVERSION)
            throw VMError{"GetEnv: version not supported"};
        
        return env;
    };
    
    void ReleaseThread(){
       
        if(env == nullptr || env == 0x0)  return;
        
        int status = vm->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (status == JNI_OK) {
            vm->DetachCurrentThread();
        }
    };
    
    bool isVMReady();
    void SetClassPath(std::string _classPath);
    std::string GetClassPath() { return classPath; };
    
    std::string VMStatus(int status);
    std::string Start();
    
    JVMLoader& operator=( JVMLoader &loader )
    {
        env = loader.GetJNIEnviorment();
        classPath = loader.GetClassPath();
        return *this;  // Assignment operator returns left side.
    }
};




class HandleEnv {
private:
    JVMLoader java;
    
public:
    HandleEnv(JVMLoader _java): java(_java){  };
    
    const JEnv& GetEnv(){
        return java.GetJNIEnviorment();
    };
    
    const JVMLoader& GetLoader(){
        return java;
    };

    void Release() {
        std::cout << "Dettaching JVM from Current Thread   0x00008888" << std::endl;
        java.ReleaseThread();
    };

};



#endif /* JVM_Loader */
