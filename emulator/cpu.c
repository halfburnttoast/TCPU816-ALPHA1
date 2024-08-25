#include <stdio.h>
#include <time.h>
#include "cpu.h"

void (*inst_matrix[0x100])(CPU *cpu);
void cpu_build_inst_matrix(void) {
    inst_matrix[0x00] = cpu_inst_nop;
    inst_matrix[0x01] = cpu_inst_edo;
    inst_matrix[0x02] = cpu_inst_edi;
    inst_matrix[0x03] = cpu_inst_ldai;
    inst_matrix[0x04] = cpu_inst_ldxi;
    inst_matrix[0x05] = cpu_inst_ldyi;
    inst_matrix[0x06] = cpu_inst_ldar;
    inst_matrix[0x07] = cpu_inst_ldarx;
    inst_matrix[0x08] = cpu_inst_ldary;
    inst_matrix[0x09] = cpu_inst_ldari;
    inst_matrix[0x0A] = cpu_inst_star;
    inst_matrix[0x0B] = cpu_inst_stxr;
    inst_matrix[0x0C] = cpu_inst_styr;
    inst_matrix[0x0D] = cpu_inst_starx;
    inst_matrix[0x0E] = cpu_inst_stary;
    inst_matrix[0x0F] = cpu_inst_stari;
    inst_matrix[0x10] = cpu_inst_inca;
    inst_matrix[0x11] = cpu_inst_incx;
    inst_matrix[0x12] = cpu_inst_incy;
    inst_matrix[0x13] = cpu_inst_deca;
    inst_matrix[0x14] = cpu_inst_decx;
    inst_matrix[0x15] = cpu_inst_decy;

    inst_matrix[0x16] = cpu_inst_tax;
    inst_matrix[0x17] = cpu_inst_txa;
    inst_matrix[0x18] = cpu_inst_tya;
    inst_matrix[0x19] = cpu_inst_tay;
    inst_matrix[0x1A] = cpu_inst_txy;
    inst_matrix[0x1B] = cpu_inst_tyx;
    inst_matrix[0x1C] = cpu_inst_tsa;
    inst_matrix[0x1D] = cpu_inst_tas;

    inst_matrix[0x1E] = cpu_inst_sec;
    inst_matrix[0x1F] = cpu_inst_clc;

    inst_matrix[0x20] = cpu_inst_addi;
    inst_matrix[0x21] = cpu_inst_addr;
    inst_matrix[0x22] = cpu_inst_addx;
    inst_matrix[0x23] = cpu_inst_addy;

    inst_matrix[0x24] = cpu_inst_subi;
    inst_matrix[0x25] = cpu_inst_subr;
    inst_matrix[0x26] = cpu_inst_subx;
    inst_matrix[0x27] = cpu_inst_suby;


    inst_matrix[0x28] = cpu_inst_nandi;
    inst_matrix[0x29] = cpu_inst_nandr;
    inst_matrix[0x2A] = cpu_inst_nandx;
    inst_matrix[0x2B] = cpu_inst_nandy;


    inst_matrix[0x2C] = cpu_inst_asr;
    inst_matrix[0x2D] = cpu_inst_asr2;
    inst_matrix[0x2E] = cpu_inst_asr4;


    inst_matrix[0x2F] = cpu_inst_andi;
    inst_matrix[0x30] = cpu_inst_ori;
    inst_matrix[0x31] = cpu_inst_nota;
    inst_matrix[0x32] = cpu_inst_xori;

    inst_matrix[0x33] = cpu_inst_jmp;
    inst_matrix[0x34] = cpu_inst_jeq;
    inst_matrix[0x35] = cpu_inst_jcs;
    inst_matrix[0x36] = cpu_inst_jmpr;

    inst_matrix[0x37] = cpu_inst_rsp;
    inst_matrix[0x38] = cpu_inst_pha;
    inst_matrix[0x39] = cpu_inst_phx;
    inst_matrix[0x3A] = cpu_inst_phy;
    inst_matrix[0x3B] = cpu_inst_pla;
    inst_matrix[0x3C] = cpu_inst_plx;
    inst_matrix[0x3D] = cpu_inst_ply;

    inst_matrix[0x3E] = cpu_inst_jsr;
    inst_matrix[0x3F] = cpu_inst_rts;
    inst_matrix[0x40] = cpu_inst_ldsa;
    inst_matrix[0x41] = cpu_inst_stsa;
    inst_matrix[0x42] = cpu_inst_cpai;
    inst_matrix[0x43] = cpu_inst_cpar;
    inst_matrix[0x44] = cpu_inst_cpxi;
    inst_matrix[0x45] = cpu_inst_cpyi;
    inst_matrix[0x46] = cpu_inst_stz;
    inst_matrix[0x47] = cpu_inst_bit;
    inst_matrix[0x48] = cpu_inst_incr;
    inst_matrix[0x49] = cpu_inst_decr;
    inst_matrix[0x4A] = cpu_inst_phi;
    inst_matrix[0x4B] = cpu_inst_rola;
    inst_matrix[0x4C] = cpu_inst_rolm;
    inst_matrix[0x4D] = cpu_inst_orr;


}

