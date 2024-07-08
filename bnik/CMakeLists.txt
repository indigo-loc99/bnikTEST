

include(GNUInstallDirs)

add_library(dstring INTERFACE)
target_compile_features(dstring INTERFACE cxx_std_23)
set(BNIK_TARGETS_PROPAGATE_TYPE INTERFACE)

if(BNIK_ENABLE_BNIK_CXX_MODULES)
  add_library(dstring_cxx_module)
  set_target_properties(dstring_cxx_module PROPERTIES OUTPUT_NAME dstring)
  target_compile_features(dstring_cxx_module PUBLIC cxx_std_23)
  #set(BNIK_TARGETS_PROPAGATE_TYPE PUBLIC)
else()
endif()


target_sources(dstring PUBLIC
                 FILE_SET bnik_dstring_headers
                 TYPE HEADERS 
                 #BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
                 FILES dstring.hpp config.hpp
              )
target_include_directories(dstring INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(dstring PROPERTIES VERSION "${bnik_VERSION_MAJOR}.${bnik_VERSION_MINOR}.8")
target_compile_definitions(dstring INTERFACE BNIK_HAS_STD_ALLOCATE_AT_LEAST=$<BOOL:${BNIK_HAS_STD_ALLOCATE_AT_LEAST}>)
target_compile_definitions(dstring INTERFACE BNIK_CXX23_CONSTEXPR_AVAILABLE=$<BOOL:${BNIK_EVERY_CLASS_MEMBER_IS_CONSTEXPR_APPLICABLE}>)

if(BNIK_ENABLE_BNIK_CXX_MODULES AND BNIK_CAN_BUILD_AS_CXX_MODULES)

  target_sources(dstring_cxx_module 
                   PUBLIC FILE_SET bnik_cxx_modules
                     TYPE CXX_MODULES 
                     BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
                     FILES dstring.ixx
                   PUBLIC FILE_SET bnik_m_dstring_headers
                     TYPE HEADERS
                     FILES dstring.hpp config.hpp
                )
  #target_sources(dstring_cxx_module PRIVATE dstring.hpp config.hpp)
#[[
  target_sources(dstring PUBLIC
                   FILE_SET bnik_dstring_headers
                   TYPE HEADERS 
                   FILES dstring.hpp config.hpp common_defs.hpp
                )
]]
  set_target_properties(dstring_cxx_module PROPERTIES VERSION "${bnik_VERSION_MAJOR}.${bnik_VERSION_MINOR}.8" SOVERSION 0 INTERFACE_bnik_VERSION_MAJOR_MINOR "${bnik_VERSION_MAJOR}.${bnik_VERSION_MINOR}")
  set_property(TARGET dstring_cxx_module APPEND PROPERTY COMPATIBLE_INTERFACE_STRING bnik_VERSION_MAJOR_MINOR)
  target_compile_definitions(dstring_cxx_module PUBLIC BNIK_HAS_STD_ALLOCATE_AT_LEAST=$<BOOL:${BNIK_HAS_STD_ALLOCATE_AT_LEAST}>)
  target_compile_definitions(dstring_cxx_module PUBLIC BNIK_CXX23_CONSTEXPR_AVAILABLE=$<BOOL:${BNIK_EVERY_CLASS_MEMBER_IS_CONSTEXPR_APPLICABLE}>)
  if(BNIK_USE_CMAKE_CXX_MODULE_STD)
    target_compile_definitions(dstring_cxx_module PUBLIC BNIK_HAS_CXX_MODULE_STD=$<BOOL:${BNIK_HAS_CXX_MODULE_STD}>)
  endif()

endif()




if(STD_LIBRARY_ROOT)
  if(WIN32 AND MSVC)
    include(StandaloneCxxStdLib)
    target_link_libraries(dstring ${BNIK_TARGETS_PROPAGATE_TYPE} StandaloneStdLibrary)
  endif()
endif()




#[[
install(TARGETS dstring
       EXPORT bnik_targets
       FILE_SET bnik_dstring_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bnik"
)
]]

if(BNIK_ENABLE_BNIK_CXX_MODULES)
  install(TARGETS dstring_cxx_module
         EXPORT bnik_targets
         FILE_SET bnik_cxx_modules DESTINATION "${CMAKE_INSTALL_DATADIR}/bnik"
         FILE_SET bnik_m_dstring_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bnik"
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
