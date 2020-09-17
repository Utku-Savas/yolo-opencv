#include "utils.h"

std::string base_name(const std::string path) { 
    return path.substr(path.find_last_of("/\\") + 1); 
}

std::vector<std::string> read_directory(const std::string& name){
    std::vector<std::string> v;

    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);

    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] == "." || v[i] == ".."){
            v.erase(v.begin() + i);
            i--;
        }
    }

    return v;
}

std::vector<std::string> read_classes(const std::string path){
    std::vector<std::string> c;
    {
        std::ifstream class_file(path);
        if (!class_file)
        {
            std::cerr << "failed to open classes.txt\n";
            
        }

        std::string line;
        while (std::getline(class_file, line))
            c.push_back(line);
    }
    return c;
}

void write_txt(std::string cname, cv::Rect rect, std::string fpath){
    
    std::ofstream file;
    file.open(fpath);

    file << cname << " " << rect.x << " " << rect.y << " " << rect.x + rect.width << " " << rect.y + rect.height <<  std::endl;

    file.close();
    
}