# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/local/cmake/3.8.1/bin/cmake

# The command to remove a file.
RM = /usr/local/cmake/3.8.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /lore/clougj/Learning_Codes/trilinos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /lore/clougj/Learning_Codes/trilinos/build

# Include any dependencies generated for this target.
include CMakeFiles/tpetra1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tpetra1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tpetra1.dir/flags.make

CMakeFiles/tpetra1.dir/tpetra1.cpp.o: CMakeFiles/tpetra1.dir/flags.make
CMakeFiles/tpetra1.dir/tpetra1.cpp.o: ../tpetra1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/lore/clougj/Learning_Codes/trilinos/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tpetra1.dir/tpetra1.cpp.o"
	/usr/local/mpich3/3.1.2-thread-multiple/bin/mpicxx  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tpetra1.dir/tpetra1.cpp.o -c /lore/clougj/Learning_Codes/trilinos/tpetra1.cpp

CMakeFiles/tpetra1.dir/tpetra1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tpetra1.dir/tpetra1.cpp.i"
	/usr/local/mpich3/3.1.2-thread-multiple/bin/mpicxx $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /lore/clougj/Learning_Codes/trilinos/tpetra1.cpp > CMakeFiles/tpetra1.dir/tpetra1.cpp.i

CMakeFiles/tpetra1.dir/tpetra1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tpetra1.dir/tpetra1.cpp.s"
	/usr/local/mpich3/3.1.2-thread-multiple/bin/mpicxx $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /lore/clougj/Learning_Codes/trilinos/tpetra1.cpp -o CMakeFiles/tpetra1.dir/tpetra1.cpp.s

CMakeFiles/tpetra1.dir/tpetra1.cpp.o.requires:

.PHONY : CMakeFiles/tpetra1.dir/tpetra1.cpp.o.requires

CMakeFiles/tpetra1.dir/tpetra1.cpp.o.provides: CMakeFiles/tpetra1.dir/tpetra1.cpp.o.requires
	$(MAKE) -f CMakeFiles/tpetra1.dir/build.make CMakeFiles/tpetra1.dir/tpetra1.cpp.o.provides.build
.PHONY : CMakeFiles/tpetra1.dir/tpetra1.cpp.o.provides

CMakeFiles/tpetra1.dir/tpetra1.cpp.o.provides.build: CMakeFiles/tpetra1.dir/tpetra1.cpp.o


# Object files for target tpetra1
tpetra1_OBJECTS = \
"CMakeFiles/tpetra1.dir/tpetra1.cpp.o"

# External object files for target tpetra1
tpetra1_EXTERNAL_OBJECTS =

