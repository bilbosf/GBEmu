#pragma once
#include <cstdint>
#include <iostream>
#include <vector>

class Bus;

class Flags_reg {
public:
	bool Z, N, H, C;

	Flags_reg(uint8_t reg = 0x00) {
		Z = (reg >> 7) & 1;
		N = (reg >> 6) & 1;
		H = (reg >> 5) & 1;
		C = (reg >> 4) & 1;
	}

	operator uint8_t() const {
		uint8_t reg = 0x00;
		reg += Z ? (1 << 7) : 0;
		reg += N ? (1 << 6) : 0;
		reg += H ? (1 << 5) : 0;
		reg += C ? (1 << 4) : 0;
		return reg;
	}

	Flags_reg& operator= (const Flags_reg& reg) {
		Z = reg.Z;
		N = reg.N;
		H = reg.H;
		C = reg.C;
		return *this;
	}

	friend std::ostream& operator<< (std::ostream& out, const Flags_reg& reg) {
		out << reg.Z << reg.N << reg.H << reg.C << "0000";
		return out;
	}
};

class GBcpu {
public:
	GBcpu();
	~GBcpu();

	// Registers
	uint8_t a = 0x00;
	uint8_t b = 0x00;
	uint8_t c = 0x00;
	uint8_t d = 0x00;
	uint8_t e = 0x00;
	uint8_t h = 0x00;
	uint8_t l = 0x00;

	uint16_t sp = 0x00;
	uint16_t pc = 0x00;

	Flags_reg f = Flags_reg(0x00);

	// Utility functions to access pairs of registers as a single 16 bit register
	void     set_af(uint16_t val);
	uint16_t get_af();
	void     set_bc(uint16_t val);
	uint16_t get_bc();
	void     set_de(uint16_t val);
	uint16_t get_de();
	void     set_hl(uint16_t val);
	uint16_t get_hl();
	void     set_sp(uint16_t val); // For the sake of consistency, also do the same for SP, even though it is already uint16_t
	uint16_t get_sp();

	// Bus connectivity
	void connectBus(Bus* b) { bus = b; }

public:

	// Bus connectivity
	Bus* bus;
	uint8_t read(uint16_t addr);
	void    write(uint16_t addr, uint8_t data);

	uint8_t opcode; // Used to store the first byte of the current instruction
	bool prefix = false; // Determines whether the next instruction will be decoded with the prefixed or non-prefixed table

	std::vector<uint8_t(GBcpu::*)()> lookup_noprefix, lookup_prefix; // Instruction lookup tables

	uint8_t get_arg(uint8_t opcode); // Utility function used to decode which register is the argument to a given instruction and return its value
	uint8_t* get_arg_ptr(uint8_t opcode);

	uint8_t fetch_byte(); // Utility function used by instructions to fetch extra bytes and increment PC

	// 8-bit load instructions
	uint8_t LD_r_r();    uint8_t LD_r_n();
	uint8_t LD_A_Mrr();  uint8_t LD_A_Mnn();
	uint8_t LD_Mrr_A();  uint8_t LD_Mnn_A();
	uint8_t LDH_A_Mn();  uint8_t LDH_Mn_A();
	uint8_t LD_A_MC();   uint8_t LD_MC_A();
	uint8_t LDI_MHL_A(); uint8_t LDI_A_MHL();
	uint8_t LDD_MHL_A(); uint8_t LDD_A_MHL();

	// 16-bit load instructions
	uint8_t LD_rr_nn();  uint8_t LD_Mnn_SP();  uint8_t LD_SP_HL();
	uint8_t PUSH_rr();   uint8_t POP_rr();

	// 8-bit arithmetic/logic instructions
	uint8_t ADD_A_r();   uint8_t ADD_A_n();
	uint8_t ADC_A_r();   uint8_t ADC_A_n();
	uint8_t SUB_r();     uint8_t SUB_n();
	uint8_t SBC_r();     uint8_t SBC_n();
	uint8_t AND_r();     uint8_t AND_n();
	uint8_t XOR_r();     uint8_t XOR_n();
	uint8_t OR_r();      uint8_t OR_n();
	uint8_t CP_r();      uint8_t CP_n();
	uint8_t INC_r();     uint8_t INC_n();
	uint8_t DEC_r();     uint8_t DEC_n();
	uint8_t DAA();       uint8_t CPL();

	// 16-bit arithmetic/logic instructions
	uint8_t ADD_HL_rr(); uint8_t ADD_SP_dd();
	uint8_t INC_rr();    uint8_t DEC_rr();
	uint8_t LD_HL_SP_dd();

	// Rotate and shift instructions
	uint8_t RLCA();      uint8_t RLA();
	uint8_t RRCA();      uint8_t RRA();
	uint8_t RLC_r();     uint8_t RL_r();
	uint8_t RRC_r();     uint8_t RR_r();
	uint8_t SLA_r();     uint8_t SRA_r();
	uint8_t SWAP_r();    uint8_t SRL_r();

	// Single bit operation instructions
	uint8_t BIT_n_r();
	uint8_t SET_n_r();
	uint8_t RES_n_r();

	// CPU control instructions
	uint8_t CCF();        uint8_t SCF();
	uint8_t HALT();       uint8_t STOP();
	uint8_t DI();         uint8_t EI();
	uint8_t NOP();

	// JUMP instructions
	uint8_t JP_nn();      uint8_t JP_HL();
	uint8_t JP_f_nn();
	uint8_t JR_PC_dd();   uint8_t JR_f_PC_dd();
	uint8_t CALL_nn();    uint8_t CALL_f_nn();
	uint8_t RET();        uint8_t RET_f();
	uint8_t RETI();       uint8_t RST_n();

	// Non-instructions used in this implementation
	uint8_t PREFIX();
};