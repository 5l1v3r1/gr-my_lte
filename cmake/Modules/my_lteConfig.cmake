INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_MY_LTE my_lte)

FIND_PATH(
    MY_LTE_INCLUDE_DIRS
    NAMES my_lte/api.h
    HINTS $ENV{MY_LTE_DIR}/include
        ${PC_MY_LTE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    MY_LTE_LIBRARIES
    NAMES gnuradio-my_lte
    HINTS $ENV{MY_LTE_DIR}/lib
        ${PC_MY_LTE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MY_LTE DEFAULT_MSG MY_LTE_LIBRARIES MY_LTE_INCLUDE_DIRS)
MARK_AS_ADVANCED(MY_LTE_LIBRARIES MY_LTE_INCLUDE_DIRS)

