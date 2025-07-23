# Assembler_Compiler_Project

 This project features a custom made assembler and compiler to break down higher level code into machine code. The assembler is written using Lex (.l) and Yacc (.y) and will support all instructions in RISC-V (pseudo instructions included). The compiler will be added at a later phase and support loops, conditionals, variables, and more.

## Features

 - RISC-V assembler with support for multiple instruction types (R, I, S, etc.)
 - Built with Lex and Yacc
 - Compiler for a custom high level language
 - Supports pseudo instructions

## Usage

 To build the assembler, run the following command in the project directory:

 ```bash
 make
 ```

 To run the assembler on an input file:

 ```bash
 ./assembler <filename>.asm
 ```

 To clean build files:

 ```bash
 make clean
 ```

## Installation
 
 Make sure you have 'gcc', 'flex', and 'bison' installed.

 On Ubuntu/Debian, you can install them with:

 ```bash
 sudo apt-get install build-essential flex bison
 ```

## Contributing

 Contributions are welcome! Feel free to open issues or submit pull requests at any time.

## License

 This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.