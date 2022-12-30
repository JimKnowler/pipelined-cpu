#include "OpcodeTypeI.h"

namespace assembler { namespace opcode {
    uint32_t OpcodeTypeI::Assemble() const {
        uint32_t data = (static_cast<uint8_t>(m_opcode) << 24)
                        | ((m_r & 0xf) << 20)
                        | m_i;

        return data;
    }
}}