void cpu_sleep(void) {
    int ms = 1000 * 5;
    clock_t start_time = clock();
    while(clock() < start_time + ms) {};
}

void cpu_reset(CPU *cpu) {
    cpu->reg_acc = 0;
    cpu->reg_ir = 0;
    cpu->reg_pc = 0;
    cpu->is_running = 0;
    for(uint8_t i = 0; i < 4; i++) {
        cpu->reg_file[i] = 0;
    }
    for(unsigned int i = 0; i < RAM_SIZE; i++) {
        cpu->ram[i] = 0;
    }
    cpu_build_inst_matrix();
    printf("# CPU RESET\n");
}


void cpu_run(CPU *cpu) {
    uint16_t cycles = 0;
    uint16_t cycle_max = 150;
    cpu->is_running = 1;
    while(cpu->is_running) {
        cpu->reg_ir = cpu->ram[cpu->reg_pc];
        cpu->reg_pc++;
        cpu_do_inst(cpu);
        //cpu_print_state(cpu);
        //cpu_sleep();
    }
}

void cpu_do_inst(CPU *cpu) {
    static uint8_t ser_count = 0;
    static unsigned long cycles = 0;
    if(inst_matrix[cpu->reg_ir] == 0) {
        printf("\r\n\r\n# - FATAL ERROR -\r\nINVALID OPCODE: %02X\r\n", cpu->reg_ir);
        cpu_print_state(cpu);
        exit(1);
    }
    (*inst_matrix[cpu->reg_ir])(cpu);
}

void cpu_print_state(CPU *cpu) {
    printf("PC  : %04X\r\n", cpu->reg_pc);
    printf("IR  : %02X\r\n", cpu->reg_ir);
    printf("ACC : %02X\r\n", cpu->reg_acc);
    printf("FILE: RA: %02X, RX: %02X, RY: %02X, RT: %02X\n\r",
        cpu->reg_file[REG_A],
        cpu->reg_file[REG_X],
        cpu->reg_file[REG_Y],
        cpu->reg_file[REG_TMP]
    );
    printf("SP  : %02X\r\n", cpu->ram[STACK_PTR]);
    printf("FLAGS (Z:C): %X:%X\r\n", cpu->flag_zf, cpu->flag_cf);
    printf("-------------------\r\n");

    printf("Stack space:");
    for(int i = 0xFF00; i < RAM_SIZE; i++) {
        if(i % 8 == 0)
            printf("\r\n%02X: ", i);
        printf("%02X ", cpu->ram[i]);
    }
    printf("\r\n");
}




// ALU
// only updates ACC register and flags
void cpu_alu_add(CPU *cpu, const uint8_t op, const uint8_t cin) {
    int sum = (int)cpu->reg_acc + (int)op + (int)cin;
    uint8_t out = (uint8_t)sum;
    cpu->flag_cf = sum > 0xFF ? 1 : 0;
    cpu->flag_zf = out == 0 ? 1 : 0;
    cpu->reg_acc = out;
}



// Data fetching

uint8_t cpu_get_imm_u8(CPU *cpu) {
    uint8_t data = cpu->ram[cpu->reg_pc];
    cpu->reg_pc++;
    return data;
}

// addresses are stored little endian, so we should flip them
uint16_t cpu_get_imm_u16(CPU *cpu) {
    uint16_t data = cpu->ram[cpu->reg_pc];
    cpu->reg_pc++;
    data = data | (cpu->ram[cpu->reg_pc] << 8);
    cpu->reg_pc++;
    return data;
}

