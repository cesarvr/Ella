//
//  queue_manager.hpp
//  nanProject
//
//  Created by Cesar Valdez on 06/03/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef queue_manager_hpp
#define queue_manager_hpp

#include <nan.h>
#include "jvm_global.hpp"
#include "jvm_handler.h"
/*
 template <typename FN>
 class Wrap: HandleEnv {
 
 public:
 
 FN fn;
 
 Wrap(JEnv env, FN _fn):HandleEnv(env), fn(_fn){};
 
 template <typename... Args>
 auto call( Args... args) -> decltype(fn(args...)) {
 
 try{
 //   isNull(args...);
 Utils::isNull(args...);
 return fn(args...);
 
 }catch(VMError& error){
 GetEnv()->ExceptionDescribe();
 throw error;
 };
 }
 };

 
 */



template <typename Function, typename DoneCallBack>
class AsyncWrap : public Nan::AsyncWorker
{
    //using X = decltype(Function);
    
    bool result;
    Function procedure;
    DoneCallBack doneCallBack;

    
    
public:
    
    AsyncWrap(Nan::Callback *callBack, Function _fn, DoneCallBack _donecb):
    Nan::AsyncWorker(callBack),
    procedure(_fn),
    doneCallBack(_donecb)
    {
    }
    
    void Execute(){
        try {
            std::cout << "executing" << std::endl;
            
            
             result = procedure();
     
            
        } catch (VMError& error ) {
            throw error;
        }
    }
    
    void HandleOKCallback () {
        
        doneCallBack(callback, result);
    }
};



#endif /* queue_manager_hpp */
