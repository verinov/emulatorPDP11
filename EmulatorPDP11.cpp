#include "EmulatorPDP11.h"

#include <cstring>
#include <fstream>

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define ROM_ADDR (mem_ + 48 * 1024)



EmulatorPDP11::EmulatorPDP11() {

    char* video = videomem();

    Reset();

// filling in ROM
    // picture
    std::fstream file;
    file.open("screen.bmp", std::fstream::in | std::fstream::binary);
    file.seekg(62 + 256); // 256 - margin from both sides
    file.read(mem_ + 48*1024 + 512, 16*1024 - 512);
    file.close();
    //TODO: op codes are to be written here

    for (int i = 4; i < 256 - 4; ++i) {
        for (int j = 0; j < 512; ++j) {
            video[i*64 + j/8] &= (255-(1<<(j%8)));
            video[i*64 + j/8] |= (1<<(j%8))* !(bool)(mem_[48*1024 + 256 + i*64 + j/8] & (1<<(8-j%8)));
        }
    }

    for (int i = 0; i < 4*(512/8); ++i) {
        video[i] = 0;
    }
    for (int i = 1024*16 - 4*(512/8); i < 1024*16; ++i) {
        video[i] = 0;
    }
    for (int i = 4; i < 252; ++i) {
        video[i*(512/8) + 0] &= 16*15;
        video[i*(512/8) + 63] &= 15;
    }
}

char* EmulatorPDP11::videomem() {
    return mem_ + 32 * 1024; // video mem address
}

EmulatorPDP11::EmulatorPDP11(const char* source, size_t count) :
                    EmulatorPDP11() {
    memcpy(mem_, source, count);
}


EmulatorPDP11::~EmulatorPDP11() {
}


size_t EmulatorPDP11::WriteROM(const char* source, size_t count) {
    memcpy(ROM_ADDR, source, count); // ROM start address
    return count;
}

void EmulatorPDP11::Run() {

}

void EmulatorPDP11::Stop() {

}

void EmulatorPDP11::Step() {

}

void EmulatorPDP11::Reset() { // TODO: what is reset?
    memset(mem_, 0xff, 48*1024);

    pc_ = 48 * 1024; // ROM start address
    regs_[0] = regs_[1] = regs_[2] = regs_[3] = 0;
}

void EmulatorPDP11::op_halt(void* a, void* b)
{

}

void EmulatorPDP11::op_wait(void* a, void* b)
{

}

#define BYTE_MSB(byte) (byte>>7)
#define WORD_MSB(word) (word>>15)

void EmulatorPDP11::op_rti(void* a, void*b){return;}
void EmulatorPDP11::op_bpt(void* a, void*b){return;}
void EmulatorPDP11::op_iot(void* a, void*b){return;}
void EmulatorPDP11::op_reset(void* a, void*b){return;}
void EmulatorPDP11::op_rtt(void* a, void*b){return;}
void EmulatorPDP11::op_jmp(void* a, void*b){return;}
void EmulatorPDP11::op_rts(void* a, void*b){return;}
void EmulatorPDP11::op_spl(void* a, void*b){return;}
void EmulatorPDP11::op_nop(void* a, void*b){return;}
void EmulatorPDP11::op_clc(void* a, void*b){return;}
void EmulatorPDP11::op_clv(void* a, void*b){return;}
void EmulatorPDP11::op_clz(void* a, void*b){return;}
void EmulatorPDP11::op_cln(void* a, void*b){return;}
void EmulatorPDP11::op_ccc(void* a, void*b){return;}
void EmulatorPDP11::op_sec(void* a, void*b){return;}
void EmulatorPDP11::op_sev(void* a, void*b){return;}
void EmulatorPDP11::op_sez(void* a, void*b){return;}
void EmulatorPDP11::op_sen(void* a, void*b){return;}
void EmulatorPDP11::op_scc(void* a, void*b){return;}
void EmulatorPDP11::op_swab(void* a, void*b){return;}
void EmulatorPDP11::op_br(void* a, void*b){return;}
void EmulatorPDP11::op_bne(void* a, void*b){return;}
void EmulatorPDP11::op_beq(void* a, void*b){return;}
void EmulatorPDP11::op_bge(void* a, void*b){return;}
void EmulatorPDP11::op_blt(void* a, void*b){return;}
void EmulatorPDP11::op_bgt(void* a, void*b){return;}
void EmulatorPDP11::op_ble(void* a, void*b){return;}
void EmulatorPDP11::op_jsr(void* a, void*b){return;}
void EmulatorPDP11::op_clr(void* a, void*b){return;}
void EmulatorPDP11::op_com(void* a, void*b){return;}

