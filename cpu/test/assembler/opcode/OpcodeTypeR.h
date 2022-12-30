#pragma once

#include "../IOpcode.h"

namespace assembler {
    namespace opcode {
        class OpcodeTypeR : public IOpcode {
        public:
            OpcodeTypeR(Opcode Opcode) : IOpcode(Opcode) {}

        public: // IOpcode
            virtual uint32_t Assemble() const override;
        };
    }
}