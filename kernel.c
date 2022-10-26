//Craig Kimball 10/21/22
//This is a kernel for part 1 of OSProjectB

void printString(char*);
void printChar(char);

int main()
{
    char line[80];
    makeInterrupt21();

    printString("Hello World!\0");
    interrupt(0x21, 0, line, 0, 0);

    while(1){}
    return 0;
}


void printChar(char c)
{
    // Prints out character c using an interrupt
    interrupt(0x10, 0xe * 256 + c, 0, 0, 0);
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


void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    if (AX == 0)
    {
        printString(BX);
    }
    else if (AX == 1)
    {
        //readString(CX);
    }
    else if (AX == 2)
    {
        //readSector(DX);
    }
    else
    {
        printString("Please enter 0, 1, or 2 for AX");
    }
}
