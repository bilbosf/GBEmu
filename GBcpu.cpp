#include "GBcpu.h"
#include "Bus.h"
#include <iostream>

#define DEBUG


GBcpu::GBcpu()
{
	using a = GBcpu;

	lookup_noprefix =
	{
		//  x0              x1              x2              x3              x4              x5              x6              x7              x8              x9              xA              xB              xC              xD              xE              xF
			&a::NOP,        &a::LD_rr_nn,   &a::LD_Mrr_A,   &a::INC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::RLCA,       &a::LD_Mnn_SP,  &a::ADD_HL_rr,  &a::LD_A_Mrr,   &a::DEC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::RRCA,     //0x
			&a::STOP,       &a::LD_rr_nn,   &a::LD_Mrr_A,   &a::INC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::RLA,        &a::JR_PC_dd,   &a::ADD_HL_rr,  &a::LD_A_Mrr,   &a::DEC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::RRA,      //1x
			&a::JR_f_PC_dd, &a::LD_rr_nn,   &a::LDI_MHL_A,  &a::INC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::DAA,        &a::JR_f_PC_dd, &a::ADD_HL_rr,  &a::LDI_A_MHL,  &a::DEC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::CPL,      //2x
			&a::JR_f_PC_dd, &a::LD_rr_nn,   &a::LDD_MHL_A,  &a::INC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::SCF,        &a::JR_f_PC_dd, &a::ADD_HL_rr,  &a::LDD_A_MHL,  &a::DEC_rr,     &a::INC_r,      &a::DEC_r,      &a::LD_r_n,     &a::CCF,      //3x
			&a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,   //4x
			&a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,   //5x
			&a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,   //6x
			&a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::HALT,       &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,     &a::LD_r_r,   //7x
			&a::ADD_A_r,    &a::ADD_A_r,    &a::ADD_A_r,    &a::ADD_A_r,    &a::ADD_A_r,    &a::ADD_A_r,    &a::ADD_A_r,    &a::ADD_A_r,    &a::ADC_A_r,    &a::ADC_A_r,    &a::ADC_A_r,    &a::ADC_A_r,    &a::ADC_A_r,    &a::ADC_A_r,    &a::ADC_A_r,    &a::ADC_A_r,  //8x
			&a::SUB_r,      &a::SUB_r,      &a::SUB_r,      &a::SUB_r,      &a::SUB_r,      &a::SUB_r,      &a::SUB_r,      &a::SUB_r,      &a::SBC_r,      &a::SBC_r,      &a::SBC_r,      &a::SBC_r,      &a::SBC_r,      &a::SBC_r,      &a::SBC_r,      &a::SBC_r,    //9x
			&a::AND_r,      &a::AND_r,      &a::AND_r,      &a::AND_r,      &a::AND_r,      &a::AND_r,      &a::AND_r,      &a::AND_r,      &a::XOR_r,      &a::XOR_r,      &a::XOR_r,      &a::XOR_r,      &a::XOR_r,      &a::XOR_r,      &a::XOR_r,      &a::XOR_r,    //Ax
			&a::OR_r,       &a::OR_r,       &a::OR_r,       &a::OR_r,       &a::OR_r,       &a::OR_r,       &a::OR_r,       &a::OR_r,       &a::CP_r,       &a::CP_r,       &a::CP_r,       &a::CP_r,       &a::CP_r,       &a::CP_r,       &a::CP_r,       &a::CP_r,     //Bx
			&a::RET_f,      &a::POP_rr,     &a::JP_f_nn,    &a::JP_nn,      &a::CALL_f_nn,  &a::PUSH_rr,    &a::ADD_A_n,    &a::RST_n,      &a::RET_f,      &a::RET,        &a::JP_f_nn,    &a::PREFIX,     &a::CALL_f_nn,  &a::CALL_nn,    &a::ADC_A_n,    &a::RST_n,    //Cx
			&a::RET_f,      &a::POP_rr,     &a::JP_f_nn,    &a::NOP,        &a::CALL_f_nn,  &a::PUSH_rr,    &a::SUB_n,      &a::RST_n,      &a::RET_f,      &a::RETI,       &a::JP_f_nn,    &a::NOP,        &a::CALL_f_nn,  &a::NOP,        &a::SBC_n,      &a::RST_n,    //Dx
			&a::LDH_Mn_A,   &a::POP_rr,     &a::LD_MC_A,    &a::NOP,        &a::NOP,        &a::PUSH_rr,    &a::AND_n,      &a::RST_n,      &a::ADD_SP_dd,  &a::JP_HL,      &a::LD_Mnn_A,   &a::NOP,        &a::NOP,        &a::NOP,        &a::XOR_n,      &a::RST_n,    //Ex
			&a::LDH_A_Mn,   &a::POP_rr,     &a::LD_A_MC,    &a::DI,         &a::NOP,        &a::PUSH_rr,    &a::OR_n,       &a::RST_n,      &a::LD_HL_SP_dd,&a::LD_SP_HL,   &a::LD_A_Mnn,   &a::EI,         &a::NOP,        &a::NOP,        &a::CP_n,       &a::RST_n,    //Fx
	};

	lookup_prefix =
	{
		//  x0              x1              x2              x3              x4              x5              x6              x7              x8              x9              xA              xB              xC              xD              xE              xF
			&a::RLC_r,      &a::RLC_r,      &a::RLC_r,      &a::RLC_r,      &a::RLC_r,      &a::RLC_r,      &a::RLC_r,      &a::RLC_r,      &a::RRC_r,      &a::RRC_r,      &a::RRC_r,      &a::RRC_r,      &a::RRC_r,      &a::RRC_r,      &a::RRC_r,      &a::RRC_r,    //0x
			&a::RL_r,       &a::RL_r,       &a::RL_r,       &a::RL_r,       &a::RL_r,       &a::RL_r,       &a::RL_r,       &a::RL_r,       &a::RR_r,       &a::RR_r,       &a::RR_r,       &a::RR_r,       &a::RR_r,       &a::RR_r,       &a::RR_r,       &a::RR_r,     //1x
			&a::SLA_r,      &a::SLA_r,      &a::SLA_r,      &a::SLA_r,      &a::SLA_r,      &a::SLA_r,      &a::SLA_r,      &a::SLA_r,      &a::SRA_r,      &a::SRA_r,      &a::SRA_r,      &a::SRA_r,      &a::SRA_r,      &a::SRA_r,      &a::SRA_r,      &a::SRA_r,    //2x
			&a::SWAP_r,     &a::SWAP_r,     &a::SWAP_r,     &a::SWAP_r,     &a::SWAP_r,     &a::SWAP_r,     &a::SWAP_r,     &a::SWAP_r,     &a::SRL_r,      &a::SRL_r,      &a::SRL_r,      &a::SRL_r,      &a::SRL_r,      &a::SRL_r,      &a::SRL_r,      &a::SRL_r,    //3x
			&a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,  //4x
			&a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,  //5x
			&a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,  //6x
			&a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,    &a::BIT_n_r,  //7x
			&a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,  //8x
			&a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,  //9x
			&a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,  //Ax
			&a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,    &a::RES_n_r,  //Bx
			&a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,  //Cx
			&a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,  //Dx
			&a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,  //Ex
			&a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,    &a::SET_n_r,  //Fx
	};
}

