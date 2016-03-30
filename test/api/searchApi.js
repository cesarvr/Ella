var jvm = require('../../build/Debug/ella.node') || require('./build/Debug/ella.node'); //debug mode.
var fs = require('fs');
var path = require('path');


// macosx location of jar.
jvm.setClassPath('-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/pdfbox/pdfbox-app-1.8.11.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/');


/*
jvm.setClassPath('../demo/lib/itext-5.5.8/itextpdf-5.5.8.jar:../demo/PDFHtml/bin/:../demo/lib/itext-5.5.8/xmlworker-5.5.8.jar:../demo/lib/pdfbox-app-1.8.11.jar');
*/


var PDF = function(cb) {

    var javaObject;


    jvm.start(function(java) {
        javaObject = java.New("pdf/P2HService");

        var obj = {
            readAndCachePDF: function(cb) {
                fs.readdir('../pdfs/', function(err, items) {
                    var hash = {};
                    for (var i = 0; i < items.length; i++)
                        var stack = function() {
                            var pdf_name = items[i];
                            console.log('sapik->', pdf_name, 'ext->', path.extname(pdf_name), ' ??->', path.extname(pdf_name) === '.pdf');
                            if (path.extname(pdf_name) === '.pdf') {
                                console.log('loading pdf -> ', pdf_name);
                                javaObject.Strip("../pdfs/" + pdf_name, function(str) {
                                    console.log('adding...');
                                    hash[pdf_name] = str;
                                    cb(hash);
                                });
                            } else
                                cb({});
                        }();
                });
            },

            previewPDF: function(pdfName, cb) {

                javaObject.preview("../pdfs/" + pdfName, function(buffer) {
                    console.log('preview of:', pdfName);
                    cb(buffer.toString('base64'));
                });
            }
        }

        cb(obj);
    });


};

module.exports = PDF;
