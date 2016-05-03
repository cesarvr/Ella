'use strict';
var fs = require('fs');
var child = require('child_process');

//Helpers
var exist = function(group) {

    for (var i in group) {
        var path = group[i];
        if (!fs.existsSync('.'+path))
            return false;
    }

    return true;
}




// Installer steps.
var download = function(next) {
    console.log('proceed download OpenJDK.');

    var arch = process.arch;

    return false;
};

var look_java_home = function() {
    console.log('looking for JAVA_HOME.');


    return false;
};

var lookup_jvm = function() {

    var jdk6 = ['./jdk/include', './jdk/include/linux'];

  //  if (exist(jdk6)) {

        console.log(JSON.stringify(jdk6) + ',' );
   //     return true;
   // }


 //   return false;
};


var install = function() {

    var steps = [lookup_jvm, look_java_home, download];
    var state = false;

    for (var i = 0; i < steps.length; i++)
        if (steps[i]()) break;

    return state;
};



module.exports = {
    include: lookup_jvm,
    install: install
};
