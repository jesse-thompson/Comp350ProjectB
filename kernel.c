// Team Awesome: Jesse Thompson, TJ Bourget, Sean Tammelleo, Craig Kimello
// COMP 350 - 001
// ProjectB

void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int,int,int,int);

void main()
{
    // Tests printString()
//    printString("Hello World");

    // Tests readString()
//    char line[80];
//    printString("Enter a line: ");
//    readString(line);
//    printString(line);

    // Tests readSector()
    char buffer[512];
    readSector(buffer, 30);
    printString(buffer);

    while (1);
}


void printString(char* chars)
{
    // Prints out the provided string to the console using interrupts
    int index = 0;

    while(chars[index] != 0x0)
    {
        interrupt(0x10, 0xe * 256 + chars[index], 0, 0, 0);
        index++;
    }
}

void readString(char* letter)
{
    int index = 0;
    //getting letter and printing to screen
    letter[index] = interrupt(0x16,0,0,0,0);
    printChar(letter[index]);

    //check if enter key is pressed and under string limit but not zero
    while (letter[index] != 0xd  && 0 < index < 80)
    {
        char inputLetter = interrupt(0x16,0,0,0,0);
        //if input is backspace
        if (inputLetter == 0x8){
            index--;
            printChar(0x8);
            printChar(' ');
            printChar(0x8);
        }
        else{
            index++;
            letter[index] = inputLetter;
            printChar(letter[index]);
        }
    }
    //once enter key pressed
    letter[index+1] = 0xa;
    letter[index+2] = 0x0;
    printChar(0xa);
}

void printChar(char c)
{
    interrupt(0x10, 0xe*256+c,0,0,0);
}

void readSector(char* buffer, int sector)
{
    int ah = 2;             // tells BIOS to read sector
    int al = 1;             // number of sectors to use
    int ax = ah * 256 + al;

    int ch = 0;             // track number
    int cl = sector + 1;    // relative sector number
    int cx = ch * 256 + cl;

    int dh = 0;             // head number
    int dl = 0x80;          // device number
    int dx = dh * 256 + dl;

    interrupt(0x13, ax, buffer, cx, dx);
}

// Chooses the proper interrupt function call based on the value of 'ax'
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
        default:
            printString("No interrupt function correlated with AX number");

    }
}

