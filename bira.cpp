#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <vector>
#include <string>

#include "codec.h"

#include <LIEF/ELF.hpp>
#include <LIEF/enums.hpp>
#include "BPatch.h"
#include "Symtab.h"
#include "Function.h"
#include "Variable.h"
#include "Module.h"

#include "BPatch_binaryEdit.h"
#include "BPatch_addressSpace.h"
#include "BPatch_function.h"
#include "BPatch_image.h"
#include "BPatch_point.h"
#include "BPatch_process.h"
#include "BPatch_thread.h"
#include "BPatch_type.h"

using namespace LIEF::ELF;

using namespace std;

using namespace Dyninst;

// to-do: replace LIEF with dyninst.
vector<instr_t*>
decode(LIEF::span<const unsigned char> &text_content, uint file_offset, vector<uint> &ins_code_list)
{
    vector<instr_t*> decode_list;
    for(int i = 0; i < text_content.size(); i+=4) {
        instr_t *ins = instr_create();
	instr_init(ins);
	byte* pc = &text_content[i];
	decode_common(pc, file_offset + i, ins);
	if(ins->opcode == OP_udf) {
	    printf("udf: %u\n", *(uint*)pc);
	}
	/* can do some modify on ins!*/
	ins_code_list.push_back(*(uint*)pc);
	decode_list.push_back(ins);
    }

    return decode_list;
}

void
encode(vector<instr_t*> &instr_list, uint file_offset, vector<uint> &ins_code_list) {
    int count = 0;
    for(auto ins : instr_list) {
	byte* pc = ins_code_list[count];
    	decode_info_t di;
	uint tmp = encode_common(count*4 + file_offset, ins, &di);
	if(tmp == 0xffffffff) {
	    printf("opcode = %d, udf: %u\n", ins->opcode, pc);
	}
	ins_code_list[count] = tmp;
	count ++;
    } 
}

vector<uint8_t>
getByteslist(vector<instr_t*> &instr_list, vector<uint> &ins_code_list)
{
    vector<uint8_t> data;
    int count = 0;
    for(auto i : instr_list) {
        byte* pc = (byte*)&ins_code_list[count];
        data.push_back(*pc);
        data.push_back(*(pc+1));
        data.push_back(*(pc+2));
        data.push_back(*(pc+3));
	count ++;
    }
    return data;
}

static Address base, entry_func, exit_func;
uint8_t params_num;
vector<vector<reg_id_t>> entry_exit_used_regs;
reg_id_t DR_REG_ST1 = DR_REG_X18;

void
save_regs(vector<instr_t*> &ins_list, vector<uint> &ins_code_list, vector<reg_id_t> &regs)
{
    for(int i = 0; i < regs.size() - 1; i+=2) {
	// stp reg0, reg1, [sp, #-0x10]!
	printf("%d, %d\n", regs[i], regs[i+1]);
    	instr_t * stp1_ins = INSTR_CREATE_stp(opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, -16, OPSZ_16), opnd_create_reg(regs[i]), opnd_create_reg(regs[i+1]));
    	ins_list.push_back(stp1_ins);
    	ins_code_list.push_back(0);
    	// sub sp sp 16
    	instr_t* sub1_ins = INSTR_CREATE_sub(opnd_create_reg(DR_REG_XSP), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(16));
    	ins_list.push_back(sub1_ins);
    	ins_code_list.push_back(0);
    }
}

void
restore_regs(vector<instr_t*> &ins_list, vector<uint> &ins_code_list, vector<reg_id_t> &regs)
{
    for(int i = regs.size() - 1; i >=0 ; i-=2) {
	// ldp reg1, reg0, [sp]
        instr_t * ldp4_ins = INSTR_CREATE_ldp(opnd_create_reg(regs[i-1]), opnd_create_reg(regs[i]), opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, 0, OPSZ_16));
        ins_list.push_back(ldp4_ins);
        ins_code_list.push_back(0);
        // add sp sp 16
        instr_t* add4_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_XSP), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(16));
        ins_list.push_back(add4_ins);
        ins_code_list.push_back(0);
    }
}

