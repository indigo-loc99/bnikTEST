
if(USE_STANDALONE_STD_LIBRARY AND NOT DEFINED STD_LIBRARY_ROOT)
  message(FATAL_ERROR "Using a custom C++ standard library is set to ON but STD_LIBRARY_ROOT wasn't defined.")
endif()

if(MSVC)
  message(NOTICE "Looking for Microsoft C++ standard library")
  set(MICROSOFT_STL_OSS_BUILD_ROOT "${STD_LIBRARY_ROOT}")
  find_package(MicrosoftStl MODULE)

  if(NOT MicrosoftStl_FOUND)
    set(LibCxx_ROOT ${STD_LIBRARY_ROOT})
    find_package(LibCxx MODULE)
    # find more libraries etc.
    add_library(StandaloneStdLibrary ALIAS Cxx)
  else()
    if(TARGET MicrosoftStlStdModule)
      add_library(StandaloneStdLibrary ALIAS MicrosoftStlStdModule)
    else()
      add_library(StandaloneStdLibrary ALIAS MicrosoftStl)
    endif()
  endif()
endif()
