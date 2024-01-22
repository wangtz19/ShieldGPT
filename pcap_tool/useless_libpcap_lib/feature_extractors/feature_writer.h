#ifndef FEATURE_WRITER_H
#define FEATURE_WRITER_H

#include <fstream>

class FeatureWriter{
  public:
    FeatureWriter(std::string file_name = "");
    void write(std::string feature);
    void close();
  private:
    bool to_file_;
    std::ofstream file_;
};

#endif