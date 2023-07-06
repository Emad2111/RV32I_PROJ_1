This project represents a trial to implement a RISC-V disassembler.


Implementation: 
The program consists of only two functions: 

1.  printPrefix(instPC, instWord) which simply prints the prefix for the decoded instruction. 
2.  instDecExec(unsigned int instWord) which  serves two main purposes: disassembling (decoding) and executing RISC-V RV32I instructions. The process goes as follows:
   A) Parameter: The function takes as an argument an instWord of type unsigned int. This argument represents a     
       machine instruction in the form of a 32-bit word.

   B) Initialization: It defines several variables such as rd, rs1, rs2, funct3, funct7, opcode, I_imm, S_imm, B_imm, 
    U_imm, J_imm, and address. These variables are used to hold various parts of the instruction word or the program 
    counter.

   C) Decoding: The function uses bitwise operations to extract the different parts of the instruction word (e.g., 
     opcode, rd, funct3, rs1, rs2).

   D) Immediate Value Decoding: It further decodes the immediate value I_imm from the instruction.

   E) Printing Prefix: The function then calls printPrefix(instPC, instWord) to print the prefix for the decoded 
      instruction.

     F) Instruction Execution: Next, the function checks the opcode and uses switch-case logic to execute different types of instructions. For instance, if the opcode is 0x33, it corresponds to R-type instructions. If the funct3 is 0 and funct7 is 32, it prints out a SUB instruction; otherwise, it prints an ADD instruction. Similarly, for the opcode 0x13, which corresponds to I-type instructions, if funct3 is 0, it executes and prints an ADDI instruction. 
          
Limitations:
The program is limited only to (16 + 64) KB of memory according to what is stated in the handout.

Known Issues:
      Potential discrepancies in the calculation of immediate values for some instructions have been identified. These minor errors, while not ideal, could be considered understandable given the complexity of the project relative to the timeline for its completion. These miscalculations do not significantly undermine the overall functionality of the program, as there are no issues of a more serious or critical nature present.
