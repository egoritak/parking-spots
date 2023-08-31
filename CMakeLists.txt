cmake_minimum_required(VERSION 3.10)

# Название проекта
project(CameraProject)

# Устанавливаем стандарт C++ (например, C++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем CMake где искать модули OpenCV
find_package(OpenCV REQUIRED)

# Указываем директории для заголовочных файлов
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(includes)
include(clang_tidy.cmake)

# Указываем директорию для исходных файлов
file(GLOB SOURCES "src/*.cpp")

# Создаем исполняемый файл с именем, например, "cameraApp"
add_executable(cameraApp ${SOURCES}
        src/main.cpp)

# Связываем библиотеки OpenCV с нашим проектом
target_link_libraries(cameraApp ${OpenCV_LIBS})
