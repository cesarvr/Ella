# Ella

Is an NodeJS native plugin to allow Java code execution inside V8/NodeJS container. Is done in C++11 so I implement a lot of nice feature for error handling, type inference, lambdas , etc...
   

##Why 

- Cause a lot of nice libraries are writted in Java, libraries like Jasper Report, PDFBox are very useful. 
- Why not. 

## Hello World 
	```js
		var java = require('../build/Release/ella.node');
		java.start(function(jvm) {  // Async loading of the JVM and getting an instantiator back @jvm.  
			
																									
			var javaObject = jvm.New("pdf/P2HService");  // package pdf.P2HService. you get the same object members like the java declared class. (for now only accepts empty constructor).  
			
			/*   javaObject now possess the same objects as his counterpart in Java. 
					 
			 *   + add
       			 *   + toString
			*   + hashCode 
			 *   + equals , etc...
       */

			
		  javaObject.add(1,1); // call a simple Java method. 

			javaObject.add(1,1, function(r){ console.log('-->',r);  }); // call a simple method async. :) 
			
			javaObject.toString()  // you get object@hashcode			 

		});

	```
	

## Things that work for now. 

- Object with single constructor, instantiation.  
- Method Calling. 
- Async calls are little buggy for know for more than 15+ concurrent threads. 


## Things that don't work.

- Instancianting objects with N>1 arguments. 
- Method overloading, this doens't exist in javascript. 
- Method only accept for now int/string/float.  









 
