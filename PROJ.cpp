// testing project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
//#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <string>

using namespace std;

unsigned int pc;
unsigned char memory[(16 + 64) * 1024] = { NULL };
uint32_t regs[32];

string RN[32] = { "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6" };





void printPrefix(unsigned int instA, unsigned int instW) {
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}


void instDecExec(unsigned int instWord)
{


	unsigned int rd, rs1, rs2, funct3, funct7, opcode;
	int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;

	unsigned int instPC = pc - 4;


	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	funct7 = (instWord >> 25) & 0x0000007f;
	///////////////////////////////I_imm/////////////////////////////////////////////////////
	unsigned int immI = (funct7 << 5) | rs2;
	string ii = to_string(immI);
	I_imm = stoi(ii);
	//////////////////////////////S_imm///////////////////////////////////////////////////////
	unsigned int immS = (funct7 << 5) | rd;
	string ss = to_string(immS);
	S_imm = stoi(ss);
	if (((instWord >> 31) & 0x0001) == 1) S_imm -= (1 << 12);
	//////////////////////////////////B_imm///////////////////////////////////////////////////
	unsigned int immB = ((funct7 & 0x40) << 5) | ((rd & 0x1) << 11) | ((rd & 0x1E) << 1) | ((funct7 & 0x3F) << 5);
	string bb = to_string(immB);
	B_imm = stoi(bb);
	if (((instWord >> 31) & 0x0001) == 1) B_imm -= (1 << 13);
	///////////////////////////////////U_imm///////////////////////////////////////////////////////
	U_imm = (funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12);
	//string uu = to_string(immU);
	//U_imm = stoi(uu);
	//////////////////////////////////J_imm/////////////////////////////////////////////////////////
	unsigned int tmp = (((funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12)) >> 11);
	unsigned int immJ = (tmp & 0x100000) | ((tmp & 0x1FE) << 11) | ((tmp & 0x200) << 2) | ((tmp & 0xFFC00) >> 9);
	//string imm = labelize(immJ);
	string jj = to_string(immJ);
	J_imm = stoi(jj);
	if (((instWord >> 31) & 0x0001) == 1) J_imm -= (1 << 21);
	//_________________________________________________________________________________________________________
	printPrefix(pc, instWord);
	//_______________________________ R-Instructions_________________________________________________________________
	if (opcode == 0x33) {
		switch (funct3) {
		case 0: if (funct7 == 32) {
			cout << "\tSUB\t" << RN[rd] << ", " << RN[rs1] << ", " << RN[rs2] << "\n";

			regs[rd] = regs[rs1] - regs[rs2];
			pc += 4;
			break;
		}
			  else {
			cout << "\tADD\t" << RN[rd] << ", " << RN[rs1] << ", " << RN[rs2] << "\n";
			regs[rd] = regs[rs1] + regs[rs2];
			pc += 4;
			break;
		}

		case 1: {
			cout << "\tSLL\t" << RN[rd] << "," << RN[rs1] << "," << RN[rs2] << "\n";
			regs[rd] = regs[rs1] << regs[rs2];
			pc += 4;
			break;

		}
		case 2: {

			cout << "\tSLT\t" << RN[rd] << "," << RN[rs1] << "," << RN[rs2] << "\n";
			if (regs[rs1] < regs[rs2])
			{
				regs[rd] = 1;
			}
			else
				regs[rd] = 0;

			pc += 4;
			break;
		}

		case 3: {
			cout << "\tSLTU\t" << RN[rd] << "," << RN[rs1] << "," << RN[rs2] << "\n";
			regs[rd] = regs[rs1] < regs[rs2] ? 1 : 0;
			pc += 4;
			break;
		}
		case 4: {
			cout << "\tXOR\t" << RN[rd] << "," << RN[rs1] << ", " << RN[rs2] << "\n";
			regs[rd] = regs[rs1] ^ regs[rs2];
			pc += 4;
			break;
		}
		case 5: if (funct7 == 32) {
			cout << "\tSRA\t" << RN[rd] << ", " << RN[rs1] << ", " << RN[rs2] << "\n";
			regs[rd] = regs[rs1] >> (regs[rs2] & 0x1F);
			pc += 4;
			break;
		}
			  else {
			cout << "\tSRL\t" << RN[rd] << ", " << RN[rs1] << ", " << RN[rs2] << "\n";
			regs[rd] = regs[rs1] >> (regs[rs2] & 0x1F);
			pc += 4;
			break;
		}


		case 6: {cout << "\tOR\tx" << RN[rd] << ", " << RN[rs1] << ", " << RN[rs2] << "\n";
			regs[rd] = regs[rs1] | regs[rs2];
			pc += 4;
			break;
		}
		case 7: {
			cout << "\tAND\t" << RN[rd] << "," << RN[rs1] << ", " << RN[rs2] << "\n";
			regs[rd] = regs[rs1] & regs[rs2];
			pc += 4;
			break;
		}

		default:
			cout << "\tUnkown R Instruction \n";
		}
	}//______________________________________ I instructions______________________________________________________________________
	else if (opcode == 0x13) {
		switch (funct3) {
		case 0: {
			cout << "\tADDI\t" << RN[rd] << ", " << RN[rs1] << ", " << dec << (int)I_imm << "\n";

			regs[rd] = regs[rs1] + I_imm;
			pc += 4;
			break;
		}
		case 1: {
			cout << "\tSLLI\t" << RN[rd] << ", " << RN[rd] << ", " << I_imm << "\n";
			regs[rd] = regs[rs1] << (I_imm & 0x1F);
			pc += 4;
			break; }

		case 2: {
			cout << "\tSLTI\t" << RN[rd] << "," << RN[rs1] << ", " << dec << (int)I_imm << "\n";
			regs[rd] = (regs[rs1] < I_imm) ? 1 : 0;
			pc += 4;
			break;
		}
		case 3: {
			cout << "\tSLTIU\t" << RN[rd] << ", " << RN[rs1] << ", " << dec << (int)I_imm << "\n";
			regs[rd] = (regs[rs1] < I_imm) ? 1 : 0;
			pc += 4;
			break;
		}
		case 4: {
			cout << "\tXORI\t" << RN[rd] << ", " << RN[rs1] << ", " << dec << (int)I_imm << "\n";
			regs[rd] = regs[rs1] ^ I_imm;
			pc += 4;
			break; }

		case 5: if (funct7 == 32)
		{
			cout << "\tSRLI\t" << RN[rd] << ", " << RN[rs1] << ", " << I_imm << "\n";
			regs[rd] = regs[rs1] >> (I_imm & 0x1F);
		}
			  else {
			cout << "\tSRAI\t" << RN[rd] << ", " << RN[rs1] << "," << dec << I_imm << "\n";
			regs[rd] = regs[rs1] >> (I_imm & 0x1F);
		}
			  pc += 4;
			  break;

		case 6: {
			cout << "\tORI\t" << RN[rd] << ", " << RN[rs1] << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] | I_imm;
			pc += 4;
			break;
		}
		case 7: {
			cout << "\tANDI\t" << RN[rd] << ", " << RN[rs1] << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] & I_imm;
			pc += 4;
			break; }


		default:
			cout << "\tUnkown I Instruction \n";
		}

	}
	//__________________________________________________I-type load instructions________________________________________________
	else if (opcode == 0x03) {
		switch (funct3) {
		case 0:
		{
			cout << "\tLB\t" << RN[rd] << ", " << RN[rs1] << ", " << hex << "0";
			address = regs[rs1] + I_imm;
			if (address >= 0 && address < ((16 + 64) * 1024)) {
				regs[rd] = static_cast<int32_t>(memory[address]);
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			pc += 4;
			break;
		}

		case 1:
		{
			cout << "\tLH\t" << RN[rd] << ", " << RN[rs1] << ", " << hex << "0x" << (int)I_imm << "\n";
			address = regs[rs1] + I_imm;
			if (address >= 0 && address < ((16 + 64) * 1024 - 1)) {
				int16_t value = static_cast<int16_t>((memory[address] << 0) |
					(memory[address + 1] << 8));
				regs[rd] = static_cast<int32_t>(value);
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			pc += 4;
			break;
		}

		case 2: {
			cout << "\tLW\t" << RN[rd] << ", " << RN[rs1] << ", " << hex << "0x" << (int)I_imm << "\n";
			address = regs[rs1] + I_imm;
			if (address >= 0 && address < ((16 + 64) * 1024 - sizeof(int32_t) + 1)) {
				// Combine four bytes from memory into a 32-bit word, taking care of endianness
				int32_t value = static_cast<int32_t>((memory[address] << 0) |
					(memory[address + 1] << 8) |
					(memory[address + 2] << 16) |
					(memory[address + 3] << 24));
				regs[rd] = value;
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			pc += 4;
			break;
		}

		case 4: {
			cout << "\tLBU\t" << RN[rd] << ", " << RN[rs1] << ", " << hex << "0x" << (int)I_imm << "\n";
			address = regs[rs1] + I_imm;
			if (address >= 0 && address < ((16 + 64) * 1024)) {
				regs[rd] = static_cast<uint32_t>(memory[address]);
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			pc += 4;

			break;
		}
		case 5:
		{
			cout << "\tLHU\t" << RN[rd] << ", " << RN[rs1] << ", " << hex << "0x" << (int)I_imm << "\n";
			address = regs[rs1] + I_imm;
			if (address >= 0 && address < ((16 + 64) * 1024 - 1)) {
				uint16_t value = static_cast<uint16_t>((memory[address] << 0) |
					(memory[address + 1] << 8));
				regs[rd] = static_cast<int32_t>(value);
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			pc += 4;
			break;

		}
		cout << "\tUnkown Instruction \n";
		}
	}
	else if (opcode == 0x23) {
		switch (funct3) {
		case 0:
		{
			cout << "\tSB\t" << RN[rs2] << ", " << (int)S_imm << "(x" << RN[rs1] << ")\n";
			address = regs[rs1] + S_imm;
			if (address >= 0 && address < ((16 + 64) * 1024)) {
				memory[address] = static_cast<unsigned char>(regs[rs2] & 0xFF);
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			pc += 4;

			break;
		}
		case 1: {
			cout << "\tSH\tx" << dec << RN[rs2] << ", " << (int)S_imm << "(" << dec << RN[rs1] << ")\n";
			address = regs[rs1] + S_imm;
			if (address >= 0 && address < ((16 + 64) * 1024 - 1)) {
				int16_t value = static_cast<int16_t>(regs[rs2]);
				memory[address] = static_cast<unsigned char>(value & 0xFF);
				memory[address + 1] = static_cast<unsigned char>((value >> 8) & 0xFF);
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			break;
		}
		case 2: {
			cout << "\tSW\t" << RN[rs2] << ", " << (int)S_imm << "(x" << RN[rs1] << ")\n";
			address = regs[rs1] + S_imm;
			if (address >= 0 && address < ((16 + 64) * 1024 - sizeof(int32_t) + 1)) {
				int32_t value = regs[rs2];
				memory[address] = static_cast<unsigned char>(value & 0xFF);
				memory[address + 1] = static_cast<unsigned char>((value >> 8) & 0xFF);
				memory[address + 2] = static_cast<unsigned char>((value >> 16) & 0xFF);
				memory[address + 3] = static_cast<unsigned char>((value >> 24) & 0xFF);
			}
			else {
				cout << "ERROR: WRONG ADDRESS\n";
			}
			pc += 4;

			break; }


		}
	}
	
	else if (opcode == 0x63)													// BRANCH INSTRUCTIONS
		switch (funct3) {
		case 0: {
			cout << "\tBEQ\t" << RN[rs2] << ", " << RN[rs1] << ", " << hex << "0x" << hex << B_imm << "\n";
			if (regs[rs1] == regs[rs2]) {
				pc += B_imm; // Increment pc by the branch immediate value
			}
			else {
				pc += 4; //  move to the next instruction (increment by 4)
			}
			break;
		}
		case 1: {
			cout << "\tBNE\t" << RN[rs2] << ", " << RN[rs1] << ", " << hex << "0x" << B_imm << "\n";
			if (regs[rs1] != regs[rs2]) {
				pc += B_imm; // Increment pc by the branch immediate value
			}
			else {
				pc += 4; //  move to the next instruction (increment by 4)
			}
			break; }

		case 4: {
			cout << "\tBLT\t" << RN[rs2] << ", " << RN[rs1] << ", " << hex << "0x" << B_imm << "\n";
			if (static_cast<int32_t>(regs[rs1]) < static_cast<int32_t>(regs[rs2])) {
				pc += B_imm; // Increment pc by the branch immediate value
			}
			else {
				pc += 4; //  move to the next instruction (increment by 4)
			}
			break; }

		case 5: {
			cout << "\tBGE\t" << RN[rs2] << ", " << RN[rs1] << ", " << hex << "0x" << B_imm << "\n";
			if (static_cast<int32_t>(regs[rs1]) >= static_cast<int32_t>(regs[rs2])) {
				pc += B_imm; // Increment pc by the branch immediate value
			}
			else {
				pc += 4; // move to the next instruction (increment by 4)
			}
			break; }

		case 6: {
			cout << "\tBLTU\t" << RN[rs2] << ", " << RN[rs1] << ", " << hex << "0x" << B_imm << "\n";
			if (static_cast<uint32_t>(regs[rs1]) < static_cast<uint32_t>(regs[rs2])) {
				pc += B_imm; // Increment pc by the branch immediate value
			}
			else {
				pc += 4; //  move to the next instruction (increment by 4)
			}
			break; }

		case 7: {
			cout << "\tBGEU\t" << RN[rs2] << ", " << RN[rs1] << ", " << hex << "0x" << B_imm << "\n";
			if (static_cast<uint32_t>(regs[rs1]) >= static_cast<uint32_t>(regs[rs2])) {
				pc += B_imm; // Increment the program counter by the branch immediate value
			}
			else {
				pc += 4; // If the condition is false, move to the next instruction (increment by 4)
			}
			break; }

		}
	else if (opcode == 0x37)
	{
		cout << "\tLUI\t" << RN[rd] << ", " << dec << U_imm << "\n";
		regs[rd] = U_imm << 12;
		pc += 4;

	}

	else if (opcode == 0x17)
	{
		cout << "\tAUIPC\t" << RN[rd] << ", " << dec << U_imm << "\n";
		regs[rd] = pc + (U_imm << 12);
		pc += 4;
	}

	else if (opcode == 0x6f)
	{
		cout << "\tJAL\t" << RN[rd] << ", " << hex << J_imm << "\n";
		regs[rd] = pc + 4; // Save the return address (PC + 4) into the specified register
		pc += 4; // Jump to the target address (PC + J_imm)
	}

	else if (instWord == 0x00000073)
	{
		cout << "\tECALL\t";
		char* str;
		switch (regs[17]) {
		case 1:
			cout << "Value in regs[10]: " << regs[10] << endl;
			break;
		case 4:
			// Assuming regs[10] holds the address of a null-terminated string to print
			str = reinterpret_cast<char*>(&memory[regs[10]]);
			cout << str << endl;
			break;

		case 10:
			break;
		default:
			// Handle unsupported system call or interrupt
			cout << "Unsupported ecall: heloo" << regs[17] << endl;
			break;
		}
		pc += 4;
	}

}

int main(int argc, char* argv[]) {
	//cout << "ana dakhel 3alek ahoo";
	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;
	string bb = "Error: Invalid input file";

	// Check if the correct number of command-line arguments is provided
	if (argc < 2) {
		cout << "Error: Invalid input file\n";
		return 1;
	}

	// Open the input file
	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open()) {
		// Get the file size
		int fsize = inFile.tellg();

		// Reset the file pointer to the beginning of the file
		inFile.seekg(0, inFile.beg);

		// Read the contents of the file into the memory
		if (!inFile.read((char*)memory, fsize)) {
			cout << "Cannot read from input file\n";
			return 1;
		}
		pc = 0;
		cout << "pc = " << fsize << endl;
		// Process the instructions in the memory
		while (pc < 100) {
			// Extract the instruction word from memory
			instWord = (unsigned char)memory[pc] |
				(((unsigned char)memory[pc + 1]) << 8) |
				(((unsigned char)memory[pc + 2]) << 16) |
				(((unsigned char)memory[pc + 3]) << 24);

			//pc += 4;


			//PROJ.exe C:\Users\ae912\OneDrive\Desktop\t0.bin
			// cd "C:\Users\ae912\source\repos\PROJ\x64\Debug"

			instDecExec(instWord);

		}



		for (int i = 0; i < 32; i++) {
			cout << RN[i] << " = " << dec << regs[i] << endl;

		}
	}
	else {
		cout << "Cannot access input file\n";
		return 1;
	}

	

	return 0;
}