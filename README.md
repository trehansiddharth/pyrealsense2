pyrealsense2
============

`pyrealsense2` provides very basic bindings for the `librealsense2` library. This library exists because currently, Intel only provides Python bindings for version 1.12.1 of their library, leaving out support for the D415 and D435 cameras. Therefore, `pyrealsense2` provides a simple Python API for those cameras.

You will probably want to build with `-DCMAKE_INSTALL_PREFIX=/usr` to ensure Python bindings get installed in the right directory.

See [this gist](https://gist.github.com/trehansiddharth/06b384a5f39ab16eb1f80a724fb3e0b4) for an example of how to use these bindings.

OpenCV SWIG bindings, distributed alongside `pyrealsense2`, are developed by Renato Garcia and sourced from [this repository](https://github.com/renatoGarcia/opencv-swig) for convenience.
