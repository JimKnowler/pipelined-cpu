#pragma once

#include "../IOpcode.h"

namespace assembler {
    namespace opcode {
        class OpcodeTypeLoad : public IOpcode {
        public:
            OpcodeTypeLoad(Opcode Opcode) : IOpcode(Opcode) {}

        public: // IOpcode
            virtual uint32_t Assemble() const override;
        };
    }
}