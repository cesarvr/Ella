
var jvm = require('../build/Debug/ella.node') || require('./build/Debug/ella.node'); //debug mode.
var fs = require('fs');

jvm.setClassPath('../demo/lib/pdfbox-app-1.8.11.jar');

jvm.setClassPath('../demo/lib/itext-5.5.8/itextpdf-5.5.8.jar:../demo/PDFHtml/bin/:../demo/lib/itext-5.5.8/xmlworker-5.5.8.jar:../demo/lib/pdfbox-app-1.8.11.jar');


jvm.start(function(java) {

    var javaObject = java.New("pdf/P2HService");

    console.log(javaObject);

    console.log('generating pdf....');
    javaObject.Strip('../pdfs/www.gnu.org1457881839.pdf', function(str){
      console.log('->', str);
    }); 
    


});