void EmulatorPDP11::op_inc(void* addr, void* unused)
{
    if (addr > ROM_ADDR)
        throw;

    psw_N_ = WORD_MSB(*(uint16_t*)addr);
    psw_Z_ = *(uint16_t*)addr;
    psw_V_ = (uint32_t)(*(uint16_t*)addr) + 1 != (uint32_t)(*(uint16_t*)addr + 1);
    (*(uint16_t*)addr)++;
}

void EmulatorPDP11::op_dec(void* addr, void* unused)
{
    if (addr > ROM_ADDR)
        throw;

    psw_N_ = WORD_MSB(*(uint16_t*)addr);
    psw_Z_ = *(uint16_t*)addr;
    psw_V_ = (uint32_t)(*(uint16_t*)addr) - 1 != (uint32_t)(*(uint16_t*)addr - 1);
    (*(uint16_t*)addr)--;
}

void EmulatorPDP11::op_neg(void* a, void*b){return;}
void EmulatorPDP11::op_adc(void* a, void*b){return;}
void EmulatorPDP11::op_sbc(void* a, void*b){return;}
void EmulatorPDP11::op_tst(void* a, void*b){return;}
void EmulatorPDP11::op_ror(void* a, void*b){return;}
void EmulatorPDP11::op_rol(void* a, void*b){return;}
void EmulatorPDP11::op_asr(void* a, void*b){return;}
void EmulatorPDP11::op_asl(void* a, void*b){return;}
void EmulatorPDP11::op_mark(void* a, void*b){return;}
void EmulatorPDP11::op_mfpi(void* a, void*b){return;}
void EmulatorPDP11::op_mtpi(void* a, void*b){return;}
void EmulatorPDP11::op_sxt(void* a, void*b){return;}
void EmulatorPDP11::op_mul(void* a, void*b){return;}
void EmulatorPDP11::op_div(void* a, void*b){return;}
void EmulatorPDP11::op_ash(void* a, void*b){return;}
void EmulatorPDP11::op_ashc(void* a, void*b){return;}
void EmulatorPDP11::op_xor(void* a, void*b){return;}

void EmulatorPDP11::op_sob(void* reg, void* n)
{
    if (--(*(uint16_t*)reg))
        pc_ -= *((uint8_t*)(&n)) * 2;
}

void EmulatorPDP11::op_mov(void* src, void* dst)
{
    if (dst > ROM_ADDR)
        throw;

    *(uint16_t*)dst = *(uint16_t*)src;
    psw_N_ = WORD_MSB(*(uint16_t*)dst);
    psw_Z_ = *(uint16_t*)dst;
    psw_V_ = 0;
}

void EmulatorPDP11::op_cmp(void* a, void*b){return;}
void EmulatorPDP11::op_bit(void* a, void*b){return;}
void EmulatorPDP11::op_bic(void* a, void*b){return;}
void EmulatorPDP11::op_bis(void* a, void*b){return;}

