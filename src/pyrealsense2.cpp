#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include "pyrealsense2.hpp"

using namespace std;
using namespace cv;

FrameData::FrameData(Mat _color, Mat _depth, Mat _infrared, Mat _vertices) {
    color = _color;
    depth = _depth;
    infrared = _infrared;
    vertices = _vertices;
}

RealsenseReader::RealsenseReader(int _height, int _width, int _framerate, bool align_color) {
    height = _height;
    width = _width;
    framerate = _framerate;
    _align_color = align_color;

    pipe = rs2::pipeline();

    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_BGR8, framerate);
    cfg.enable_stream(RS2_STREAM_INFRARED, width, height, RS2_FORMAT_Y8, framerate);
    cfg.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, framerate);

    rs2::pipeline_profile pipe_profile = cfg.resolve(pipe);
    rs2::device device = pipe_profile.get_device();
    vector<rs2::sensor> sensors = device.query_sensors();
    rs2_error *err = 0;

    for (rs2::sensor element : sensors) {
        std::string module_name = element.get_info(RS2_CAMERA_INFO_NAME);
        if ("Stereo Module" == module_name) {
            constexpr size_t kEmitterEnabled = 1u;
            element.set_option(rs2_option::RS2_OPTION_EMITTER_ENABLED, 0.f);
        }
    }

    pipe.start(cfg);

    pc = rs2::pointcloud();
}

FrameData RealsenseReader::get_frames() {
    frames = pipe.wait_for_frames();
    rs2::frame color_frame = frames.get_color_frame();
    rs2::frame depth_frame;
    rs2::frame infrared_frame = frames.get_infrared_frame();
    if (_align_color) {
        rs2::align align(RS2_STREAM_COLOR);
        rs2::frameset aligned_frames = align.process(frames);
        depth_frame = aligned_frames.get_depth_frame();
    } else {
        depth_frame = frames.get_depth_frame();
    }
    Mat color(Size(width, height), CV_8UC3, (void*) color_frame.get_data(), Mat::AUTO_STEP);
    Mat depth(Size(width, height), CV_16U, (void*) depth_frame.get_data(), Mat::AUTO_STEP);
    Mat infrared(Size(width, height), CV_8U, (void*) infrared_frame.get_data(), Mat::AUTO_STEP);
    points = pc.calculate(depth_frame);
    Mat vertices(Size(3, points.size()), CV_32F, (void*) points.get_vertices(), Mat::AUTO_STEP);
    return FrameData(color, depth, infrared, vertices);
}

