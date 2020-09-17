#include <sys/types.h>

#include "net.h"
#include "constants.h"
#include "utils.h"

int main(int argc, char* argv[])
{ 
    auto net = cv::dnn::readNetFromDarknet(CFG_PATH, WEIGHT_PATH);
    //net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    //net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    if (argc == 1){
        std::cout << "USAGE: ./app <OPTION>" << "\n" << "Options:" << "\n" << "0 : Video Input" << "\n" << "1 : Image Input" << std::endl;
        return 0;
    }

    else if (std::strcmp(argv[1],"0")==0) {
        std::string video_path;

        std::cout << "Video path : ";
        std::cin >> video_path;

        video_process(video_path, net);

        return 0;
    }

    else if (std::strcmp(argv[1],"1")==0) {
        std::string img_path;

        std::cout << "Image path : ";
        std::cin >> img_path;

        frame_process(img_path, net);

    }

    else {
        std::cout << "USAGE: ./app <OPTION>" << "\n" << "Options:" << "\n" << "0 : Video Input" << "\n" << "1 : Image Input" << std::endl;
        return 0;
    }
}