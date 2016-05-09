var fs = require('fs');


var java;
try {
        //RELEASE
    java = require('./build/Debug/ella.node'); //debug mode.
} catch (e) {
        //DEBUG
    java = require('../build/Debug/ella.node'); //debug mode.

    try {

        java = require('ella');
    } catch (e) {
    }
}

var vm = {};
var strBuffer;
var pdf;
var str;


java.setClassPath(['../demo/lib', '../demo/PDFHtml/bin'], true);

java.start(function(_vm) {
    vm = _vm;

    pdf = vm.new('pdf.P2HService');

    var things = ['Rock', 'Paper', 'Scissor'];
    var s = "", size = 2000;

    console.time("string class [concat]");

    for (var i = 0; i < size; i++) {
        var thing = things[Math.floor(Math.random() * things.length)];
        s = pdf.concat(s, thing)
    }

    console.timeEnd("string class [concat]");
    console.log('str length: ', s.length, ' iterations: ',size );


    
   console.time("object allocation.");
  
    size = 10000;
    var pdfs = []; 
    for (var i = 0; i < size; i++) {
      pdfs[i] = vm.new('pdf.P2HService');
    }

   console.timeEnd("object allocation.");
   console.log('objs number: ', pdfs.length, ' iterations: ',size );
});
