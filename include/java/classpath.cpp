//
//  classpath.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 20/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "classpath.hpp"


bool checkExtension(std::string fileName, std::initializer_list<std::string> extensions) {
 
    for(auto extension: extensions)
        if(fileName.substr(fileName.find_last_of(".") + 1) == extension)
            return true;
    
    return false;
}

bool isDotDirectory(std::string dot) {
    char ch = dot.back();
    return ch == '.';
}

bool isDir(std::string path){
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

void addFileSeparator(std::string& path) {
    if(isDir(path))
        if(path.back() != '/')
            path.append("/");
}



void listAllFilesInDirectory(std::string directory, std::string& classPath, bool recursive){
    DIR *directory_strt;
    struct dirent *dir_ent;
    
    
    addFileSeparator(directory);
    
    if ((directory_strt = opendir (directory.c_str())) != NULL && isDir(directory)) {
        
        // print all the files and directories within directory
        while ((dir_ent = readdir (directory_strt)) != NULL) {
            
            if(checkExtension(dir_ent->d_name, {"jar" , "class" } ) )
                classPath += directory  + dir_ent->d_name + ":";
            
            if(dir_ent->d_type == DT_DIR && !isDotDirectory(dir_ent->d_name) && recursive)
                listAllFilesInDirectory(directory + dir_ent->d_name + '/', classPath, true);
        }
        
        classPath += directory+ ":";
        closedir (directory_strt);
    }else
        throw VMError{"Could not open directory: " + directory};
}

std::string ClassPath::LocateLibraries( std::vector<std::string> directories, bool recursive = false ){
    std::string classPath;
    
    for(auto &directory: directories)
        listAllFilesInDirectory(directory, classPath, recursive);
    
    classPath.pop_back();
    return classPath;
}