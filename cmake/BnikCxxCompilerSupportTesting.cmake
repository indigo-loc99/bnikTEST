try_compile(BNIK_HAS_STD_ALLOCATE_AT_LEAST
              SOURCE_FROM_CONTENT "has_cxx_std_allocate_at_least.cpp" "#include <memory>\n void main() {std::allocator<char> al; auto ptr = std::allocator_traits<std::allocator<char>>::allocate_at_least(al, 10);}"
              CXX_STANDARD 23
            )

set(CXX_23_CONSTEXPR_SRC_TEXT
)

try_compile(BNIK_EVERY_CLASS_MEMBER_IS_CONSTEXPR_APPLICABLE
              SOURCE_FROM_FILE Test01.cpp "${CMAKE_SOURCE_DIR}/cmake/trycompile/CanMarkAllMembersConstexpr.cpp"
              CXX_STANDARD 23
)

if(BNIK_USE_CMAKE_CXX_MODULE_STD)
  try_compile(BNIK_HAS_CXX_MODULE_STD
                SOURCE_FROM_CONTENT "has_cxx_std_module.cpp" "import std;\nint main() {\n    auto arr = std::array{0, 2, 2};\n    return arr[0];\n}\n"
                CMAKE_FLAGS -DCMAKE_EXPERIMENTAL_CXX_IMPORT_STD:STRING=${CMAKE_EXPERIMENTAL_CXX_IMPORT_STD} -DCMAKE_CXX_MODULE_STD:BOOL=ON -DCMAKE_CXX_SCAN_FOR_MODULES:BOOL=ON
                CXX_STANDARD 23
                OUTPUT_VARIABLE _OUT_LOG
             )

  message(WARNING "Result of BNIK_HAS_CXX_MODULE_STD is '${BNIK_HAS_CXX_MODULE_STD}'")
  if(NOT BNIK_HAS_CXX_MODULE_STD)
    #message(WARNING "Try-Compile test for 'BNIK_HAS_CXX_MODULE_STD' has something to say:\n${_OUT_LOG}\n")
  endif()
endif()

if(BNIK_ENABLE_BNIK_CXX_MODULES)

  block()
  set(CMAKE_CXX_SCAN_FOR_MODULES ON)

  if(CMAKE_CXX_COMPILER_ID MATCHES "^[Mm][Ss][Vv][Cc]$")
    set(_FILE_EXTENSION "ixx")
  else()
    set(_FILE_EXTENSION "cpp")
  endif()

  try_compile(BNIK_CAN_BUILD_AS_CXX_MODULES
                SOURCES_TYPE CXX_MODULE
                SOURCE_FROM_CONTENT "can_build_cxx_modules.${_FILE_EXTENSION}" "module;\nexport module FooModule;\n"

                SOURCES_TYPE NORMAL
                SOURCE_FROM_CONTENT "main.cpp" "import FooModule;\nint main() {}"

                #CMAKE_FLAGS -DCMAKE_CXX_SCAN_FOR_MODULES:BOOL=ON
                CXX_STANDARD 23
                OUTPUT_VARIABLE _OUT_LOG
  )
  endblock()

  message(WARNING "Result of BNIK_CAN_BUILD_AS_CXX_MODULES is '${BNIK_CAN_BUILD_AS_CXX_MODULES}'")
  if(NOT BNIK_CAN_BUILD_AS_CXX_MODULES)
    message(WARNING "Try-Compile test for 'BNIK_CAN_BUILD_AS_CXX_MODULES' has something to say:\n${_OUT_LOG}\n")
  endif()
endif()
