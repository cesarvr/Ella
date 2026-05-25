# Ella

This project is a NodeJS native plugin to run Java libraries and code inside NodeJS / Javascript (V8) VM, written in C++ and compatible with Node.js 6. The motivation for this was that our main product at Feedhenry was a NodeJS only cloud platform and some customers sometimes needed PDF generation and in those days the best library for that job was iText. 

It supports multithreading (delegate heavy blocking operations to Java without blocking Javascript) and type/method translation, so you can seamlessly pass values between Java and Javascript.


### Other reasons to write this plugin… 

- Because Java have very good ecosystem of libraries like PDFBox, iText, Solr, etc.., all mature and useful.
- The objective is to create wrappers around the library you want and use it in your Node app.
- Easing Node.js integration with legacy Java system. 
- Performance, comunication between JS and a JVM method is ≈<1ms.
 
### Features 
  
  - The JVM run in the same NodeJS process as a native add-on so comunication between each other is fast. 
  - Allow you to load jar/classes and use them inside NodeJS like Javascript objects literals.  
  - The API can choose the right method giving a set of arguments, so it solves the method overloading in Java. [experimental].
  - Exception handling, exception thrown by JVM are translated to Javascript [exception](http://www.w3schools.com/js/js_errors.asp). 
  
##### Multi-Thread
  - You can send blocking Java calls to a background thread and assign a callback to continue when finished.
  ![alt tag](https://github.com/cesarvr/Ella/blob/master/docs/ella_multi_thread.png) 

##### Performance
  - I also wrote [this C++ library](https://github.com/cesarvr/JNI) to handle the caching/reflection details for each instantiated object, it was very fun...
  ![alt tag](https://github.com/cesarvr/Ella/blob/master/docs/design-diagram.png) 
 
### Things to be added soon. 
	
  - support to static classes.
  - support to non-void constructor. 
  - support for java.lang.object derivatives args/return.
 
 
### Supported types [for now] 
  - Primitive
    - int
    - java.lang.String 
    - double 
    - byte[] - to Buffer to handle binaries.

  - Object [comming soon...]

### Installation

Ella requires [Node.js](https://nodejs.org/) v4+ to run.

```sh
$ export JAVA_HOME=/jdk/location/     #finish with / is important here, there is a bug in the installer :(
$ npm install ella
```

### Methods

#### setClassPath ( path: string array, recursive: true)

path: array, paths where to find the .jars/.class

recursive: true/false, if true look for jar/classes recursively. 


This method allow us to configure the classpath,  

```sh
	var ella = require('ella');
	ella.setClassPath(['/folder/with/.jars/.class', ...], true);  //if true flag, it will look recursively all jars/class. 
```



#### getClassPath ( void )

return the classpath configuration. 

This method allow us to configure the classpath.

```js
	ella.getClassPath();  //myjar1.jar:myjar2.jar   
```


#### start ( callback )

create an JVM instance asynchronously.
 
callback(instance): function take as parameter an instance of the JVM. 

This method allow us to configure the classpath.

```js
	ella.start(function(vm){  /* do some work with vm */   })
```



#### vm.new (string qualified classname)

given a classname create a new object.

```js
	var stringBuffer = vm.new('java.lang.StringBuffer');
	
	// stringBuffer.append
	// stringBuffer.insert
	// stringBuffer.substring 
	// ....
```

### sync call 

To make an sync call just call the method.

```js
	stringBuffer.append('hello'); 
	console.log(stringBuffer.toString() );  //hello. 
```


### async call 

Just call the method as normal and add a function callback as an extra parameter this extra parameter transform the call to async. 

```js
	
	var pdf = vm.new('com.pdf.Library'); // method signature  byte[] createPDF(string); 
	
	var buffer = pdf.createPDF('my_blocking.pdf');   // this call will block the interpreter in this position. 
	
	// the addition of an anonymous function make all jvm->js-method async.
	pdf.createPDF('my_async.pdf', function(buffer){  /* do some work with buffer */ }); // non-blocking call.
	
	//js code.......
```








