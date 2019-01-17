#
cmake_minimum_required (VERSION 3.0)

##
# CMake macros for the project
include (config/project_config_embeddable.cmake)

##
# Project info
set_project_names (tvlsim TvlSim)
set_project_brief ("C++ Simulated Travel-Oriented Distribution System Library")
set_project_versions (1 01 1)

##
# Project options
# The parameters are:
#  * whether or not the documentation must be built and installed
#  * whether or not the tests must be compiled and run
#  * whether or not the code coverage must be performed
set_project_options (on on off)


#####################################
##            Packaging            ##
#####################################
#
packaging_init (${PROJECT_NAME})
packaging_set_summary (
"${PROJECT_NAME} aims at providing a clean API and a simple implementation,
as a C++ library, of a Travel-oriented Distribution System.
It corresponds to the simulated version of the real-world 
Computerized Reservation Systems (CRS).
${PROJECT_NAME} makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the Boost
(C++ Standard Extensions: http://www.boost.org) library is used.")
packaging_set_contact ("Denis Arnaud <denis_arnaud - at - users dot sourceforge dot net>")
packaging_set_vendor ("Denis Arnaud")

#
packaging_set_other_options (TBZ2 "TBZ2;TGZ")


########################################
##            Dependencies            ##
########################################
#
get_external_libs (git "python 3.4" "boost 1.41" "mysql 5.0" "soci 3.0"
	"zeromq 2.0" readline curses "doxygen 1.4" "gcov 4.6.3" "lcov 1.9"
	"stdair 1.00.0" "airtsp 1.01.0" "airrac 1.00.0" "rmol 1.00.0"
	"sevmgr 1.00.0" "airinv 1.00.0" "simfqt 1.00.0"
	"trademgen 1.00.0" "travelccm 1.00.0" "simcrs 1.00.0")


##############################################
##           Build, Install, Export         ##
##############################################
##
# Initialise the building process
init_build ()

##
# Sub-modules
add_modules (${PROJECT_NAME})

##
# For other projects to use TvlSim, install a few helpers for standard
# build/packaging systems: CMake, GNU Autotools (M4), pkgconfig/pc, 
# tvlsim-config
install_dev_helper_files ()

##
# Basic documentation (i.e., AUTHORS, NEWS, README, INSTALL)
install_basic_documentation ()


##########################################
##            Documentation             ##
##########################################
#
handle_html_doc ()

##
# Subdirs
add_subdirectory (man)


#####################################
##              Tests              ##
#####################################
##
# Initialise the unit test process
init_test ()

##
# Test suites
add_test_suite (${PROJECT_NAME})


#######################################
##             GCOV                 ##
#######################################
gcov_task()


#######################################
##          Overall Status           ##
#######################################
display_status ()


#######################################
##         CMake Cache Storage       ##
#######################################
store_in_cache ()