GBcpu::~GBcpu()
{

}

void GBcpu::set_af(uint16_t val)
{
	a = (uint8_t)(val >> 8);
	f = Flags_reg((uint8_t)val);
}

uint16_t GBcpu::get_af()
{
	return (uint16_t)((uint16_t)(a << 8) + f);
}

void GBcpu::set_bc(uint16_t val)
{
	b = (uint8_t)(val >> 8);
	c = (uint8_t)val;
}

uint16_t GBcpu::get_bc()
{
	return (uint16_t)((uint16_t)(b << 8) + c);
}

void GBcpu::set_de(uint16_t val)
{
	d = (uint8_t)(val >> 8);
	e = (uint8_t)val;
}

uint16_t GBcpu::get_de()
{
	return (uint16_t)((uint16_t)(d << 8) + e);
}

void GBcpu::set_hl(uint16_t val)
{
	h = (uint8_t)(val >> 8);
	l = (uint8_t)val;
}

uint16_t GBcpu::get_hl()
{
	return (uint16_t)((uint16_t)(h << 8) + l);
}

void GBcpu::set_sp(uint16_t val)
{
	sp = val;
}

uint16_t GBcpu::get_sp()
{
	return sp;
}

uint8_t GBcpu::read(uint16_t addr)
{
	return bus->read(addr, false);
}

void GBcpu::write(uint16_t addr, uint8_t data)
{
	bus->write(addr, data);
}

uint8_t GBcpu::get_arg(uint8_t opcode)
{
	uint8_t regs[] = { b, c, d, e, h, l, 0xFF, a };
	uint8_t i = (opcode & 0x07);

	if (i == 6) return read(get_hl());
	return regs[i];
}

uint8_t* GBcpu::get_arg_ptr(uint8_t opcode)
{
	uint8_t* regs[] = { &b, &c, &d, &e, &h, &l, nullptr, &a };
	uint8_t i = (opcode & 0x07);
	return regs[i];
}

uint8_t GBcpu::fetch_byte()
{
	uint8_t byte = read(pc);
	pc++;
	return byte;
}

