/* On my honor, I have neither given nor received unauthorized aid on this assignment */
// MIPSsim.cpp : Defines the entry point for the console application.
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream> 
#include <deque>
#include <vector>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <list>

using namespace std;

// Foward Declaration
bool GetMipsInstruction(string currentInstruction, int pcAddress);
bool SimulateMipsInstruction(string &currentInstruction);
int CreateLength(string &one, string &two);
int GetRegister(string mipRegister);
int GetData(string data);
int RemoveHashMark(string immediate);
int ShiftRightLogical(string &binary, int shiftValue);
int ShiftLeftLogical(string &binary, int shiftValue);
int ShiftRightArithmetic(string &binary, int shiftValue);
int AndOperator(string regBinary, string immBinary);
int XorOperator(string regBinary, string immBinary);
int OrOperator(string regBinary, string immBinary);
long int BinaryToDecimal(string &binary);
long int TwosComplementToDecimal(string &complement);
void CreateDisassemblyFile(deque<string> linesInFile);
void CreateSimulationFile();
deque<string> LinesInInput(string filePath);
string AddBinaryStrings(string one, string two);
string DeleteSpaces(string &str);
string AddHashMark(long int value);
string AddCharR(long int value);
string AddColon(int value);
string RemoveParenthesis(string &str);
string DecimalToBinary(int value);
vector<int> dataVector;
int dataAddress;
int registerArray[32];
list<string> instructions;
list<string>::iterator it;

struct MipsInstruction
{
	int address;
	string instruction;
	string r1;
	string r2;
	string r3;
} caller;

