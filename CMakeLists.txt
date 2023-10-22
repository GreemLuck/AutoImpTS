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

set(SOURCE_FILES
        main.cpp
        parser.cpp
        include/sqlite3.c

        Performance/Benchmark.cpp
        MathIO/MatrixReadWrite.cpp

        Algebra/Auxiliary.cpp

        Algorithms/TKCM.cpp
        Algorithms/ST_MVL.cpp
        Algorithms/SPIRIT.cpp
        Algorithms/GROUSE.cpp
        Algorithms/NMFMissingValueRecovery.cpp
        Algorithms/DynaMMo.cpp
        Algorithms/SVT.cpp
        Algorithms/ROSL.cpp
        Algorithms/IterativeSVD.cpp
        Algorithms/SoftImpute.cpp

        Algorithms/CDMissingValueRecovery.cpp
        Algebra/CentroidDecomposition.cpp
        Stats/Correlation.cpp
        Algebra/RSVD.cpp
        Database/SQL.cpp 
        Performance/MisalignRedux.cpp 
        Scenarios/MissingBlock.cpp 
        Scenarios/MissingBlock.cpp
        Scenarios/MCAR.cpp 
        Scenarios/Scenario.cpp
        Scenarios/ScenarioContext.cpp
)

set(HEADER_FILES
        parser.h
        include/sqlite3.h
        Algebra/MissingBlock.hpp
        Performance/Benchmark.h
        MathIO/MatrixReadWrite.h
        Algebra/Auxiliary.h
        Algorithms/TKCM.h
        Algorithms/ST_MVL.h
        Algorithms/SPIRIT.h
        Algorithms/GROUSE.h
        Algorithms/NMFMissingValueRecovery.h
        Algorithms/DynaMMo.h
        Algorithms/SVT.h
        Algorithms/ROSL.h
        Algorithms/IterativeSVD.h
        Algorithms/SoftImpute.h
        Algorithms/CDMissingValueRecovery.h
        Algebra/CentroidDecomposition.h
        Stats/Correlation.h
        Algebra/RSVD.h
        Database/SQL.h
        Performance/MisalignRedux.h
        Scenarios/MissingBlock.h
        Scenarios/Scenario.h
        Scenarios/MCAR.h
        Scenarios/ScenarioContext.h
)

add_executable(reduced_bench ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(
        reduced_bench PUBLIC 
        ${ARMADILLO_INCLUDE_DIR} 
        ${Python3_INCLUDE_DIRS} 
        ${MLPACK_INCLUDE_DIRS}
        include
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

