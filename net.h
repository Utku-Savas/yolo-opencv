#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

void video_process(const std::string& name, cv::dnn::Net& net);
void frame_process(const std::string& name, cv::dnn::Net& net);
void detect(cv::dnn::Net& net, cv::Mat frame, std::vector<std::string> class_names, auto output_names);