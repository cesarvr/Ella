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
    
    template<typename... T>
    void isNull() {}
    
    template<typename T>
    void isNull(T v) {
        if (v == nullptr || v == 0x0) {
            throw VMError{"Error: arguments can't be null." };
        }
    }
    
    template<typename T, typename R, typename... Args>
    void isNull(T v1, R v2, Args... args ) {
        if (v1 == nullptr ||  v2 == nullptr || v1 == 0x0 || v2 == 0x0) {
            throw VMError{"Error: arguments can't be null. v1 or v2 (line 30 utils.h)" };
        }
        
        Utils::isNull(args...);
    }
    
    


    
    
    template <typename T, typename R>
    std::vector<R> IterateJObjectArray( JEnv env, jobjectArray array, T cb ) {
        
        isNull(array);
        
        std::vector<R> list;
        jint i;
        
        
        jint count = env->GetArrayLength( array );
        
        for (i=0; i < count; i++) {
            
            jobject element = env->GetObjectArrayElement(array, i);
            
            R ret = cb( env, element );
            
            list.push_back(ret);
            
            if(env->ExceptionOccurred()) {
                env->ExceptionDescribe();
                break;
            }
            
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
