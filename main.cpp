#define NO_FREERTOS

#include <stdio.h>
#include <ctime>
#include <cstdlib>
#ifndef NO_FREERTOS
#include <devices.h>
#include <task.h>
#endif

#include "opcodes.h"
#include "test.h"
#include "vm_interface.h"

void vTask() {
    printf(" Task created\n\r");
    while (1) {
        RunTest(ONE_TEST, iconst_0);
        RunTest(ONE_TEST, iconst_1);
        RunTest(ONE_TEST, iconst_2);
        RunTest(ONE_TEST, iconst_3);
        RunTest(ONE_TEST, iconst_4);
        RunTest(ONE_TEST, iconst_5);
        RunTest(ONE_TEST, iand);
        RunTest(ONE_TEST, iadd);
        RunTest(ONE_TEST, invokestatic);

        JavaVM::test_manual_mem_collection();
        //JavaVM::test_gc_mem_collection();
        break;
#ifndef NO_FREERTOS
        vTaskDelay(2000 / portTICK_RATE_MS);
#endif

    }
}


int main()
{

    // Get char - to do not start programm immediatly 
    //getchar();

    puts("Interpreter executed\n\r");

    // Call default test case
    getchar();

    srand(time(0));
#ifndef NO_FREERTOS
    xTaskCreate(vTask, "vTask", 20480, NULL, 3, NULL);

    while(1);
#else
    vTask();
#endif

    return 0;
}


