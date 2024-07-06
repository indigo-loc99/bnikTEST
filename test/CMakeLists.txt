
cmake_dependent_option(BNIK_TESTING_USE_CXX_MODULE_STD "Use standard library modules in the tests" ON "BNIK_ENABLE_BNIK_CXX_MODULES;BNIK_USE_CXX_MODULE_STD" OFF)

if(BNIK_TESTING_USE_CXX_MODULE_STD)
  if((NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "14.0")
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
      set(CMAKE_CXX_SCAN_FOR_MODULES ON)
      set_target_properties(dstring PROPERTIES CXX_SCAN_FOR_MODULES ON)
      set(BNIK_TEST_USE_LIBDSTRING_CXX_MODULES ON)
    elseif(LINUX OR MINGW)
      if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "17.0"))
        set(CMAKE_CXX_SCAN_FOR_MODULES ON)
        set_target_properties(dstring PROPERTIES CXX_SCAN_FOR_MODULES ON)
        set(BNIK_TEST_USE_LIBDSTRING_CXX_MODULES ON)
      endif()
    endif()
  endif()
endif() # BNIK_TESTING_USE_CXX_MODULE_STD


add_executable(test_suite01 test01.cpp "Dstring.Constexprness01.cpp" "Dstring.CapacityLimits01.cpp" "Dstring.Iterators01.cpp" "DString.Constexprness02.cpp" "Dstring.DestructorBehavior01.cpp")

add_executable(dummy_prog DummyProg.cpp)

target_compile_features(test_suite01 PRIVATE cxx_std_23)
target_compile_features(dummy_prog PRIVATE cxx_std_23)

if(BNIK_TEST_USE_LIBDSTRING_CXX_MODULES)
  target_compile_definitions(test_suite01 PRIVATE BNIK_TEST_USE_LIBDSTRING_CXX_MODULE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_definitions(test_suite01 PRIVATE BNIK_TEST_USE_STD_MODULE)
    target_sources(test_suite01 PRIVATE FILE_SET bnik_test_cxx_modules TYPE CXX_MODULES FILES "utils/bnik.Test.Utils.ixx" "utils/bnik.Test.Utils-IteratorTestHelpers.ixx")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 18.1.2)
    get_target_property(THE_CXX_MODULE_STD test_suite01 CXX_MODULE_STD)
    if(THE_CXX_MODULE_STD)
      target_compile_definitions(test_suite01 PRIVATE BNIK_TEST_USE_STD_MODULE)
    endif()
    target_sources(test_suite01 PRIVATE FILE_SET bnik_test_cxx_modules TYPE CXX_MODULES FILES "utils/bnik.Test.Utils.ixx" "utils/bnik.Test.Utils-IteratorTestHelpers.ixx")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 14.1)
    get_target_property(THE_CXX_MODULE_STD test_suite01 CXX_MODULE_STD)
    if(THE_CXX_MODULE_STD)
      target_compile_definitions(test_suite01 PRIVATE BNIK_TEST_USE_STD_MODULE)
    endif()
    target_sources(test_suite01 PRIVATE FILE_SET bnik_test_cxx_modules TYPE CXX_MODULES FILES "utils/bnik.Test.Utils.ixx" "utils/bnik.Test.Utils-IteratorTestHelpers.ixx")
  endif()
endif()


#target_include_directories(test_suite01 PRIVATE SYSTEM C:/llvm-project-p/out/install/win-libcxx-zlib-x64/include/c++/v1/)

#target_link_libraries(test_suite01 "D:/llvm-project-p/out/install/win-libcxx-rt/lib/libc++.lib")
target_link_libraries(test_suite01 dstring GTest::gtest_main)
target_link_libraries(dummy_prog dstring)
gtest_discover_tests(test_suite01)
