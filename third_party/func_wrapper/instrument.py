from archinfo import ArchAArch64
from collections import defaultdict
from librw_arm64.container import (DataCell, InstrumentedInstruction, Section,
                             Function)
from librw_arm64.util.logging import *
from librw_arm64.container import INSTR_SIZE
from librw_arm64.util.arm_util import get_reg_size_arm, get_access_size_arm, is_reg_32bits, get_64bits_reg, non_clobbered_registers
import random
import os

class Instrument():

    #def __init__(self, rewriter, func_name, params_num):
    def __init__(self, rewriter, func_name):
        self.rewriter = rewriter
        self.func_name = func_name
        #self.params_num = params_num

        # Get the register map
        aarch64 = ArchAArch64()
        self.regmap = defaultdict(lambda: defaultdict(dict))
        for reg in aarch64.register_list:
            if reg.general_purpose:
                self.regmap[reg.name] = reg.subregisters[0][0]

    def get_mem_instrumentation(self, instruction, idx, free, is_entry):
        enter_lbl = "COUNTER_%x" % (instruction.address)

        if is_entry:
            instrumentation = trampoline_entry
        else:
            instrumentation = trampoline_exit
        comment = "{}: {}".format(str(instruction), str(free))

        return InstrumentedInstruction(instrumentation, enter_lbl, comment)

    def do_instrument(self):
        for faddr, fn in self.rewriter.container.functions.items():
            #if fn.name == "randlc_": #cg ep
            #if fn.name == "conj_grad_": #cg
            #if fn.name == "verify_": #ep
            #if fn.name == "fftz2_": #ft
            #if fn.name == "randlc": #is
            #if fn.name == "rhs_": #lu
            #if fn.name == "resid_": #mg
            #if fn.name == "transfb_":#ua
            #if fn.name == "compute_rhs_":#bt sp
            if fn.name == self.func_name:
                free_registers = fn.analysis['free_registers'][3] if fn.analysis else None
                fn.cache[0].instrument_before(self.get_mem_instrumentation(fn.cache[2], 2, free_registers, True))
                for idx, instruction in enumerate(fn.cache):
                    if instruction.mnemonic == "ret":
                        free_registers = fn.analysis['free_registers'][idx] if fn.analysis else None
                        iinstr = self.get_mem_instrumentation(instruction, idx, free_registers, False)
                        instruction.instrument_before(iinstr)
        payload = inserted_func_pmu
        '''
        if self.params_num == 2:
            payload = inserted_func_with_two_params
        elif self.params_num == 3:
            payload = inserted_func_with_three_params
        elif self.params_num == 4:
            payload = inserted_func_with_four_params
        elif self.params_num == 8:
            payload = inserted_func_with_eight_params
        elif self.params_num == 9:
            payload = inserted_func_with_nine_params
        '''
        afl_sec = Section(".afl_sec", 0x200000, 0, None)
        afl_sec.cache.append(DataCell.instrumented(payload, 0))

        self.rewriter.container.add_data_section(afl_sec)


trampoline_entry = """
// trampoline
stp x29, lr, [sp, #-16]!
bl __entry_func
ldp x29, lr, [sp], #16
"""

trampoline_exit = """
// trampoline
stp x29, lr, [sp, #-16]!
bl __exit_func
ldp x29, lr, [sp], #16
"""

inserted_func_pmu = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_entry_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""

inserted_func_with_one_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
mov x1, x0
movz x0, #1
stp x7, x10, [sp, #-16]!
bl trace_entry_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""


inserted_func_with_two_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
mov x2, x1
mov x1, x0
movz x0, #2
stp x7, x10, [sp, #-16]!
bl trace_entry_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""

inserted_func_with_eight_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
mov x7, x6
mov x6, x5
mov x5, x4
mov x4, x3
mov x3, x2
mov x2, x1
mov x1, x0
movz x0, #8
ldp x8, x9, [sp, #32]
stp x9, x8, [sp, #-16]!
bl trace_entry_func
add sp, sp, #16
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""


inserted_func_with_two_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
mov x2, x1
mov x1, x0
movz x0, #2
stp x7, x10, [sp, #-16]!
bl trace_entry_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""

inserted_func_with_nine_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
mov x7, x6
mov x6, x5
mov x5, x4
mov x4, x3
mov x3, x2
mov x2, x1
mov x1, x0
movz x0, #9
ldp x8, x9, [sp, #112]
ldp x10, x9, [sp, #32]
stp x9, x8, [sp, #-16]!
bl trace_entry_func
add sp, sp, #16
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""


inserted_func_with_two_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
mov x2, x1
mov x1, x0
movz x0, #2
stp x7, x10, [sp, #-16]!
bl trace_entry_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""

inserted_func_with_three_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
mov x3, x2
mov x2, x1
mov x1, x0
movz x0, #3
stp x7, x10, [sp, #-16]!
bl trace_entry_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""

inserted_func_with_four_params = """
.section inserted_func, "ax", @progbits
// entry_func
.type __entry_func, @function
.globl __entry_func
__entry_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
mov x4, x3
mov x3, x2
mov x2, x1
mov x1, x0
movz x0, #4
stp x7, x10, [sp, #-16]!
bl trace_entry_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
// exit_func
.type __exit_func, @function
.globl __exit_func
__exit_func:
stp x29, lr, [sp, #-16]!
mov x29, sp
stp x0, x1, [sp, #-16]!
stp x2, x3, [sp, #-16]!
stp x4, x5, [sp, #-16]!
stp x6, x7, [sp, #-16]!
stp x8, x9, [sp, #-16]!
mrs x7, nzcv
stp x7, x10, [sp, #-16]!
bl trace_exit_func
ldp x7, x10, [sp], #16
msr nzcv, x7
ldp x8, x9, [sp], #16
ldp x6, x7, [sp], #16
ldp x4, x5, [sp], #16
ldp x2, x3, [sp], #16
ldp x0, x1, [sp], #16
ldp x29, lr, [sp], #16
ret
"""