// save NZCV FPCR FPSR
void
save_aflgs(vector<uint> &ins_code_list, vector<instr_t*> &ins_list, int stack_off)
{
    instr_t * mrs_ins1 = INSTR_CREATE_mrs(opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_NZCV));
    ins_list.push_back(mrs_ins1);
    ins_code_list.push_back(0);
    instr_t * mrs_ins2 = INSTR_CREATE_mrs(opnd_create_reg(DR_REG_X30), opnd_create_reg(DR_REG_FPCR));
    ins_list.push_back(mrs_ins2);
    ins_code_list.push_back(0);
    instr_t * stp_aflgs_ins = INSTR_CREATE_stp(opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, stack_off, OPSZ_16), opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_X30));
    ins_list.push_back(stp_aflgs_ins);
    ins_code_list.push_back(0);
    instr_t * mrs_ins3 = INSTR_CREATE_mrs(opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_FPSR));
    ins_list.push_back(mrs_ins3);
    ins_code_list.push_back(0);
    instr_t * stp_aflgs_ins2 = INSTR_CREATE_stp(opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, stack_off - 16, OPSZ_16), opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_X30));
    ins_list.push_back(stp_aflgs_ins2);
    ins_code_list.push_back(0);
}

// restore NZCV FPCR FPSR
void
restore_aflgs(vector<uint> &ins_code_list, vector<instr_t*> &ins_list)
{
    // add sp here
    instr_t* add_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_XSP), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(32));
    ins_list.push_back(add_ins);
    ins_code_list.push_back(0);

    instr_t * ldp_aflgs_ins2 = INSTR_CREATE_ldp(opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_X30), opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0,-32, OPSZ_16));
    ins_list.push_back(ldp_aflgs_ins2);
    ins_code_list.push_back(0);
    instr_t * msr_ins3 = INSTR_CREATE_msr(opnd_create_reg(DR_REG_FPSR), opnd_create_reg(DR_REG_ST1));
    ins_list.push_back(msr_ins3);
    ins_code_list.push_back(0);
    instr_t * ldp_aflgs_ins = INSTR_CREATE_ldp(opnd_create_reg(DR_REG_X30), opnd_create_reg(DR_REG_X30), opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, -16, OPSZ_16));
    ins_list.push_back(ldp_aflgs_ins);
    ins_code_list.push_back(0);
    instr_t * msr_ins2 = INSTR_CREATE_msr(opnd_create_reg(DR_REG_FPCR), opnd_create_reg(DR_REG_X30));
    ins_list.push_back(msr_ins2);
    ins_code_list.push_back(0);
    instr_t * msr_ins1 = INSTR_CREATE_msr(opnd_create_reg(DR_REG_NZCV), opnd_create_reg(DR_REG_ST1));
    ins_list.push_back(msr_ins1);
    ins_code_list.push_back(0);
}

void
restore_replaced_ins(vector<instr_t *> &replaced_instrs, vector<uint> &ins_code_list, vector<instr_t*> &ins_list, bool isBlr)
{
    // modify sp?
    int i = (isBlr) ? 1 : 0;
    for(; i < replaced_instrs.size(); i++) {
	auto ins = replaced_instrs[i];
        int opcode = instr_get_opcode(ins);
	if(opcode == OP_bl) {
	    // to-do use adrp and blr to replace
	    uint target = opnd_get_pc(instr_get_target(ins));
	    instr_t * adrp_ins2 = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_X30), OPND_CREATE_ABSMEM((void *)((target >> 12) << 12), OPSZ_0));
    	    ins_list.push_back(adrp_ins2);
    	    ins_code_list.push_back(0);
    	    instr_t* add_off_ins2 = INSTR_CREATE_add(opnd_create_reg(DR_REG_X30), opnd_create_reg(DR_REG_X30), OPND_CREATE_INT32((target << 20) >> 20));
    	    ins_list.push_back(add_off_ins2);
    	    ins_code_list.push_back(0);
    	    instr_t * blr_ins2 = INSTR_CREATE_blr(opnd_create_reg(DR_REG_X30));
    	    ins_list.push_back(blr_ins2);
    	    ins_code_list.push_back(0);
	} else {
	    // if the third ins is cmp, maybe ldp and add will change arithmetic flags?
	    ins_list.push_back(ins);
            ins_code_list.push_back(0);
	}	
    }
}

