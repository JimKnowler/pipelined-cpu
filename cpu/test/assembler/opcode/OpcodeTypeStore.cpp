#include "OpcodeTypeStore.h"

namespace assembler { namespace opcode {
    uint32_t OpcodeTypeStore::Assemble() const {
        uint32_t data = (static_cast<uint8_t>(m_opcode) << 24)
                        | ((m_i & 0xffff) << 8)
                        | ((m_rs1 & 0xf) << 4)
                        | (m_rs2 & 0xf);
        return data;
    }
}}
