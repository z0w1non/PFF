# Peep file format

## Simple definition file format
```
using namespace PFF;
using WORD = unsigned short;

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
```
```
$ g++ std=c++17 main.cpp
$ ./a < a
[0x0000 - 0x0001] 0x5a4d exe/Magic number (== 0x5a4d)
[0x0002 - 0x0003] 0x0090 exe/Number of bytes in the last block of the program
[0x0004 - 0x0005] 0x0003 exe/Number of blocks in the file that are part of the EXE file
[0x0006 - 0x0007] 0x0000 exe/Number of relocation entries stored after the header
[0x0008 - 0x0009] 0x0004 exe/Number paragraphs in the header
[0x000a - 0x000b] 0x0000 exe/Number of paragraphs of additional memory that the program will need
[0x000c - 0x000d] 0xffff exe/Maximum number of paragraphs of additional memory
[0x000e - 0x000f] 0x0000 exe/Relative value of the stack segment
[0x0010 - 0x0011] 0x00b8 exe/Initial value of the SP register
[0x0012 - 0x0013] 0x0000 exe/Word checksum
[0x0014 - 0x0015] 0x0000 exe/Initial value of the IP register
[0x0016 - 0x0017] 0x0000 exe/Initial value of the CS register
[0x0018 - 0x0019] 0x0040 exe/Offset the first relocation item in the file
[0x001a - 0x001b] 0x0000 exe/Overlay number
```

`_{ expr }` is the constructor of the template class `_<T>`.
`_{ "name", type{}}` define the name of the region and the type of that region.
When `_<T>` used as the second argument and beyond of the constructor of `_<T>{ ... }`, that expression deduced as calling to the constructer of `T<abstract_tag>`.
This type only provides information of namespace several concrete types.
If necessary the multiple hierarchies of `T<abstract_tag>` such as described below.
```
_{"A",
    _{"B",
        _{"C",
            _{"value", unsigned char{}}
        }
    }
}.parse();
```
```
$ g++ std=c++17 main.cpp
$ ./a < in.txt
[0x0000 - 0x0000] 0x00 A/B/C/value
```

## From where to where
`parse()` use `std::cin` and `std::cout` as the default arguments.
When the arguments are not explicitly specified, the program parse standard input, and output result of that to standard output. If necessary specify  values such as described in the following.
```
auto in = std::ifstream{"sample.exe"};
auto out = std::ofstream{"result.txt"};
_{ ... }
.parse(in, out);
```
