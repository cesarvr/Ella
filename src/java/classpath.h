//
//  classpath.h
//  LibJNI
//
//  Created by Cesar Valdez on 20/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef classpath_hpp
#define classpath_hpp

#include <iostream>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>


#include "jvm_global.h"


/* Classpath handle here allowing recursive search for .jar and .class */

class ClassPath {

    
public:
    static std::string LocateLibraries(std::vector<std::string> directories, bool recusive);
    
private:
    std::string classPath;
};



#endif /* classpath_hpp */
