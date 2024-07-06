include(CheckIncludeFileCXX)

if(NOT DEFINED MICROSOFT_STL_OSS_BUILD_ROOT)
  message(WARNING "You must give Microsoft STL's build root with the variable/cache MICROSOFT_STL_OSS_BUILD_ROOT")
endif()

set(TARGET_ARCH_TEST_CPP [[
#include <cstdio>
int main() {
#if defined(_M_X64)
# if defined(_M_ARM64EC)
    constexpr const char* arch = "arm64ec";
# else
    constexpr const char* arch = "amd64";
# endif
#elif defined(_M_IX86)
    constexpr const char* arch = "i386";
#elif defined(_M_ARM64)
    constexpr const char* arch = "arm64";
#elif defined(_M_ARM)
    constexpr const char* arch = "arm";
#else
# warning "Unsupported compiler used"
    constexpr const char* arch = "UNSUPPORTED-COMPILER";
#endif

    std::printf("%s", arch);
    return 0;
}
]])

try_run(ARCH_QUERY_RETVAL ARCH_QUERY_COMPILED
          SOURCE_FROM_VAR "TryCompileArchitectureQuery.cpp" TARGET_ARCH_TEST_CPP
          RUN_OUTPUT_STDOUT_VARIABLE _MICROSOFT_STL_ARCH_SUFFIX
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(_CONFIG_SUFFIX d)
else()
  set(_CONFIG_SUFFIX "")
endif()
 find_library(STD_LIB_STATIC_FILE libcpmt${_CONFIG_SUFFIX} HINTS ${MICROSOFT_STL_OSS_BUILD_ROOT}/out PATH_SUFFIXES lib/${_MICROSOFT_STL_ARCH_SUFFIX} NO_DEFAULT_PATH)
 find_library(STD_LIB_SHARED_IMPLIB_FILE msvcprt${_CONFIG_SUFFIX} HINTS ${MICROSOFT_STL_OSS_BUILD_ROOT}/out PATH_SUFFIXES lib/${_MICROSOFT_STL_ARCH_SUFFIX} NO_DEFAULT_PATH)

 if(MSVC_TOOLSET_VERSION MATCHES "14[0-9]")
   set(_MSVC_ABI_CODENAME 140)
 else()
   # binaries build with VS2013 (12.X) and earlier likely won't work with this library
   message(AUTHOR_WARNING "MSVC_TOOLSET_VERION?")
   set(_MSVC_ABI_CODENAME "")
 endif()

 find_file(STD_LIB_SHARED_FILE msvcp${_MSVC_ABI_CODENAME}${_CONFIG_SUFFIX}_oss.dll HINTS ${MICROSOFT_STL_OSS_BUILD_ROOT}/out PATH_SUFFIXES bin/${_MICROSOFT_STL_ARCH_SUFFIX} NO_DEFAULT_PATH)

 find_file(_MICROSOFT_STL_INCLUDES_FILE
             NAMES __msvc_all_public_headers.hpp
             HINTS ${MICROSOFT_STL_OSS_BUILD_ROOT}/out/inc
             
             NO_DEFAULT_PATH
          )

find_file(_MICROSOFT_STL_STD_IXX_FILE
            NAMES std.ixx
            HINTS ${MICROSOFT_STL_OSS_BUILD_ROOT}/out/modules
            NO_DEFAULT_PATH
         )

find_file(_MICROSOFT_STL_STD_COMPAT_IXX_FILE
            NAMES std.compat.ixx
            HINTS ${MICROSOFT_STL_OSS_BUILD_ROOT}/out/modules
            NO_DEFAULT_PATH
         )

if(EXISTS "${_MICROSOFT_STL_INCLUDES_FILE}")
  CHECK_INCLUDE_FILE_CXX(${_MICROSOFT_STL_INCLUDES_FILE} _MICROSOFT_STL_OSS_HAS_ALL_PUBLIC_HEADERS)
endif()

cmake_path(GET _MICROSOFT_STL_INCLUDES_FILE PARENT_PATH _MICROSOFT_STL_INCLUDES_DIR)
cmake_path(GET _MICROSOFT_STL_STD_IXX_FILE PARENT_PATH _MICROSOFT_STL_CXX_MODULES_DIR)

if(_MICROSOFT_STL_INCLUDES_FILE AND STD_LIB_STATIC_FILE AND STD_LIB_SHARED_FILE AND STD_LIB_SHARED_IMPLIB_FILE)
  add_library(MicrosoftStl_static STATIC IMPORTED GLOBAL)

  target_include_directories(MicrosoftStl_static SYSTEM INTERFACE ${_MICROSOFT_STL_INCLUDES_DIR})

  add_library(MicrosoftStl SHARED IMPORTED GLOBAL)

  target_include_directories(MicrosoftStl SYSTEM INTERFACE ${_MICROSOFT_STL_INCLUDES_DIR})

  if(_MICROSOFT_STL_STD_IXX_FILE)
    add_library(_MicrosoftStlStdModule INTERFACE IMPORTED GLOBAL)
    #target_include_directories(_MicrosoftStlStdModule SYSTEM INTERFACE ${_MICROSOFT_STL_INCLUDES_DIR})
    target_compile_features(_MicrosoftStlStdModule INTERFACE cxx_std_20)
    target_sources(_MicrosoftStlStdModule INTERFACE 
                     FILE_SET microsoft_stl_std_modules
                     TYPE CXX_MODULES
                     BASE_DIRS ${_MICROSOFT_STL_CXX_MODULES_DIR}
                     FILES ${_MICROSOFT_STL_CXX_MODULES_DIR}/std.ixx
    )

    add_library(MicrosoftStlStdModule_static INTERFACE IMPORTED GLOBAL)
    target_link_libraries(MicrosoftStlStdModule_static INTERFACE _MicrosoftStlStdModule MicrosoftStl_static)

    add_library(MicrosoftStlStdModule OBJECT) #INTERFACE IMPORTED GLOBAL)
    target_sources(MicrosoftStlStdModule PUBLIC 
                   FILE_SET microsoft_stl_std_modules
                   TYPE CXX_MODULES
                   BASE_DIRS ${_MICROSOFT_STL_CXX_MODULES_DIR}
                   FILES ${_MICROSOFT_STL_CXX_MODULES_DIR}/std.ixx
    )
    target_compile_features(MicrosoftStlStdModule PUBLIC cxx_std_23)
    target_include_directories(MicrosoftStlStdModule SYSTEM PUBLIC ${_MICROSOFT_STL_INCLUDES_DIR})
    #set_target_properties(MicrosoftStlStdModule PROPERTIES IMPORTED_LOCATION_${CMAKE_BUILD_TYPE} ${STD_LIB_SHARED_FILE})
    #set_target_properties(MicrosoftStlStdModule PROPERTIES IMPORTED_IMPLIB ${STD_LIB_SHARED_IMPLIB_FILE})
    #set_target_properties(MicrosoftStlStdModule PROPERTIES IMPORTED_IMPLIB_${CMAKE_BUILD_TYPE} ${STD_LIB_SHARED_IMPLIB_FILE})
    target_link_libraries(MicrosoftStlStdModule MicrosoftStl) #INTERFACE MicrosoftStl _MicrosoftStlStdModule)
  endif()
  if(_MICROSOFT_STL_STD_COMPAT_IXX_FILE)
    add_library(_MicrosoftStlStdCompatModule INTERFACE IMPORTED GLOBAL)
    #target_include_directories(_MicrosoftStlStdCompatModule SYSTEM INTERFACE ${_MICROSOFT_STL_INCLUDES_DIR})
    target_sources(_MicrosoftStlStdCompatModule INTERFACE 
                     FILE_SET microsoft_stl_std_modules
                     TYPE CXX_MODULES
                     BASE_DIRS ${_MICROSOFT_STL_CXX_MODULES_DIR}
                     FILES std.compat.ixx
    )
    add_library(MicrosoftStlStdCompatModule_static INTERFACE IMPORTED GLOBAL)
    target_link_libraries(MicrosoftStlStdCompatModule_static INTERFACE _MicrosoftStlStdCompactModule MicrosoftStl_static)
    add_library(MicrosoftStlStdCompatModule INTERFACE IMPORTED GLOBAL)
    target_compile_features(MicrosoftStlStdCompatModule INTERFACE cxx_std_20)
    target_link_libraries(MicrosoftStlStdCompatModule INTERFACE _MicrosoftStlStdCompatModule MicrosoftStl)
  endif()

  if(CMAKE_CONFIGURATION_TYPES)
   #set_target_properties(MicrosoftStl_static PROPERTIES IMPORTED_LOCATION_Debug "${STD_LIB_FILE_Debug}")
  else()
   set_target_properties(MicrosoftStl_static PROPERTIES IMPORTED_LOCATION_${CMAKE_BUILD_TYPE} ${STD_LIB_STATIC_FILE})

   set_target_properties(MicrosoftStl PROPERTIES IMPORTED_LOCATION_${CMAKE_BUILD_TYPE} ${STD_LIB_SHARED_FILE})
   set_target_properties(MicrosoftStl PROPERTIES IMPORTED_IMPLIB ${STD_LIB_SHARED_IMPLIB_FILE})
   set_target_properties(MicrosoftStl PROPERTIES IMPORTED_IMPLIB_${CMAKE_BUILD_TYPE} ${STD_LIB_SHARED_IMPLIB_FILE})
  endif()

endif()

if(_MICROSOFT_STL_INCLUDES_FILE AND _MICROSOFT_STL_OSS_HAS_ALL_PUBLIC_HEADERS AND STD_LIB_STATIC_FILE AND STD_LIB_SHARED_FILE AND STD_LIB_SHARED_IMPLIB_FILE)
  set(MicrosoftStl_FOUND TRUE)
else()
  set(MicrosoftStl_FOUND FALSE)
endif()
