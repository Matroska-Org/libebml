add_library(utf8-cpp INTERFACE)

target_include_directories(utf8-cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_sources(utf8-cpp INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/source/utf8/checked.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/utf8/core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/utf8/cpp11.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/utf8/cpp17.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/utf8/checked.h
)
