#include "feature_writer.h"

#include <iostream>
#include <fstream>

FeatureWriter::FeatureWriter(std::string file_name){
    if(file_name == ""){
        to_file_ = false;
    }
    else{
        to_file_ = true;
        file_.open(file_name);
    }
}

void FeatureWriter::write(std::string feature){
    if(to_file_)
        file_ << feature;
    else
        std::cout << feature;
}

void FeatureWriter::close(){
    if(to_file_)
        file_.close();
}