#
cmake_minimum_required (VERSION 2.6)

##
# CMake macros for the project
include (config/project_config_embeddable.cmake)

##
# Project info
set_project_names (dsim DSim)
set_project_brief ("C++ Simulated Travel-Oriented Distribution System Library")
set_project_versions (99 99 99)

##
# Project options
# The parameters are:
# The parameters are:
#  * whether or not the tests must be compiled and run
#  * whether or not the documentation must be built and installed
#  * whether or not the code coverage must be performed
set_project_options (on on off)


#####################################
##            Packaging            ##
#####################################
#
packaging_init (${PROJECT_NAME})
packaging_set_summary ("${PROJECT_NAME} aims at providing a clean API and a simple implementation, as a C++ library, of a Travel-oriented Distribution System. It corresponds to the simulated version of the real-world Computerized Reservation Systems (CRS).
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
get_external_libs (git "python 2.6" "boost 1.41" readline mysql "soci 3.0"
  "stdair 0.45" "trademgen 0.1" "travelccm 0.5" "airsched 0.1" "sevmgr 0.1"
  "airrac 0.2" "rmol 0.25" "airinv 0.1" "simfqt 0.1" "simcrs 0.1" doxygen 
  "gcov 4.6.3" "lcov 1.9")


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
# For other projects to use DSim, install a few helpers for standard
# build/packaging systems: CMake, GNU Autotools (M4), pkgconfig/pc, 
# dsim-config
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


#######################################
##          DSim config file         ##
#######################################
module_config_add (config dsim)
