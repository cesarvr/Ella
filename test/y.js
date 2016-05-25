var ella = require('../build/Debug/ella'); 

ella.start(function(vm){
  var s = vm.new('java.lang.StringBuffer');
  console.log(s);
  // s.append('hello');
  //s.append('world');
  //s.reverse();
  //console.log('->',s.toString());
   console.time("object allocation.");
  
    size = 10000;
    var pdfs = []; 
    for (var i = 0; i < size; i++) {
      pdfs[i] = vm.new('java.lang.String');
    }

   console.timeEnd("object allocation.");
 
})

