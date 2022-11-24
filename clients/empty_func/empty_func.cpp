#include <cstdint>
#include <iostream>
#include <vector>

thread_local uint64_t depth, lr;
thread_local std::vector<uint64_t> saved_lr;

void my_constructor1(void) __attribute__((constructor));
void my_constructor2(void) __attribute__((constructor(102)));
void my_constructor3(void) __attribute__((constructor(103)));
void my_destructor1(void) __attribute__((destructor));
void my_constructor1(void) /* This is the 3rd constructor */
{                        /* function to be called */
    printf("Called my_constructor1()\n");
}
void my_constructor2(void) /* This is the 1st constructor */
{                         /* function to be called */
    printf("Called my_constructor2()\n");
}
void my_constructor3(void) /* This is the 2nd constructor */
{                         /* function to be called */
    printf("Called my_constructor3()\n");
}
void my_destructor1(void) /* This is the 2nd constructor */
{                         /* function to be called */
    printf("Called my_destructor1()\n");
}

extern "C" void trace_entry_func(uint64_t lr) {
    saved_lr.push_back(lr);
    depth++;
    return;
}

extern "C" uint64_t trace_exit_func() {
    depth--;
    lr = saved_lr[depth];
    saved_lr.pop_back();
    return lr;
}
