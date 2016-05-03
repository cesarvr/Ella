var java

try {
    java = require('../build/Debug/ella.node')
} catch (e) {
    java = require('./build/Debug/ella.node'); //debug mode.

}

// linux location of the jars.
//java.setClassPath('../demo/lib/itext-5.5.8/itextpdf-5.5.8.jar:../demo/PDFHtml/bin/:../demo/lib/itext-5.5.8/xmlworker-5.5.8.jar');


 java.setClassPath(['../demo/lib', '../demo/PDFHtml/bin'], true);


java.start(function(jvm) {
/*
    console.log(jvm);

    console.log('loading->')
    var str = jvm.New('java.lang.String'); //("pdf/P2HService");

    console.log('Calling PDF Generator method');

    console.log('jvm->', jvm);
    console.log('pdf->', str);

    console.log('String::hashcode-> ', str.hashCode());

*/
   var pdf = jvm.New("pdf/P2HService");


    var s = pdf.add(5000, 5000);
    console.log('->', s);
});
