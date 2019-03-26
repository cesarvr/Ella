'use strict';
var fs = require('fs');
var child = require('child_process');
var file = require('file-tools');
var path = require('path');
var install_dir = './jdk';
var jdk6 = [install_dir + '/include', install_dir + '/linux', install_dir + '/libjvm.so'];
var os = require('os');


var app_folder = path.dirname(require.main.filename);

var complete_path = function(path) {

  if( process.platform === "win32") {
    if(path.slice(-1) !== '\\') path += '\\' 
  }else {
    if(path.slice(-1) !== '/') path += '/' 
  }

  return path
}


//Helpers
//


// Installer steps.
var download = function(next) {
  console.log('proceed download OpenJDK.');
  var arch = process.arch;

  return false;
};

var search_jdk = function(path, options) {
  path = complete_path(path)
  console.log('looking for jdk in->', path);

  var libjvm = null
  var include = null
  var linux = null
  var jdk =[]
  if(process.platform === "win32"){
     libjvm = file.search(path, "jvm.lib", options);
     include = file.search(path, 'include', options);
     jdk = [include, libjvm];
  }else{
     libjvm =  file.search(path, "libjvm.so", options);
     include = file.search(path, 'include', options);
     linux =   file.search(path, 'linux', options);
     jdk = [include, linux, libjvm];
  }
 

  
  var err = null;
  var paths = jdk.map(function(file){
    return file.path
  })

  if (file.exists(paths)) {
    console.log('paths->', paths);
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

  if(path === '') return false;
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

var handle_macosx = function(){
  return os.platform() === 'darwin'
}

var pre_install = function() {
  if (!fs.existsSync(install_dir)) {
    fs.mkdirSync(install_dir);
  };
};

var install = function() {

  var steps = [pre_install, handle_macosx, lookup_jvm, /*jdk_debug,*/ look_java_home, download];

  for (var i = 0; i < steps.length; i++)
    if (steps[i]()) break;

  return false;
};

install();
