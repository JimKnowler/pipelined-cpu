#include "OpcodeTypeALU.h"

namespace assembler { namespace opcode {
    uint32_t OpcodeTypeALU::Assemble() const {
        uint32_t data = (static_cast<uint8_t>(m_opcode) << 24)
                        | ((m_rd & 0xf) << 20)
                        | ((m_rs1 & 0xf) << 4)
                        | ((m_rs2 & 0xf) << 0);

        return data;
    }
}}
