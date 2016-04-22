//
//  utils.h
//  Prueba
//
//  Created by Cesar Valdez on 02/03/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include "jvm_global.hpp"

namespace Utils {
    

    
    template<typename T>
    void chkNull(T&& param){
        assert(param != nullptr);
    }
    
    template<typename T>
    void isNull(T& first) {
        chkNull(first);
    }
    
    template<typename T, typename... Args>
    void isNull(T& first, Args... args) {
        chkNull(first);
        
        isNull(args...);
    }
    
    template <typename T, typename R>
    std::vector<R> IterateJObjectArray( JEnv env, jobjectArray array, T cb ) {
        
        isNull(array);
        
        std::vector<R> list;
        
        
        jint count = env->GetArrayLength( array );
        
        for (int i=0; i < count; i++) {
            
            jobject element = env->GetObjectArrayElement(array, i);
            
            R ret = cb( env, element );
            
            list.push_back(ret);
            
            if(env->ExceptionOccurred())
                env->ExceptionDescribe();
            
            env->DeleteLocalRef( element );
        }
        return list;
    };
    
    
    template <typename Data>
    std::vector <Data> GetArrayFromJVM(std::shared_ptr<JNIEnv> env, jarray arrayObject ) {
        
        isNull(arrayObject);
        
        std::vector <Data> data;
        jint count = env->GetArrayLength( (jbyteArray) arrayObject );
        
        data.resize(count);
        
        std::unique_ptr<Data[]> buffer (new Data[count]);
        
        env->GetByteArrayRegion ( (jbyteArray) arrayObject , 0, count, (jbyte*)&data[0] );
        
        return data;
    }
    
    
}
#endif /* utils.h */
