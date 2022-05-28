#include "ZoraGA/RV32I.h"

namespace ZoraGA::RVVM::RV32
{

rv_err RV32I::isValid(rv32_inst_fmt inst)
{
    return ( op_aa_match(inst) && op_bbb_match(inst) && op_cc_match(inst) ) ? RV_EOK : RV_EUNDEF;
}

rv_err RV32I::exec(rv32_inst_fmt inst, rv32_regs_base &regs, rv32_mem_infos &mem_infos)
{
    bool ok = false;
    rv_err err = RV_EUNDEF;
    inst_args args;
    do{
        args.inst = inst;
        args.regs = &regs;
        args.mems = &mem_infos;
        if (inst.cc == 0b00) {
            switch(inst.bbb) {
                /* I type, lb/lh/lw/lbu/lhu */
                case 0b000:
                    if (inst.I.funct3 == 0b000) {
                        err = lb(args);
                    } else if (inst.I.funct3 == 0b001) {
                        err = lh(args);
                    } else if (inst.I.funct3 == 0b010) {
                        err = lw(args);
                    } else if (inst.I.funct3 == 0b100) {
                        err = lbu(args);
                    } else if (inst.I.funct3 == 0b101) {
                        err = lhu(args);
                    } else {
                        // err = RV_EUNDEF;
                    }
                    break;

                /* No matched */
                case 0b001:
                    break;

                /* I type, fence */
                case 0b011:
                    if (inst.I.funct3 == 0b000) {
                        err = fence(args);
                    } else {
                        // err = RV_EUNDEF;
                    }
                    break;

                /* I type, addi/slti/sltiu/xori/ori/andi/slli/srli/srai */
                case 0b100:
                    if (inst.I.funct3 == 0b000) {
                        err = addi(args);
                    } else if (inst.I.funct3 == 0b010) {
                        err = slti(args);
                    } else if (inst.I.funct3 == 0b011) {
                        err = sltiu(args);
                    } else if (inst.I.funct3 == 0b100) {
                        err = xori(args);
                    } else if (inst.I.funct3 == 0b110) {
                        err = ori(args);
                    } else if (inst.I.funct3 == 0b111) {
                        err = andi(args);
                    } else if (inst.I.funct3 == 0b001) {
                        err = slli(args);
                    } else if (inst.I.funct3 == 0b101) {
                        if (inst.I.imm_11_0 == 0) {
                            err = srli(args);
                        } else if (inst.I.imm_11_0 == 0b0100000) {
                            err = srai(args);
                        } else {
                            // err = RV_EUNDEF;
                        }
                    } else {
                        // err = RV_EUNDEF;
                    }
                    break;

                /* U type, auipc */
                case 0b101:
                    err = auipc(args);
                    break;

                /* No matched */
                case 0b110:
                    break;

                default:
                    break;
            }
            if (!ok) break;
        }
        else if (inst.cc == 0b01) {
            switch(inst.bbb) {
                /* S type, sb/sh/sw */
                case 0b000:
                    if (inst.S.funct3 == 0b000) {
                        err = sb(args);
                    } else if (inst.S.funct3 == 0b001) {
                        err = sh(args);
                    } else if (inst.S.funct3 == 0b010) {
                        err = sw(args);
                    } else {
                        err = RV_EUNDEF;
                    }
                    break;

                /* No matched */
                case 0b001:
                    break;
                
                /* No matched */
                case 0b011:
                    break;

                /* R type, add/sub/sll/slt/sltu/xor/srl/sra/or/and */
                case 0b100:
                    if (inst.R.funct3 == 0b000) {
                        if (inst.R.funct7 == 0) {
                            err = add(args);
                        } else if (inst.R.funct7 == 0b0100000) {
                            err = sub(args);
                        } else {
                            // err = RV_EUNDEF;
                        }
                    } else if (inst.R.funct3 == 0b001) {
                        err = sll(args);
                    } else if (inst.R.funct3 == 0b010) {
                        err = slt(args);
                    } else if (inst.R.funct3 == 0b011) {
                        err = sltu(args);
                    } else if (inst.R.funct3 == 0b100) {
                        err = xxor(args);
                    } else if (inst.R.funct3 == 0b101) {
                        if (inst.R.funct7 == 0) {
                            err = srl(args);
                        } else if (inst.R.funct7 = 0b0100000) {
                            err = sra(args);
                        } else {
                            // err = RV_EUNDEF;
                        }
                    } else if (inst.R.funct3 == 0b110) {
                        err = orr(args);
                    } else if (inst.R.funct3 == 0b111) {
                        err = andd(args);
                    }
                    break;

                /* U type, lui */
                case 0b101:
                    err = lui(args);
                    break;

                /* No matched */                
                case 0b110:
                    break;

                default:
                    break;
            }
            if (!ok) break;
        }
        else if (inst.cc == 0b11) {
            switch(inst.bbb) {
                /* B type, beq/bne/blt/bge/bltu/bgeu */
                if (inst.B.funct3 == 0b000) {
                    err = beq(args);
                } else if (inst.B.funct3 == 0b001) {
                    err = bne(args);
                } else if (inst.B.funct3 == 0b100) {
                    err = blt(args);
                } else if (inst.B.funct3 = 0b101) {
                    err = bge(args);
                } else if (inst.B.funct3 == 0b110) {
                    err = bltu(args);
                } else if (inst.B.funct3 == 0b111) {
                    err = bgeu(args);
                } else {
                    // err = RV_EUNDEF;
                }
                case 0b000:
                    break;

                /* I type, jalr */
                case 0b001:
                    err = jalr(args);
                    break;

                /* No matched */
                case 0b011:
                    break;

                /* I type, ecall/ebreak */
                case 0b100:
                    if (inst.I.funct3 == 0b000) {
                        if (inst.I.imm_11_0 == 0) {
                            err = ecall(args);
                        } else if (inst.I.imm_11_0 == 1) {
                            err = ebreak(args);
                        } else {
                            // err = RV_EUNDEF;
                        }
                    }
                    break;

                /* No matched */
                case 0b101:
                    break;

                /* No matched */
                case 0b110:
                    break;
                default:
                    break;
            }
            if (!ok) break;
        }
    }while(0);
    return err;
}

bool RV32I::op_aa_match(rv32_inst_fmt inst)
{
    return inst.aa == 0b11;
}

bool RV32I::op_bbb_match(rv32_inst_fmt inst)
{
    switch(inst.bbb)
    {
        case 0b000:
        case 0b001:
        case 0b011:
        case 0b100:
        case 0b101:
        case 0b110:
            return true;
        case 0b010:
        case 0b111:
        default:
            return false;
    }
}

bool RV32I::op_cc_match(rv32_inst_fmt inst)
{
    switch(inst.cc)
    {
        case 0b00:
        case 0b01:
        case 0b11:
            return true;
        case 0b10:
        default:
            return false;
    }
}

rv_err RV32I::lb(inst_args args)
{}

rv_err RV32I::lh(inst_args args)
{}

rv_err RV32I::lw(inst_args args)
{}

rv_err RV32I::lbu(inst_args args)
{}

rv_err RV32I::lhu(inst_args args)
{}

rv_err RV32I::fence(inst_args args)
{}

rv_err RV32I::addi(inst_args args)
{}

rv_err RV32I::slti(inst_args args)
{}

rv_err RV32I::sltiu(inst_args args)
{}

rv_err RV32I::xori(inst_args args)
{}

rv_err RV32I::ori(inst_args args)
{}

rv_err RV32I::andi(inst_args args)
{}

rv_err RV32I::slli(inst_args args)
{}

rv_err RV32I::srli(inst_args args)
{}

rv_err RV32I::srai(inst_args args)
{}

rv_err RV32I::auipc(inst_args args)
{}

rv_err RV32I::sb(inst_args args)
{}

rv_err RV32I::sh(inst_args args)
{}

rv_err RV32I::sw(inst_args args)
{}

rv_err RV32I::add(inst_args args)
{}

rv_err RV32I::sub(inst_args args)
{}

rv_err RV32I::sll(inst_args args)
{}

rv_err RV32I::slt(inst_args args)
{}

rv_err RV32I::sltu(inst_args args)
{}

rv_err RV32I::xxor(inst_args args)
{}

rv_err RV32I::srl(inst_args args)
{}

rv_err RV32I::sra(inst_args args)
{}

rv_err RV32I::orr(inst_args args)
{}

rv_err RV32I::andd(inst_args args)
{}

rv_err RV32I::lui(inst_args args)
{}

rv_err RV32I::beq(inst_args args)
{}

rv_err RV32I::bne(inst_args args)
{}

rv_err RV32I::blt(inst_args args)
{}

rv_err RV32I::bge(inst_args args)
{}

rv_err RV32I::bltu(inst_args args)
{}

rv_err RV32I::bgeu(inst_args args)
{}

rv_err RV32I::jalr(inst_args args)
{}

rv_err RV32I::ecall(inst_args args)
{}

rv_err RV32I::ebreak(inst_args args)
{}

}