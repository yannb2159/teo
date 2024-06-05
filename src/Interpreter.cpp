#include "Interpreter.hpp"
#include <fstream>
#include <iostream>

Interpreter::Interpreter() {
  m_programPC = 0;
  m_ptr = 0;
  m_program.resize(0x0);
  m_memory.fill(0x0);
}

bool IsValidOperation(char c) {
    std::string validOps = "><+-.,[]";
    if (validOps.find(c) != std::string::npos) return true;
    else return false;
}

char LexerNext(Lexer* lexer) {
    while (lexer->pos < lexer->content.size() && !IsValidOperation(lexer->content[lexer->pos])) {
        lexer->pos += 1;
    }

    if (lexer->pos >= lexer->content.size()) return 0;
    return lexer->content[lexer->pos++];
}

void Interpreter::LoadProgramFromFile(const std::string& programPath) {
    std::ifstream programFile(programPath);

    if (!programFile.is_open())
        throw std::runtime_error("[ERROR] Unable to open the program file !");

    Lexer lexer;
    lexer.pos = 0;
    char ch;
    while (programFile.good()) {
        programFile.get(ch);
        lexer.content.push_back(ch);
    }

    char c = LexerNext(&lexer);

    while(c) {
        switch (c) {
            case '>':
            case '<':
            case '+':
            case '-':
            case '.':
            case ',':
                {
                    std::size_t streak = 1;

                    char s = LexerNext(&lexer);
                    while (c == s) {
                        streak += 1;
                        s = LexerNext(&lexer);
                    }

                    Op op = {.kind = static_cast<OpKind>(c), .operand = streak };
                    m_program.push_back(op);
                    c = s;
                }
                break;
            case '[':
            case ']':
                {
                    Op op = {.kind = static_cast<OpKind>(c), .operand = 0};
                    m_program.push_back(op);
                    c = LexerNext(&lexer);
                }
                break;
            default: break;
        }
    }

    programFile.close();
}

void Interpreter::PrecomputeJumps() {
    std::vector<uint64_t> stack;

    uint64_t pc = 0;

    while (pc < m_program.size()) {
        OpKind op = m_program[pc].kind;

        switch (op) {
            case OpKind::JmpIfZero: stack.push_back(pc); break;
            case OpKind::JmpIfNotZero:
                {
                    uint64_t target = stack.back();
                    stack.pop_back();
                    m_jumpMap[pc] = target;
                    m_jumpMap[target] = pc;
                }
                break;
            default: break;
        }

        pc += 1;
    }
}


void Interpreter::Interpret() {
    PrecomputeJumps();

    while (m_programPC < m_program.size()) {
        Op op = m_program[m_programPC];

        switch (op.kind) {
            case OpKind::IncrPtr: m_ptr += op.operand; break;
            case OpKind::DecrPtr: m_ptr -= op.operand; break;
            case OpKind::IncrByte: m_memory[m_ptr] += op.operand; break;
            case OpKind::DecrByte: m_memory[m_ptr] -= op.operand; break;
            case OpKind::ByteOut:
                {
                    for (std::size_t i = 0; i < op.operand; ++i) {
                        char byteAsChar = static_cast<char>(m_memory[m_ptr]);
                        std::cout << byteAsChar;
                        std::cout.flush();
                    }
                }
                break;
            case OpKind::ByteIn:
                for (std::size_t i = 0; i < op.operand; ++i) {
                    char byteInput;
                    std::cin >> byteInput;
                    m_memory[m_ptr] = static_cast<uint8_t>(byteInput);
                }
            case OpKind::JmpIfZero:
                if (m_memory[m_ptr] == 0)
                    m_programPC = m_jumpMap[m_programPC];
                break;
            case OpKind::JmpIfNotZero:
                if (m_memory[m_ptr] != 0)
                    m_programPC = m_jumpMap[m_programPC];
                break;
            default: break;
        }

        m_programPC += 1;
    }
}
