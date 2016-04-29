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


    it('calling a method with string arg:  [java] StringBuffer -> append', function() {

        assert.isObject(strBuffer, 'loading  java->StringBuffer');
        assert.isFunction(strBuffer.append, 'StringBuffer.append');
        assert.isFunction(strBuffer.toString, 'StringBuffer.toString');


        console.log(strBuffer.append('Hello World'));
        //console.log('o -> ', strBuffer.toString());
    });


    /*




            var strBuffer;
        it('loading class', function() {
        
            assert.isObject(vm, 'vm should be a object');
            console.log('vm->', vm);
            strBuffer = vm.New('java/lang/StringBuffer');


            assert.isObject(strBuffer, 'loading  java->StringBuffer');
        });

        it('calling a method with string arg:  [java] StringBuffer -> append', function() {
        
            assert.isFunction(strBuffer.append, 'StringBuffer.append');
            assert.isFunction(strBuffer.toString, 'StringBuffer.toString');

            strBuffer.append('Hello World');

            console.log('o -> ', strBuffer.toString());
        });
    */
})
