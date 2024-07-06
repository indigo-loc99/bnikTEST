

include(GNUInstallDirs)


if(BNIK_ENABLE_BNIK_CXX_MODULES)
  add_library(dstring)
  target_compile_features(dstring PUBLIC cxx_std_23)
  set(BNIK_TARGETS_PROPAGATE_TYPE PUBLIC)
else()
  add_library(dstring INTERFACE)
  target_compile_features(dstring INTERFACE cxx_std_23)
  set(BNIK_TARGETS_PROPAGATE_TYPE INTERFACE)
endif()



if(BNIK_ENABLE_BNIK_CXX_MODULES AND BNIK_CAN_BUILD_AS_CXX_MODULES)

  target_sources(dstring PUBLIC
                   FILE_SET bnik_cxx_modules
                   TYPE CXX_MODULES 
                   BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
                   FILES dstring.ixx
                )
  target_sources(dstring PUBLIC
                   FILE_SET bnik_dstring_headers
                   TYPE HEADERS 
                   FILES dstring.hpp config.hpp common_defs.hpp
                )

else()
  target_sources(dstring PUBLIC
                   FILE_SET bnik_dstring_headers
                   TYPE HEADERS 
                   #BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
                   FILES dstring.hpp config.hpp
                )
  target_include_directories(dstring INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
endif()

set_target_properties(dstring PROPERTIES VERSION "${bnik_VERSION_MAJOR}.${bnik_VERSION_MINOR}.8" SOVERSION 0 INTERFACE_bnik_VERSION_MAJOR_MINOR "${bnik_VERSION_MAJOR}.${bnik_VERSION_MINOR}")
set_property(TARGET dstring APPEND PROPERTY COMPATIBLE_INTERFACE_STRING bnik_VERSION_MAJOR_MINOR)



if(STD_LIBRARY_ROOT)
  if(WIN32 AND MSVC)
    include(StandaloneCxxStdLib)
    target_link_libraries(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} StandaloneStdLibrary)
  endif()
endif()


if(BNIK_HAS_STD_ALLOCATE_AT_LEAST)
  target_compile_definitions(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} BNIK_HAS_STD_ALLOCATE_AT_LEAST=1)
else()
  target_compile_definitions(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} BNIK_HAS_STD_ALLOCATE_AT_LEAST=0)
endif()

if(BNIK_EVERY_CLASS_MEMBER_IS_CONSTEXPR_APPLICABLE)
  target_compile_definitions(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} BNIK_CXX23_CONSTEXPR_AVAILABLE=1)
else()
  target_compile_definitions(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} BNIK_CXX23_CONSTEXPR_AVAILABLE=0)
endif()

if(BNIK_USE_CMAKE_CXX_MODULE_STD)
  if(BNIK_HAS_CXX_MODULE_STD)
    target_compile_definitions(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} BNIK_HAS_CXX_MODULE_STD=1)
  else()
    target_compile_definitions(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} BNIK_HAS_CXX_MODULE_STD=0)
  endif()
endif()

if(NOT BNIK_ENABLE_BNIK_CXX_MODULES)

  install(TARGETS dstring
         EXPORT bnik_targets
         FILE_SET bnik_dstring_headers
  )

else()

  install(TARGETS dstring
         EXPORT bnik_targets
         FILE_SET bnik_dstring_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bnik"
         FILE_SET bnik_cxx_modules DESTINATION "${CMAKE_INSTALL_DATADIR}/bnik"
         CXX_MODULES_BMI DESTINATION cxxbmi/bnik
  )
endif()

install(EXPORT bnik_targets
        FILE bnikTargets.cmake
        NAMESPACE bnik::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bnik
        CXX_MODULES_DIRECTORY cxxbmi/bnik
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/bnikConfig.cmake"
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bnik)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/bnikConfigVersion.cmake" VERSION "${bnik_VERSION_MAJOR}.${bnik_VERSION_MINOR}.8" COMPATIBILITY ExactVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/bnikConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/bnikConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bnik)

export(EXPORT bnik_targets FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/bnikTargets.cmake" NAMESPACE bnik::)



# TODO: Add tests and install targets if needed.
