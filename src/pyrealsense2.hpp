#ifndef __RUN_REALSENSE_H__
#define __RUN_REALSENSE_H__

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class FrameData {
public:
    Mat color;
    Mat depth;
    Mat infrared;
    Mat vertices;
    FrameData(Mat, Mat, Mat, Mat);
};

class RealsenseReader {
    rs2::pipeline pipe;
    rs2::frameset frames;
    rs2::pointcloud pc;
    rs2::points points;
    int height;
    int width;
    int framerate;
    bool _align_color;
public:
    RealsenseReader(int, int, int, bool align_color=true);
    FrameData get_frames();
};

#endif
