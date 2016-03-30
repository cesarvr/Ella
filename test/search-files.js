var jvm = require('../build/Debug/ella.node') || require('./build/Debug/ella.node'); //debug mode.
var fs = require('fs');
var path = require('path');



//linux VM
//jvm.setClassPath('../demo/lib/itext-5.5.8/itextpdf-5.5.8.jar:../demo/PDFHtml/bin/:../demo/lib/itext-5.5.8/xmlworker-5.5.8.jar:../demo/lib/pdfbox-app-1.8.11.jar');



// macosx location of jar.
jvm.setClassPath('-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/pdfbox/pdfbox-app-1.8.11.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/');




jvm.start(function(java) {

    var javaObject = java.New("pdf/P2HService");

    console.log(javaObject);
        console.log('generating pdf....');
        javaObject.Strip('/Users/cvaldez/Documents/javascript/pdfs/www.gnu.org1459374731.pdf', function(str){
          console.log('->', str);
        }); 
/*
    fs.readdir('../pdfs/', function(err, items) {
        console.log(items);

        for (var i = 0; i < items.length; i++) {
            console.log(items[i]);

            if (path.extname(items[i]) === '.pdf'){
                console.log('loading->', items[i]);

                try {
                javaObject.Strip("../pdfs/" + items[i], function(str) {
                    console.log('->', str);
                });
                }catch(e){
                  console.log(e);
                }
            }

        }
    });
*/

});
