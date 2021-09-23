#pragma once
#include <cstdint>
#include <array>
#include "GBcpu.h"

class Bus {
public:
	Bus();
	~Bus();

	GBcpu cpu;
	std::array<uint8_t, 64 * 1024> ram{ 0 };

	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool bReadOnly = false);
};
