%module realsense2

%include <opencv.i>
%cv_instantiate_all_defaults

%{
    #include "pyrealsense2.hpp"
%}

%include "pyrealsense2.hpp"
