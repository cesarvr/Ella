## LibJNI
Is as C++ library to ecapsulate all complexity of JNI in an object oriented fashion. 


## Classes

### JVMLoader
 - This App is in charge of the initialization of the VM and Classpath handling.

### Object 
 - Class that take care of the object creation inside the JVM.

### Reflect 
 - In charge of the reflection mechanism, useful to gather information from the object we are dealing with, like arguments, return, method signature and method pointer, all this information is saved inside **JavaMethod** class. 

### Value 
 - Is a nice template class in charge of handling the type-value transition between C++ and Java, if you want to support a new Java type we just need to create a new template specialization.
 
### Invoke 
 - Wraps the JNI-Method calling inside some error check mechanism, and it binds C++ types to the correct JNI function call.