int main(int argc, char *argv[])
{
	// Get File Path
	string inputText = argv[1];

	// Get lines of text
	deque<string> inputLines = LinesInInput(inputText);
	
	// initialize registers
	for (int i = 0; i < 32; i++)
	{
		registerArray[i] = 0;
	}
	CreateDisassemblyFile(inputLines);
	CreateSimulationFile();
	
    return 0;
}
// Gets lines of binary instructions
deque<string> LinesInInput(string filePath)
{
	deque<string> fileLines;

	// Open File
	ifstream input;
	input.open(filePath.c_str());

	if (input.is_open())
	{
		string line;

		// Read the file line by line
		while (getline(input, line))
		{
			// Line storage
			fileLines.push_back(line);
		}
		input.close();
	}
	return fileLines;
}
// Disassembles Binary to Mips
bool GetMipsInstruction(string currentInstruction, int pcAddress)
{
	string mipsCategory = currentInstruction.substr(0, 3);
	string mipsOpcode = currentInstruction.substr(3, 3);
	caller.r1.clear();
	caller.r2.clear();
	caller.r3.clear();
	//Category 1
	if (mipsCategory.compare("001") == 0)
	{
		caller.address = pcAddress;
		//NOP
		if (mipsOpcode.compare("000") == 0)
		{
			caller.address = pcAddress;
			caller.instruction = "NOP";
		}
		//J
		else if (mipsOpcode.compare("001") == 0)
		{
			caller.address = pcAddress;
			caller.instruction = "J";
			string jump = currentInstruction.substr(6, 26);
			string timesFour = "00";
			jump.append(timesFour);
			string pcValue = bitset<32>(pcAddress).to_string();
			string jTarget = pcValue.substr(0, 4);
			jTarget.append(jump);
			long int jumpTarget = TwosComplementToDecimal(jTarget);
			string jumpValue = AddHashMark(jumpTarget);

			caller.r1 = jumpValue;
		}
		//BEQ
		else if (mipsOpcode.compare("010") == 0)
		{
			caller.instruction = "BEQ";

			string destRegister = currentInstruction.substr(6, 5);
			string src1Register = currentInstruction.substr(11, 5);
			string src2Register = currentInstruction.substr(16, 16);

			long int desRegis = BinaryToDecimal(destRegister);
			long int src1Regis = BinaryToDecimal(src1Register);
			long int src2Regis = TwosComplementToDecimal(src2Register);

			string rs = AddCharR(desRegis);
			string rt = AddCharR(src1Regis);
			string offset = AddHashMark(src2Regis);

			caller.r1 = rs;
			caller.r2 = rt;
			caller.r3 = offset;
		}
		//BNE
		else if (mipsOpcode.compare("011") == 0)
		{
			caller.instruction = "BNE";
			string destRegister = currentInstruction.substr(6, 5);
			string src1Register = currentInstruction.substr(11, 5);
			string src2Register = currentInstruction.substr(16, 16);

			long int desRegis = BinaryToDecimal(destRegister);
			long int src1Regis = BinaryToDecimal(src1Register);
			long int src2Regis = TwosComplementToDecimal(src2Register);

			string rs = AddCharR(desRegis);
			string rt = AddCharR(src1Regis);
			string offset = AddHashMark(src2Regis);

			caller.r1 = rs;
			caller.r2 = rt;
			caller.r3 = offset;
		}
		//BGTZ
		else if (mipsOpcode.compare("100") == 0)
		{
			caller.instruction = "BGTZ";
			string destRegister = currentInstruction.substr(6, 5);
			string src1Register = currentInstruction.substr(16, 16);

			long int desRegis = BinaryToDecimal(destRegister);
			long int src1Regis = TwosComplementToDecimal(src1Register);

			string rs = AddCharR(desRegis);
			string rt = AddHashMark(src1Regis);

			caller.r1 = rs;
			caller.r2 = rt;
		}
		//SW
		else if (mipsOpcode.compare("101") == 0)
		{
			caller.instruction = "SW";
			string src1Register = currentInstruction.substr(6, 5);
			string destRegister = currentInstruction.substr(11, 5);
			string src2Register = currentInstruction.substr(16, 16);

			long int desRegis = BinaryToDecimal(src1Register);
			long int src1Regis = BinaryToDecimal(destRegister);
			long int src2Regis = TwosComplementToDecimal(src2Register);

			string rs = AddCharR(src1Regis);
			string rt = AddCharR(desRegis);
			string leftP = "(";
			string rightP = ")";

			leftP.append(rt);
			leftP.append(rightP);

			stringstream s2;
			s2 << src2Regis;
			string sr2;
			s2 >> sr2;
			sr2.append(leftP);

			caller.r1 = rs;
			caller.r2 = sr2;
		}
		//LW
		else if (mipsOpcode.compare("110") == 0)
		{
			caller.instruction = "LW";

			string src1Register = currentInstruction.substr(6, 5);
			string destRegister = currentInstruction.substr(11, 5);
			string src2Register = currentInstruction.substr(16, 16);

			long int desRegis = BinaryToDecimal(src1Register);
			long int src1Regis = BinaryToDecimal(destRegister);
			long int src2Regis = TwosComplementToDecimal(src2Register);

			string rs = AddCharR(src1Regis);
			string rt = AddCharR(desRegis);
			string leftP = "(";
			string rightP = ")";

			leftP.append(rt);
			leftP.append(rightP);

			stringstream s2;
			s2 << src2Regis;
			string sr2;
			s2 >> sr2;
			sr2.append(leftP);

			caller.r1 = rs;
			caller.r2 = sr2;
		}
		//BREAK
		else if (mipsOpcode.compare("111") == 0)
		{
			caller.instruction = "BREAK";
			return true;
		}
	}
	//Category 2
	else if (mipsCategory.compare("010") == 0)
	{
		string destRegister = currentInstruction.substr(6, 5);
		string src1Register = currentInstruction.substr(11, 5);
		string src2Register = currentInstruction.substr(16, 5);

		long int desRegis = BinaryToDecimal(destRegister);
		long int src1Regis = BinaryToDecimal(src1Register);
		long int src2Regis = BinaryToDecimal(src2Register);

		string destR = AddCharR(desRegis);
		string src1R = AddCharR(src1Regis);
		string src2R = AddCharR(src2Regis);

		caller.address = pcAddress;
		caller.r1 = destR;
		caller.r2 = src1R;
		caller.r3 = src2R;
		
		//XOR
		if (mipsOpcode.compare("000") == 0)
		{
			caller.instruction = "XOR";
		}
		//MUL
		else if (mipsOpcode.compare("001") == 0)
		{
			caller.instruction = "MUL";
		}
		//ADD
		else if (mipsOpcode.compare("010") == 0)
		{
			caller.instruction = "ADD";
		}
		//SUB
		else if (mipsOpcode.compare("011") == 0)
		{
			caller.instruction = "SUB";
		}
		//AND
		else if (mipsOpcode.compare("100") == 0)
		{
			caller.instruction = "AND";
		}
		//OR
		else if (mipsOpcode.compare("101") == 0)
		{
			caller.instruction = "OR";
		}
		//ADDU
		else if (mipsOpcode.compare("110") == 0)
		{
			caller.instruction = "ADDU";
		}
		//SUBU
		else if (mipsOpcode.compare("111") == 0)
		{
			caller.instruction = "SUBU";
		}

	}
	//Category 3
	else if (mipsCategory.compare("100") == 0)
	{
		string destRegister = currentInstruction.substr(6, 5);
		string src1Register = currentInstruction.substr(11, 5);

		long int desRegis = BinaryToDecimal(destRegister);
		long int src1Regis = BinaryToDecimal(src1Register);

		string destR = AddCharR(desRegis);
		string src1R = AddCharR(src1Regis);

		caller.address = pcAddress;
		caller.r1 = destR;
		caller.r2 = src1R;
		//ORI
		if (mipsOpcode.compare("000") == 0)
		{
			string immediateValue = currentInstruction.substr(16, 16);
			long int immediate = BinaryToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "ORI";
			caller.r3 = immed;
		}
		//XORI
		else if (mipsOpcode.compare("001") == 0)
		{
			string immediateValue = currentInstruction.substr(16, 16);
			long int immediate = BinaryToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "XORI";
			caller.r3 = immed;
		}
		//ADDI
		else if (mipsOpcode.compare("010") == 0)
		{
			string immediateValue = currentInstruction.substr(16, 16);
			long int immediate = TwosComplementToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "ADDI";
			caller.r3 = immed;
		}
		//SUBI
		else if (mipsOpcode.compare("011") == 0)
		{
			string immediateValue = currentInstruction.substr(16, 16);
			long int immediate = TwosComplementToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "SUBI";
			caller.r3 = immed;
		}
		//ANDI
		else if (mipsOpcode.compare("100") == 0)
		{
			string immediateValue = currentInstruction.substr(16, 16);
			long int immediate = BinaryToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "ANDI";
			caller.r3 = immed;
		}
		//SRL
		else if (mipsOpcode.compare("101") == 0)
		{
			string immediateValue = currentInstruction.substr(27, 5);
			long int immediate = BinaryToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "SRL";
			caller.r3 = immed;
		}
		//SRA
		else if (mipsOpcode.compare("110") == 0)
		{
			string immediateValue = currentInstruction.substr(27, 5);
			long int immediate = BinaryToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "SRA";
			caller.r3 = immed;
		}
		//SLL
		else if (mipsOpcode.compare("111") == 0)
		{
			string immediateValue = currentInstruction.substr(27, 5);
			long int immediate = BinaryToDecimal(immediateValue);
			string immed = AddHashMark(immediate);
			caller.instruction = "SLL";
			caller.r3 = immed;
		}

	}
	return false;
}
// Takes the instruction order from List and performs whatever it needs
bool SimulateMipsInstruction(string &currentInstruction)
{
	string temp = currentInstruction;
	stringstream stream(temp);
	string stringMips;
	stream >> stringMips;
	string instructionAddress = stringMips;
	stream >> stringMips;
	if (stringMips.compare("NOP") == 0)
	{
		++it;
	}
	else if (stringMips.compare("J") == 0)
	{
		stream >> stringMips;
		string immediate = stringMips;
		int shiftValue = RemoveHashMark(immediate);
		int temp;
		istringstream ss(instructionAddress);
		ss >> temp;
		shiftValue = (shiftValue - temp)/4;
		advance(it, shiftValue);
	}
	else if (stringMips.compare("BEQ") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);

		stream >> stringMips;
		string immediate = stringMips;
		int immediateValue = RemoveHashMark(immediate);

		if (registerArray[destReg] == registerArray[source1Reg])
		{
			immediateValue = immediateValue + 1;
			advance(it, immediateValue);
		}
		else
		{
			++it;
		}
	}
	else if (stringMips.compare("BNE") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);

		stream >> stringMips;
		string immediate = stringMips;
		int immediateValue = RemoveHashMark(immediate);

		if (registerArray[destReg] != registerArray[source1Reg])
		{
			immediateValue = immediateValue + 1;
			advance(it, immediateValue);
		}
		else
		{
			++it;
		}
	}
	else if (stringMips.compare("BGTZ") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string immediate = stringMips;
		int shiftValue = RemoveHashMark(immediate);
		
		if (registerArray[destReg] > 0)
		{
			shiftValue = shiftValue + 1;
			advance(it, shiftValue);
		}
		else
		{
			++it;
		}
	}
	else if (stringMips.compare("SW") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source = stringMips;
		string data = stringMips;

		source = RemoveParenthesis(source);
		int sourceReg = GetRegister(source);
		int dataSlot = GetData(data);
		int loadData = ((registerArray[sourceReg] + dataSlot) - dataAddress) / 4;
		 dataVector[loadData] = registerArray[destReg];
		++it;
	}
	else if (stringMips.compare("LW") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source = stringMips;
		string data = stringMips;

		source = RemoveParenthesis(source);
		int sourceReg = GetRegister(source);
		int dataSlot = GetData(data);
		int loadData = ((registerArray[sourceReg] + dataSlot) - dataAddress) / 4;
		int dataValue = dataVector[loadData];

		registerArray[destReg] = dataValue;
		++it;
	}
	else if (stringMips.compare("BREAK") == 0)
	{
		return true;
	}
	else if (stringMips.compare("XOR") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int reg1Value = registerArray[source1Reg];

		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);
		int reg2Value = registerArray[source2Reg];
		string reg1Binary = DecimalToBinary(reg1Value);
		string reg2Binary = DecimalToBinary(reg2Value);

		int newValue = XorOperator(reg1Binary, reg2Binary);
		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("MUL") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);

		registerArray[destReg] = registerArray[source1Reg] * registerArray[source2Reg];
		++it;
	}
	else if (stringMips.compare("ADD") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);

		registerArray[destReg] = registerArray[source1Reg] + registerArray[source2Reg];
		++it;
	}
	else if (stringMips.compare("SUB") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);

		registerArray[destReg] = registerArray[source1Reg] - registerArray[source2Reg];
		++it;
	}
	else if (stringMips.compare("AND") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int reg1Value = registerArray[source1Reg];

		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);
		int reg2Value = registerArray[source2Reg];
		string reg1Binary = DecimalToBinary(reg1Value);
		string reg2Binary = DecimalToBinary(reg2Value);

		int newValue = AndOperator(reg1Binary, reg2Binary);
		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("OR") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int reg1Value = registerArray[source1Reg];

		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);
		int reg2Value = registerArray[source2Reg];
		string reg1Binary = DecimalToBinary(reg1Value);
		string reg2Binary = DecimalToBinary(reg2Value);

		int newValue = OrOperator(reg1Binary, reg2Binary);
		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("ADDU") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);

		registerArray[destReg] = registerArray[source1Reg] + registerArray[source2Reg];
		++it;
	}
	else if (stringMips.compare("SUBU") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		stream >> stringMips;
		string source2 = stringMips;
		int source2Reg = GetRegister(source2);

		registerArray[destReg] = registerArray[source1Reg] - registerArray[source2Reg];
		++it;
	}
	else if (stringMips.compare("ORI") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int decimalValue = registerArray[source1Reg];

		stream >> stringMips;
		string immediate = stringMips;
		int immediateValue = RemoveHashMark(immediate);
		string regBinary = DecimalToBinary(decimalValue);
		string immBinary = DecimalToBinary(immediateValue);

		int newValue = OrOperator(regBinary, immBinary);
		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("XORI") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int decimalValue = registerArray[source1Reg];

		stream >> stringMips;
		string immediate = stringMips;
		int immediateValue = RemoveHashMark(immediate);
		string regBinary = DecimalToBinary(decimalValue);
		string immBinary = DecimalToBinary(immediateValue);

		int newValue = XorOperator(regBinary, immBinary);
		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("ADDI") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);

		stream >> stringMips;
		string immediate = stringMips;
		int immediateValue = RemoveHashMark(immediate);

		registerArray[destReg] = registerArray[source1Reg] + immediateValue;
		++it;
	}
	else if (stringMips.compare("SUBI") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);

		stream >> stringMips;
		string immediate = stringMips;
		int immediateValue = RemoveHashMark(immediate);

		registerArray[destReg] = registerArray[source1Reg] - immediateValue;
		++it;
	}
	else if (stringMips.compare("ANDI") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int decimalValue = registerArray[source1Reg];

		stream >> stringMips;
		string immediate = stringMips;
		int immediateValue = RemoveHashMark(immediate);
		string regBinary = DecimalToBinary(decimalValue);
		string immBinary = DecimalToBinary(immediateValue);

		int newValue = AndOperator(regBinary, immBinary);
		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("SRL") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int decimalValue = registerArray[source1Reg];

		stream >> stringMips;
		string immediate = stringMips;
		int shiftValue = RemoveHashMark(immediate);
		string binary = DecimalToBinary(decimalValue);
		int newValue = ShiftRightLogical(binary, shiftValue);

		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("SRA") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int decimalValue = registerArray[source1Reg];

		stream >> stringMips;
		string immediate = stringMips;
		int shiftValue = RemoveHashMark(immediate);
		string binary = DecimalToBinary(decimalValue);
		int newValue = ShiftRightArithmetic(binary, shiftValue);

		registerArray[destReg] = newValue;
		++it;
	}
	else if (stringMips.compare("SLL") == 0)
	{
		stream >> stringMips;
		string dest = stringMips;
		int destReg = GetRegister(dest);

		stream >> stringMips;
		string source1 = stringMips;
		int source1Reg = GetRegister(source1);
		int decimalValue = registerArray[source1Reg];

		stream >> stringMips;
		string immediate = stringMips;
		int shiftValue = RemoveHashMark(immediate);
		string binary = DecimalToBinary(decimalValue);
		int newValue = ShiftLeftLogical(binary, shiftValue);

		registerArray[destReg] = newValue;
		++it;
	}
	return false;
}

