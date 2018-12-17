#include "PFF.hpp"

using BYTE = unsigned char;
using WORD = unsigned short;
using DWORD = unsigned int;

int main() {
    using namespace PFF;

    _{"exe",
        _{"Magic number (== 0x5a4d)", WORD{}},
        _{"Number of bytes in the last block of the program", WORD{}},
        _{"Number of blocks in the file that are part of the EXE file", WORD{}},
        _{"Number of relocation entries stored after the header", WORD{}},
        _{"Number paragraphs in the header", WORD{}},
        _{"Number of paragraphs of additional memory that the program will need", WORD{}},
        _{"Maximum number of paragraphs of additional memory", WORD{}},
        _{"Relative value of the stack segment", WORD{}},
        _{"Initial value of the SP register", WORD{}},
        _{"Word checksum", WORD{}},
        _{"Initial value of the IP register", WORD{}},
        _{"Initial value of the CS register", WORD{}},
        _{"Offset the first relocation item in the file", WORD{}},
        _{"Overlay number", WORD{}},
    }
    .parse();
}
