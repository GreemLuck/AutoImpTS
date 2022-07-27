# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/dev/reduced-bench

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/dev/reduced-bench

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/dev/reduced-bench/CMakeFiles /mnt/c/dev/reduced-bench/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/dev/reduced-bench/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named reduced_bench

# Build rule for target.
reduced_bench: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 reduced_bench
.PHONY : reduced_bench

# fast build rule for target.
reduced_bench/fast:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/build
.PHONY : reduced_bench/fast

Algebra/Auxiliary.o: Algebra/Auxiliary.cpp.o

.PHONY : Algebra/Auxiliary.o

# target to build an object file
Algebra/Auxiliary.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/Auxiliary.cpp.o
.PHONY : Algebra/Auxiliary.cpp.o

Algebra/Auxiliary.i: Algebra/Auxiliary.cpp.i

.PHONY : Algebra/Auxiliary.i

# target to preprocess a source file
Algebra/Auxiliary.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/Auxiliary.cpp.i
.PHONY : Algebra/Auxiliary.cpp.i

Algebra/Auxiliary.s: Algebra/Auxiliary.cpp.s

.PHONY : Algebra/Auxiliary.s

# target to generate assembly for a file
Algebra/Auxiliary.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/Auxiliary.cpp.s
.PHONY : Algebra/Auxiliary.cpp.s

Algebra/CentroidDecomposition.o: Algebra/CentroidDecomposition.cpp.o

.PHONY : Algebra/CentroidDecomposition.o

# target to build an object file
Algebra/CentroidDecomposition.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/CentroidDecomposition.cpp.o
.PHONY : Algebra/CentroidDecomposition.cpp.o

Algebra/CentroidDecomposition.i: Algebra/CentroidDecomposition.cpp.i

.PHONY : Algebra/CentroidDecomposition.i

# target to preprocess a source file
Algebra/CentroidDecomposition.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/CentroidDecomposition.cpp.i
.PHONY : Algebra/CentroidDecomposition.cpp.i

Algebra/CentroidDecomposition.s: Algebra/CentroidDecomposition.cpp.s

.PHONY : Algebra/CentroidDecomposition.s

# target to generate assembly for a file
Algebra/CentroidDecomposition.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/CentroidDecomposition.cpp.s
.PHONY : Algebra/CentroidDecomposition.cpp.s

Algebra/RSVD.o: Algebra/RSVD.cpp.o

.PHONY : Algebra/RSVD.o

# target to build an object file
Algebra/RSVD.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/RSVD.cpp.o
.PHONY : Algebra/RSVD.cpp.o

Algebra/RSVD.i: Algebra/RSVD.cpp.i

.PHONY : Algebra/RSVD.i

# target to preprocess a source file
Algebra/RSVD.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/RSVD.cpp.i
.PHONY : Algebra/RSVD.cpp.i

Algebra/RSVD.s: Algebra/RSVD.cpp.s

.PHONY : Algebra/RSVD.s

# target to generate assembly for a file
Algebra/RSVD.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algebra/RSVD.cpp.s
.PHONY : Algebra/RSVD.cpp.s

Algorithms/CDMissingValueRecovery.o: Algorithms/CDMissingValueRecovery.cpp.o

.PHONY : Algorithms/CDMissingValueRecovery.o

# target to build an object file
Algorithms/CDMissingValueRecovery.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/CDMissingValueRecovery.cpp.o
.PHONY : Algorithms/CDMissingValueRecovery.cpp.o

Algorithms/CDMissingValueRecovery.i: Algorithms/CDMissingValueRecovery.cpp.i

.PHONY : Algorithms/CDMissingValueRecovery.i

# target to preprocess a source file
Algorithms/CDMissingValueRecovery.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/CDMissingValueRecovery.cpp.i
.PHONY : Algorithms/CDMissingValueRecovery.cpp.i

Algorithms/CDMissingValueRecovery.s: Algorithms/CDMissingValueRecovery.cpp.s

.PHONY : Algorithms/CDMissingValueRecovery.s

# target to generate assembly for a file
Algorithms/CDMissingValueRecovery.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/CDMissingValueRecovery.cpp.s
.PHONY : Algorithms/CDMissingValueRecovery.cpp.s