// Unsigned
long int BinaryToDecimal(string &binary)
{
	char *ptr;
	char arr[32];
	strcpy(arr, binary.c_str());
	long int binaryDecimal = strtol(arr, &ptr, 2);
	return binaryDecimal;
}
// Singed/Twos complement
long int TwosComplementToDecimal(string &complement)
{
	char *ptr;
	char arr[32];

	if (complement[0] == '0')
	{
		strcpy(arr, complement.c_str());
		long int binaryDecimal = strtol(arr, &ptr, 2);
		return binaryDecimal;
	}
	int size = complement.size();
	// if all value is 1's return -1
	size_t zeroFound = complement.find("0");
	if (zeroFound == string::npos)
	{
		long int twoComplement = -1;
		return twoComplement;
	}
	else if (size == 16)
	{
		bitset<16> meow(complement);
		meow.flip();
		string one = "1";
		string result = AddBinaryStrings(one, meow.to_string());

		strcpy(arr, result.c_str());
		long int data = strtol(arr, &ptr, 2);
		data = data * -1;
		return data;
	}
	else
	{
		bitset<32> meow(complement);
		meow.flip();
		string one = "1";
		string result = AddBinaryStrings(one, meow.to_string());

		strcpy(arr, result.c_str());
		long int data = strtol(arr, &ptr, 2); 
		data = data * -1;
		return data;
	}

}
// helps convert from two's complement to binary
string AddBinaryStrings(string one, string two)
{
	string resultString = "";
	int carry = 0;
	int strOne = one.size() - 1;
	int strTwo = two.size() - 1;
	while (strOne >= 0 || strTwo >= 0 || carry == 1)
	{
		carry += ((strOne >= 0) ? one[strOne] - '0' : 0);
		carry += ((strTwo >= 0) ? two[strTwo] - '0' : 0);
		resultString = char(carry % 2 + '0') + resultString;
		carry /= 2;
		strOne--; strTwo--;
	}
	return resultString;
}
// Deletes the empty spaces.
string DeleteSpaces(string &newString)
{
	newString.erase(remove(newString.begin(), newString.end(), ' '), newString.end());
	return newString;
}
// Adds # to string immediate
string AddHashMark(long int value)
{
	string hash = "#";
	stringstream temp;
	temp << value;
	string empty;
	temp >> empty;
	hash.append(empty);
	return hash;
}
// Adds R infront of register string
string AddCharR(long int value)
{
	string r = "R";
	stringstream temp;
	temp << value;
	string empty;
	temp >> empty;
	r.append(empty);
	return r;
}
// appends colon to an int
string AddColon(int value)
{
	string colon = ":";
	stringstream temp;
	temp << value;
	string empty = temp.str();
	empty.append(colon);
	return empty;
}
// removes parenthesis from lw/sw
string RemoveParenthesis(string &str)
{
	string temp;
	string leftP = "(";
	string rightP = ")";
	int left = str.find(leftP);
	int right = str.find(rightP);
	temp = str.substr(left + 1, right - left - 1);
	return temp;
}
// converts decimal into binary
string DecimalToBinary(int value)
{
	string binaryStr;
	if (value >= 0)
	{
		binaryStr = bitset<32>(value).to_string();
		return binaryStr;
	}
	else
	{
		value = value * -1;
		bitset<32> meow(value);
		meow.flip();
		string one = "1";
		string result = AddBinaryStrings(one, meow.to_string());
		return result;
	}
}
// XOR logic
int XorOperator(string regBinary, string immBinary)
{
	stringstream ss;
	for (int i = 0; i < regBinary.length(); i++)
	{
		ss << (regBinary.at(i) ^ immBinary.at(i));
	}
	string convert;
	convert = ss.str();
	int result = TwosComplementToDecimal(convert);
	return result;
}
// OR logic
int OrOperator(string regBinary, string immBinary)
{
	stringstream ss;
	for (int i = 0; i < regBinary.length(); i++)
	{
		ss << (regBinary.at(i) | immBinary.at(i));
	}
	string convert;
	convert = ss.str();
	int result = TwosComplementToDecimal(convert);
	return result;
}
// AND logic
int AndOperator(string regBinary, string immBinary)
{
	stringstream ss;
	for (int i = 0; i < regBinary.length(); i++)
	{
		ss << (regBinary.at(i) & immBinary.at(i));
	}
	string convert;
	convert = ss.str();
	int result = TwosComplementToDecimal(convert);
	return result;
}
// removes # from immediate values
int RemoveHashMark(string immediate)
{
	int immediateValue;
	immediate.erase(0, 1);
	istringstream ss(immediate);
	ss >> immediateValue;
	return immediateValue;
}
// removes the R from R5 
int GetRegister(string mipRegister)
{
	int registerValue;
	mipRegister.erase(0, 1);
	istringstream ss(mipRegister);
	ss >> registerValue;
	return registerValue;
}
//Gets immediate from lw/sw xxx(R0) gets the xxx
int GetData(string data)
{
	int value;
	string leftP = "(";
	int findP = data.find(leftP);
	data = data.substr(0, findP);
	istringstream ss(data);
	ss >> value;
	return value;
}
// SLL logic
int ShiftLeftLogical(string &binary, int shiftValue)
{
	string zero = "0";
	for (int i = 0; i < shiftValue; i++)
	{
		binary.erase(0, 1);
		binary = binary + zero;
	}
	long int adjustedValue = TwosComplementToDecimal(binary);
	return adjustedValue;
}
// SRA logic
int ShiftRightArithmetic(string &binary, int shiftValue)
{
	string zero = "0";
	string one = "1";
	if (binary[0] == '0')
	{
		for (int i = 0; i < shiftValue; i++)
		{
			binary.erase(31, 1);
			binary = zero + binary;
		}
	}
	else
	{
		for (int i = 0; i < shiftValue; i++)
		{
			binary.erase(31, 1);
			binary = one + binary;
		}
	}
	long int adjustedValue = TwosComplementToDecimal(binary);
	return adjustedValue;
}
// SRL logic
int ShiftRightLogical(string &binary, int shiftValue)
{	
	string zero = "0";
	for (int i = 0; i < shiftValue; i++)
	{
		binary.erase(31, 1);
		binary = zero + binary;
	}
	long int adjustedValue = BinaryToDecimal(binary);
	return adjustedValue;
}

