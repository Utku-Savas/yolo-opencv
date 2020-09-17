#include "net.h"
#include "utils.h"
#include "constants.h"


void video_process(const std::string& name, cv::dnn::Net& net)
{    
    cv::VideoCapture source(name);

    const std::string window_name = base_name(name);

    double fps = source.get(cv::CAP_PROP_FPS);
    
    auto output_names = net.getUnconnectedOutLayersNames();
    
    std::vector<std::string> class_names = read_classes(CLASS_PATH);

    cv::Mat frame;

    cv::namedWindow(window_name);

    while(1)
    {
        source >> frame;
        if (frame.empty())
        {
            break;
        }
        
        detect(net, frame, class_names, output_names);

        cv::imshow(window_name, frame );
        if (cv::waitKey(1000 / fps) == 27){
            break;
        } 
    }

    source.release();

    cv::destroyAllWindows();
}


void frame_process(const std::string& name, cv::dnn::Net& net)
{
    const std::string window_name = base_name(name);

    auto output_names = net.getUnconnectedOutLayersNames();
    
    std::vector<std::string> class_names = read_classes(CLASS_PATH);

    cv::namedWindow(window_name);

    cv::Mat frame = cv::imread(name);

    detect(net, frame, class_names, output_names);

    cv::imshow(window_name, frame);

    cv::waitKey(0) == 27;
    cv::destroyAllWindows();
}


void detect(cv::dnn::Net& net, cv::Mat frame, std::vector<std::string> class_names, auto output_names)
{ 
    cv::Mat blob;
    std::vector<cv::Mat> detections;

    cv::dnn::blobFromImage(frame, blob, 0.00392, cv::Size(WIDTH, HEIGHT), cv::Scalar(), true, false, CV_32F);
    net.setInput(blob);
    
    net.forward(detections, output_names);

    std::vector<int> indices[NUM_CLASSES];
    std::vector<cv::Rect> boxes[NUM_CLASSES];
    std::vector<float> scores[NUM_CLASSES];
    
    for (auto& output : detections)
    {
        const auto num_boxes = output.rows;
        for (int i = 0; i < num_boxes; i++)
        {
            auto x = output.at<float>(i, 0) * frame.cols;
            auto y = output.at<float>(i, 1) * frame.rows;
            auto width = output.at<float>(i, 2) * frame.cols;
            auto height = output.at<float>(i, 3) * frame.rows;
            cv::Rect rect(x - width/2, y - height/2, width, height);

            for (int c = 0; c < NUM_CLASSES; c++)
            {
                auto confidence = *output.ptr<float>(i, 5 + c);
                if (confidence >= CONFIDENCE_THRESHOLD)
                {
                    boxes[c].push_back(rect);
                    scores[c].push_back(confidence);
                }
            }
        }
    }
    
    for (int c = 0; c < NUM_CLASSES; c++)
        cv::dnn::NMSBoxes(boxes[c], scores[c], 0.0, NMS_THRESHOLD, indices[c]);
    
    for (int c= 0; c < NUM_CLASSES; c++)
    {   
        for (size_t i = 0; i < indices[c].size(); ++i)
        {
            auto idx = indices[c][i];

            cv::rectangle(frame, boxes[c][idx], cv::Scalar(0, 255, 0));
            cv::putText(frame, class_names[c], cv::Point(boxes[c][idx].x, boxes[c][idx].y), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 2);
            cv::putText(frame, std::to_string(scores[c][idx]).substr(0,4), cv::Point(boxes[c][idx].x+boxes[c][idx].width, boxes[c][idx].y), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 2);

        }
    }
}