// add blr support(blr flg)
// compare the blr reg with func_addr(adrp add to a reg)
// if match, call hook blr hook
// else blr and ret
//
// adrp
// add
// cmp reg1 reg2
// if equal continue hook
// else blr and ret
void
handleBlr(vector<instr_t *> &instrs, uint func_addr, int bOffset, vector<uint> &ins_code_list, vector<instr_t*> &ins_list, bool replace_before)
{
    instr_t * adrp_ins = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_X30), OPND_CREATE_ABSMEM((void *)((func_addr >> 12) << 12), OPSZ_0));
    ins_list.push_back(adrp_ins);
    ins_code_list.push_back(0);
    instr_t* add_off_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_X30), opnd_create_reg(DR_REG_X30), OPND_CREATE_INT32(func_addr & 0xfff));
    ins_list.push_back(add_off_ins);
    ins_code_list.push_back(0);
    instr_t* cmp_ins = INSTR_CREATE_cmp(opnd_create_reg(DR_REG_X30), opnd_create_reg(opnd_get_reg(instr_get_src(instrs[0], 0))));
    ins_list.push_back(cmp_ins);
    ins_code_list.push_back(0);

    // if eq, jump to continue after ret
    app_pc pc = replace_before ? bOffset+8*4:bOffset+(8 + instrs.size())*4;
    instr_t* beq_ins = XINST_CREATE_jump_cond(DR_PRED_EQ, opnd_create_pc(pc));
    ins_list.push_back(beq_ins);
    ins_code_list.push_back(0);
    
    // call func
    ins_list.push_back(instrs[0]);
    ins_code_list.push_back(0);

    // aflgs attention!
    // add sp sp 16
    instr_t* add_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_XSP), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(16));
    ins_list.push_back(add_ins);
    ins_code_list.push_back(0);
    // ldp x29, x30, [sp, #0x10]
    instr_t * ldp_ins = INSTR_CREATE_ldp(opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_X30), opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, -16, OPSZ_16));
    ins_list.push_back(ldp_ins);
    ins_code_list.push_back(0);

    // restore the replace ins.
    if(!replace_before) {
        restore_replaced_ins(instrs, ins_code_list, ins_list, true);
    }

    // ret
    instr_t* ret_ins = INSTR_CREATE_ret(opnd_create_reg(DR_REG_X30));
    ins_list.push_back(ret_ins);
    ins_code_list.push_back(0);
}

void
call_external_func(vector<uint> &ins_code_list, vector<instr_t*> &ins_list, uint external_func_addr)
{
    // ldr hook addr and blr hook
    instr_t * hook_adrp_ins = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_X30), OPND_CREATE_ABSMEM((void *)((external_func_addr >> 12) << 12), OPSZ_0));
    ins_list.push_back(hook_adrp_ins);
    ins_code_list.push_back(0);
    instr_t* hook_add_off_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_X30), opnd_create_reg(DR_REG_X30), OPND_CREATE_INT32(external_func_addr & 0xfff));
    ins_list.push_back(hook_add_off_ins);
    ins_code_list.push_back(0);

    instr_t* ldr_ins = INSTR_CREATE_ldr(opnd_create_reg(DR_REG_X30),
                opnd_create_base_disp_aarch64(DR_REG_X30, DR_REG_NULL, 0, false,
                                              0, 0, OPSZ_8));
    ins_list.push_back(ldr_ins);
    ins_code_list.push_back(0);

    instr_t * hook_blr_ins = INSTR_CREATE_blr(opnd_create_reg(DR_REG_X30));
    ins_list.push_back(hook_blr_ins);
    ins_code_list.push_back(0);
}

void
push_params_below_X2930(vector<uint> &ins_code_list, vector<instr_t*> &ins_list, int stack_params_num)
{
    if(stack_params_num < 0) return;
    uint old_stack_off = stack_params_num * 16;
    for(int i = 0; i < stack_params_num; i++) {
	int count = (i + 1) * -16;
        instr_t * ldp_ins = INSTR_CREATE_ldp(opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_X30), opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, old_stack_off + count, OPSZ_16));
        ins_list.push_back(ldp_ins);
        ins_code_list.push_back(0);

        instr_t * stp_ins = INSTR_CREATE_stp(opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, count-16, OPSZ_16), opnd_create_reg(DR_REG_ST1), opnd_create_reg(DR_REG_X30));
        ins_list.push_back(stp_ins);
        ins_code_list.push_back(0);
    }

}

bool
restore_replaced_ins_before(int stack_params_num, vector<instr_t *> &replaced_instrs, vector<uint> &ins_code_list, vector<instr_t*> &ins_list, bool isBlr)
{
    bool use_sp = false;
    for(auto i : replaced_instrs) {
	if(i->opcode == OP_str || i->opcode == OP_stp) {
	    int op_num = instr_num_dsts(i);
	    // printf("opcode: %d\n", i->opcode);
	    for(int j = 0; j < op_num; j++) {
	        // printf("x3 == %d, sp == %d, opnd_get_reg(opnd): %d, op kind: %d\n", DR_REG_X3, DR_REG_XSP, opnd_get_reg(instr_get_dst(i, j)), instr_get_dst(i, j).kind);
	        if(opnd_uses_reg(instr_get_dst(i, j), DR_REG_XSP)) {
	            printf("opcode: %d\n", i->opcode);
		    use_sp = true;
		    break;
	        }
	    }
	} else {
	    int op_num = instr_num_srcs(i);
	    for(int j = 0; j < op_num; j++) {
                if(opnd_uses_reg(instr_get_src(i, j), DR_REG_XSP)) {
                    use_sp = true;
                    break;
                }
            }
	}
	if(use_sp) break;
    }

    // if replaced ins == OP_bl, we must mov sp first.
    if(!use_sp) {
        // sub sp sp 16
	int stack_sub_off = 16;
	if(params_num - 8 > 0) stack_sub_off += (stack_params_num << 4);
        instr_t* sub_aflg_ins1 = INSTR_CREATE_sub(opnd_create_reg(DR_REG_XSP), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(stack_sub_off));
        ins_list.push_back(sub_aflg_ins1);
        ins_code_list.push_back(0);
    }

    restore_replaced_ins(replaced_instrs, ins_code_list, ins_list, isBlr);   
    return use_sp;
}

