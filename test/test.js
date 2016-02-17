var java = require('../build/Release/ella.node');

java.start(function(jvm){

  console.log(jvm);

  console.log('loading->')
  var javaObject = jvm.New("pdf/P2HService");
  
  console.log( "def pdf/P2HService-> ", javaObject );
  console.log( "(pdf/P2HService).toString ", javaObject.toString );
  console.log( "(pdf/P2HService).toString() ", javaObject.toString() );

});
