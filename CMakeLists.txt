cmake_minimum_required(VERSION 3.16)
project(reduced_bench)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fopenmp")

find_package(Armadillo REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development)

message("Python_FOUND:${Python_FOUND}")
message("Python_VERSION:${Python_VERSION}")
message("Python_Development_FOUND:${Python_Development_FOUND}")
message("Python_LIBRARIES:${Python_LIBRARIES}")
message("Python Include Dir:${Python3_INCLUDE_DIRS}")
message("Python Lib dir: ${Python3_LIBRARIES}")
add_executable(
        reduced_bench

        main.cpp
        parser.h parser.cpp
        include/sqlite3.h include/sqlite3.c
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
        AutoParam/SuccessiveHalving.cpp AutoParam/SuccessiveHalving.h
        Database/SQL.cpp Database/SQL.h)
target_include_directories(reduced_bench PUBLIC ${ARMADILLO_INCLUDE_DIR} ${Python3_INCLUDE_DIRS} include)
target_link_libraries(reduced_bench ${ARMADILLO_LIBRARIES} ${Python3_LIBRARIES} -lmlpack -ldl)