Algorithms/DynaMMo.o: Algorithms/DynaMMo.cpp.o

.PHONY : Algorithms/DynaMMo.o

# target to build an object file
Algorithms/DynaMMo.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/DynaMMo.cpp.o
.PHONY : Algorithms/DynaMMo.cpp.o

Algorithms/DynaMMo.i: Algorithms/DynaMMo.cpp.i

.PHONY : Algorithms/DynaMMo.i

# target to preprocess a source file
Algorithms/DynaMMo.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/DynaMMo.cpp.i
.PHONY : Algorithms/DynaMMo.cpp.i

Algorithms/DynaMMo.s: Algorithms/DynaMMo.cpp.s

.PHONY : Algorithms/DynaMMo.s

# target to generate assembly for a file
Algorithms/DynaMMo.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/DynaMMo.cpp.s
.PHONY : Algorithms/DynaMMo.cpp.s

Algorithms/GROUSE.o: Algorithms/GROUSE.cpp.o

.PHONY : Algorithms/GROUSE.o

# target to build an object file
Algorithms/GROUSE.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/GROUSE.cpp.o
.PHONY : Algorithms/GROUSE.cpp.o

Algorithms/GROUSE.i: Algorithms/GROUSE.cpp.i

.PHONY : Algorithms/GROUSE.i

# target to preprocess a source file
Algorithms/GROUSE.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/GROUSE.cpp.i
.PHONY : Algorithms/GROUSE.cpp.i

Algorithms/GROUSE.s: Algorithms/GROUSE.cpp.s

.PHONY : Algorithms/GROUSE.s

# target to generate assembly for a file
Algorithms/GROUSE.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/GROUSE.cpp.s
.PHONY : Algorithms/GROUSE.cpp.s

Algorithms/IterativeSVD.o: Algorithms/IterativeSVD.cpp.o

.PHONY : Algorithms/IterativeSVD.o

# target to build an object file
Algorithms/IterativeSVD.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/IterativeSVD.cpp.o
.PHONY : Algorithms/IterativeSVD.cpp.o

Algorithms/IterativeSVD.i: Algorithms/IterativeSVD.cpp.i

.PHONY : Algorithms/IterativeSVD.i

# target to preprocess a source file
Algorithms/IterativeSVD.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/IterativeSVD.cpp.i
.PHONY : Algorithms/IterativeSVD.cpp.i

Algorithms/IterativeSVD.s: Algorithms/IterativeSVD.cpp.s

.PHONY : Algorithms/IterativeSVD.s

# target to generate assembly for a file
Algorithms/IterativeSVD.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/IterativeSVD.cpp.s
.PHONY : Algorithms/IterativeSVD.cpp.s

Algorithms/NMFMissingValueRecovery.o: Algorithms/NMFMissingValueRecovery.cpp.o

.PHONY : Algorithms/NMFMissingValueRecovery.o

# target to build an object file
Algorithms/NMFMissingValueRecovery.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/NMFMissingValueRecovery.cpp.o
.PHONY : Algorithms/NMFMissingValueRecovery.cpp.o

Algorithms/NMFMissingValueRecovery.i: Algorithms/NMFMissingValueRecovery.cpp.i

.PHONY : Algorithms/NMFMissingValueRecovery.i

# target to preprocess a source file
Algorithms/NMFMissingValueRecovery.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/NMFMissingValueRecovery.cpp.i
.PHONY : Algorithms/NMFMissingValueRecovery.cpp.i

Algorithms/NMFMissingValueRecovery.s: Algorithms/NMFMissingValueRecovery.cpp.s

.PHONY : Algorithms/NMFMissingValueRecovery.s

# target to generate assembly for a file
Algorithms/NMFMissingValueRecovery.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/NMFMissingValueRecovery.cpp.s
.PHONY : Algorithms/NMFMissingValueRecovery.cpp.s

Algorithms/ROSL.o: Algorithms/ROSL.cpp.o

.PHONY : Algorithms/ROSL.o

# target to build an object file
Algorithms/ROSL.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/ROSL.cpp.o
.PHONY : Algorithms/ROSL.cpp.o

