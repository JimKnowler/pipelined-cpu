#pragma once

#include "../IOpcode.h"

namespace assembler {
    namespace opcode {
        class OpcodeTypeStore : public IOpcode {
        public:
            OpcodeTypeStore(Opcode Opcode) : IOpcode(Opcode) {}

        public: // IOpcode
            virtual uint32_t Assemble() const override;
        };
    }
}