tpetra1: CMakeFiles/tpetra1.dir/tpetra1.cpp.o
tpetra1: CMakeFiles/tpetra1.dir/build.make
tpetra1: /usr/local/trilinos/latest/lib/libpiro.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/librol.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstokhos_muelu.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstokhos_ifpack2.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstokhos_amesos2.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstokhos_tpetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstokhos_sacado.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstokhos.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtempus.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/librythmos.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libmuelu-adapters.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libmuelu-interface.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libmuelu.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/liblocathyra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/liblocaepetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/liblocalapack.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libloca.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libnoxepetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libnoxlapack.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libnox.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libteko.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstratimikos.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstratimikosbelos.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstratimikosaztecoo.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstratimikosamesos.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstratimikosml.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstratimikosifpack.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libifpack2-adapters.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libifpack2.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libanasazitpetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libModeLaplace.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libanasaziepetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libanasazi.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libamesos2.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libbelostpetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libbelosepetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libbelos.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libml.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libifpack.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libzoltan2.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libamesos.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libgaleri-xpetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libgaleri-epetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libaztecoo.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libisorropia.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/liboptipack.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libxpetra-sup.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libxpetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libthyratpetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libthyraepetraext.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libthyraepetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libthyracore.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libepetraext.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtrilinosss.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtpetraext.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtpetrainout.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtpetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libkokkostsqr.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtpetraclassiclinalg.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtpetraclassicnodeapi.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtpetraclassic.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libtriutils.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libglobipack.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libepetra.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libminitensor.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/librtop.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libkokkoskernels.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libphalanx.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libpumi.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libma.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libapf_stk.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libapf_zoltan.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_io_util.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_io.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_mesh_base.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_topology.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_util_use_cases.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_util_registry.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_util_diag.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_util_env.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_util_parallel.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libstk_util_util.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libaprepro_lib.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libio_info_lib.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIonit.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIotr.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIohb.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIogn.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIovs.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIopg.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIoexo_fac.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIopx.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIofx.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIoex.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libIoss.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libpamgen_extras.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libpamgen.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libspr.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libapf_sim.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libgmi_sim.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libmapvarlib.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libsuplib_cpp.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libsuplib_c.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libsuplib.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libsupes.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libchaco.a
tpetra1: /usr/local/trilinos/latest/lib/libnemesis.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libexoIIv2for32.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libexodus_for.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libgtest.so.12.11
tpetra1: /usr/lib/libdl.so
tpetra1: /usr/local/trilinos/tpls/lib/libhdf5.so
tpetra1: /usr/lib/libz.so
tpetra1: /usr/local/trilinos/tpls/lib/libhdf5_hl.so
tpetra1: /usr/local/parmetis/4.0.3/mpich3.1.2/lib/libparmetis.so
tpetra1: /usr/local/parmetis/4.0.3/mpich3.1.2/lib/libmetis.so
tpetra1: /usr/lib/liblapack.so
tpetra1: /usr/lib/libblas.so
tpetra1: /usr/local/trilinos/tpls/lib/libyaml-cpp.so
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimField.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimAcis260.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/acisKrnl/libSpaACIS.so
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimParasolid280.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/psKrnl/libpskernel.so
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimModel.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimPartitionedMesh-mpi.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimMeshing.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimDiscrete.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimMeshTools.a
tpetra1: /net/common/meshSim/11.0-170427dev/lib/x64_rhel6_gcc44/libSimPartitionWrapper-mpich3.1.2.a
tpetra1: /usr/local/trilinos/tpls/lib/libboost_program_options.so
tpetra1: /usr/local/trilinos/tpls/lib/libboost_system.so
tpetra1: /usr/local/trilinos/tpls/lib/libnetcdf.so
tpetra1: /usr/local/trilinos/latest/lib/libintrepid2.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libintrepid.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libsacado.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libparma.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libmds.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libteuchoskokkoscomm.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libteuchoskokkoscompat.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libteuchosremainder.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libteuchosnumerics.so.12.11
tpetra1: /usr/lib/liblapack.so
tpetra1: /usr/local/trilinos/latest/lib/libteuchoscomm.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libteuchosparameterlist.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libteuchoscore.so.12.11
tpetra1: /usr/local/trilinos/tpls/lib/libyaml-cpp.so
tpetra1: /usr/local/trilinos/latest/lib/libshards.so.12.11
tpetra1: /usr/lib/libblas.so
tpetra1: /usr/local/trilinos/latest/lib/libzoltan.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libkokkosalgorithms.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libkokkoscontainers.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libkokkoscore.so.12.11
tpetra1: /usr/lib/libdl.so
tpetra1: /usr/local/parmetis/4.0.3/mpich3.1.2/lib/libparmetis.so
tpetra1: /usr/local/parmetis/4.0.3/mpich3.1.2/lib/libmetis.so
tpetra1: /usr/local/trilinos/latest/lib/libapf.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libgmi.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libpcu.so.12.11
tpetra1: /usr/local/trilinos/latest/lib/libexodus.so.12.11
tpetra1: /usr/local/trilinos/tpls/lib/libhdf5.so
tpetra1: /usr/lib/libz.so
tpetra1: /usr/local/trilinos/tpls/lib/libhdf5_hl.so
tpetra1: /usr/local/trilinos/tpls/lib/libnetcdf.so
tpetra1: CMakeFiles/tpetra1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/lore/clougj/Learning_Codes/trilinos/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tpetra1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tpetra1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tpetra1.dir/build: tpetra1

.PHONY : CMakeFiles/tpetra1.dir/build

CMakeFiles/tpetra1.dir/requires: CMakeFiles/tpetra1.dir/tpetra1.cpp.o.requires

.PHONY : CMakeFiles/tpetra1.dir/requires

CMakeFiles/tpetra1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tpetra1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tpetra1.dir/clean

CMakeFiles/tpetra1.dir/depend:
	cd /lore/clougj/Learning_Codes/trilinos/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /lore/clougj/Learning_Codes/trilinos /lore/clougj/Learning_Codes/trilinos /lore/clougj/Learning_Codes/trilinos/build /lore/clougj/Learning_Codes/trilinos/build /lore/clougj/Learning_Codes/trilinos/build/CMakeFiles/tpetra1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tpetra1.dir/depend

