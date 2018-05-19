CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(pyrealsense2)

SET(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(SWIG REQUIRED)
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(realsense2 REQUIRED)

INCLUDE(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES("/home/nvidia/opencv-swig/lib"
    "/home/nvidia/vision_ws/install_isolated/include"
    ${OpenCV_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src
)

ADD_LIBRARY(pyrealsense2 SHARED src/pyrealsense2.cpp)
TARGET_LINK_LIBRARIES(pyrealsense2
    ${OpenCV_LIBRARIES}
    ${PYTHON_LIBRARIES}
    realsense2
)

SET_PROPERTY(SOURCE src/pyrealsense2.i PROPERTY CPLUSPLUS ON)
SWIG_ADD_MODULE(realsense2 python src/pyrealsense2.i)
SWIG_LINK_LIBRARIES(realsense2 pyrealsense2 ${PYTHON_LIBRARIES})

EXECUTE_PROCESS(COMMAND python3 -c "if True:
    from distutils import sysconfig as sc
    print(sc.get_python_lib(prefix='', plat_specific=True))"
    OUTPUT_VARIABLE PYTHON_SITE
    OUTPUT_STRIP_TRAILING_WHITESPACE)

INSTALL(TARGETS pyrealsense2
    DESTINATION ${PYTHON_SITE})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/_realsense2.so
    ${CMAKE_CURRENT_BINARY_DIR}/realsense2.py
    DESTINATION ${PYTHON_SITE})
