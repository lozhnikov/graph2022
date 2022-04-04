
file(GLOB NLOHMANN_JSON_SEARCH_PATHS
  ${CMAKE_BINARY_DIR}/deps/nlohmann-json-[0-9]*.[0-9]*.[0-9]*
  /usr/include/
  /usr/local/include/
)

find_path(NLOHMANN_JSON_INCLUDE_DIR
  NAMES nlohmann/json.hpp
  PATHS ${NLOHMANN_JSON_SEARCH_PATHS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(NlohmannJson
    REQUIRED_VARS NLOHMANN_JSON_INCLUDE_DIR)

mark_as_advanced(NLOHMANN_JSON_INCLUDE_DIR)

