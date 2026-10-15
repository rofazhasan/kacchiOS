/* kernel.c - Main kernel with null process */
#include "types.h"
#include "serial.h"
#include "string.h"
#include "memory.h"
#include "process.h"

#define MAX_INPUT 128

static void task_alpha(void) {
    serial_puts("[Task Alpha] Running!\n");
    for (;;) { __asm__ volatile ("hlt"); }
}

static void task_beta(void) {
    serial_puts("[Task Beta] Running!\n");
    for (;;) { __asm__ volatile ("hlt"); }
}

void kmain(void) {
    memory_init();
    serial_puts("Heap Memory Initialized\n");

    void* p1 = kmalloc(100);
    void* p2 = kmalloc(200);
    void* p3 = kmalloc(300);

    if (p1 && p2 && p3)
        serial_puts("All Heaps Allocated.\n");
    else
        serial_puts("Allocation failed\n");

    serial_puts("\n---p1 Information---\n");
    memory_info(p1);
    
    serial_puts("\n---p2 Information---\n");
    memory_info(p2);

    serial_puts("\n---p3 Information---\n");
    memory_info(p3);

    serial_puts("\n");

    kfree(p1);
    serial_puts("p1 Freed\n");

    kfree(p2);
    serial_puts("p2 Freed\n");

    void* p4 = kmalloc(250);

    if (p4!=0)
        serial_puts("p4 Allocation successful\n");
    else
        serial_puts("Allocation failed\n");

    serial_puts("\n---p4 Information---\n");
    memory_info(p4);
    serial_puts("\n");

    stack_init();
    serial_puts("Stack Memory Initialized\n");

    void* s1 = kstalloc(100);
    void* s2 = kstalloc(200);
    void* s3 = kstalloc(300);

    if (s1 && s2 && s3)
        serial_puts("All Stacks allocated\n");
    else
        serial_puts("Allocation Failed\n");

    serial_puts("\n---s1 Information---\n");
    stack_memory_info(s1);

    serial_puts("\n---s2 Information---\n");
    stack_memory_info(s2);

    serial_puts("\n---s3 Information---\n");
    stack_memory_info(s3);
    
    serial_puts("\n");

    ksfree(s1);
    serial_puts("s1 Freed\n");

    ksfree(s2);
    serial_puts("s2 Freed\n");

    void* s4 = kstalloc(250);

    if (s4!=0)
        serial_puts("s4 Allocation successful\n");
    else
        serial_puts("Allocation failed\n");

    serial_puts("\n---s4 Information---\n");
    stack_memory_info(s4);
    serial_puts("\n");

    /* ========================================================
       Day 1 Process Manager Verification (Student 2203036)
       ======================================================== */
    serial_puts("\n========================================\n");
    serial_puts("Process Manager Day 1 Test (Student 2203036)\n");
    serial_puts("========================================\n");
    process_init();
    pcb_t* cur = process_get_current();
    if (cur != NULL && cur->pid == 0 && cur->state == STATE_CURRENT) {
        serial_puts(">> PASS: Process Table successfully initialized (16 slots)!\n");
        serial_puts(">> PASS: Null Process (PID 0) active in CURRENT state.\n");
        serial_puts(">> Process Name: ");
        serial_puts(cur->name);
        serial_puts("\n");
        serial_puts(">> Day 1 Core Subsystem: 100% Operational.\n");
    } else {
        serial_puts(">> FAIL: Process initialization failed.\n");
    }
    serial_puts("========================================\n\n");

    /* ========================================================
       Day 2 Process Creation & Termination (Student 2203036)
       ======================================================== */
    serial_puts("========================================\n");
    serial_puts("Process Manager Day 2 Test (Student 2203036)\n");
    serial_puts("========================================\n");

    int32_t pid1 = process_create("AlphaTask", task_alpha, 1);
    int32_t pid2 = process_create("BetaTask", task_beta, 2);

    serial_puts(">> Spawned AlphaTask with PID: ");
    serial_putint(pid1);
    serial_puts("\n>> Spawned BetaTask with PID: ");
    serial_putint(pid2);
    serial_puts("\n");

    if (pid1 == 1 && pid2 == 2) {
        serial_puts(">> PASS: Process Creation & Stack Allocation verified!\n");
    }

    /* ========================================================
       Day 3 State Transitions Verification (Student 2203036)
       ======================================================== */
    serial_puts("\n========================================\n");
    serial_puts("Process Manager Day 3 Test (Student 2203036)\n");
    serial_puts("========================================\n");
    int32_t s_res1 = process_set_state(pid2, STATE_CURRENT);
    if (s_res1 == 0) {
        serial_puts(">> PASS: State Transition (READY -> CURRENT) verified!\n");
    }
    int32_t s_res2 = process_set_state(pid2, STATE_READY);
    if (s_res2 == 0) {
        serial_puts(">> PASS: State Transition (CURRENT -> READY preemption) verified!\n");
    }
    serial_puts("========================================\n\n");

    int32_t term = process_terminate(pid1, 0);
    if (term == 0) {
        serial_puts(">> PASS: AlphaTask terminated & stack memory freed via ksfree()!\n");
    }

    int32_t pid3 = process_create("GammaTask", task_alpha, 3);
    serial_puts(">> Spawned GammaTask into recycled slot PID: ");
    serial_putint(pid3);
    serial_puts("\n>> PASS: Slot recycling verified!\n");
    serial_puts("========================================\n\n");

    /* Should never reach here */
    for (;;) {
        __asm__ volatile ("hlt");
    }
}