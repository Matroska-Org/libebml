cmake_minimum_required(VERSION 3.5)

project(ebml VERSION 2.0.0)

include(CMakeDependentOption)

include( FeatureSummary )
function(feature_info_on_off)
  set(option_name ${ARGV0})
  if(${option_name})
    add_feature_info(${option_name} ${option_name} ${ARGV1})
  else()
    add_feature_info(${option_name} $<NOT:${option_name}> ${ARGV2})
  endif()
endfunction(feature_info_on_off)

cmake_dependent_option(BUILD_SHARED_LIBS "Build libebml as a shared library (except Windows)" OFF "NOT WIN32" OFF)
feature_info_on_off(BUILD_SHARED_LIBS "will build as a dynamic library" "will build as a static library")

option(DISABLE_PKGCONFIG "Disable PkgConfig module generation" OFF)
feature_info_on_off(DISABLE_PKGCONFIG "won't generate pkgconfig config file" "will generate pkgconfig config file")

option(DISABLE_CMAKE_CONFIG "Disable CMake package config module generation" OFF)
feature_info_on_off(DISABLE_CMAKE_CONFIG "won't generate CMake config file" "will generate CMake config file")

option(BUILD_TESTING "Build tests" OFF)
feature_info_on_off(BUILD_TESTING "will build test code" "will build without test code")

option(DEV_MODE "Developer mode with extra compilation checks" OFF)
feature_info_on_off(DEV_MODE "added developer mode extra compilation checks" "default build mode")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
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

find_package(utf8cpp 3.2.0)
if(NOT utf8cpp_FOUND)
  include(FetchContent REQUIRED)
  FetchContent_Declare(
        utf8cpp
        GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
        GIT_TAG v3.2.5
        GIT_PROGRESS TRUE
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(utf8cpp)
endif()

if(DEV_MODE)
  message(STATUS "Using developer mode for ${CMAKE_CXX_COMPILER_ID}")
  set(CMAKE_CXX_EXTENSIONS OFF)
  add_cxx_flag_if_supported(-Wno-error=unused-command-line-argument
                            -Wall -Wextra -Wpedantic -Wfatal-errors -fstack-protector-strong
                            -Wcast-align
                            -W4)
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

target_link_libraries(ebml PRIVATE $<BUILD_INTERFACE:utf8cpp>)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
  if(${CMAKE_CXX_BYTE_ORDER} STREQUAL "BIG_ENDIAN")
    set(BUILD_BIG_ENDIAN 1)
  else()
    set(BUILD_BIG_ENDIAN 0)
  endif()
else()
  include(TestBigEndian)
  test_big_endian(BUILD_BIG_ENDIAN)
endif()
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
  add_test(NAME test_id COMMAND test_id)

  add_executable(test_header test/test_header.cxx)
  target_link_libraries(test_header PUBLIC ebml)
  add_test(NAME test_header COMMAND test_header)

  add_executable(test_infinite test/test_infinite.cxx)
  target_link_libraries(test_infinite PUBLIC ebml)
  add_test(NAME test_infinite COMMAND test_infinite)

  add_executable(test_utfstring test/test_utfstring.cxx)
  target_link_libraries(test_utfstring PUBLIC ebml)
  add_test(NAME test_utfstring COMMAND test_utfstring)

  add_executable(test_macros test/test_macros.cxx)
  target_link_libraries(test_macros PUBLIC ebml)
  add_test(NAME test_macros COMMAND test_macros)

  add_executable(test_crc test/test_crc.cxx)
  target_link_libraries(test_crc PUBLIC ebml)
  add_test(NAME test_crc COMMAND test_crc)

  add_executable(test_missing test/test_missing.cxx)
  target_link_libraries(test_missing PUBLIC ebml)
  add_test(NAME test_missing COMMAND test_missing)

  add_executable(test_versioning test/test_versioning.cxx)
  target_link_libraries(test_versioning PUBLIC ebml)
  add_test(NAME test_versioning COMMAND test_versioning)

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

feature_summary(DESCRIPTION "libebml options:" WHAT ENABLED_FEATURES)
feature_summary(DESCRIPTION "libebml disabled options:" WHAT DISABLED_FEATURES)
feature_summary(DESCRIPTION "libebml packages used:" WHAT PACKAGES_FOUND)
