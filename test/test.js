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
    it('turning on the jvm', function(done) {

        java.setClassPath(['/Users/cvaldez/Desktop/NWR/java/lib/', '/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/' ], true);

        assert.notEqual(java.getClassPath(), '', 'should expect something here.');
        console.log('classpath ->' ,java.getClassPath());

        java.start(function(_vm){
          vm = _vm;
          assert.isObject(vm, 'vm should be a object');
          done();
        });

    });




    var strBuffer;
    var pdf;
    it('loading class', function() {

        assert.isObject(vm, 'vm should be a object');
        console.log('vm->', vm);
        strBuffer = vm.New('java.lang.StringBuffer');
        var str = vm.New('java.lang.String');
        pdf = vm.New('pdf.P2HService');
        console.log('hashcode->', strBuffer);
        console.log('hashcode->', str);
        console.log('hashcode->', pdf);

        assert.isObject(strBuffer, 'loading  java->StringBuffer');
    });



    it('calling a method with string arg:  [java] StringBuffer -> append', function() {

        assert.isObject(strBuffer, 'loading  java->StringBuffer');
        assert.isFunction(strBuffer.append, 'StringBuffer.append');
        assert.isFunction(strBuffer.toString, 'StringBuffer.toString');
        
        console.log('pdf->', pdf);
        var s = pdf.concat("hello", "world")
        console.log('JS LAND->', s);
        //assert.isString(s, 'concat return string');

        //console.log(strBuffer.append('Hello World'));
        //console.log('o -> ', strBuffer.hashCode() );
    });


    /*




            it('calling a method with string arg:  [java] StringBuffer -> append', function() {
        
            assert.isFunction(strBuffer.append, 'StringBuffer.append');
            assert.isFunction(strBuffer.toString, 'StringBuffer.toString');

            strBuffer.append('Hello World');

            console.log('o -> ', strBuffer.toString());
        });
    */
})
