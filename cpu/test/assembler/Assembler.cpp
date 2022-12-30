#include "Assembler.h"

#include "opcode/OpcodeTypeI.h"
#include "opcode/OpcodeTypeR.h"

namespace assembler {
    Assembler::Assembler() {

    }

    Assembler::~Assembler() {

    }

    Assembler& Assembler::NOP() {
        StartOpcode(Opcode::NOP);

        return *this;
    }

    Assembler& Assembler::LDA() {
        StartOpcode(Opcode::LDA);

        return *this;
    }

    Assembler& Assembler::STA() {
        StartOpcode(Opcode::STA);

        return *this;
    }

    Assembler& Assembler::ADD() {
        StartOpcode(Opcode::ADD);

        return *this;
    }

    Assembler& Assembler::SUB() {
        StartOpcode(Opcode::SUB);

        return *this;
    }

    Assembler& Assembler::r(uint8_t r) {
        CheckCurrentOpcode();

        CurrentOpcode->r(r);

        return *this;
    }

    Assembler& Assembler::rs1(uint8_t rs1) {
        CheckCurrentOpcode();

        CurrentOpcode->rs1(rs1);

        return *this;
    }

    Assembler& Assembler::rs2(uint8_t rs2) {
        CheckCurrentOpcode();

        CurrentOpcode->rs2(rs2);

        return *this;
    }

    Assembler& Assembler::rd(uint8_t rd) {
        CheckCurrentOpcode();

        CurrentOpcode->rd(rd);

        return *this;
    }

    Assembler& Assembler::i(uint16_t i) {
        CheckCurrentOpcode();

        CurrentOpcode->i(i);

        return *this;
    }

    uint32_t Assembler::AssembleCurrentOpcode() const {
        CheckCurrentOpcode();

        return CurrentOpcode->Assemble();
    }

    void Assembler::CheckCurrentOpcode() const {
        if (CurrentOpcode) {
            return;
        }

        throw std::runtime_error("Assembler: CurrentOpcode is nullptr");
    }

    void Assembler::FinishCurrentOpcode() {
        if (CurrentOpcode) {
            Opcodes.push_back(std::move(CurrentOpcode));
        }
    }

    void Assembler::StartOpcode(Opcode Opcode) {
        FinishCurrentOpcode();

        switch (Opcode) {
            case Opcode::NOP:
            case Opcode::LDA:
            case Opcode::STA:
            {
                auto NewOpcode = std::make_unique<opcode::OpcodeTypeI>(Opcode);
                CurrentOpcode = std::move(NewOpcode);
                break;
            }
            case Opcode::ADD:
            case Opcode::SUB:
                CurrentOpcode = std::make_unique<opcode::OpcodeTypeR>(Opcode);
                break;
            default:
                throw std::runtime_error("Assembler: unknown opcode type");
        }
    }
}
