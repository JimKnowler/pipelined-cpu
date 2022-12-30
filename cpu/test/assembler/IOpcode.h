#pragma once

#include <cstdint>

#include "Opcode.h"

namespace assembler {
    class IOpcode {
    public:
        IOpcode(Opcode opcode);
        virtual ~IOpcode() {}

        void rs1(uint8_t rs1);
        void rs2(uint8_t rs2);
        void rd(uint8_t rd);

        void r(uint8_t r);
        void i(uint16_t i);

        virtual uint32_t Assemble() const = 0;

    protected:
        Opcode m_opcode;

        // R Type
        uint8_t m_rs1;
        uint8_t m_rs2;
        uint8_t m_rd;

        // I Type
        uint8_t m_r;
        uint16_t m_i;
    };
}