void EmulatorPDP11::op_add(void* src, void* dst)
{
    if (dst > ROM_ADDR)
        throw;

    psw_V_ = (uint32_t)(*(uint16_t*)dst) + (uint32_t)(*(uint16_t*)src) !=
            (uint32_t)(*(uint16_t*)dst + *(uint16_t*)src);
    *(uint16_t*)dst += *(uint16_t*)src;
    psw_N_ = WORD_MSB(*(uint16_t*)dst);
    psw_Z_ = *(uint16_t*)dst;
}

void EmulatorPDP11::op_movb(void* src, void* dst)
{
    if (dst > ROM_ADDR)
        throw;

    *(uint8_t*)dst = *(uint8_t*)src;
    psw_N_ = BYTE_MSB(*(uint8_t*)dst);
    psw_Z_ = *(uint8_t*)dst;
    psw_V_ = 0;

}

void EmulatorPDP11::op_cmpb(void* a, void*b){return;}
void EmulatorPDP11::op_bitb(void* a, void*b){return;}
void EmulatorPDP11::op_bicb(void* a, void*b){return;}
void EmulatorPDP11::op_bisb(void* a, void*b){return;}

void EmulatorPDP11::op_sub(void* src, void* dst)
{
    if (dst > ROM_ADDR)
        throw;

    psw_V_ = (uint32_t)(*(uint16_t*)dst) - (uint32_t)(*(uint16_t*)src) !=
            (uint32_t)(*(uint16_t*)dst - *(uint16_t*)src);
    *(uint16_t*)dst -= *(uint16_t*)src;
    psw_N_ &= WORD_MSB(*(uint16_t*)dst);
    psw_Z_ &= *(uint16_t*)dst;
}

void EmulatorPDP11::op_bpl(void* a, void*b){return;}
void EmulatorPDP11::op_bmi(void* a, void*b){return;}
void EmulatorPDP11::op_bhi(void* a, void*b){return;}
void EmulatorPDP11::op_blos(void* a, void*b){return;}
void EmulatorPDP11::op_bvc(void* a, void*b){return;}
void EmulatorPDP11::op_bvs(void* a, void*b){return;}
void EmulatorPDP11::op_bcc(void* a, void*b){return;}
void EmulatorPDP11::op_bcs(void* a, void*b){return;}
void EmulatorPDP11::op_emt(void* a, void*b){return;}
void EmulatorPDP11::op_sys(void* a, void*b){return;}
void EmulatorPDP11::op_clrb(void* a, void*b){return;}
void EmulatorPDP11::op_comb(void* a, void*b){return;}

void EmulatorPDP11::op_incb(void* addr, void* unused)
{
    if (addr > ROM_ADDR)
        throw;

    psw_V_ = ((uint16_t)(*(uint8_t*)addr) + 1) != (uint16_t)(*(uint8_t*)addr + 1);
    (*(uint8_t*)addr)++;
    psw_N_ = BYTE_MSB(*(uint8_t*)addr);
    psw_Z_ = *(uint8_t*)addr;
}
void EmulatorPDP11::op_decb(void* addr, void* unused)
{
    if (addr > ROM_ADDR)
        throw;

    psw_V_ = ((uint16_t)(*(uint8_t*)addr) - 1) != (uint16_t)(*(uint8_t*)addr - 1);
    (*(uint8_t*)addr)--;
    psw_N_ = BYTE_MSB(*(uint8_t*)addr);
    psw_Z_ = *(uint8_t*)addr;
}

void EmulatorPDP11::op_negb(void* a, void*b){return;}
void EmulatorPDP11::op_adcb(void* a, void*b){return;}
void EmulatorPDP11::op_sbcb(void* a, void*b){return;}
void EmulatorPDP11::op_tstb(void* a, void*b){return;}
void EmulatorPDP11::op_rorb(void* a, void*b){return;}
void EmulatorPDP11::op_rolb(void* a, void*b){return;}
void EmulatorPDP11::op_asrb(void* a, void*b){return;}
void EmulatorPDP11::op_aslb(void* a, void*b){return;}
