
file(GLOB HTTPLIB_SEARCH_PATHS
  ${CMAKE_BINARY_DIR}/deps/cpp-httplib-[0-9]*.[0-9]*.[0-9]*
  /usr/include/
  /usr/local/include/
)

find_path(HTTPLIB_INCLUDE_DIR
  NAMES httplib.h
  PATHS ${HTTPLIB_SEARCH_PATHS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Httplib
    REQUIRED_VARS HTTPLIB_INCLUDE_DIR)

mark_as_advanced(HTTPLIB_INCLUDE_DIR)

