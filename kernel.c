// Jesse Thompson, TJ Bourget
// COMP 350
// ProjectB

//Prototypes
void printChar(char); 
void printString(char*);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int,int,int,int);

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

//Sean's function
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

//Craig's Function
void printString(char* chars)
{
    chars[sizeof(chars)] = 0x0;
    // print out the char array
}

//TJ's function
void printChar(char c)
{
    interrupt(0x10, 0xe*256+c,0,0,0);
}

//Jesse's function
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
}

//Chooses the proper interrupt function call based on the value of 'ax'
void handleInterrupt21(int ax, int bx, int cx, int dx)
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
