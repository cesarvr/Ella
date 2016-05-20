//
//  values.h
//  LibJNI
//
//  Created by Cesar Valdez on 06/04/2016.
//

#ifndef values_hpp
#define values_hpp

#include "jvm_global.h"
#include "jvm_handler.h"
#include "utils.h"

class JavaMethod;
class ArgumentTypeInfo;
class Arguments;

namespace LibJNI {
    
    // We need to check the arguments at runtime so this class create a common
    // interface, for interacting
    // with the Value class.
    
    class BaseJavaValue {
    public:
        // Override with the value expected by JNI for java Argument.
        virtual std::string GetType() {
            throw VMError{"Type not implemented yet."};
            return "";
        };
        
        // Override with the value expected by JNI for java Argument.
        virtual std::string GetJVMType() {
            throw VMError{"Type not implemented yet."};
            return "";
        };
        
        
        // Override with the handling behavior for Native to JNI type.
        virtual jvalue GetJavaValue(JEnv &env) {
            throw VMError{"GetJavaValue not implemented yet for this type."};
        };
        
    };
    
    // This class is in charge to handle the arguments and the return type. Here are
    // implemented the basic
    // accessors and trivials constructors.
    
    template <class JNIType, class NativeType> class Value : public BaseJavaValue {
    public:
        typedef JNIType Type;
        
        Value(){};
        
        Value(std::string _type) : type(_type){
            nativeType = GetNativeType(_type);
        };
        
        Value(std::string _type, NativeType _value) : value(_value), type(_type) {
            nativeType = GetNativeType(_type);
        }
        
        std::string GetType() { return type; };
        
        bool isCompatible(std::string&& _type){
            if( _type == type )
                return true;
            return false;
        };
        
        void Set(JEnv &env, JNIType object) { value = (NativeType)object; }
        
        void Set(NativeType _value) { value = _value; }
        
        // accessors for native type.
        std::string GetJVMType(){ return nativeType; }
        
        std::string GetNativeType(std::string& type){
            for(auto t : native)
                if(t.first == type)
                    return t.second;
            
            return "L"+type+";";
        }
        
        NativeType& Get() { return value; }
        
    protected:
        NativeType value;
        std::string type;
        std::string nativeType;
        jvalue jniValue;
        
        
        //JVM native mapping types.
        std::vector< std::pair<std::string, std::string> > native =
        {
            std::make_pair("boolean", "Z"),
            std::make_pair("byte", "B"),
            std::make_pair("double", "D"),
            std::make_pair("float", "F"),
            std::make_pair("int", "I"),
            std::make_pair("long", "J"),
            std::make_pair("object", "L"),
            std::make_pair("short", "S"),
            std::make_pair("void", "V"),
            std::make_pair("array", "[")
        };
    };
    
    // dealing with jobjects this is just reusing the accessors.
    struct ObjectValue: public Value<jobject, jobject> {
        
        ObjectValue(): Value("java.lang.Object") {};
        
        ObjectValue(std::string className) :
        Value( Utils::normalizeClassName(std::move(className) )) {}
        
        //Override.
        bool isCompatible(std::string&& _type){
            type = _type;
            return true;
        }
    };
    
    //   reusing accessors and mapping types < JNI, JNI >, types get cast in
    //   Set(Native),
    //   so if you need more sophisticated treatment override that method.
    
    struct ObjectArray : public Value<jobject, jobjectArray> {
        ObjectArray() : Value("object") {}
    };
    
    // Here we override the BaseJavaValue::GetJavaValue, cause this data need some
    // treatment.
    
    struct IntValue : public Value<jint, int> {
        
        IntValue() : Value("int") {}
        IntValue(int x) : Value("int", x) {}
        
        jvalue GetJavaValue(JEnv &env) {
            jniValue.i = value;
            return jniValue;
        }
    };
    
    struct FloatValue : public Value<jfloat, float> {
        
        FloatValue() : Value("float") {}
        FloatValue(float x) : Value("float", x) {}
        
        jvalue GetJavaValue(JEnv &env) {
            jniValue.f = value;
            return jniValue;
        }
    };
    
    struct DoubleValue : public Value<jdouble, double> {
        DoubleValue() : Value("double") {}
        DoubleValue(float x) : Value("double", x) {}
        
        jvalue GetJavaValue(JEnv &env) {
            jniValue.d = value;
            return jniValue;
        }
    };
    
    
    //   We override Set and GetJavaValue.
    
    struct StringValue : public Value<jstring, std::string> {
        
        StringValue() : Value("java.lang.String") {}
        StringValue(std::string str) : Value("java.lang.String", str) {}
        
        void Set(JEnv &env, jobject object) {
            jstring tmp = (jstring)object;
            
            if (tmp == nullptr)
                value = "Undefined";
            
            const char *str = env->GetStringUTFChars((jstring)tmp, NULL);
            value = str;
            env->ReleaseStringUTFChars((jstring)tmp, str);
        };
        
        jvalue GetJavaValue(JEnv &env) {
            jvalue javaValue;
            javaValue.l = env->NewStringUTF(value.c_str());
            return javaValue;
        };
    };
    
    /*  ====== ArrayValue =======    */
    
    //    ArrayValue: T
    
    // Reusing code to iterate over ArrayType in JNI.
    
    template <typename ValueType> struct ArrayValue : public ValueType {
        typedef typename ValueType::Type Type;
        ArrayValue(std::string type) : ValueType(type){};
        
        template <typename F, typename T> void Set(F &fn, JEnv &env, jobject _array) {
            Type array = (Type)_array;
            
            jint count = env->GetArrayLength(array);
            this->value.resize(count);
            
            fn(env.get(), array, 0, count, (T *)&this->value[0]);
        }
    };
    
    // IntArrayValue
    
    //  I treat here Java native int[] arrays, so we mix [ArrayValue, Object] and
    //  then inherit their members
    //  overriding the Set method that is the one we specialize before.
    
    class IntArrayValue : public ArrayValue<Value<jintArray, std::vector<int>>> {
    public:
        IntArrayValue() : ArrayValue("[I"){};
        
        void Set(JEnv &env, jobject _array) {
            auto Interface = env->functions->GetIntArrayRegion;
            ArrayValue::Set<decltype(Interface), jint>(Interface, env, _array);
        };
    };
    
    //    ByteArrayValue
    //  Same as before but we change the function callback.
    
    class ByteArrayValue
    : public ArrayValue<Value<jbyteArray, std::vector<signed char>>> {
    public:
        ByteArrayValue() : ArrayValue("[B"){};
        
        void Set(JEnv &env, jobject _array) {
            auto Interface = env->functions->GetByteArrayRegion;
            ArrayValue::Set<decltype(Interface), jbyte>(Interface, env, _array);
        };
    };
}

#endif /* values_hpp */
