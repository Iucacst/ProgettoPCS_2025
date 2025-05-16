list(APPEND GeodeticSolid_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp)
list(APPEND GeodeticSolid_sources ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp)

list(APPEND GeodeticSolid_headers ${CMAKE_CURRENT_SOURCE_DIR}/GeodeticSolid.hpp)

list(APPEND GeodeticSolid_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(GeodeticSolid_sources ${GeodeticSolid_sources} PARENT_SCOPE)
set(GeodeticSolid_headers ${GeodeticSolid_headers} PARENT_SCOPE)
set(GeodeticSolid_includes ${GeodeticSolid_includes} PARENT_SCOPE)
