#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <map>

enum class OpKind {
    IncrPtr = '>',
    DecrPtr = '<',
    IncrByte = '+',
    DecrByte = '-',
    ByteOut = '.',
    ByteIn = ',',
    JmpIfZero = '[',
    JmpIfNotZero = ']',  
};

struct Op {
    OpKind kind;
    std::size_t operand;  
};

struct Lexer {
    std::string content;
    std::size_t pos;  
};

char LexerNext(Lexer* lexer);
bool IsValidOperation(char c);

class Interpreter {
    public:
        Interpreter();

        void LoadProgramFromFile(const std::string& programPath);
        void Interpret();
    private:
        static constexpr uint32_t MEMORY_CAPACITY = 100000;
        void PrecomputeJumps();

        uint64_t m_programPC;
        uint64_t m_ptr;
        std::map<uint64_t, uint64_t> m_jumpMap;
        std::array<uint8_t, MEMORY_CAPACITY> m_memory;
        std::vector<Op> m_program;
};

#endif
