// Jesse Thompson, TJ Bourget
// COMP 350
// ProjectB

#include <stdio.h>
#include <string.h>

void printChar(char);   // prototype function to establish the function to avoid errors

void main ()
{
    // vars must be defined at top of file
    char myLetter = 'W';
    char line[80];
    char buffer[512];

    //Test interrupts
    makeInterrupt21();
    //Test printChar()
    interrupt(0x21,100,myLetter,0,0);
    //Test readString()
    interrupt(0x21,1,line,0,0);
    //Test printString()
    interrupt(0x21,0,line,0,0);
    //Test readSector() (requires printString() to be functional to see if it worked)
    interrupt(0x21,2,buffer,30,0);
    interrupt(0x21,0,buffer,0,0);

    while(1);
}

void readString(char* argv[])
{
    int i = 0;
    char* currentString[80];
    while (*argv[i] != 0xd)
    {
        interrupt(0x16, currentString[i] = argv[i]);
        i++;
    }

    *argv[i + 1] = 0xa;
    *argv[i + 2] = 0x0;

    return;
}

void printString(char* chars)
{
    chars[sizeof(chars)] = 0x0;
    // print out the char array
}

void printChar(char c)
{
    // call _interrupt(number, ax, bx, cx, dx)
    char ah = 0xe;
    char al = c;
    int ax = ah * 256 + al;         // combines ah and al into ax, 256 = 0x100
    // can also be interrupt(0x10, 0xe * 256 + c, 0, 0, 0);
}

void readSector(char* buffer, int sector)
{
    int ah = 2;             // tells BIOS to read sector
    int al = 1;             // number of sectors to use
    int bx = buffer;        // address where data should be stored
    int ch = 0;             // track number
    int cl = sector + 1;    // relative sector number
    int dh = 0;             // head number
    int dl = 0x80;          // device number

    interrupt(0x13, buffer = sector);

//Chooses the proper interrupt function call based on the value of 'ax'
{
	switch(ax)
	{
		case 0:
			printString(bx);
			break;
		case 1:
			readString(bx);
			break;
		case 2:
			readSector(bx, cx);
			break;
		case 100:
			readChar(bx);
			break;
		default:
			printString("No interrupt function correlated with AX number");
	}
}
