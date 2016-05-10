var fs = require('fs');

var java;
try {
    console.log('trying [release]')
        //RELEASE
    java = require('../build/Release/ella.node'); //debug mode.
} catch (e) {
    console.log('fail: trying [debug]')
        //DEBUG
    java = require('../build/Debug/ella.node'); //debug mode.
}

var vm = {};
var strBuffer;
var pdf;
var str;


java.setClassPath(['../demo/lib', '../demo/PDFHtml/bin'], true);

java.start(function(_vm) {
    vm = _vm;

    pdf = vm.New('pdf.P2HService');

var things = ['Rock', 'Paper', 'Scissor'];
var s="";


    console.time("object allocation");

    for(var i =0; i<2000; i++){
    
    var thing = things[Math.floor(Math.random()*things.length)];
    
      s = pdf.concat(s, thing)
    }

    console.timeEnd("object allocation");
    console.log('str length->', s.length);




    pdf.add(5000, 5000, function(s) {

        console.log('s->', s);
    });



    for (var i = 0; i < 10; i++) {
        pdf.concatHeavy("hello", "world", 1000, function(s) {

            console.log('s->', s);
        });



    }

    var pdfs = [];
    console.time("object allocation");
    for (var i = 0; i < 5000; i++) {
        pdfs.push(vm.New('pdf.P2HService'));
    }
    console.timeEnd("object allocation");

  

});
