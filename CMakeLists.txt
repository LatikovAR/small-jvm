cmake_minimum_required()

project (tiny_vm)

set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED NO_ASM)
    set(NO_ASM false)
endif ()

set(SOURCE_LIST
    main.cpp
    test.cpp
    vm_gc_test.cpp
    vm.cpp
)

include_directories(memalloc gc)

add_subdirectory (memalloc)

add_compile_options(-Wall -Wextra)

add_executable(tiny_vm ${SOURCE_LIST})

if(NO_FREERTOS)
    target_compile_options(tiny_vm PUBLIC "-DNO_FREERTOS")
endif()

if(CMAKE_CROSSCOMPILE AND NOT NO_ASM)
    target_compile_options(tiny_vm PUBLIC "-DASM")
    message("!! Build assembly-version")
endif()