// Disassembles instructions into disassembly.txt
void CreateDisassemblyFile(deque<string> linesInFile)
{
	ofstream disassembly;
	disassembly.open("disassembly.txt");

	int address = 64;
	string tab = "\t";
	string space = " ";
	string comma = ", ";
	bool breakReached = false;
	// Disassemble Instructions
	while (!breakReached)
	{
		string currentLine = linesInFile.front();
		string mipsInstruction;
		stringstream add;
		add << address;
		mipsInstruction = add.str();
		string newLine = DeleteSpaces(currentLine);
		breakReached = GetMipsInstruction(newLine, address);
		disassembly << newLine << '\t' << address << '\t' << caller.instruction << ' ';

		mipsInstruction.append(tab);
		mipsInstruction.append(caller.instruction);
		mipsInstruction.append(space);
		if (!caller.r1.empty())
		{	
			mipsInstruction.append(caller.r1);
			disassembly << caller.r1;
			if (!caller.r2.empty())
			{
				mipsInstruction.append(comma);
				mipsInstruction.append(caller.r2);
				disassembly << ", " << caller.r2;

				if (!caller.r3.empty())
				{	
					mipsInstruction.append(comma);
					mipsInstruction.append(caller.r3);
					disassembly << ", " << caller.r3;
				}
			}
		}
		instructions.push_back(mipsInstruction);
		disassembly << '\n';
		linesInFile.pop_front();
		address = address + 4;
	}

	dataAddress = address;
	// Data After Break
	while (!linesInFile.empty())
	{

		string currentLine = linesInFile.front();
		string newLine = DeleteSpaces(currentLine);

		disassembly << newLine << '\t' << address << '\t';
		long int data = TwosComplementToDecimal(newLine);
		dataVector.push_back(data);
		disassembly << data << '\n';

		address = address + 4;

		linesInFile.pop_front();
	}
	disassembly.close();
}
// Simulations instructions into simulation.txt
void CreateSimulationFile()
{
	ofstream simulation;
	simulation.open("simulation.txt");
	string dashes = "--------------------";
	string registers = "Registers";
	string data = "Data";
	string rZeroZero = "R00:";
	string rZeroEight = "R08:";
	string rOneSix = "R16:";
	string rTwoFour = "R24:";
	string cycle = "Cycle ";
	int cycleNumber = 1;
	bool isBreak = false;
	it = instructions.begin();
	while (!isBreak)
	{
		string cycleNum = AddColon(cycleNumber);
		simulation << dashes << '\n';
		simulation << cycle << cycleNum << '\t' << *it << '\n';
		simulation << '\n' << registers << '\n';
		simulation << rZeroZero << '\t';

		string currentInstruction = *it;

		isBreak = SimulateMipsInstruction(currentInstruction);

		for (int i = 0; i < 32; i++)
		{
			simulation << registerArray[i] << '\t';
			if ((i + 1) == 8)
			{
				simulation << '\n' << rZeroEight << '\t';
			}
			else if ((i + 1) == 16)
			{
				simulation << '\n' << rOneSix << '\t';
			}
			else if ((i + 1) == 24)
			{
				simulation << '\n' << rTwoFour << '\t';
			}
		}
		simulation << '\n' << '\n' << data << '\n';
		int tracker = dataAddress;
		string trackerStr = AddColon(tracker);
		simulation << trackerStr << '\t';
		for (int i = 0; i < dataVector.size(); i++)
		{
			simulation << dataVector[i] << '\t';
			if ((i + 1) % 8 == 0 && (i + 1) != dataVector.size())
			{
				tracker = tracker + 32;
				trackerStr = AddColon(tracker);
				simulation << '\n' << trackerStr << '\t';
			}
		}
		cycleNumber++;
		simulation << '\n';
	}
	simulation.close();
}
