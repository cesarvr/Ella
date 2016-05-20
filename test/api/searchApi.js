var jvm = require('../../build/Debug/ella.node') || require('./build/Debug/ella.node'); //debug mode.
var fs = require('fs');

//jvm.setClassPath('../demo/lib/pdfbox-app-1.8.11.jar');

//jvm.setClassPath('../demo/lib/itext-5.5.8/itextpdf-5.5.8.jar:../demo/PDFHtml/bin/:../demo/lib/itext-5.5.8/xmlworker-5.5.8.jar:../demo/lib/pdfbox-app-1.8.11.jar');


jvm.setClassPath(['/Users/cvaldez/Desktop/NWR/java/lib', '/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin'], true);

var PDF = function(cb) {

    var javaObject;


    jvm.start(function(java) {
        javaObject = java.New("pdf/P2HService");

        var obj = {
            readAndCachePDF: function(cb) {
                fs.readdir('../pdfs/', function(err, items) {
                  console.log(items);
                    var hash = {};
                    for (var i = 0; i < items.length; i++)
                        var stack = function() {
                            var k = items[i];
                            javaObject.Strip("../pdfs/" + items[i], function(str) {
                                hash[k] = str;
                                cb(hash);
                            });
                        }();
                });
            },

            previewPDF: function(pdfName, cb){
              
              javaObject.preview("../pdfs/" + pdfName, function(buffer) {
                cb(buffer.toString('base64'));
              });
            }
        }

        cb(obj);
    });


};

module.exports = PDF;