static uint special_offset;
SymtabAPI::Region *special_section;

// append new ins into .special
// if isBlr, instrs[0] == blr
vector<uint8_t>
appendCode(vector<instr_t *> &instrs, uint func_addr, bool newSection, bool isBlr, bool replace_before) {
    uint file_offset;
    if(newSection) 
	file_offset = base;
    else 
        file_offset = special_section->getDiskOffset();
    vector<uint> ins_code_list;
    vector<instr_t*> ins_list;

    if(replace_before)
        restore_replaced_ins(instrs, ins_code_list, ins_list, isBlr);   

    // stp x29, x30, [sp, #-0x10]!
    instr_t * stp_ins1 = INSTR_CREATE_stp(opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, -16, OPSZ_16), opnd_create_reg(DR_REG_X29), opnd_create_reg(DR_REG_X30));
    ins_list.push_back(stp_ins1);
    ins_code_list.push_back(0);

    instr_t* sub_ins1 = INSTR_CREATE_sub(opnd_create_reg(DR_REG_XSP), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(16));
    ins_list.push_back(sub_ins1);
    ins_code_list.push_back(0);

    // instr_t * mov_x29_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_X29), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(0));
    // ins_list.push_back(mov_x29_ins);
    // ins_code_list.push_back(0);
    
    if(isBlr) {
	// use ins_list size to cal ins nums and update the offset!!!
	// restore the sp after move params!!!
        if(newSection)	
            handleBlr(instrs, func_addr, file_offset + 4 * ins_list.size(), ins_code_list, ins_list, replace_before);
	else
	    handleBlr(instrs, func_addr, file_offset + 4 * ins_list.size(), ins_code_list, ins_list, replace_before);
    }

    save_regs(ins_list, ins_code_list, entry_exit_used_regs[0]);
    
    instr_t * mov_x2_x1 = INSTR_CREATE_add(opnd_create_reg(DR_REG_X2), opnd_create_reg(DR_REG_X1), OPND_CREATE_INT32(0));
    ins_list.push_back(mov_x2_x1);
    ins_code_list.push_back(0);
    instr_t * mov_x1_x0 = INSTR_CREATE_add(opnd_create_reg(DR_REG_X1), opnd_create_reg(DR_REG_X0), OPND_CREATE_INT32(0));
    ins_list.push_back(mov_x1_x0);
    ins_code_list.push_back(0);

    /*
    // read thread id
    instr_t * mrs_thread_id_ins = INSTR_CREATE_mrs(opnd_create_reg(DR_REG_X1), opnd_create_reg(DR_REG_TPIDR_EL0));
    ins_list.push_back(mrs_thread_id_ins);
    ins_code_list.push_back(0);
    */	
    instr_t * save_x30_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_X0), opnd_create_reg(DR_REG_X30), OPND_CREATE_INT32(0));
    ins_list.push_back(save_x30_ins);
    ins_code_list.push_back(0);

    call_external_func(ins_code_list, ins_list, entry_func);

    restore_regs(ins_list, ins_code_list, entry_exit_used_regs[0]);

    instr_t * ldp_ins1 = INSTR_CREATE_ldp(opnd_create_reg(DR_REG_X29), opnd_create_reg(DR_REG_X30), opnd_create_base_disp(DR_REG_XSP, DR_REG_NULL, 0, 0, OPSZ_16));
    ins_list.push_back(ldp_ins1);
    ins_code_list.push_back(0);

    instr_t* add_ins1 = INSTR_CREATE_add(opnd_create_reg(DR_REG_XSP), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(16));
    ins_list.push_back(add_ins1);
    ins_code_list.push_back(0);

    /*
    instr_t * mov_x29_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_X29), opnd_create_reg(DR_REG_XSP), OPND_CREATE_INT32(0));
    ins_list.push_back(mov_x29_ins);
    ins_code_list.push_back(0);
*/

    if(isBlr) {
	ins_list.push_back(instrs[0]);
    	ins_code_list.push_back(0);
    } else {
    // blr func
    	instr_t * adrp_ins = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_X30), OPND_CREATE_ABSMEM((void *)((func_addr >> 12) << 12), OPSZ_0));
    	ins_list.push_back(adrp_ins);
    	ins_code_list.push_back(0);
    	instr_t* add_off_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_X30), opnd_create_reg(DR_REG_X30), OPND_CREATE_INT32((func_addr << 20) >> 20));
    	ins_list.push_back(add_off_ins);
    	ins_code_list.push_back(0);
    	instr_t * blr_ins = INSTR_CREATE_blr(opnd_create_reg(DR_REG_X30));
    	ins_list.push_back(blr_ins);
    	ins_code_list.push_back(0);
    }

    save_regs(ins_list, ins_code_list, entry_exit_used_regs[1]);

    call_external_func(ins_code_list, ins_list, exit_func);

    instr_t * restore_x30_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_X30), opnd_create_reg(DR_REG_X0), OPND_CREATE_INT32(0));
    ins_list.push_back(restore_x30_ins);
    ins_code_list.push_back(0);

    restore_regs(ins_list, ins_code_list, entry_exit_used_regs[1]);

    if(!replace_before) {
        restore_replaced_ins(instrs, ins_code_list, ins_list, isBlr);   
    }

    // ret
    instr_t* ret_ins = INSTR_CREATE_ret(opnd_create_reg(DR_REG_X30));
    ins_list.push_back(ret_ins);
    ins_code_list.push_back(0);

    encode(ins_list, file_offset, ins_code_list);
    vector<uint8_t> data = getByteslist(ins_list, ins_code_list);
    return data;
}