uint16_t cpu_get_indirect_u16(CPU *cpu) {
    uint16_t address = cpu->ram[cpu->reg_pc];
    cpu->reg_pc++;
    address = address | (cpu->ram[cpu->reg_pc] << 8);
    cpu->reg_pc++;
    return (uint16_t)(cpu->ram[address] | (cpu->ram[address + 1] << 8));
}


// clears carry flag, updates zero flag
void cpu_load_acc(CPU *cpu, const uint8_t value) {
    cpu->flag_cf = 0;
    cpu->reg_acc = value;
    cpu->flag_zf = value == 0 ? 1 : 0;
}

// clears acc value, does not affect CF
void cpu_clear_acc(CPU *cpu) {
    cpu->reg_acc = 0;
    cpu->flag_zf = 1;
}


// Instructions


void cpu_inst_nop(CPU *cpu) {
    __asm__("nop");
    printf("# NOP instruction.\n\r");
    cpu_print_state(cpu);
    exit(1);
}

void cpu_inst_edo(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    uint8_t data = cpu->reg_file[REG_A];
    cpu_load_acc(cpu, data);
    if(address == 1) {
        putc(data, stdout);
        fflush(stdout);
    }
}

void cpu_inst_edi(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    uint8_t data = get_chr();
    if(address == 2) {
        cpu_load_acc(cpu, data);
        cpu->reg_file[REG_A] = cpu->reg_acc;
    }
}

void cpu_inst_ldai(CPU *cpu) {
    uint8_t data = cpu_get_imm_u8(cpu);
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_A] = data;
}

void cpu_inst_ldxi(CPU *cpu) {
    uint8_t data = cpu_get_imm_u8(cpu);
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_X] = data;
}

void cpu_inst_ldyi(CPU *cpu) {
    uint8_t data = cpu_get_imm_u8(cpu);
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_Y] = data;
}

void cpu_inst_ldar(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    uint8_t data = cpu->ram[address];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_A] = data;
}

void cpu_inst_ldarx(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu) + cpu->reg_file[REG_X];
    uint8_t data = cpu->ram[address];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_A] = data;
}

void cpu_inst_ldary(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu) + cpu->reg_file[REG_Y];
    uint8_t data = cpu->ram[address];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_A] = data;
}

void cpu_inst_ldari(CPU *cpu) {
    uint16_t address = cpu_get_indirect_u16(cpu);
    uint8_t data = cpu->ram[address];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_A] = data;
}

void cpu_inst_star(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu->ram[address] = cpu->reg_file[REG_A];
}

void cpu_inst_stxr(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu->ram[address] = cpu->reg_file[REG_X];
}

void cpu_inst_styr(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu->ram[address] = cpu->reg_file[REG_Y];
}

void cpu_inst_starx(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu) + cpu->reg_file[REG_X];
    cpu->ram[address] = cpu->reg_file[REG_A];
}

void cpu_inst_stary(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu) + cpu->reg_file[REG_Y];
    cpu->ram[address] = cpu->reg_file[REG_A];
}

void cpu_inst_stari(CPU *cpu) {
    uint16_t address = cpu_get_indirect_u16(cpu);
    cpu->ram[address] = cpu->reg_file[REG_A];
}

void cpu_inst_inca(CPU *cpu) {
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, 0, 1);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_incx(CPU *cpu) {
    cpu_load_acc(cpu, cpu->reg_file[REG_X]);
    cpu_alu_add(cpu, 0, 1);
    cpu->reg_file[REG_X] = cpu->reg_acc;
}

void cpu_inst_incy(CPU *cpu) {
    cpu_load_acc(cpu, cpu->reg_file[REG_Y]);
    cpu_alu_add(cpu, 0, 1);
    cpu->reg_file[REG_Y] = cpu->reg_acc;
}

void cpu_inst_deca(CPU *cpu) {
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, 0xFF, 0);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_decx(CPU *cpu) {
    cpu_load_acc(cpu, cpu->reg_file[REG_X]);
    cpu_alu_add(cpu, 0xFF, 0);
    cpu->reg_file[REG_X] = cpu->reg_acc;
}

void cpu_inst_decy(CPU *cpu) {
    cpu_load_acc(cpu, cpu->reg_file[REG_Y]);
    cpu_alu_add(cpu, 0xFF, 0);
    cpu->reg_file[REG_Y] = cpu->reg_acc;
}

void cpu_inst_tax(CPU *cpu) {
    cpu->reg_file[REG_X] = cpu->reg_file[REG_A];
}