/*
// Possible opcodes:
// - 8x (x in [0, 5]): ADD A, r
// - 86:               ADD A, (HL)
uint8_t GBcpu::ADD_A_r() {
	uint8_t arg = get_argument(opcode);
	uint8_t prev_a = a;
	a += arg;

	f.Z = (a == 0x00);
	f.N = false;
	f.H = (((a & 0x0F) + (arg & 0x0F)) > 0x0F);
	f.C = (a < prev_a);

	return (opcode & 0x0F) == 0x06 ? 2 : 1;
}
*/

uint8_t GBcpu::LD_r_r()
{
	return uint8_t();
}

uint8_t GBcpu::LD_r_n()
{
	return uint8_t();
}

uint8_t GBcpu::LD_A_Mrr()
{
	return uint8_t();
}

uint8_t GBcpu::LD_A_Mnn()
{
	return uint8_t();
}

uint8_t GBcpu::LD_Mrr_A()
{
	return uint8_t();
}

uint8_t GBcpu::LD_Mnn_A()
{
	return uint8_t();
}

uint8_t GBcpu::LDH_A_Mn()
{
	return uint8_t();
}

uint8_t GBcpu::LDH_Mn_A()
{
	return uint8_t();
}

uint8_t GBcpu::LD_A_MC()
{
	return uint8_t();
}

uint8_t GBcpu::LD_MC_A()
{
	return uint8_t();
}

uint8_t GBcpu::LDI_MHL_A()
{
	uint16_t hl = get_hl();
	write(hl, a);
	set_hl(hl + 1);
	return 2;
}

uint8_t GBcpu::LDI_A_MHL()
{
	uint16_t hl = get_hl();
	a = read(hl);
	set_hl(hl + 1);
	return 2;
}

uint8_t GBcpu::LDD_MHL_A()
{
	uint16_t hl = get_hl();
	write(hl, a);
	set_hl(hl - 1);
	return 2;
}

uint8_t GBcpu::LDD_A_MHL()
{
	uint16_t hl = get_hl();
	a = read(hl);
	set_hl(hl - 1);
	return 2;
}

uint8_t GBcpu::LD_rr_nn()
{
	using set_16bit_reg = void (GBcpu::*)(uint16_t);
	set_16bit_reg args[] = { &GBcpu::set_bc, &GBcpu::set_de, &GBcpu::set_hl, &GBcpu::set_sp };

	uint16_t nn = fetch_byte() << 8;
	nn += fetch_byte();

	uint8_t i = (opcode & 0xF0) >> 4;
	(this->*args[i])(nn);

	return 3;
}

uint8_t GBcpu::LD_Mnn_SP()
{
	return uint8_t();
}

uint8_t GBcpu::LD_SP_HL()
{
	return uint8_t();
}

uint8_t GBcpu::PUSH_rr()
{
	return uint8_t();
}

uint8_t GBcpu::POP_rr()
{
	return uint8_t();
}

uint8_t GBcpu::ADD_A_r()
{
	return uint8_t();
}

uint8_t GBcpu::ADD_A_n()
{
	return uint8_t();
}

uint8_t GBcpu::ADC_A_r()
{
	return uint8_t();
}

uint8_t GBcpu::ADC_A_n()
{
	return uint8_t();
}

uint8_t GBcpu::SUB_r()
{
	return uint8_t();
}

uint8_t GBcpu::SUB_n()
{
	return uint8_t();
}

uint8_t GBcpu::SBC_r()
{
	return uint8_t();
}

uint8_t GBcpu::SBC_n()
{
	return uint8_t();
}

uint8_t GBcpu::AND_r()
{
	return uint8_t();
}

uint8_t GBcpu::AND_n()
{
	return uint8_t();
}

uint8_t GBcpu::XOR_r()
{
	uint8_t arg = get_arg(opcode);
	a = a ^ arg;

	f.Z = (a == 0x00);
	f.N = f.H = f.C = false;

	return 1;
}

uint8_t GBcpu::XOR_n()
{
	return uint8_t();
}

uint8_t GBcpu::OR_r()
{
	return uint8_t();
}

uint8_t GBcpu::OR_n()
{
	return uint8_t();
}

uint8_t GBcpu::CP_r()
{
	return uint8_t();
}

uint8_t GBcpu::CP_n()
{
	return uint8_t();
}

uint8_t GBcpu::INC_r()
{
	return uint8_t();
}

uint8_t GBcpu::INC_n()
{
	return uint8_t();
}

uint8_t GBcpu::DEC_r()
{
	return uint8_t();
}

uint8_t GBcpu::DEC_n()
{
	return uint8_t();
}

