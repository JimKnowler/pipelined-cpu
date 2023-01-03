#include "OpcodeTypeLoad.h"

namespace assembler { namespace opcode {
    uint32_t OpcodeTypeLoad::Assemble() const {
        uint32_t data = (static_cast<uint8_t>(m_opcode) << 24)
                        | ((m_rd & 0xf) << 20)
                        | ((m_i & 0xffff) << 4)
                        | (m_rs1 & 0xf);

        return data;
    }
}}
