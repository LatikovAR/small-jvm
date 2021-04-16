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
 * frame 0 - created with vm constructor so won't use it
 * 1, 2, 7 - root frames
 * dependencies: 1 -> 3, 1 -> 4, 4 -> 5, 2 -> 4, 2 -> 6
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

    vm.frame_[1] = new Frame(1, 1, vm.memory_);
    vm.frame_[2] = new Frame(1, 1, vm.memory_);
    vm.frame_[3] = new Frame(1, 1, vm.memory_);
    vm.frame_[4] = new Frame(1, 1, vm.memory_);
    vm.frame_[5] = new Frame(1, 1, vm.memory_);
    vm.frame_[6] = new Frame(1, 1, vm.memory_);
    vm.frame_[7] = new Frame(1, 1, vm.memory_);

    #ifdef LOG_ON
    std::cout << "All frames allocated\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    delete vm.frame_[7];

    #ifdef LOG_ON
    std::cout << "Root frame 7 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    delete vm.frame_[2];
    delete vm.frame_[6];

    #ifdef LOG_ON
    std::cout << "Root frame 2 deleted\n\r";
    std::cout << vm.memory_ << "\n\r\n\r";
    #endif

    delete vm.frame_[1];
    delete vm.frame_[3];
    delete vm.frame_[4];
    delete vm.frame_[5];

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

    vm.frame_[1] = new Frame(1, 1, vm.memory_);
    vm.frame_[2] = new Frame(1, 1, vm.memory_);
    vm.frame_[3] = new Frame(1, 1, vm.memory_);
    vm.frame_[4] = new Frame(1, 1, vm.memory_);
    vm.frame_[5] = new Frame(1, 1, vm.memory_);
    vm.frame_[6] = new Frame(1, 1, vm.memory_);
    vm.frame_[7] = new Frame(1, 1, vm.memory_);

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