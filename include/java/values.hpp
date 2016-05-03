//
//  values.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 06/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef values_hpp
#define values_hpp

#include "jvm_global.hpp"
#include "jvm_handler.h"
#include "utils.h"

class JavaMethod;
class ArgumentTypeInfo;
class Arguments;

namespace LibJNI {
    
    /*
     * Value : BaseValue
     *
     * Class in charge of handling the differents type between JAVA and C++, to implement a new type just need
     *  to make a template-specialization of the type you want to handle.
     *
     */
    class BaseJavaValue {
    public:
        // Override with the value expected by JNI for java Argument.
        virtual std::string GetType() { throw VMError{"Type not implemented yet."}; return ""; };
        
        //Override with the handling behavior for Native to JNI type.
        virtual jvalue GetJavaValue(JEnv& env) { throw VMError{"GetJavaValue not implemented yet for this type."}; };
    };

    
    template <class JNIType, class NativeType>
    class Value: public BaseJavaValue {
    public:
        Value(std::string _type): type(_type) {};
        
        Value(std::string _type, NativeType _value):
        value(_value),
        type(_type){}
        
        typedef JNIType Type;
        
        std::string GetType() { return type; };
        
        void Set(JEnv& env, JNIType object) {
            value = (NativeType) object;
        }
        
        void Set(NativeType _value){
            value = _value;
        }
        
        NativeType Get(){ return value; }
        
        
    protected:
        NativeType value;
        std::string type;
        jvalue jniValue;
    };
    
    
    
    struct JObject : public Value<jobject, jobject> {
        JObject(): Value("object") {}
    };
    
    struct ObjectArray : public Value<jobject, jobjectArray> {
        ObjectArray(): Value("object") {}
    };
    
    struct IntValue : public Value<jint, int> {

        IntValue(): Value("int") {}
        IntValue(int x): Value("int", x) {}
        
        jvalue GetJavaValue(JEnv& env) {
            jniValue.i = value;
            return jniValue;
        }
    };
    
    struct FloatValue : public Value<jfloat, float> {

        FloatValue(): Value("float") {}
        FloatValue(float x): Value("float", x) {}
        
        jvalue GetJavaValue(JEnv& env) {
            jniValue.f = value;
            return jniValue;
        }
    };
    
    
    
    struct StringValue : public Value<jstring, std::string> {

        StringValue(): Value("java.lang.String") {}
        StringValue(std::string str): Value("java.lang.String", str) {}
        
        void Set(JEnv& env, jobject object) {
            jstring tmp = (jstring) object;
            
            if(tmp == nullptr) value =  "Undefined";
            
            const char *str =env->GetStringUTFChars( (jstring)tmp , NULL );
            value = str;
            env->ReleaseStringUTFChars( (jstring)tmp ,str );
        };
        
        jvalue GetJavaValue(JEnv& env)  {
            jvalue javaValue;
            javaValue.l =  env->NewStringUTF( value.c_str() );
            return javaValue;
        };
        
    };

    /*  ====== ArrayValue =======    */

    template <typename T>
    struct ArrayValue: public T {
        typedef typename T::Type Type;
        ArrayValue(std::string type): T(type) {};
        
        
        template <typename F>
        void Set(F& fn, JEnv& env, jobject _array) {
            Type array = (Type) _array;
           
            jint count = env->GetArrayLength( array );
            this->value.resize(count);
            
            fn (env.get(), array , 0, count, &this->value[0] );
            env->DeleteLocalRef(array);
        }
    };
    
    
    class IntArrayValue : public ArrayValue< Value<jintArray, std::vector<int>> > {
    public:
        IntArrayValue(): ArrayValue("[I") {};
        
        void Set(JEnv& env, jobject _array) {
            ArrayValue::Set(env->functions->GetIntArrayRegion, env, _array);
        };
    };

    
    class ByteArrayValue : public ArrayValue< Value<jbyteArray, std::vector<signed char>> > {
    public:
        ByteArrayValue(): ArrayValue("[B") {};
        
        void Set(JEnv& env, jobject _array) {
            ArrayValue::Set(env->functions->GetByteArrayRegion, env, _array);
        };
    };
    
    
}



#endif /* values_hpp */