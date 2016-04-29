#include "jvm_handler.h"

JVMLoader::JVMLoader() {
    
    SetClassPath("-Djava.class.path=*:."); //use current directory as classpath.
}

void JVMLoader::SetClassPath(std::string _classPath) {
    
    classPath = "-Djava.class.path=.:" +  _classPath;
}

bool JVMLoader::isVMReady() {
    
    return jvmStatus;
}

std::string JVMLoader::VMStatus(int status) {
    if(status != JNI_ERR) {
        jvmStatus = true;
        return "[jvm]: started successfully.";
    } else {
        jvmStatus = false;
        return "[jvm]: error starting virtual machine.";
    }
}

void JVMLoader::ReleaseThread(){
    vm->DetachCurrentThread();
};

std::string JVMLoader::GetClassPath() {
    return classPath;
};


const std::shared_ptr<JNIEnv>& JVMLoader::GetJNIEnviorment() {
    
    if(env == nullptr || env == 0x0) throw VMError{"JVM: has not been initialize."};
    
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


/* use the one loaded */
std::string JVMLoader::Start() {

#ifdef __linux__
    
    void* handle = dlopen("./linux/libjvm.so", RTLD_LAZY);
    
    if (!handle) throw VMError{ "Fatal: Can't load dynamic library libjvm.so" }; 
    
    create_vm = (CreateJVM) dlsym(handle, "JNI_CreateJavaVM");
    
    const char *dlsym_error = dlerror();
    
    if (dlsym_error) {
        std::string msg = "Cannot find symbol: JNI_CreateJavaVM || msg: ";
        msg = msg + dlsym_error;
        
        dlclose(handle);
        throw VMError{"Fatal: "+ msg };
    }
    
#else
	create_vm = JNI_CreateJavaVM;
#endif

    
    JavaVMOption options[1];
    JavaVMInitArgs vm_args;
    
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    
    options[0].optionString = (char *)classPath.c_str();
    vm_args.options = options;
    
    int status = create_vm((JavaVM**)&vm, (void**)&env, &vm_args);
    
    return VMStatus(status);
}

