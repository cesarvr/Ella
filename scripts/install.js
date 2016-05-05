'use strict';
var fs = require('fs');
var child = require('child_process');
var file = require('file-tools');
var path = require('path');
var install_dir = './jdk';
var jdk6 = [install_dir + '/include', install_dir + '/linux', install_dir + '/libjvm.so'];


var app_folder = path.dirname(require.main.filename);

//Helpers
//


// Installer steps.
var download = function(next) {
    console.log('proceed download OpenJDK.');

    var arch = process.arch;

    return false;
};

var search_jdk = function(path, options) {
    console.log('looking for jdk in->', path);

    console.log(path)
    var libjvm = file.search(path, "libjvm.so", options);
    var include = file.search(path, 'include', options);
    var linux = file.search(path, 'linux', options);

    var jdk = [include, linux, libjvm];
    var err = null;

    if (file.exists([include.path, linux.path, libjvm.path])) {
        console.log('include->', include.path);
        console.log('linux->', linux.path);
        console.log('lib->', libjvm.path);


        jdk.forEach(function(file) {


            fs.symlink(file.path, install_dir + '/' + file.file, function(err, arg) {
                err = err;
            });
        });

        if (err) console.log('error => ', err);
        else
            return true;
    }

    console.log('jdk files: not found');

    return false;
}

var look_java_home = function() {
    console.log('looking for JAVA_HOME.');

    var path = process.env['JAVA_HOME'];

    var options = {};
    options.exclude = ['bin', 'man', 'db'];


    return search_jdk(path, options);
};

var jdk_debug = function() {

    console.log('looking in user folder for jdk [DEBUG]');

    var options = {};
    options.exclude = ['.git', 'www', 'build', 'node_modules', 'docs', 'src'];

    return search_jdk(app_folder + '/../', options);
};

var look_user_folder = function() {

    console.log('looking in user folder for jdk');

    var path = process.env['PWD'];
    var options = {};
    options.exclude = ['.git', 'www', 'build', 'node_modules', 'docs', 'src'];


    return search_jdk(path + '/', options);
};



var lookup_jvm = function() {
    console.log('checking user jdk');

    if (file.exists(jdk6)) {
        console.log('found one jdk < 8');
        return true;
    }

    console.log('not found..');
    return false;
};


var pre_install = function() {
    if (!fs.existsSync(install_dir)) {
        fs.mkdirSync(install_dir);
    };
};

var install = function() {

    var steps = [pre_install, lookup_jvm, /*jdk_debug,*/ look_user_folder, look_java_home, download];

    for (var i = 0; i < steps.length; i++)
        if (steps[i]()) break;

    return false;
};

install();
