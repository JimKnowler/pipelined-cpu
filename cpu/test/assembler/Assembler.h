#pragma once

#include <memory>
#include <vector>

#include "IOpcode.h"

namespace assembler {
    class Assembler {
    public:
        Assembler();
        ~Assembler();

        // opcodes
        Assembler& NOP();
        Assembler& LDA();
        Assembler& STA();
        Assembler& ADD();
        Assembler& SUB();

        // operands
        Assembler& r(uint8_t r);
        Assembler& rs1(uint8_t rs1);
        Assembler& rs2(uint8_t rs2);
        Assembler& rd(uint8_t rd);
        Assembler& i(uint8_t i);

    private:
        void CheckCurrentOpcode();
        void FinishCurrentOpcode();
        void StartOpcode(Opcode Opcode);

        std::unique_ptr<IOpcode> CurrentOpcode;

        std::vector<std::unique_ptr<IOpcode>> Opcodes;
    };
}
