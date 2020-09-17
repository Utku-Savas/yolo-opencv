#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>


std::string base_name(const std::string path);
std::vector<std::string> read_directory(const std::string& name);
std::vector<std::string> read_classes(const std::string path);
void write_txt(std::string cname, cv::Rect rect, std::string fpath);
