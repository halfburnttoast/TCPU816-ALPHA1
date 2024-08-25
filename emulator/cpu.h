#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <stdint.h>
#define RAM_SIZE 0x10000
#define STACK_START 0xFF00
#define STACK_PTR 0xFFFF

typedef unsigned char uint8_t;

enum Reg_Select {REG_A, REG_X, REG_Y, REG_TMP};

typedef struct {
    uint8_t reg_acc;
    uint8_t reg_ir;
    uint8_t flag_zf;
    uint8_t flag_cf;
    uint8_t reg_file[4];
    uint16_t reg_pc;
    unsigned long cycles;
    unsigned long cycle_limit;
    uint8_t ram[RAM_SIZE];
    uint8_t is_running;
} CPU;


void cpu_reset(CPU *cpu);
void cpu_do_inst(CPU *cpu);
void cpu_print_state(CPU *cpu);


// Data fetch

uint8_t cpu_get_imm_u8(CPU *cpu);
uint16_t cpu_get_imm_u16(CPU *cpu);

// acc operations
void cpu_load_acc(CPU *cpu, const uint8_t value);

// CPU INSTRUCTIONS
void cpu_inst_nop(CPU *cpu);
void cpu_inst_edo(CPU *cpu);
void cpu_inst_edi(CPU *cpu);
void cpu_inst_ldai(CPU *cpu);
void cpu_inst_ldxi(CPU *cpu);
void cpu_inst_ldyi(CPU *cpu);
void cpu_inst_ldar(CPU *cpu);
void cpu_inst_ldarx(CPU *cpu);
void cpu_inst_ldary(CPU *cpu);
void cpu_inst_ldari(CPU *cpu);
void cpu_inst_star(CPU *cpu);
void cpu_inst_stxr(CPU *cpu);
void cpu_inst_styr(CPU *cpu);
void cpu_inst_starx(CPU *cpu);
void cpu_inst_stary(CPU *cpu);
void cpu_inst_stari(CPU *cpu);
void cpu_inst_inca(CPU *cpu);
void cpu_inst_incx(CPU *cpu);
void cpu_inst_incy(CPU *cpu);
void cpu_inst_deca(CPU *cpu);
void cpu_inst_decx(CPU *cpu);
void cpu_inst_decy(CPU *cpu);

void cpu_inst_tax(CPU *cpu);
void cpu_inst_txa(CPU *cpu);
void cpu_inst_tya(CPU *cpu);
void cpu_inst_tay(CPU *cpu);
void cpu_inst_txy(CPU *cpu);
void cpu_inst_tyx(CPU *cpu);
void cpu_inst_tas(CPU *cpu);
void cpu_inst_tsa(CPU *cpu);

void cpu_inst_sec(CPU *cpu);
void cpu_inst_clc(CPU *cpu);


void cpu_inst_addi(CPU *cpu);
void cpu_inst_addr(CPU *cpu);
void cpu_inst_addx(CPU *cpu);
void cpu_inst_addy(CPU *cpu);

void cpu_inst_subi(CPU *cpu);
void cpu_inst_subr(CPU *cpu);
void cpu_inst_subx(CPU *cpu);
void cpu_inst_suby(CPU *cpu);

void cpu_inst_nandi(CPU *cpu);
void cpu_inst_nandr(CPU *cpu);
void cpu_inst_nandx(CPU *cpu);
void cpu_inst_nandy(CPU *cpu);

void cpu_inst_asr(CPU *cpu);
void cpu_inst_asr2(CPU *cpu);
void cpu_inst_asr4(CPU *cpu);

void cpu_inst_andi(CPU *cpu);
void cpu_inst_ori(CPU *cpu);
void cpu_inst_nota(CPU *cpu);
void cpu_inst_xori(CPU *cpu);

void cpu_inst_jmp(CPU *cpu);
void cpu_inst_jeq(CPU *cpu);
void cpu_inst_jcs(CPU *cpu);
void cpu_inst_jmpr(CPU *cpu);



void cpu_inst_rsp(CPU *cpu);
void cpu_inst_pha(CPU *cpu);
void cpu_inst_phx(CPU *cpu);
void cpu_inst_phy(CPU *cpu);
void cpu_inst_pla(CPU *cpu);
void cpu_inst_plx(CPU *cpu);
void cpu_inst_ply(CPU *cpu);


void cpu_inst_jsr(CPU *cpu);
void cpu_inst_rts(CPU *cpu);
void cpu_inst_ldsa(CPU *cpu);
void cpu_inst_stsa(CPU *cpu);


void cpu_inst_cpai(CPU *cpu);
void cpu_inst_cpar(CPU *cpu);
void cpu_inst_cpxi(CPU *cpu);
void cpu_inst_cpyi(CPU *cpu);


void cpu_inst_stz(CPU *cpu);
void cpu_inst_bit(CPU *cpu);





void cpu_inst_incr(CPU *cpu);
void cpu_inst_decr(CPU *cpu);

void cpu_inst_phi(CPU *cpu);
void cpu_inst_phi(CPU *cpu);

void cpu_inst_rola(CPU *cpu);
void cpu_inst_rolm(CPU *cpu);
void cpu_inst_orr(CPU *cpu);


#endif // CPU_H_INCLUDED
