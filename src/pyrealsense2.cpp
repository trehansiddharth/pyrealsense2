#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include "pyrealsense2.hpp"

using namespace std;
using namespace cv;

FrameData::FrameData(Mat _color, Mat _depth, Mat _vertices) {
    color = _color;
    depth = _depth;
    vertices = _vertices;
}

RealsenseReader::RealsenseReader(int _height, int _width, int _framerate) {
    height = _height;
    width = _width;
    framerate = _framerate;

    pipe = rs2::pipeline();

    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_BGR8, framerate);
    cfg.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, framerate);
    pipe.start(cfg);

    pc = rs2::pointcloud();
}

FrameData RealsenseReader::get_frames() {
    frames = pipe.wait_for_frames();
    rs2::frame color_frame = frames.get_color_frame();
    rs2::frame depth_frame = frames.get_depth_frame();
    Mat color(Size(width, height), CV_8UC3, (void*) color_frame.get_data(), Mat::AUTO_STEP);
    Mat depth(Size(width, height), CV_16U, (void*) depth_frame.get_data(), Mat::AUTO_STEP);
    points = pc.calculate(depth_frame);
    Mat vertices(Size(3, points.size()), CV_32F, (void*) points.get_vertices(), Mat::AUTO_STEP);
    return FrameData(color, depth, vertices);
}
