cmake_minimum_required(VERSION 3.16)
project(reduced_bench)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fopenmp")

find_package(Armadillo REQUIRED)
find_package(PythonLibs REQUIRED)
add_executable(
        reduced_bench

        main.cpp
        parser.h parser.cpp
        include/sqlite3.h include/sqlite3.c
        Testing.h Testing.cpp
        Algebra/MissingBlock.hpp

        Performance/Benchmark.cpp Performance/Benchmark.h
        MathIO/MatrixReadWrite.cpp MathIO/MatrixReadWrite.h

        Algebra/Auxiliary.cpp Algebra/Auxiliary.h

        Algorithms/TKCM.cpp Algorithms/TKCM.h
        Algorithms/ST_MVL.cpp Algorithms/ST_MVL.h
        Algorithms/SPIRIT.cpp Algorithms/SPIRIT.h
        Algorithms/GROUSE.cpp Algorithms/GROUSE.h
        Algorithms/NMFMissingValueRecovery.cpp Algorithms/NMFMissingValueRecovery.h
        Algorithms/DynaMMo.cpp Algorithms/DynaMMo.h
        Algorithms/SVT.cpp Algorithms/SVT.h
        Algorithms/ROSL.cpp Algorithms/ROSL.h
        Algorithms/IterativeSVD.cpp Algorithms/IterativeSVD.h
        Algorithms/SoftImpute.cpp Algorithms/SoftImpute.h

        Algorithms/CDMissingValueRecovery.cpp Algorithms/CDMissingValueRecovery.h
        Algebra/CentroidDecomposition.cpp Algebra/CentroidDecomposition.h
        Algebra/MissingBlock.hpp
        Stats/Correlation.cpp Stats/Correlation.h
        Algebra/RSVD.cpp Algebra/RSVD.h
)
target_include_directories(reduced_bench PUBLIC ${ARMADILLO_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} include)
target_link_libraries(reduced_bench ${ARMADILLO_LIBRARIES} ${PYTHON_LIBRARIES} -lmlpack -ldl)

