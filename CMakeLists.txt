
cmake_minimum_required (VERSION 3.13)

include(CMakeDependentOption)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("bnik" VERSION 0.0)

option(BNIK_ENABLE_BNIK_CXX_MODULES "Make bnik libraries consumable as C++ modules")
cmake_dependent_option(BNIK_USE_CXX_MODULE_STD "Prefer to use standard library dependencies as C++ modules using 'import std[.compat];' when possible" OFF "BNIK_ENABLE_BNIK_CXX_MODULES" OFF)
option(BNIK_ENABLE_TESTS "Build and run the tests" OFF)

if(BNIK_ENABLE_TESTS)

  include(FetchContent)

  FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git 
	  GIT_TAG e4fdb87e76b9fc4b01c54ad81aea19d6e994b994
  )
endif()


if(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD STREQUAL "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
  set(BNIK_USE_CMAKE_CXX_MODULE_STD ON)
endif()

if(LINUX AND NOT BNIK_USE_CMAKE_CXX_MODULE_STD)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(BNIK_ENABLE_TESTS)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(googletest)
endif()

if(BNIK_USE_CMAKE_CXX_MODULE_STD)
  set(CMAKE_CXX_MODULE_STD ON)
endif()

# do tests on supported compiler features
include(BnikCxxCompilerSupportTesting)

# Include sub-projects.
add_subdirectory ("bnik")

if(BNIK_ENABLE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory("test")
endif()
