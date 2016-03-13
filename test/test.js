//var Segfault = require('segfault');
//Segfault.registerHandler("./seg/");

//var java = require('../build/Release/ella.node');

var java = require('../build/Debug/ella.node') || require('./build/Debug/ella.node'); //debug mode.

var fs = require('fs');
var exec = require('child_process').exec;
var url = require('url');

var http = require('http');
var https = require('https');

// macosx location of jar.
//java.setClassPath('-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/');


// linux location of the jars.
java.setClassPath('../demo/lib/itext-5.5.8/itextpdf-5.5.8.jar:../demo/PDFHtml/bin/:../demo/lib/itext-5.5.8/xmlworker-5.5.8.jar');

console.log('classpath->', java.getClassPath());

java.start(function(jvm) {

    console.log(jvm);

    console.log('loading->')
    var javaObject = jvm.New("pdf/P2HService");

    console.log('Calling PDF Generator method');

    /*
    javaObject.html2pdf("<html><body> This is my Project </body></html>", function(buffer) {
        console.log('pdf generated... ');
        var wstream = fs.createWriteStream('my_pdf.pdf');
        wstream.write(buffer);
        wstream.end();

        child = exec("open -a Preview ./my_pdf.pdf", function(error, stdout, stderr) {
            if (error !== null) {
                console.log('exec error: ' + error);
            }
        });


    });
*/
    /*
    process.argv.forEach(function (val, index, array) {
        if(index > 1) {
          console.log(index + ': ' + val);
          makePDF(val);
        }
    });

    */

    makePDF('www.gnu.org');

    function makePDF(address) {

        var options = {
            host: url.parse(address).host,
            path: url.parse(address).pathname,
            port: 443
        };


        console.log('->', options);
        var str = '';
        var callback = function(response) {

            //another chunk of data has been recieved, so append it to `str`
            response.on('data', function(chunk) {
                str += chunk;
            });

            //the whole response has been recieved, so we just print it out here
            response.on('end', function() {
                javaObject.html2pdf(str, function(buffer) {

                    var name = '../pdfs/' + options.host + Math.floor(Date.now() / 1000) + '.pdf';

                    var wstream = fs.createWriteStream(name);
                    wstream.write(buffer);
                    wstream.end();

                    /*
                    child = exec('open -a Preview ' + name , function(error, stdout, stderr) {
                        if (error !== null) {
                            console.log('exec error: ' + error);
                        }
                    });

                   */
                });

            });
        }

        //      https.request(options, callback).end();

    }

    console.log('generating pdf....');
});

console.log("if Async is working this should be show in first :-] ");
