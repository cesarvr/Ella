var jvm = require('../build/Debug/ella.node'); //debug mode.
var fs = require('fs');

jvm.setClassPath('../demo/lib/pdfbox-app-1.8.11.jar');

jvm.setClassPath('../demo/lib/itext-5.5.8/itextpdf-5.5.8.jar:../demo/PDFHtml/bin/:../demo/lib/itext-5.5.8/xmlworker-5.5.8.jar:../demo/lib/pdfbox-app-1.8.11.jar');


jvm.start(function(java) {

    var obj = java.New("pdf/P2HService");
    
    console.log('integer stress'); 

    /*
    for(var i=0; i<10; i++){
      obj.add(1,2, function(r){ console.log('->', r) } );
    }
*/

    console.log('-> string stress');
/*
    for(var i=0; i<10000; i++){
      obj.concat("dead", "beef", function(r){ console.log('->',r);});
    }
*/

    for(var i=0; i<10000; i++){
  console.log('-->', obj.concatHeavy("con", "cat", 2000, function(r){ console.log('->',r);  }));
    }

});
