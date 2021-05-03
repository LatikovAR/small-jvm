#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <cassert>
#include "vm_interface.h"
#include "test.h"

/*
 * Garbage collection uses for frames in JVM.
 *
 * Mem structure and dependency tree the same in both tests.
 *
 * First of all builds the simple dependency tree:
 * 1, 2, 7 - root frames
 * dependencies: 1 -> 3, 2 -> 4, 4 -> 5, 2 -> 4, 2 -> 6
 *
 * Then checks deleting root frames in order:
 * 7, 2, 1
 *
 * After every step calls garbage collection and logs memory condition
 *
 * Frame parametres isn't important in this test
*/


//Marking memory blocks for gc doesn't need so won't be done
void JavaVM::test_manual_mem_collection() {
    JavaVM vm;

    #ifdef LOG_ON
    std::cout << "Test_manual_mem_collection started\n\r\n\r";
    #endif

    //only for test
    MethodInfo def_info;
    def_info.code_info_.max_stack_ = 1;
    def_info.code_info_.max_locals_ = 1;

    for(size_t i = 0; i < 7; ++i) {
        vm.CreateFrame(&def_info);
    }

    vm.gc_.RegisterRootObject(vm.frame_[0]->mem_ptr_for_gc());
    vm.gc_.RegisterRootObject(vm.frame_[1]->mem_ptr_for_gc());
    vm.gc_.RegisterRootObject(vm.frame_[2]->mem_ptr_for_gc());
    vm.gc_.RegisterRootObject(vm.frame_[3]->mem_ptr_for_gc());
    vm.gc_.RegisterRootObject(vm.frame_[4]->mem_ptr_for_gc());
    vm.gc_.RegisterRootObject(vm.frame_[5]->mem_ptr_for_gc());
    vm.gc_.RegisterRootObject(vm.frame_[6]->mem_ptr_for_gc());
    vm.gc_.RegisterRootObject(vm.frame_[7]->mem_ptr_for_gc());

    #ifdef LOG_ON
    std::cout << "All frames allocated\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    vm.gc_.UnregisterRootObject(vm.frame_[7]->mem_ptr_for_gc());
    vm.gc_.FullGc();

    #ifdef LOG_ON
    std::cout << "Root frame 7 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    vm.gc_.UnregisterRootObject(vm.frame_[2]->mem_ptr_for_gc());
    vm.gc_.UnregisterRootObject(vm.frame_[6]->mem_ptr_for_gc());
    vm.gc_.FullGc();

    #ifdef LOG_ON
    std::cout << "Root frame 2 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    vm.gc_.UnregisterRootObject(vm.frame_[1]->mem_ptr_for_gc());
    vm.gc_.UnregisterRootObject(vm.frame_[3]->mem_ptr_for_gc());
    vm.gc_.UnregisterRootObject(vm.frame_[4]->mem_ptr_for_gc());
    vm.gc_.UnregisterRootObject(vm.frame_[5]->mem_ptr_for_gc());
    vm.gc_.FullGc();

    #ifdef LOG_ON
    std::cout << "Root frame 1 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";

    std::cout << "End test_manual_mem_collection \n\r\n\r";
    #endif
}

void JavaVM::test_gc_mem_collection() {
    JavaVM vm;

    #ifdef LOG_ON
    std::cout << "Test_gc_mem_collection started\n\r";
    #endif

    //only for test
    MethodInfo def_info;
    def_info.code_info_.max_stack_ = 1;
    def_info.code_info_.max_locals_ = 1;

    for(size_t i = 0; i < 7; ++i) {
        vm.CreateFrame(&def_info);
    }

    vm.gc_.RegisterRootObject(vm.frame_[1]);
    vm.gc_.RegisterRootObject(vm.frame_[2]);
    vm.gc_.RegisterRootObject(vm.frame_[7]);

    vm.gc_.LinkToObj(vm.frame_[1], vm.frame_[3]);
    vm.gc_.LinkToObj(vm.frame_[1], vm.frame_[4]);
    vm.gc_.LinkToObj(vm.frame_[2], vm.frame_[4]);
    vm.gc_.LinkToObj(vm.frame_[2], vm.frame_[6]);
    vm.gc_.LinkToObj(vm.frame_[4], vm.frame_[5]);

    #ifdef LOG_ON
    std::cout << "All frames allocated\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    vm.gc_.UnregisterRootObject(vm.frame_[7]);
    vm.gc_.FullGc();

    #ifdef LOG_ON
    std::cout << "Root frame 7 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    vm.gc_.UnregisterRootObject(vm.frame_[2]);
    vm.gc_.FullGc();

    #ifdef LOG_ON
    std::cout << "Root frame 2 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    vm.gc_.UnregisterRootObject(vm.frame_[1]);
    vm.gc_.FullGc();

    #ifdef LOG_ON
    std::cout << "Root frame 1 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";

    std::cout << "End test_gc_mem_collection \n\r\n\r";
    #endif
}
