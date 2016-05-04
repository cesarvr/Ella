var assert = require('chai').assert;


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

describe('ella', function() {

    it('creating object and checking members', function() {
        assert.isObject(java, '');

        assert.isFunction(java.getClassPath, 'getClassPath function');
        assert.isFunction(java.start, 'start function');
        assert.isFunction(java.setClassPath, 'setClassPath function');
    });

    var vm = null;

    it(' testing jar files search: ', function() {

        console.time('recursive search');
        java.setClassPath(['/Users/cvaldez/Desktop/NWR/java/lib', '/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin'], true);
        
        //java.setClassPath(['../demo/lib', '../demo/PDFHtml/bin'], true);
        console.timeEnd('recursive search');

        assert.notEqual(java.getClassPath(), '', 'should expect something here.');
        //console.log('classpath ->' ,java.getClassPath());
    });





    it('turning on the jvm', function(done) {

        java.setClassPath(['/Users/cvaldez/Desktop/NWR/java/lib/', '/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/'], true);

        //java.setClassPath(['../demo/lib', '../demo/PDFHtml/bin'], true);
        assert.notEqual(java.getClassPath(), '', 'should expect something here.');
        //console.log('classpath ->' ,java.getClassPath());

        java.start(function(_vm) {
            vm = _vm;
            assert.isObject(vm, 'vm should be a object');
            done();
        });

    });




    var strBuffer;
    var pdf;
    var str;

    it('loading class, and checking hashcode method call', function() {

        assert.isObject(vm, 'vm should be a object');
        console.log('vm->', vm);
        strBuffer = vm.New('java.lang.StringBuffer');
        str = vm.New('java.lang.String');
        pdf = vm.New('pdf.P2HService');

        assert.isNumber(strBuffer.hashCode(), 'type number expected');
        assert.isNumber(str.hashCode(), 'type number expected');
        assert.isNumber(pdf.hashCode(), 'type number expected');

        assert.isObject(strBuffer, 'loading  java->StringBuffer');
    });


    it('loading an array of 1000 java objects', function() {
        var pdfs = [];

        console.time("object allocation");
        for (var i = 0; i < 1000; i++) {
            pdfs.push(vm.New('pdf.P2HService'));
        }
        console.timeEnd("object allocation");

        for (var i = 0; i < 999; i++) {
            var pdf = pdfs[i];
            assert.isObject(pdf, 'object needed here');
            assert.isNumber(pdf.hashCode(), 'type number expected');
            //assert.equals(pdfs.length, 1000 , '1000 objects');
        }


    });

    it('calling methods with Args(String...)', function() {

        assert.isObject(pdf, 'loading  pdf');
        assert.isFunction(pdf.concat, 'pdf.concat');

        var s = pdf.concat("hello", "world")
        assert.isString(s, 'concat return string');
    });

    it('calling methods with Args(Int...)', function() {

        assert.isObject(pdf, 'loading  pdf');
        assert.isFunction(pdf.add, 'pdf.add');

        var s = pdf.add(5000, 5000);
        assert.equal(s, 5000 + 5000, 'equals ' + (5000 + 5000));
    });


    it('calling methods with Args(Int...)  [async]', function(done) {

        assert.isObject(pdf, 'loading  pdf');
        assert.isFunction(pdf.add, 'pdf.add');

        pdf.add(5000, 5000, function(s) {
            assert.equal(s, 5000 + 5000, 'equals ' + (5000 + 5000));
            done();
        });
    });

    it('calling methods with Args(String...)  [async]', function(done) {

        assert.isObject(pdf, 'loading  pdf');
        assert.isFunction(pdf.add, 'pdf.concat');

        pdf.concat("hello", "world", function(s) {
            assert.isString(s, 'concat return string');
            done();
        });
    });

    it('calling methods with Args(String...): Sleep Thread in Java side.  [async]', function(done) {

        assert.isObject(pdf, 'loading  pdf');
        assert.isFunction(pdf.add, 'pdf.concat');
        assert.isFunction(pdf.concatHeavy, 'pdf.concatHeavy');

        pdf.concatHeavy("hello", "world", 1000, function(s) {
            assert.isString(s, 'concat return string');
            done();
        });
    });


    it('calling void<method> ', function(){
    
        var str = vm.New('java.lang.StringBuffer');
        assert.isObject(str, 'object instanciated'); 
        str.append('Hello World');

        assert.equal(str.toString(),'Hello World', 'should return a string');
    });

})
