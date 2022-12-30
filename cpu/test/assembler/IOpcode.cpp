#include "IOpcode.h"

namespace assembler {
    IOpcode::IOpcode(Opcode opcode) : m_opcode(opcode) {
        m_rs1 = 0;
        m_rs2 = 0;
        m_rd = 0;

        m_r = 0;
        m_i = 0;
    } 

    void IOpcode::rs1(uint8_t rs1) {
        m_rs1 = rs1;
    }

    void IOpcode::rs2(uint8_t rs2) {
        m_rs2 = rs2;
    }

    void IOpcode::rd(uint8_t rd) {
        m_rd = rd;
    }

    void IOpcode::r(uint8_t r) {
        m_r = r;
    }
    
    void IOpcode::i(uint16_t i) {
        m_i = i;
    }
}