Algorithms/ROSL.i: Algorithms/ROSL.cpp.i

.PHONY : Algorithms/ROSL.i

# target to preprocess a source file
Algorithms/ROSL.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/ROSL.cpp.i
.PHONY : Algorithms/ROSL.cpp.i

Algorithms/ROSL.s: Algorithms/ROSL.cpp.s

.PHONY : Algorithms/ROSL.s

# target to generate assembly for a file
Algorithms/ROSL.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/ROSL.cpp.s
.PHONY : Algorithms/ROSL.cpp.s

Algorithms/SPIRIT.o: Algorithms/SPIRIT.cpp.o

.PHONY : Algorithms/SPIRIT.o

# target to build an object file
Algorithms/SPIRIT.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SPIRIT.cpp.o
.PHONY : Algorithms/SPIRIT.cpp.o

Algorithms/SPIRIT.i: Algorithms/SPIRIT.cpp.i

.PHONY : Algorithms/SPIRIT.i

# target to preprocess a source file
Algorithms/SPIRIT.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SPIRIT.cpp.i
.PHONY : Algorithms/SPIRIT.cpp.i

Algorithms/SPIRIT.s: Algorithms/SPIRIT.cpp.s

.PHONY : Algorithms/SPIRIT.s

# target to generate assembly for a file
Algorithms/SPIRIT.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SPIRIT.cpp.s
.PHONY : Algorithms/SPIRIT.cpp.s

Algorithms/ST_MVL.o: Algorithms/ST_MVL.cpp.o

.PHONY : Algorithms/ST_MVL.o

# target to build an object file
Algorithms/ST_MVL.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/ST_MVL.cpp.o
.PHONY : Algorithms/ST_MVL.cpp.o

Algorithms/ST_MVL.i: Algorithms/ST_MVL.cpp.i

.PHONY : Algorithms/ST_MVL.i

# target to preprocess a source file
Algorithms/ST_MVL.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/ST_MVL.cpp.i
.PHONY : Algorithms/ST_MVL.cpp.i

Algorithms/ST_MVL.s: Algorithms/ST_MVL.cpp.s

.PHONY : Algorithms/ST_MVL.s

# target to generate assembly for a file
Algorithms/ST_MVL.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/ST_MVL.cpp.s
.PHONY : Algorithms/ST_MVL.cpp.s

Algorithms/SVT.o: Algorithms/SVT.cpp.o

.PHONY : Algorithms/SVT.o

# target to build an object file
Algorithms/SVT.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SVT.cpp.o
.PHONY : Algorithms/SVT.cpp.o

Algorithms/SVT.i: Algorithms/SVT.cpp.i

.PHONY : Algorithms/SVT.i

# target to preprocess a source file
Algorithms/SVT.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SVT.cpp.i
.PHONY : Algorithms/SVT.cpp.i

Algorithms/SVT.s: Algorithms/SVT.cpp.s

.PHONY : Algorithms/SVT.s

# target to generate assembly for a file
Algorithms/SVT.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SVT.cpp.s
.PHONY : Algorithms/SVT.cpp.s

Algorithms/SoftImpute.o: Algorithms/SoftImpute.cpp.o

.PHONY : Algorithms/SoftImpute.o

# target to build an object file
Algorithms/SoftImpute.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SoftImpute.cpp.o
.PHONY : Algorithms/SoftImpute.cpp.o

Algorithms/SoftImpute.i: Algorithms/SoftImpute.cpp.i

.PHONY : Algorithms/SoftImpute.i

# target to preprocess a source file
Algorithms/SoftImpute.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SoftImpute.cpp.i
.PHONY : Algorithms/SoftImpute.cpp.i

Algorithms/SoftImpute.s: Algorithms/SoftImpute.cpp.s

.PHONY : Algorithms/SoftImpute.s

# target to generate assembly for a file
Algorithms/SoftImpute.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/SoftImpute.cpp.s
.PHONY : Algorithms/SoftImpute.cpp.s

Algorithms/TKCM.o: Algorithms/TKCM.cpp.o

.PHONY : Algorithms/TKCM.o

