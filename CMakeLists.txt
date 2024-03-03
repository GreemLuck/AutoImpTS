cmake_minimum_required(VERSION 3.16)
project(reduced_bench)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fopenmp")

find_package(Armadillo REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development)
find_package(mlpack REQUIRED)

message("Python_FOUND:${Python_FOUND}")
message("Python_VERSION:${Python_VERSION}")
message("Python_Development_FOUND:${Python_Development_FOUND}")
message("Python_LIBRARIES:${Python_LIBRARIES}")
message("Python Include Dir:${Python3_INCLUDE_DIRS}")
message("Python Lib dir: ${Python3_LIBRARIES}")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*hpp")

add_executable(reduced_bench ${SOURCES} ${HEADERS})

target_include_directories(
        reduced_bench PUBLIC 
        ${ARMADILLO_INCLUDE_DIR} 
        ${Python3_INCLUDE_DIRS} 
        ${MLPACK_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(reduced_bench PRIVATE -pg) # Progiling
target_link_options(reduced_bench PRIVATE -pg) # Profiling

target_link_libraries(
        reduced_bench
        ${MLPACK_LIBRARIES} 
        ${ARMADILLO_LIBRARIES} 
        ${Python3_LIBRARIES} 
        -ldl
)

