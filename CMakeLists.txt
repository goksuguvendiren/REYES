cmake_minimum_required(VERSION 3.10)
project(REYES)

set(CMAKE_CXX_STANDARD 17)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(/usr/local/include/)

set(SOURCE_FILES BasicScene.cpp Ri.cpp Reyes.cpp Sphere.cpp Sphere.hpp glm_ostream.hpp Mesh.cpp Mesh.hpp shaders.hpp shaders.cpp Texture.cpp Texture.hpp Cone.hpp)
add_executable(REYES ${SOURCE_FILES})
#target_link_libraries(REYES X11)
#target_link_libraries(REYES Threads::Threads)
target_link_libraries(REYES ${OpenCV_LIBRARIES})