# target to build an object file
Algorithms/TKCM.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/TKCM.cpp.o
.PHONY : Algorithms/TKCM.cpp.o

Algorithms/TKCM.i: Algorithms/TKCM.cpp.i

.PHONY : Algorithms/TKCM.i

# target to preprocess a source file
Algorithms/TKCM.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/TKCM.cpp.i
.PHONY : Algorithms/TKCM.cpp.i

Algorithms/TKCM.s: Algorithms/TKCM.cpp.s

.PHONY : Algorithms/TKCM.s

# target to generate assembly for a file
Algorithms/TKCM.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Algorithms/TKCM.cpp.s
.PHONY : Algorithms/TKCM.cpp.s

MathIO/MatrixReadWrite.o: MathIO/MatrixReadWrite.cpp.o

.PHONY : MathIO/MatrixReadWrite.o

# target to build an object file
MathIO/MatrixReadWrite.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/MathIO/MatrixReadWrite.cpp.o
.PHONY : MathIO/MatrixReadWrite.cpp.o

MathIO/MatrixReadWrite.i: MathIO/MatrixReadWrite.cpp.i

.PHONY : MathIO/MatrixReadWrite.i

# target to preprocess a source file
MathIO/MatrixReadWrite.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/MathIO/MatrixReadWrite.cpp.i
.PHONY : MathIO/MatrixReadWrite.cpp.i

MathIO/MatrixReadWrite.s: MathIO/MatrixReadWrite.cpp.s

.PHONY : MathIO/MatrixReadWrite.s

# target to generate assembly for a file
MathIO/MatrixReadWrite.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/MathIO/MatrixReadWrite.cpp.s
.PHONY : MathIO/MatrixReadWrite.cpp.s

Performance/Benchmark.o: Performance/Benchmark.cpp.o

.PHONY : Performance/Benchmark.o

# target to build an object file
Performance/Benchmark.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Performance/Benchmark.cpp.o
.PHONY : Performance/Benchmark.cpp.o

Performance/Benchmark.i: Performance/Benchmark.cpp.i

.PHONY : Performance/Benchmark.i

# target to preprocess a source file
Performance/Benchmark.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Performance/Benchmark.cpp.i
.PHONY : Performance/Benchmark.cpp.i

Performance/Benchmark.s: Performance/Benchmark.cpp.s

.PHONY : Performance/Benchmark.s

# target to generate assembly for a file
Performance/Benchmark.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Performance/Benchmark.cpp.s
.PHONY : Performance/Benchmark.cpp.s

Stats/Correlation.o: Stats/Correlation.cpp.o

.PHONY : Stats/Correlation.o

# target to build an object file
Stats/Correlation.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Stats/Correlation.cpp.o
.PHONY : Stats/Correlation.cpp.o

Stats/Correlation.i: Stats/Correlation.cpp.i

.PHONY : Stats/Correlation.i

# target to preprocess a source file
Stats/Correlation.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Stats/Correlation.cpp.i
.PHONY : Stats/Correlation.cpp.i

Stats/Correlation.s: Stats/Correlation.cpp.s

.PHONY : Stats/Correlation.s

# target to generate assembly for a file
Stats/Correlation.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Stats/Correlation.cpp.s
.PHONY : Stats/Correlation.cpp.s

Testing.o: Testing.cpp.o

.PHONY : Testing.o

# target to build an object file
Testing.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Testing.cpp.o
.PHONY : Testing.cpp.o

Testing.i: Testing.cpp.i

.PHONY : Testing.i

# target to preprocess a source file
Testing.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Testing.cpp.i
.PHONY : Testing.cpp.i

Testing.s: Testing.cpp.s

.PHONY : Testing.s

# target to generate assembly for a file
Testing.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/Testing.cpp.s
.PHONY : Testing.cpp.s

include/sqlite3.o: include/sqlite3.c.o

.PHONY : include/sqlite3.o

# target to build an object file
include/sqlite3.c.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/include/sqlite3.c.o
.PHONY : include/sqlite3.c.o

include/sqlite3.i: include/sqlite3.c.i

.PHONY : include/sqlite3.i

