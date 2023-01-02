#pragma once

#include <memory>
#include <vector>

#include "IOpcode.h"

#include "../memory/SRAM.hpp"

namespace assembler {
    class Assembler {
    public:
        Assembler();
        ~Assembler();

        // opcodes
        Assembler& NOP();
        Assembler& LW();
        Assembler& SW();
        Assembler& ADD();
        Assembler& SUB();

        // operands
        Assembler& r(uint8_t r);
        Assembler& rs1(uint8_t rs1);
        Assembler& rs2(uint8_t rs2);
        Assembler& rd(uint8_t rd);
        Assembler& i(uint16_t i);

        // assemble to machine code
        uint32_t AssembleCurrentOpcode() const;
        void Assemble(memory::SRAM& memory) const;

    private:
        void CheckCurrentOpcode() const;
        void FinishCurrentOpcode();
        void StartOpcode(Opcode Opcode);

        std::unique_ptr<IOpcode> CurrentOpcode;

        std::vector<std::unique_ptr<IOpcode>> Opcodes;
    };
}
