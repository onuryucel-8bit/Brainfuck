//Sources
/*

https://www.youtube.com/watch?v=4uNM73pfJn0
https://www.youtube.com/watch?v=dxJpGVaCAyU
https://www.youtube.com/watch?v=tcHaMWktCYE

https://en.wikipedia.org/wiki/Brainfuck
https://tr.wikipedia.org/wiki/Brainfuck
https://esolangs.org/wiki/Brainfuck

*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>

#include "Fileio/FileReader.h"

const int RAM_LIMIT = 100;
char RAM[RAM_LIMIT];

int position = 0;

std::string programPath = "prog.txt";

std::string program;
int instructionPointer = 0;

/*
*	pos[ => pos]
*	pos] => pos[
*/
std::unordered_map<int, int> loopPositionTable;

void clearRAM()
{
	for (size_t i = 0; i < RAM_LIMIT; i++)
	{
		RAM[i] = 0;
	}
}

void printRAM()
{
	std::cout << "-----------------------\n";
	std::cout << "printing RAM\n";
	for (size_t i = 0; i < RAM_LIMIT; i++)
	{
		std::cout <<"["<< i << "]= " << (int)RAM[i] << " ";

		if ((i + 1) % 16 == 0)
		{
			std::cout << "\n";
		}
	}
}

void loadLoopPositions()
{
	std::stack<size_t> stack;

	for (size_t i = 0; i < program.length(); i++)
	{
		if (program[i] == '[')
		{
			//push index of begginning loop
			stack.push(i);
		}
		else if (program[i] == ']')
		{
			size_t begLoop = stack.top();
			stack.pop();
			//pos [ => pos ]
			loopPositionTable[begLoop] = i;
			//pos ] => pos [
			loopPositionTable[i] = begLoop;
		}
	}
}

int main()
{
	FileReader fr;
	program = fr.read(programPath.c_str());

	clearRAM();

	loadLoopPositions();

	char command = 0;
	while(instructionPointer < program.length())
	{
		command = program[instructionPointer];

		switch (command)
		{
		case '+':			
			RAM[position]++;
			break;
		case '-':			
			RAM[position]--;
			break;
		case '<':
			if (position - 1 < 0)
			{
				position = RAM_LIMIT - 1;
			}
			else
			{
				position--;
			}			
			break;
		case '>':			
			if (position + 1 >= RAM_LIMIT)
			{
				position = 0;
			}
			else
			{
				position++;
			}			
			break;
		case '.':
			std::cout << RAM[position];
			break;
		case ',':
			std::cin >> RAM[position];
			break;
		case '[':
			if (RAM[position] == 0)
			{
				instructionPointer = loopPositionTable[instructionPointer];
			}
			break;
		case ']':
			if (RAM[position] != 0)
			{
				instructionPointer = loopPositionTable[instructionPointer];
			}
			break;

		default:
			break;
		}

		instructionPointer++;
	}

	printRAM();
}
