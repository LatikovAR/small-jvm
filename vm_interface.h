// vm_interface.h
#ifndef VM_INTERFACE_H
#define VM_INTERFACE_H

#include <stdint.h>
#include <assert.h>

#include "gc.h"

#define TEST

#define CONST_PULL_SIZE 8
#define MAX_FUNCTIONS 10
#define STACK_SIZE 32768
#define DEFAULT_FRAME_STACK_SIZE 256
#define DEFAULT_LOCALS_SIZE 64


// TODO - dynamic frame
#define FRAME_NUM 10
#define LOG_ON


class JavaVM {
    uint64_t* const_pull_[CONST_PULL_SIZE];

    uint64_t stack_[STACK_SIZE];

    Memory memory_;

    Gc gc_;

    // Programm counter
    uint8_t pc_ = 0;

    //===========FUNCTION=FOR=USER===========
public:
    JavaVM();
    ~JavaVM();

    // Execute bytecode sequence
    void Execute(uint8_t* bc);

    // Special getter for stack_pointer position
    uint64_t GetSP();

    // For replacing stack point explicitly (for tests)
    void MoveSP(int i);

    // TODO rewrite define in command of VM
    // It is ONLY for test system without this commands
    uint64_t RetStackVal(uint64_t pos);
    void SetStackVal(uint64_t pos, uint64_t val);
    int FillConstPull(uint8_t num_pull, uint64_t value);

    //===========FUNCTION=FOR=USER===========

private:
    void Execute_iand();
    void Execute_iconst_i(uint8_t i);
    void Execute_iadd();
    void Execute_invokestatic(uint8_t* bc);
    void Execute_bipush(uint8_t* bc);




    //=====================================================
    //=====================================================
private:
    class Frame {
    public:
        uint16_t size_local_variable_;
        uint16_t size_operand_stack_;
        uint64_t* local_variable_;
        uint64_t* operand_stack_;
        uint64_t return_value_;
        uint64_t sp_;

    public:
        Frame(uint16_t size_stack, uint16_t size_locals, uint64_t* data_memory);
        ~Frame();
        void* mem_ptr_for_gc() { return this; }
    };


    // ONLY for ONE thread
    Frame* frame_[FRAME_NUM];
    Frame* curr_frame_ = nullptr;
    // TODO Move to local variable
    uint64_t fp_ = 0;

public:

    // It is SIMPLE version
    // !!!An important simplification!!!
    // The constant pool contains the number of the called function
    // MAX NUMBER OF FUNCTION IS 10
    struct MethodInfo {
        // uint16_t             access_flags;
        uint16_t name_index;
        uint16_t descriptor_index;

        // !NOT SUPPORT!
        // uint16_t             attributes_count;
        // attribute_info attributes[attributes_count];

        struct CodeAttribute {
            // uint16_t attribute_name_index;
            // uint32_t attribute_length;
            uint16_t max_stack_;
            uint16_t max_locals_;
            uint32_t code_length_;
            uint8_t* code_; // Size = code_length

            // uint16_t exception_table_length;
            // {   u2 start_pc;
            //     u2 end_pc;
            //     u2 handler_pc;
            //     u2 catch_type;
            // } exception_table[exception_table_length];
            // u2 attributes_count;
            // attribute_info attributes[attributes_count];

            CodeAttribute();
            ~CodeAttribute();

            void SetValues(uint16_t max_stack,
                           uint16_t max_locals,
                           uint32_t code_length);
            void UploadCode(uint32_t size, uint8_t* code);
        } code_info_;


    };

    MethodInfo method_info_[MAX_FUNCTIONS];

    void CreateFrame(MethodInfo* info);
    void DeleteFrame();

    //for tests
    static void test_manual_mem_collection();
    static void test_gc_mem_collection();
};

#endif // VM_INTERFACE_H
