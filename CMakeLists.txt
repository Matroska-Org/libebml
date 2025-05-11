cmake_minimum_required(VERSION 3.5)

project(ebml VERSION 1.4.5)

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

option(ENABLE_WIN32_IO "Build the Windows I/O helper class" OFF)
feature_info_on_off(ENABLE_WIN32_IO "will build Windows I/O code" "will not build Windows I/O code")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(libebml_SOURCES
  src/Debug.cpp
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
  src/EbmlSubHead.cpp
  src/EbmlUInteger.cpp
  src/EbmlUnicodeString.cpp
  src/EbmlVersion.cpp
  src/EbmlVoid.cpp
  src/IOCallback.cpp
  src/MemIOCallback.cpp
  src/MemReadIOCallback.cpp
  src/SafeReadIOCallback.cpp
  src/StdIOCallback.cpp)

if(WIN32 AND ENABLE_WIN32_IO)
   list(APPEND libebml_SOURCES
     src/platform/win32/WinIOCallback.cpp)
endif()

set(libebml_PUBLIC_HEADERS
  ebml/Debug.h
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

set(libebml_C_PUBLIC_HEADERS ebml/c/libebml_t.h)

add_library(ebml ${libebml_SOURCES} ${libebml_PUBLIC_HEADERS} ${libebml_C_PUBLIC_HEADERS})
if(WIN32)
  include(CheckIncludeFile)
  check_include_file(winapifamily.h HAVE_WINAPIFAMILY_H)
  if(HAVE_WINAPIFAMILY_H)
    target_compile_definitions(ebml PUBLIC HAVE_WINAPIFAMILY_H)
  endif()
endif()
set_target_properties(ebml PROPERTIES
  VERSION 5.0.0
  SOVERSION 5
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON
)
target_include_directories(ebml
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
if(MSVC)
  target_compile_definitions(ebml PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(ebml PRIVATE $<BUILD_INTERFACE:utf8cpp>)

include(GenerateExportHeader)
generate_export_header(ebml EXPORT_MACRO_NAME EBML_DLL_API)
target_sources(ebml
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/ebml_export.h
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(ebml PUBLIC EBML_STATIC_DEFINE)
endif()

install(TARGETS ebml
  EXPORT EBMLTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${libebml_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ebml)
install(FILES ${libebml_C_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ebml/c)
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
  if (HAVE_WINAPIFAMILY_H)
    set(WINAPIFAMILY_PC -DHAVE_WINAPIFAMILY_H)
  endif()
  if(NOT BUILD_SHARED_LIBS)
    set(STATIC_DEFINE_CFLAGS -DEBML_STATIC_DEFINE)
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
