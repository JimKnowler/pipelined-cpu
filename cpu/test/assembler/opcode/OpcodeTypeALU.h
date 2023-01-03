#pragma once

#include "../IOpcode.h"

namespace assembler {
    namespace opcode {
        class OpcodeTypeALU : public IOpcode {
        public:
            OpcodeTypeALU(Opcode Opcode) : IOpcode(Opcode) {}

        public: // IOpcode
            virtual uint32_t Assemble() const override;
        };
    }
}