/* **********************************************************
 * Copyright (c) 2016-2021 ARM Limited. All rights reserved.
 * **********************************************************/

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of ARM Limited nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL ARM LIMITED OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#ifndef CODEC_H
#define CODEC_H 1

#include <stdbool.h>
#include <string.h>
#define DR_FAST_IR 
typedef unsigned char byte;
#include "instr_opnd_api.h"
#include "instr_inline_api.h"


#define ENCFAIL (uint)0xFFFFFFFF /* a value that is not a valid instruction */

int
instr_num_srcs(instr_t *instr);

int
instr_num_dsts(instr_t *instr);

bool
instr_is_app(instr_t *instr);

bool
instr_raw_bits_valid(instr_t *instr);

bool
instr_has_allocated_bits(instr_t *instr);

bool
instr_operands_valid(instr_t *instr);

/** Assumes opnd is an immediate integer and returns its value. */
ptr_int_t
opnd_get_immed_int(opnd_t opnd);

/** Assumes \p opnd is an instr_t (near, far, or memory) operand and returns its value. */
instr_t *
opnd_get_instr(opnd_t opnd);

opnd_t
opnd_create_reg(reg_id_t r);

opnd_t
opnd_create_reg_partial(reg_id_t r, opnd_size_t subsize);

opnd_t
opnd_create_reg_ex(reg_id_t r, opnd_size_t subsize, dr_opnd_flags_t flags);

bool
opnd_is_instr(opnd_t op);

void *
instr_get_note(instr_t *instr);

opnd_t
opnd_create_pc(app_pc pc);

typedef enum {
    BYTE_REG = 0,
    HALF_REG = 1,
    SINGLE_REG = 2,
    DOUBLE_REG = 3,
    QUAD_REG = 4,
    NOT_A_REG = DR_REG_INVALID
} aarch64_reg_offset;

#define BITS(_enc, bitmax, bitmin)    \
    ((((uint32)(_enc)) >> (bitmin)) & \
     (uint32)((1ULL << ((bitmax) - (bitmin) + 1)) - 1ULL))

#endif /* CODEC_H */
