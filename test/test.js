var java = require('../build/Debug/ella.node');
var fs = require('fs');

java.start(function(jvm) {

    console.log(jvm);

    console.log('loading->')
    var javaObject = jvm.New("pdf/P2HService");
/*
    console.log("def pdf/P2HService-> ", javaObject);
    console.log("(pdf/P2HService).toString ", javaObject.toString);
    console.log("(pdf/P2HService).toString() ", javaObject.toString());

    console.log("(pdf/P2HService).concat() ", javaObject.concat("Michael", "Jordan"));
    console.log("(pdf/P2HService).add() ", javaObject.add(11, 1000));
    // for(var i =0; i<100000; i++) {
    // }



    console.log("(pdf/P2HService).hashCode() ", javaObject.hashCode());

    // console.log( "(pdf/P2HService).html2pdf() ", javaObject.html2pdf("<html><body> This is my Project </body></html>") );
    var buffer = javaObject.html2pdf("<html><body> This is my Project </body></html>");
    var wstream = fs.createWriteStream('my_pdf.pdf');
    wstream.write(buffer);
    wstream.end(); */
});

console.log("if Async is working this should be show in first :-] ");
