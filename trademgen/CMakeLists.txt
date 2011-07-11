##
# Convert the configuration headers (basically, just replace the @<variable>@
# variables).
configure_file (config.h.in config.h @ONLY)
set (PROJ_PATH_CFG_SRC config/${PACKAGE}-paths.hpp.in)
set (PROJ_PATH_CFG ${CMAKE_CURRENT_BINARY_DIR}/config/${PACKAGE}-paths.hpp)
configure_file (${PROJ_PATH_CFG_SRC} ${PROJ_PATH_CFG} @ONLY)

# Add the 'hdr_cfg' target, depending on the converted header
add_custom_target (hdr_cfg ALL DEPENDS ${PROJ_PATH_CFG})


##
# Library sources. All listed here for simplicity, but tailored
# compilations can be done in a given subdir if a CMakeList is
# provided there and add_subdirectory is used here (and the
# corresponding sources removed from here, of course).
# The libraries could then be assembled as CMake modules.
file (GLOB trademgenlib_root_HEADERS *.hpp)
file (GLOB trademgenlib_basic_HEADERS basic/*.hpp)
file (GLOB trademgenlib_bom_HEADERS bom/*.hpp)
file (GLOB trademgenlib_factory_HEADERS factory/*.hpp)
file (GLOB trademgenlib_dbadaptor_HEADERS dbadaptor/*.hpp)
file (GLOB trademgenlib_command_HEADERS command/*.hpp)
file (GLOB trademgenlib_service_HEADERS service/*.hpp)
set (trademgenlib_HEADERS
  ${trademgenlib_root_HEADERS} ${trademgenlib_basic_HEADERS}
  ${trademgenlib_bom_HEADERS} ${trademgenlib_factory_HEADERS}
  ${trademgenlib_dbadaptor_HEADERS} ${trademgenlib_command_HEADERS}
  ${trademgenlib_service_HEADERS} ${PROJ_PATH_CFG})
file (GLOB trademgenlib_SOURCES
  *.cpp basic/*.cpp bom/*.cpp factory/*.cpp
  dbadaptor/*.cpp command/*.cpp service/*.cpp)
set (trademgenlib_SOURCES ${trademgenlib_HEADERS} ${trademgenlib_SOURCES})


## Assembling
# Source files for the library
add_library (${TRADEMGEN_LIB_TARGET} SHARED ${trademgenlib_SOURCES})
# Dependencies on other libraries (Boost, MySQL, SOCI, StdAir)
target_link_libraries (${TRADEMGEN_LIB_TARGET} 
  ${BOOST_LIBS_FOR_LIB} ${MYSQL_LIBRARIES}
  ${SOCI_LIBRARIES} ${SOCIMYSQL_LIBRARIES}
  ${STDAIR_LIBRARIES})
#
add_dependencies (${TRADEMGEN_LIB_TARGET} hdr_cfg)
# Filename and SONAME
if (WIN32)
  set_target_properties (${TRADEMGEN_LIB_TARGET} PROPERTIES 
	OUTPUT_NAME ${TRADEMGEN_LIB_NAME} 
	VERSION ${GENERIC_LIB_VERSION}
	PUBLIC_HEADER "${trademgenlib_root_HEADERS}")
else()
  set_target_properties (${TRADEMGEN_LIB_TARGET} PROPERTIES 
	OUTPUT_NAME ${TRADEMGEN_LIB_NAME}
	VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
	PUBLIC_HEADER "${trademgenlib_root_HEADERS}")
endif()


## Installation
# Library
install (TARGETS ${TRADEMGEN_LIB_TARGET} 
  EXPORT TrademgenLibraryDepends
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/${PACKAGE}" COMPONENT dev)