void
encode_section(SymtabAPI::Symtab *symTab, uint &wrapper_addr, vector<instr_t *> &instrs, uint func_addr, std::string &secName, bool newSection, bool isBlr, bool replaced_before)
{
    // save context (i.e. lr)
    // get page offset in lr: and lr lr 0xfff
    // subs lr lr (pc_offset & 0xfff)
    // b.ne XINST_CREATE_jump_cond(DR_PRED_NE, opnd_create_pc(ret addr))
    // blr (to .special accordingly)
    // ret
    vector<uint8_t> data;
    if(newSection) {
        // instrs.size() == 1, we need to replace 2 ins at all.
        // add blr flg
	unsigned char empty[] = {};
        int section_size = 1 << 12;// + regs.size() * 16;// + regs.size() * 32; // is BLR + x!!

        symTab->addRegion(base, (void*)(empty), section_size, secName.c_str(), SymtabAPI::Region::RT_TEXT, true);

        SymtabAPI::Region *new_section;
        if(!symTab->findRegion(new_section, secName.c_str()))
            cout << "findRegion err" << endl;

        wrapper_addr = new_section->getMemOffset();
        // data = encode_new_section(instrs, func_addr, isBlr, replaced_before);
        data = appendCode(instrs, func_addr, newSection, isBlr, replaced_before);
        base += section_size;

        unsigned char* rawData = new unsigned char[data.size()];
        for(int i = 0; i < data.size(); i++)
            rawData[i] = data[i];
        if(!new_section->setPtrToRawData((void*)rawData, data.size()))
            cout << "setPtrToRawData err!" << endl;
    } else {
        // instrs.size() == 2, we need to replace 3 ins at all.
        // no jumptable needed, just using adrp, add off, blr
        // pc_offset == i+2 th ins offset
        // (i+2)*4 + text_offset?
        wrapper_addr = special_section->getDiskOffset() + special_offset;

        // data = appendCode(func_addr, instrs, isBlr, replaced_before);
        data = appendCode(instrs, func_addr, newSection, isBlr, replaced_before);
        uint special_code_size = data.size();

        unsigned char* rawData = new unsigned char[data.size()];
        for(int i = 0; i < data.size(); i++)
            rawData[i] = data[i];
        if(!special_section->patchData(special_offset, (void*)rawData, special_code_size))
             cout << "special_section patchData err!" << endl;
        special_offset += special_code_size;
    }
    printf("wrapper_addr = %llx\n", wrapper_addr);
}

vector<instr_t*>
decode(vector<uint8_t> &text_content, uint file_offset)
{
    vector<instr_t*> decode_list;
    for(int i = 0; i < text_content.size(); i+=4) {
        instr_t *ins = instr_create();
	instr_init(ins);
	byte* pc = &text_content[i];
	decode_common(pc, file_offset + i, ins);
	decode_list.push_back(ins);
    }

    return decode_list;
}

