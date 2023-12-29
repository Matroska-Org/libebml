cmake_minimum_required(VERSION 3.1.2)

project(ebml VERSION 2.0.0)

include(CMakeDependentOption)

option(DISABLE_PKGCONFIG "Disable PkgConfig module generation" OFF)
option(DISABLE_CMAKE_CONFIG "Disable CMake package config module generation" OFF)
cmake_dependent_option(BUILD_SHARED_LIBS "Build libebml as a shared library (except Windows)" OFF "NOT WIN32" OFF)
option(BUILD_TESTING "Build tests" OFF)
option(DEV_MODE "Developer mode with extra compilation checks" OFF)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCXXCompilerFlag)
include(CMakeParseArguments)
function(add_cxx_flag_if_supported)
  foreach(flagname ${ARGN})
    string(REPLACE "-" "_" TEST_RESULT ${flagname})
    check_cxx_compiler_flag(${flagname} HAVE_FLAG_${TEST_RESULT})
    # message(STATUS "supported ${flagname}: ${HAVE_FLAG_${TEST_RESULT}}")

    if (HAVE_FLAG_${TEST_RESULT})
      add_compile_options(${flagname})
    endif()
  endforeach()
endfunction()

if(DEV_MODE)
  message(STATUS "Using developer mode for ${CMAKE_CXX_COMPILER_ID}")
  set(CMAKE_CXX_EXTENSIONS OFF)
  add_cxx_flag_if_supported(-Wall -Wno-comment -Wfatal-errors -fstack-protector-strong -Wnon-virtual-dtor -Wextra
                            -Wunused -Wpedantic -Woverloaded-virtual -Wshadow
                            -Wno-self-assign -Wno-mismatched-tags -Wno-inconsistent-missing-override
                            -Wno-potentially-evaluated-expression -Wno-extra-semi -Wno-gnu-zero-variadic-macro-arguments)
  add_cxx_flag_if_supported(-Qunused-arguments)
endif()

set(libebml_SOURCES
  src/EbmlBinary.cpp
  src/EbmlContexts.cpp
  src/EbmlCrc32.cpp
  src/EbmlDate.cpp
  src/EbmlDummy.cpp
  src/EbmlElement.cpp
  src/EbmlFloat.cpp
  src/EbmlHead.cpp
  src/EbmlMaster.cpp
  src/EbmlSInteger.cpp
  src/EbmlStream.cpp
  src/EbmlString.cpp
  src/EbmlUInteger.cpp
  src/EbmlUnicodeString.cpp
  src/EbmlVersion.cpp
  src/EbmlVoid.cpp
  src/IOCallback.cpp
  src/MemIOCallback.cpp
  src/MemReadIOCallback.cpp
  src/SafeReadIOCallback.cpp
  src/StdIOCallback.cpp)

set(libebml_PUBLIC_HEADERS
  ebml/EbmlBinary.h
  ebml/EbmlConfig.h
  ebml/EbmlContexts.h
  ebml/EbmlCrc32.h
  ebml/EbmlDate.h
  ebml/EbmlDummy.h
  ebml/EbmlElement.h
  ebml/EbmlEndian.h
  ebml/EbmlFloat.h
  ebml/EbmlHead.h
  ebml/EbmlId.h
  ebml/EbmlMaster.h
  ebml/EbmlSInteger.h
  ebml/EbmlStream.h
  ebml/EbmlString.h
  ebml/EbmlSubHead.h
  ebml/EbmlTypes.h
  ebml/EbmlUInteger.h
  ebml/EbmlUnicodeString.h
  ebml/EbmlVersion.h
  ebml/EbmlVoid.h
  ebml/IOCallback.h
  ebml/MemIOCallback.h
  ebml/MemReadIOCallback.h
  ebml/SafeReadIOCallback.h
  ebml/StdIOCallback.h)

add_library(ebml ${libebml_SOURCES} ${libebml_PUBLIC_HEADERS})
set_target_properties(ebml PROPERTIES
  VERSION 6.0.0
  SOVERSION 6
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON
)
target_include_directories(ebml
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
if(WIN32)
  target_compile_definitions(ebml PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

include(TestBigEndian)
test_big_endian(BUILD_BIG_ENDIAN)
if(BUILD_BIG_ENDIAN)
  target_compile_definitions(ebml PRIVATE BUILD_BIG_ENDIAN)
else()
  target_compile_definitions(ebml PRIVATE BUILD_LITTLE_ENDIAN)
endif()

include(GenerateExportHeader)
generate_export_header(ebml EXPORT_MACRO_NAME EBML_DLL_API)
target_sources(ebml
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/ebml_export.h
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(ebml PUBLIC EBML_STATIC_DEFINE)
endif()

if (BUILD_TESTING)
  enable_testing()

  add_executable(test_id test/test_id.cxx)
  target_link_libraries(test_id PUBLIC ebml)
  add_test(test_id test_id)

  add_executable(test_infinite test/test_infinite.cxx)
  target_link_libraries(test_infinite PUBLIC ebml)
  add_test(test_infinite test_infinite)

  add_executable(test_utfstring test/test_utfstring.cxx)
  target_link_libraries(test_utfstring PUBLIC ebml)
  add_test(test_utfstring test_utfstring)

  add_executable(test_macros test/test_macros.cxx)
  target_link_libraries(test_macros PUBLIC ebml)
  add_test(test_macros test_macros)

endif(BUILD_TESTING)


install(TARGETS ebml
  EXPORT EBMLTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${libebml_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ebml)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ebml_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ebml)

if(NOT DISABLE_PKGCONFIG)
  set(prefix ${CMAKE_INSTALL_PREFIX})
  set(exec_prefix "\$\{prefix\}")
  if (IS_ABSOLUTE ${CMAKE_INSTALL_LIBDIR})
    set(libdir "${CMAKE_INSTALL_LIBDIR}")
  else()
    set(libdir "\$\{prefix\}/${CMAKE_INSTALL_LIBDIR}")
  endif()
  if (IS_ABSOLUTE ${CMAKE_INSTALL_INCLUDEDIR})
    set(includedir "${CMAKE_INSTALL_INCLUDEDIR}")
  else()
    set(includedir "\$\{prefix\}/${CMAKE_INSTALL_INCLUDEDIR}")
  endif()
  set(PACKAGE_VERSION ${PROJECT_VERSION})

  get_target_property(EBML_DEFINITIONS ebml INTERFACE_COMPILE_DEFINITIONS)
  if(NOT "${EBML_DEFINITIONS}" STREQUAL "EBML_DEFINITIONS-NOTFOUND")
    set(EBML_DEFINITIONS "-D${EBML_DEFINITIONS}")
    list(JOIN EBML_DEFINITIONS " -D" EBML_DEFINITIONS)
  endif()

  configure_file(libebml.pc.in libebml.pc @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libebml.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

if(NOT DISABLE_CMAKE_CONFIG)
  set(CMAKE_INSTALL_PACKAGEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/EBML)
  include(CMakePackageConfigHelpers)
  configure_package_config_file(EBMLConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/EBMLConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_PACKAGEDIR})
  write_basic_package_version_file(EBMLConfigVersion.cmake COMPATIBILITY SameMajorVersion)
  install(EXPORT EBMLTargets NAMESPACE EBML:: DESTINATION ${CMAKE_INSTALL_PACKAGEDIR})
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/EBMLConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/EBMLConfigVersion.cmake
      DESTINATION ${CMAKE_INSTALL_PACKAGEDIR})
endif()
