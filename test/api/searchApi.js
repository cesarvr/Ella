'use strict';
var fs = require('fs');
var jvm = {};

try {
    console.log('trying [release]')
        //RELEASE
    jvm = require('../../build/Debug/ella.node'); //debug mode.
} catch (e) {
    console.log('fail: trying [debug]')
        //DEBUG
    try {
        jvm = require('../build/Debug/ella.node'); //debug mode.
    } catch (e) {
        try {
            jvm = require('ella');
        } catch (e) {
            console.log('Ella not found!');
        }
    }
}



jvm.setClassPath(['../demo/lib/', '../demo/PDFHtml/'], true);

console.log(jvm.getClassPath());


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
                            var k = items[i];
                            javaObject.Strip("../pdfs/" + items[i], function(str) {
                                //console.log('sapik->', k);
                                hash[k] = str;
                                cb(hash);
                            });
                        }();
                });
            },

            previewPDF: function(pdfName, cb) {

                javaObject.preview("../pdfs/" + pdfName, function(buffer) {
                    cb(buffer.toString('base64'));
                });
            }
        }

        cb(obj);
    });


};

module.exports = PDF;