vector<reg_id_t>
getEntryExitFuncUsedRegs(std::unique_ptr<const Binary> &lib, string &trace_func_name)
{
    Symbol * trace_func = lib->get_symbol(trace_func_name);
    printf("trace_func va = %d, size = %d\n", trace_func->value(), trace_func->size());
    vector<uint8_t> trace_func_content = lib->get_content_from_virtual_address(trace_func->value(), trace_func->size());

    vector<uint> ins_code_list;
    vector<instr_t*> decode_list = decode(trace_func_content, 0);

    set<reg_id_t> reg_set;
    for(int i = 0; i < decode_list.size(); i++) {
	int dst_num = instr_num_dsts(decode_list[i]);
	for(int j = 0; j < dst_num; j++) {
	    // reg_get_size
	    if(!opnd_is_reg(instr_get_dst(decode_list[i], j))) continue;
    	    reg_id_t reg = opnd_get_reg(instr_get_dst(decode_list[i], j));
	    if(reg >= DR_REG_W0 && reg < DR_REG_W29) reg -= (DR_REG_W0 - DR_REG_X0);
	    if(reg >= DR_REG_X0 && reg < DR_REG_ST1) reg_set.insert(reg);
	}
    }

    reg_set.insert(DR_REG_X0);
    reg_set.insert(DR_REG_X1);
    reg_set.insert(DR_REG_X2);
    reg_set.insert(DR_REG_X3);
    reg_set.insert(DR_REG_X4);
    reg_set.insert(DR_REG_X5);
    reg_set.insert(DR_REG_X6);
    reg_set.insert(DR_REG_X7);
    reg_set.insert(DR_REG_X8);
    reg_set.insert(DR_REG_X9);
    reg_set.insert(DR_REG_X10);

    vector<reg_id_t> regs;
    for(auto reg : reg_set) regs.push_back(reg);
    if(regs.size() % 2 != 0) regs.push_back(DR_REG_X29);

    return regs;
}

bool
check_opcode(int opcode) 
{
    switch(opcode) {
	case OP_bcond:
	case OP_cbnz:
	case OP_tbnz:
	case OP_cbz:
	case OP_tbz:

	case OP_b:
	case OP_bl:

	case OP_ret:
		return true;
	default: return false;
    }
    return false;
}

// stp x29, x30, [sp, #16]!
// can not replace it
bool check_stp_x30(instr_t * ins)
{
    if(ins->opcode != OP_stp) return false;
    for(int j = 0; j < 2; j++) {
        if(opnd_uses_reg(instr_get_src(ins, j), DR_REG_X30)) {
	    return true;
        }
    }
    return false;
}

