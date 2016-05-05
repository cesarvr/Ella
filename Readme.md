# Ella

Is an NodeJS native plugin to allow Java code execution inside V8/NodeJS container. Is done in C++11 so I implement a lot of nice feature for error handling, type inference, lambdas , etc...
   



##Why 

- because Java have very good ecosystem of libraries like PDFBox, iText, Solr, etc.., all mature and useful.

 
## Features 
  
  - the JVM run in the same NodeJS process as a native add-on. 
  - Allow you to load jar/classes and use them inside NodeJS like Javascript objects literals.  
  - You can send heavy Java methods calls to the background thread and assign a javascript callback when it finish.  
	

## Things that work for now. 

- Object with single constructor, instantiation.  
- Method Calling. 
- Async calls are little buggy for know for more than 15+ concurrent threads. 


## Things that don't work.

- Instancianting objects with N>1 arguments. 
- Method overloading, this doens't exist in javascript. 
- Method only accept for now int/string/float.  