void cpu_inst_txa(CPU *cpu) {
    uint8_t data = cpu->reg_file[REG_X];
    cpu->reg_file[REG_A] = data;
    cpu_load_acc(cpu, data);
}

void cpu_inst_tya(CPU *cpu) {
    uint8_t data = cpu->reg_file[REG_Y];
    cpu->reg_file[REG_A] = data;
    cpu_load_acc(cpu, data);
}

void cpu_inst_tay(CPU *cpu) {
    cpu->reg_file[REG_Y] = cpu->reg_file[REG_A];
}

void cpu_inst_txy(CPU *cpu) {
    cpu->reg_file[REG_Y] = cpu->reg_file[REG_X];
}

void cpu_inst_tyx(CPU *cpu) {
    cpu->reg_file[REG_X] = cpu->reg_file[REG_Y];
}

void cpu_inst_tas(CPU *cpu) {
    uint8_t val = cpu->reg_file[REG_A];
    cpu_load_acc(cpu, val);
    cpu->ram[STACK_PTR] = val;
}

void cpu_inst_tsa(CPU *cpu) {
    uint8_t val = cpu->ram[STACK_PTR];
    cpu_load_acc(cpu, val);
    cpu->reg_file[REG_A] = val;
}

void cpu_inst_sec(CPU *cpu) {
    cpu->flag_cf = 1;
}

void cpu_inst_clc(CPU *cpu) {
    cpu->flag_cf = 0;
}