void
modify_text_dyn(std::unique_ptr<const Binary> &binary, uint func_addr, SymtabAPI::Symtab *symTab)
{
    LIEF::ELF::Section* section = binary->text_section();
    LIEF::span<const unsigned char> text_content = section->content();
    uint file_offset = section->file_offset();
    cout << ".text offset = " << file_offset << endl;
    vector<uint> ins_code_list;
    vector<instr_t*> decode_list = decode(text_content, file_offset, ins_code_list);
    set<uint> branch_addrs;

    int count = 0;
    for(int i = 0; i < decode_list.size(); i++) {
        if(check_opcode(decode_list[i]->opcode)) {
	    branch_addrs.emplace(opnd_get_pc(instr_get_target(decode_list[i])));
	}
    }

    for(int i = 0; i < decode_list.size(); i++) {
	bool isBlr = false;

	if(decode_list[i]->opcode == OP_blr) {
	    isBlr = true;
	}

        if((isBlr || (decode_list[i]->opcode == OP_bl && opnd_get_pc(instr_get_target(decode_list[i])) == func_addr))) {
	    bool can_replace_before = (branch_addrs.find(file_offset + (i)*4) == branch_addrs.end());
	    bool can_replace_after = (branch_addrs.find(file_offset + (i+1)*4) == branch_addrs.end());
	    printf("%llx\n", file_offset + (i)*4);
	    if(!can_replace_before && !can_replace_after) {
	        printf("skip!\n");	
		continue;
	    }
	    printf("Got it!\n");
            uint wrapper_addr;

            vector<instr_t *> instrs;
	    if(isBlr) instrs.push_back(decode_list[i]);
	    
	    uint opcode_ins_3 = instr_get_opcode(decode_list[i-3]);
	    uint opcode_ins_2 = instr_get_opcode(decode_list[i-2]);
	    uint opcode_ins_1 = instr_get_opcode(decode_list[i-1]);

	    reg_id_t DR_REG_BR = DR_REG_X30;
            instr_t * blr_ins = INSTR_CREATE_blr(opnd_create_reg(DR_REG_X30));
            // instr_t * blr_ins = INSTR_CREATE_br(opnd_create_reg(DR_REG_BR));

	    // try to replace ins after bl
	    if(check_stp_x30(decode_list[i-2]) || check_opcode(opcode_ins_1) || !can_replace_before) {
	        uint opcode_ins1 = instr_get_opcode(decode_list[i+1]);
	        uint opcode_ins2 = instr_get_opcode(decode_list[i+2]);
		if(check_stp_x30(decode_list[i+1]) || check_opcode(opcode_ins1) || !can_replace_after) {
		    printf("skip!\n");
		    continue;
		}
		instrs.push_back(decode_list[i+1]);
		if(check_stp_x30(decode_list[i+2]) || (opcode_ins2 != OP_b && check_opcode(opcode_ins2)) || branch_addrs.find(file_offset + (i+2)*4) != branch_addrs.end()) {
		    std::string secName = ".mysection" + std::to_string(i);
                    encode_section(symTab, wrapper_addr, instrs, func_addr, secName, true, isBlr, false);

                    instr_t * adrp_ins = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_BR), OPND_CREATE_ABSMEM((void *)((wrapper_addr >> 12) << 12), OPSZ_0));
                    decode_list[i] = adrp_ins;
                    decode_list[i+1] = blr_ins;
                    i++;
		} else {
		    instrs.push_back(decode_list[i+2]);
		    std::string secName = ".special";
		    encode_section(symTab, wrapper_addr, instrs, func_addr, secName, false, isBlr, false);

                    instr_t * adrp_ins = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_BR), OPND_CREATE_ABSMEM((void *)((wrapper_addr >> 12) << 12), OPSZ_0));
	            instr_t* add_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_BR), opnd_create_reg(DR_REG_BR), OPND_CREATE_INT32((wrapper_addr << 20) >> 20));
                    decode_list[i] = adrp_ins;
                    decode_list[i+1] = add_ins;
                    decode_list[i+2] = blr_ins;
                    i+=2;
		}
		continue;
	    }

	    if(check_opcode(opcode_ins_2) || branch_addrs.find(file_offset + (i-1)*4) != branch_addrs.end()) {
                instrs.push_back(decode_list[i-1]);
                std::string secName = ".mysection" + std::to_string(i);
                encode_section(symTab, wrapper_addr, instrs, func_addr, secName, true, isBlr, true);

                instr_t * adrp_ins = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_BR), OPND_CREATE_ABSMEM((void *)((wrapper_addr >> 12) << 12), OPSZ_0));
                decode_list[i-1] = adrp_ins;
                decode_list[i] = blr_ins;
	    } else {
                instrs.push_back(decode_list[i-2]);
                instrs.push_back(decode_list[i-1]);
		std::string secName = ".special";
		encode_section(symTab, wrapper_addr, instrs, func_addr, secName, false, isBlr, true);

                instr_t * adrp_ins = INSTR_CREATE_adrp(opnd_create_reg(DR_REG_BR), OPND_CREATE_ABSMEM((void *)((wrapper_addr >> 12) << 12), OPSZ_0));
	        instr_t* add_ins = INSTR_CREATE_add(opnd_create_reg(DR_REG_BR), opnd_create_reg(DR_REG_BR), OPND_CREATE_INT32((wrapper_addr << 20) >> 20));
                decode_list[i-2] = adrp_ins;
                decode_list[i-1] = add_ins;
                decode_list[i] = blr_ins;
	    }
        }
        count ++;
    }
    encode(decode_list, file_offset, ins_code_list);

    SymtabAPI::Region *text_section;
    if(!symTab->findRegion(text_section, ".text"))
        cout << "findRegion .text err" << endl;
    vector<uint8_t> data = getByteslist(decode_list, ins_code_list);
    unsigned char* rawData = new unsigned char[data.size()];
    for(int i = 0; i < data.size(); i++)
        rawData[i] = data[i];
    if(!text_section->setPtrToRawData((void*)rawData, data.size()))
        cout << ".text setPtrToRawData err!" << endl;
    
}

void addSpecialSectionPages(SymtabAPI::Symtab *symTab) {
    static unsigned char empty[] = {};
    int pageSize = 1 << 12;

    base = symTab->getFreeOffset(50*1024*1024);
    base += (1024*1024);
    base -= (base & (1024*1024-1));

    symTab->addRegion(base, (void*)(empty), 48, ".extsym", SymtabAPI::Region::RT_TEXTDATA, true);

    SymtabAPI::Region *extsym_section;
    if(!symTab->findRegion(extsym_section, ".extsym"))
        cout << "findRegion .extsym err" << endl;
    base += 48;

    symTab->addRegion(base, (void*)(empty), 4 * pageSize - 48, ".special", SymtabAPI::Region::RT_TEXT, true);

    if(!symTab->findRegion(special_section, ".special"))
        cout << "findRegion .special err" << endl;
    base += 4 * pageSize - 48;
}