# target to preprocess a source file
include/sqlite3.c.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/include/sqlite3.c.i
.PHONY : include/sqlite3.c.i

include/sqlite3.s: include/sqlite3.c.s

.PHONY : include/sqlite3.s

# target to generate assembly for a file
include/sqlite3.c.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/include/sqlite3.c.s
.PHONY : include/sqlite3.c.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/main.cpp.s
.PHONY : main.cpp.s

parser.o: parser.cpp.o

.PHONY : parser.o

# target to build an object file
parser.cpp.o:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/parser.cpp.o
.PHONY : parser.cpp.o

parser.i: parser.cpp.i

.PHONY : parser.i

# target to preprocess a source file
parser.cpp.i:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/parser.cpp.i
.PHONY : parser.cpp.i

parser.s: parser.cpp.s

.PHONY : parser.s

# target to generate assembly for a file
parser.cpp.s:
	$(MAKE) -f CMakeFiles/reduced_bench.dir/build.make CMakeFiles/reduced_bench.dir/parser.cpp.s
.PHONY : parser.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... reduced_bench"
	@echo "... Algebra/Auxiliary.o"
	@echo "... Algebra/Auxiliary.i"
	@echo "... Algebra/Auxiliary.s"
	@echo "... Algebra/CentroidDecomposition.o"
	@echo "... Algebra/CentroidDecomposition.i"
	@echo "... Algebra/CentroidDecomposition.s"
	@echo "... Algebra/RSVD.o"
	@echo "... Algebra/RSVD.i"
	@echo "... Algebra/RSVD.s"
	@echo "... Algorithms/CDMissingValueRecovery.o"
	@echo "... Algorithms/CDMissingValueRecovery.i"
	@echo "... Algorithms/CDMissingValueRecovery.s"
	@echo "... Algorithms/DynaMMo.o"
	@echo "... Algorithms/DynaMMo.i"
	@echo "... Algorithms/DynaMMo.s"
	@echo "... Algorithms/GROUSE.o"
	@echo "... Algorithms/GROUSE.i"
	@echo "... Algorithms/GROUSE.s"
	@echo "... Algorithms/IterativeSVD.o"
	@echo "... Algorithms/IterativeSVD.i"
	@echo "... Algorithms/IterativeSVD.s"
	@echo "... Algorithms/NMFMissingValueRecovery.o"
	@echo "... Algorithms/NMFMissingValueRecovery.i"
	@echo "... Algorithms/NMFMissingValueRecovery.s"
	@echo "... Algorithms/ROSL.o"
	@echo "... Algorithms/ROSL.i"
	@echo "... Algorithms/ROSL.s"
	@echo "... Algorithms/SPIRIT.o"
	@echo "... Algorithms/SPIRIT.i"
	@echo "... Algorithms/SPIRIT.s"
	@echo "... Algorithms/ST_MVL.o"
	@echo "... Algorithms/ST_MVL.i"
	@echo "... Algorithms/ST_MVL.s"
	@echo "... Algorithms/SVT.o"
	@echo "... Algorithms/SVT.i"
	@echo "... Algorithms/SVT.s"
	@echo "... Algorithms/SoftImpute.o"
	@echo "... Algorithms/SoftImpute.i"
	@echo "... Algorithms/SoftImpute.s"
	@echo "... Algorithms/TKCM.o"
	@echo "... Algorithms/TKCM.i"
	@echo "... Algorithms/TKCM.s"
	@echo "... MathIO/MatrixReadWrite.o"
	@echo "... MathIO/MatrixReadWrite.i"
	@echo "... MathIO/MatrixReadWrite.s"
	@echo "... Performance/Benchmark.o"
	@echo "... Performance/Benchmark.i"
	@echo "... Performance/Benchmark.s"
	@echo "... Stats/Correlation.o"
	@echo "... Stats/Correlation.i"
	@echo "... Stats/Correlation.s"
	@echo "... Testing.o"
	@echo "... Testing.i"
	@echo "... Testing.s"
	@echo "... include/sqlite3.o"
	@echo "... include/sqlite3.i"
	@echo "... include/sqlite3.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... parser.o"
	@echo "... parser.i"
	@echo "... parser.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

