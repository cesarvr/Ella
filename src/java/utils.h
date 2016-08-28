//
//  utils.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 17/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp



#include "jvm_global.h"

using namespace std;


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
    
    template <typename T>
    inline void raiseNullError(T& t, std::string message){
        if (t == nullptr) {
            throw VMError{ message };
        }
    }
    
    template <typename Collection>
    inline bool CheckParams(Collection& values,
                            vector<string>&& args ) {
     
        if(values.size() != args.size())
            return false;
        
        int index=0;
        for(auto arg: values)
            if(arg->GetType() != args[index++])
                return false;
        
        return true;
    }

    
    inline std::string normalizeClassName(std::string&& classname) {
        std::replace(classname.begin(), classname.end(), '.', '/');
        return std::move(classname);
    }
    
    template <typename T, typename R>
    std::vector<R> IterateJObjectArray( JEnv env, jobjectArray array, T cb ) {
        
        isNull(array);
        
        std::vector<R> list;
        
        jint count = env->GetArrayLength( array );
        list.reserve(count);
        
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

    // Iterate a objectArray and apply a function, the loop breaks when T return true.
    // useful for quick linear search.
    template <typename T, typename R>
    bool Find( JEnv env, jobjectArray array, T cb ) {
        
        isNull(array);
        
        bool ret = false;
        jint count = env->GetArrayLength( array );
        
        for (int i=0; i < count; i++) {
            
            jobject element = env->GetObjectArrayElement(array, i);
            
            if((ret = cb( env, element )))
                break;
            
            if(env->ExceptionOccurred())
                env->ExceptionDescribe();
            
            env->DeleteLocalRef( element );
        }
        
        return ret;
    };

    
}










#endif /* utils_hpp */
