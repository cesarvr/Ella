#include "jvm_handler.h"

JVMLoader::JVMLoader() {
    
    SetClassPath("-Djava.class.path=*:."); //use current directory as classpath.
};

void JVMLoader::SetClassPath(std::string _classPath) {
    
    classPath = _classPath;
};

bool JVMLoader::isVMReady() {
    
    return isJvmStarted;
}

std::string JVMLoader::VMStatus(int status) {
    if(status != JNI_ERR) {
        isJvmStarted = true;
        return "[jvm]: started successfully.";
    } else {
        isJvmStarted = false;
        return "[jvm]: error starting virtual machine.";
    }
};


/* use the one loaded */
std::string JVMLoader::Start() {

#ifdef __linux__
    
    void* handle = dlopen("./libjvm.so", RTLD_LAZY);
    
    if (!handle) {
        info.GetReturnValue().Set( Nan::New(" [jvm]: fatal! can't load shared library. libjvm.so").ToLocalChecked() );
        return;
    }
    
    create_vm = (CreateJVM) dlsym(handle, "JNI_CreateJavaVM");
    
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        
        std::string msg = "Cannot find symbol: JNI_CreateJavaVM || msg: ";
        msg = msg + dlsym_error;
        info.GetReturnValue().Set( Nan::New(msg).ToLocalChecked() );
        
        dlclose(handle);
        return;
    }
    
#elif __APPLE__
    create_vm = JNI_CreateJavaVM;
#endif
    
    JavaVMOption options[1];
    JavaVMInitArgs vm_args;
    
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    
    options[0].optionString = (char *)classPath.c_str();
    vm_args.options = options;
    
    int status = create_vm(&vm, (void**)&env, &vm_args);
    
    return VMStatus(status);
};

