#pragma once

#include "../IOpcode.h"

namespace assembler {
    namespace opcode {
        class OpcodeTypeI : public IOpcode {
        public:
            OpcodeTypeI(Opcode Opcode) : IOpcode(Opcode) {}

        public: // IOpcode
            virtual uint32_t Assemble() const override;
        };
    }
}