void cpu_inst_addi(CPU *cpu) {
    cpu_clear_acc(cpu);
    uint8_t val = cpu_get_imm_u8(cpu);
    cpu_alu_add(cpu, cpu->reg_file[REG_A], cpu->flag_cf);
    cpu_alu_add(cpu, val, cpu->flag_cf);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_addr(CPU *cpu) {
    cpu_clear_acc(cpu);
    uint8_t val = cpu->ram[cpu_get_imm_u16(cpu)];
    cpu_alu_add(cpu, cpu->reg_file[REG_A], cpu->flag_cf);
    cpu_alu_add(cpu, val, cpu->flag_cf);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_addx(CPU *cpu) {
    cpu_clear_acc(cpu);
    uint8_t val = cpu->reg_file[REG_X];
    cpu_alu_add(cpu, cpu->reg_file[REG_A], cpu->flag_cf);
    cpu_alu_add(cpu, val, cpu->flag_cf);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_addy(CPU *cpu) {
    cpu_clear_acc(cpu);
    uint8_t val = cpu->reg_file[REG_Y];
    cpu_alu_add(cpu, cpu->reg_file[REG_A], cpu->flag_cf);
    cpu_alu_add(cpu, val, cpu->flag_cf);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_subi(CPU *cpu) {
    uint8_t carry = cpu->flag_cf;
    uint8_t operand = cpu_get_imm_u8(cpu);
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, ~operand, carry);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_subr(CPU *cpu) {
    uint8_t carry = cpu->flag_cf;
    uint8_t operand = cpu->ram[cpu_get_imm_u16(cpu)];
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, ~operand, carry);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_subx(CPU *cpu) {
    uint8_t carry = cpu->flag_cf;
    uint8_t val = cpu->reg_file[REG_X];
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, ~val, carry);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_suby(CPU *cpu) {
    uint8_t carry = cpu->flag_cf;
    uint8_t val = cpu->reg_file[REG_Y];
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, ~val, carry);
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_nandi(CPU *cpu) {
    uint8_t val = cpu_get_imm_u8(cpu);
    cpu_load_acc(cpu, ~(cpu->reg_file[REG_A] & val));
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_nandr(CPU *cpu) {
    uint8_t val = cpu->ram[cpu_get_imm_u16(cpu)];
    cpu_load_acc(cpu, ~(cpu->reg_file[REG_A] & val));
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_nandx(CPU *cpu) {
    uint8_t val = cpu->reg_file[REG_X];
    cpu_load_acc(cpu, ~(cpu->reg_file[REG_A] & val));
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_nandy(CPU *cpu) {
    uint8_t val = cpu->reg_file[REG_Y];
    cpu_load_acc(cpu, ~(cpu->reg_file[REG_A] & val));
    cpu->reg_file[REG_A] = cpu->reg_acc;
}

void cpu_inst_asr(CPU *cpu) {
    uint16_t v = ((uint16_t)cpu->reg_file[REG_A] << 1);
    v = v | ((v >> 8) & 1);
    cpu_load_acc(cpu, (uint8_t)v);
    cpu->reg_file[REG_A] = (uint8_t)v;
}

void cpu_inst_asr2(CPU *cpu) {
    uint16_t v = ((uint16_t)cpu->reg_file[REG_A] << 2);
    v = v | ((v >> 8) & 3);
    cpu_load_acc(cpu, (uint8_t)v);
    cpu->reg_file[REG_A] = (uint8_t)v;
}

void cpu_inst_asr4(CPU *cpu) {
    uint16_t v = ((uint16_t)cpu->reg_file[REG_A] << 4);
    v = v | ((v >> 8) & 0xF);
    cpu_load_acc(cpu, (uint8_t)v);
    cpu->reg_file[REG_A] = (uint8_t)v;
}

void cpu_inst_andi(CPU *cpu) {
    uint8_t val = cpu_get_imm_u8(cpu);
    uint8_t result = (cpu->reg_file[REG_A] & val);
    cpu_load_acc(cpu, result);
    cpu->reg_file[REG_A] = result;
}

void cpu_inst_ori(CPU *cpu) {
    uint8_t val = cpu_get_imm_u8(cpu);
    uint8_t result = (cpu->reg_file[REG_A] | val);
    cpu_load_acc(cpu, result);
    cpu->reg_file[REG_A] = result;
}

void cpu_inst_nota(CPU *cpu) {
    uint8_t val = ~(cpu->reg_file[REG_A]);
    cpu_load_acc(cpu, val);
    cpu->reg_file[REG_A] = val;

}

void cpu_inst_xori(CPU *cpu) {
    uint8_t val = cpu_get_imm_u8(cpu);
    uint8_t result = (cpu->reg_file[REG_A] ^ val);
    cpu_load_acc(cpu, result);
    cpu->reg_file[REG_A] = result;
}



void cpu_inst_jmp(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu->reg_pc = address;
}

void cpu_inst_jeq(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    if(cpu->flag_zf)
        cpu->reg_pc = address;
}

void cpu_inst_jcs(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    if(cpu->flag_cf)
        cpu->reg_pc = address;
}

void cpu_inst_jmpr(CPU *cpu) {
    uint16_t address = cpu_get_indirect_u16(cpu);
    cpu->reg_pc = address;
}



void cpu_inst_rsp(CPU *cpu) {
    cpu->ram[STACK_PTR] = 0xFE;
}

void cpu_inst_pha(CPU *cpu) {
    cpu->ram[STACK_START | cpu->ram[STACK_PTR]] = cpu->reg_file[REG_A];
    cpu->ram[STACK_PTR]--;
}

void cpu_inst_phx(CPU *cpu) {
    cpu->ram[STACK_START | cpu->ram[STACK_PTR]] = cpu->reg_file[REG_X];
    cpu->ram[STACK_PTR]--;
}

void cpu_inst_phy(CPU *cpu) {
    cpu->ram[STACK_START | cpu->ram[STACK_PTR]] = cpu->reg_file[REG_Y];
    cpu->ram[STACK_PTR]--;
}

void cpu_inst_pla(CPU *cpu) {
    cpu->ram[STACK_PTR]++;
    uint8_t data = cpu->ram[STACK_START | cpu->ram[STACK_PTR]];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_A] = data;
}

void cpu_inst_plx(CPU *cpu) {
    cpu->ram[STACK_PTR]++;
    uint8_t data = cpu->ram[STACK_START | cpu->ram[STACK_PTR]];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_X] = data;
}

void cpu_inst_ply(CPU *cpu) {
    cpu->ram[STACK_PTR]++;
    uint8_t data = cpu->ram[STACK_START | cpu->ram[STACK_PTR]];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_Y] = data;
}

void cpu_inst_jsr(CPU *cpu) {
    uint16_t cur_addr = cpu->reg_pc;
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu->ram[STACK_START | cpu->ram[STACK_PTR]] = (uint8_t)(cur_addr & 0xFF);
    cpu->ram[STACK_PTR]--;
    cpu->ram[STACK_START | cpu->ram[STACK_PTR]] = (uint8_t)((cur_addr >> 8) & 0xFF);
    cpu->ram[STACK_PTR]--;
    cpu->reg_pc = address;
}

void cpu_inst_rts(CPU *cpu) {
    uint16_t address = 0;
    cpu->ram[STACK_PTR]++;
    address = (uint16_t)(cpu->ram[STACK_START | cpu->ram[STACK_PTR]]) << 8;
    cpu->ram[STACK_PTR]++;
    address = address | cpu->ram[STACK_START | cpu->ram[STACK_PTR]];
    cpu->reg_pc = address;
    cpu->reg_pc += 2;
}

void cpu_inst_ldsa(CPU *cpu) {
    uint16_t address = STACK_START | cpu->ram[STACK_PTR];
    uint8_t offset = cpu_get_imm_u8(cpu);
    address = address + offset;
    uint8_t data = cpu->ram[offset];
    cpu_load_acc(cpu, data);
    cpu->reg_file[REG_A] = data;
}

void cpu_inst_stsa(CPU *cpu) {
    uint16_t address = STACK_START | cpu->ram[STACK_PTR];
    uint8_t offset = cpu_get_imm_u8(cpu);
    address = address + offset;
    cpu->ram[address] = cpu->reg_file[REG_A];
}

void cpu_inst_cpai(CPU *cpu) {
    uint8_t operand = cpu_get_imm_u8(cpu);
    cpu_clear_acc(cpu);
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, ~operand, 1);
}

void cpu_inst_cpar(CPU *cpu) {
    uint8_t operand = cpu->ram[cpu_get_imm_u16(cpu)];
    cpu_clear_acc(cpu);
    cpu_load_acc(cpu, cpu->reg_file[REG_A]);
    cpu_alu_add(cpu, ~operand, 1);
}

void cpu_inst_cpxi(CPU *cpu) {
    uint8_t operand = cpu_get_imm_u8(cpu);
    cpu_clear_acc(cpu);
    cpu_load_acc(cpu, cpu->reg_file[REG_X]);
    cpu_alu_add(cpu, ~operand, 1);
}

void cpu_inst_cpyi(CPU *cpu) {
    uint8_t operand = cpu_get_imm_u8(cpu);
    cpu_clear_acc(cpu);
    cpu_load_acc(cpu, cpu->reg_file[REG_Y]);
    cpu_alu_add(cpu, ~operand, 1);
}

void cpu_inst_stz(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu->ram[address] = 0;
}

void cpu_inst_bit(CPU *cpu) {
    cpu_clear_acc(cpu);
    uint8_t value = cpu->reg_file[REG_A] & cpu_get_imm_u8(cpu);
    cpu_load_acc(cpu, value);
    cpu->reg_file[REG_A] = value;
}

void cpu_inst_incr(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu_load_acc(cpu, cpu->ram[address]);
    cpu_alu_add(cpu, 0, 1);
    cpu->ram[address] = cpu->reg_acc;
}

void cpu_inst_decr(CPU *cpu) {
    uint16_t address = cpu_get_imm_u16(cpu);
    cpu_load_acc(cpu, cpu->ram[address]);
    cpu_alu_add(cpu, 0xFF, 0);
    cpu->ram[address] = cpu->reg_acc;
}

void cpu_inst_phi(CPU *cpu) {
    cpu->ram[STACK_START | cpu->ram[STACK_PTR]] = cpu_get_imm_u8(cpu);
    cpu->ram[STACK_PTR]--;
}

void cpu_inst_rola(CPU *cpu) {
    cpu_clear_acc(cpu);
    uint16_t value = cpu->reg_file[REG_A];
    value = (value << 1) & (value >> 7);
    cpu->reg_file[REG_A] = (uint8_t)value;
    if((uint8_t)value & 0x80 == 0) {
        cpu->flag_cf = 1;
    }
}

void cpu_inst_rolm(CPU *cpu) {
    cpu_clear_acc(cpu);
    uint16_t address = cpu_get_imm_u16(cpu);
    uint16_t value = cpu->ram[address];
    value = (value << 1) & (value >> 7);
    cpu->ram[address] = (uint8_t)value;
    if((uint8_t)value & 0x80 == 0) {
        cpu->flag_cf = 1;
    }
}

void cpu_inst_orr(CPU *cpu) {
    uint8_t value = cpu->ram[cpu_get_imm_u16(cpu)];
    value = value | cpu->reg_file[REG_A];
    cpu_clear_acc(cpu);
    cpu_load_acc(cpu, value);
    cpu->reg_file[REG_A] = value;
}
