# - Try to find Maguey
# Once done this will define
#  MAGUEY_FOUND - System has Maguey
#  MAGUEY_INCLUDE_DIRS - The Maguey include directories
#  MAGUEY_LIBRARIES - The libraries needed to use Maguey
#  MAGUEY_DEFINITIONS - Compiler switches required for using Maguey

find_package(PkgConfig)
pkg_check_modules(PC_MAGUEY QUIET maguey-1.0.0)
set(MAGUEY_DEFINITIONS ${PC_MAGUEY_CFLAGS_OTHER})

find_path(MAGUEY_INCLUDE_DIR maguey/maguey.h
          HINTS ${PC_MAGUEY_INCLUDEDIR} ${PC_MAGUEY_INCLUDE_DIRS}
          PATH_SUFFIXES maguey )

find_library(MAGUEY_LIBRARY NAMES maguey
             HINTS ${PC_MAGUEY_LIBDIR} ${PC_MAGUEY_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set MAGUEY_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(maguey  DEFAULT_MSG
                                  MAGUEY_LIBRARY MAGUEY_INCLUDE_DIR)

mark_as_advanced(MAGUEY_INCLUDE_DIR MAGUEY_LIBRARY )

set(MAGUEY_LIBRARIES ${MAGUEY_LIBRARY} )
set(MAGUEY_INCLUDE_DIRS ${MAGUEY_INCLUDE_DIR} )