static int reloc_num = 0;
Address addExternalFuncSymbol(SymtabAPI::Symtab *symTab, SymtabAPI::Symtab *lib, string func_name) {
    vector<SymtabAPI::Function*> ret;
    if(!lib->findFunctionsByName(ret, move(func_name))) {
	cout << "cannot find " << endl;
	return false;
    }

    vector<SymtabAPI::Symbol *> syms;

    if(!ret[0]->getSymbols(syms)) {
	cout << "cannot find symbols that refer " << endl;
        return false;
    }

    // try to find a dynamic symbol
    // (take first static symbol if none are found)
    SymtabAPI::Symbol *referring = syms[0];
    for (unsigned k=0; k<syms.size(); k++) {
        if (syms[k]->isInDynSymtab()) {
            referring = syms[k];
            break;
        }
    }

    unsigned int jump_slot_size;
    switch (symTab->getAddressWidth()) {
    case 4: jump_slot_size =  4; break; // l: not needed
    case 8:
      jump_slot_size = 24;
      break;
    default: assert(0 && "Encountered unknown address width");
    }

    Address relocation_address = base - 4 * (1 << 12) + reloc_num * jump_slot_size;// add into .extsym
    reloc_num++;

    Dyninst::SymtabAPI::Region *extsym = NULL;
    symTab->findRegion(extsym, ".extsym");
    assert(extsym);
    // Create the relocationEntry
    Dyninst::SymtabAPI::relocationEntry localRel(relocation_address, referring->getMangledName(), referring,
                             Dyninst::SymtabAPI::relocationEntry::getGlobalRelType(symTab->getAddressWidth(), referring));

    symTab->addExternalSymbolReference(referring, extsym, localRel);

    return relocation_address;
}

/* agrv[1] == <exe> argv[2] == <lib> argv[3] == <func>*/
int main(int argc, char** argv) {
    std::unique_ptr<const Binary> binary{Parser::parse(argv[1])};
    std::unique_ptr<const Binary> lib{Parser::parse(argv[2])};
    char *binaryPath = argv[2];
    SymtabAPI::Symtab *symTab, *libhook;

    string libhookPathStr(binaryPath);
    if(!SymtabAPI::Symtab::openFile(libhook, libhookPathStr)) {
        cerr << "error: file cannot be parsed";
        return -1;
    }

    binaryPath = argv[1];
    string binaryPathStr(binaryPath);

    if(!SymtabAPI::Symtab::openFile(symTab, binaryPathStr)) {
        cerr << "error: file cannot be parsed";
        return -1;
    }

    vector<SymtabAPI::Function*> target_func;
    if(!symTab->findFunctionsByName(target_func, move(argv[3]))) {
        cout << "cannot find " << argv[3] << endl;
        return -1;
    }

    uint func_addr = target_func[0]->getOffset();
    printf("target_func offset: %llx\n", func_addr);

    vector<SymtabAPI::localVar *> params;
    if(!target_func[0]->getParams(params)) {
        params_num = 0;
	// cout << "cannot getParams for " << argv[3] << endl;
        //return -1;
    }

    params_num = params.size();
    printf("params_num: %d\n", params_num);

    addSpecialSectionPages(symTab);

    string entry_func_name("trace_entry_func");
    entry_func = addExternalFuncSymbol(symTab, libhook, entry_func_name);
    assert(entry_func);
    printf("entry_func = %llx\n", entry_func);
    vector<reg_id_t> regs0 = getEntryExitFuncUsedRegs(lib, entry_func_name);
    printf("X0 = %u\n", DR_REG_X0);
    for(auto reg : regs0) printf("%u\n", reg);

    string exit_func_name("trace_exit_func");
    exit_func = addExternalFuncSymbol(symTab, libhook, exit_func_name);
    assert(exit_func);
    printf("exit_func = %llx\n", exit_func);
    vector<reg_id_t> regs1 = getEntryExitFuncUsedRegs(lib, exit_func_name);
    printf("X0 = %u\n", DR_REG_X0);
    for(auto reg : regs1) printf("%u\n", reg);

    entry_exit_used_regs.push_back(regs0);
    entry_exit_used_regs.push_back(regs1);

    modify_text_dyn(binary, func_addr, symTab);
    if(!symTab->addLibraryPrereq(argv[2])) {
        cerr << "error: add library";
        return -1;
    }

    string outfile(argv[1]);
    outfile += ".dyn"; 

    if (!symTab->emit(outfile)) {
	 cout << "emit new binary error!\n";
         return -1;
    }

    return 0;
}