uint8_t GBcpu::DAA()
{
	return uint8_t();
}

uint8_t GBcpu::CPL()
{
	return uint8_t();
}

uint8_t GBcpu::ADD_HL_rr()
{
	return uint8_t();
}

uint8_t GBcpu::ADD_SP_dd()
{
	return uint8_t();
}

uint8_t GBcpu::INC_rr()
{
	return uint8_t();
}

uint8_t GBcpu::DEC_rr()
{
	return uint8_t();
}

uint8_t GBcpu::LD_HL_SP_dd()
{
	return uint8_t();
}

uint8_t GBcpu::RLCA()
{
	return uint8_t();
}

uint8_t GBcpu::RLA()
{
	return uint8_t();
}

uint8_t GBcpu::RRCA()
{
	return uint8_t();
}

uint8_t GBcpu::RRA()
{
	return uint8_t();
}

uint8_t GBcpu::RLC_r()
{
	return uint8_t();
}

uint8_t GBcpu::RL_r()
{
	return uint8_t();
}

uint8_t GBcpu::RRC_r()
{
	return uint8_t();
}

uint8_t GBcpu::RR_r()
{
	return uint8_t();
}

uint8_t GBcpu::SLA_r()
{
	return uint8_t();
}

uint8_t GBcpu::SRA_r()
{
	return uint8_t();
}

uint8_t GBcpu::SWAP_r()
{
	return uint8_t();
}

uint8_t GBcpu::SRL_r()
{
	return uint8_t();
}

uint8_t GBcpu::BIT_n_r()
{
	uint8_t n = (opcode & 0x38) >> 3;
	uint8_t r = get_arg(opcode);

	f.Z = ((r >> n) & 0x01) == 0;
	f.N = false;
	f.H = true;

	return opcode == 0x46 ? 4 : 2;
}

uint8_t GBcpu::SET_n_r()
{
	uint8_t n = (opcode & 0x38) >> 3;

	if (opcode == 0xc6)
	{
		uint16_t hl = get_hl();
		uint8_t r = read(hl);
		write(hl, r | (0x01 << n));
		return 4;
	}

	uint8_t* r_ptr = get_arg_ptr(opcode);
	*r_ptr |= (0x01 << n);
	return 2;
}

uint8_t GBcpu::RES_n_r()
{
	uint8_t n = (opcode & 0x38) >> 3;

	if (opcode == 0xc6)
	{
		uint16_t hl = get_hl();
		uint8_t r = read(hl);
		write(hl, r & ~(0x01 << n));
		return 4;
	}

	uint8_t* r_ptr = get_arg_ptr(opcode);
	*r_ptr &= ~(0x01 << n);
	return 2;
}

uint8_t GBcpu::CCF()
{
	return uint8_t();
}

uint8_t GBcpu::SCF()
{
	return uint8_t();
}

uint8_t GBcpu::HALT()
{
	return uint8_t();
}

uint8_t GBcpu::STOP()
{
	return uint8_t();
}

uint8_t GBcpu::DI()
{
	return uint8_t();
}

uint8_t GBcpu::EI()
{
	return uint8_t();
}

uint8_t GBcpu::NOP()
{
	return uint8_t();
}

uint8_t GBcpu::JP_nn()
{
	return uint8_t();
}

uint8_t GBcpu::JP_HL()
{
	return uint8_t();
}

uint8_t GBcpu::JP_f_nn()
{
	return uint8_t();
}

uint8_t GBcpu::JR_PC_dd()
{
	int8_t dd = fetch_byte();
	pc += dd;
	return 3;
}

uint8_t GBcpu::JR_f_PC_dd()
{
	bool cond[] = { !f.Z, f.Z, !f.C, f.C };
	int8_t dd = fetch_byte();

	if (cond[((opcode & 0x38) >> 3) % 4])
	{
		pc += dd;
		return 3;
	}

	return 2;
}

uint8_t GBcpu::CALL_nn()
{
	return uint8_t();
}

uint8_t GBcpu::CALL_f_nn()
{
	return uint8_t();
}

uint8_t GBcpu::RET()
{
	return uint8_t();
}

uint8_t GBcpu::RET_f()
{
	return uint8_t();
}

uint8_t GBcpu::RETI()
{
	return uint8_t();
}

uint8_t GBcpu::RST_n()
{
	return uint8_t();
}

uint8_t GBcpu::PREFIX()
{
	prefix = true;
	return 0;
}

int main()
{
	Bus gb;
	gb.cpu.opcode = 0xaa;
	gb.cpu.a = 0x03;
	gb.cpu.d = 0x02;
	gb.cpu.XOR_r();
	std::cout << std::hex << (int)gb.cpu.a << std::endl;

}
