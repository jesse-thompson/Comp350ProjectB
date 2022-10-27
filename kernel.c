//Craig Kimball 10/21/22
// TJ
// Jesse
// Sean

//This is a kernel for parts 1 and 2 of OSProjectB

void printString(char*);
void printChar(char);
void readString();

int main()
{
    char line[80];

    makeInterrupt21();

    // Testing printString as a function
    printString("Hello World!\r\n\0");

    // Testing printString as an interrupt
    interrupt(0x21, 0, "Enter in some text: \0", 0, 0); // I get how to call printString using interrupt 0x21, but not how to pass in a string to it.

    //
    interrupt(0x21, 1, line, 0, 0);

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

void readString(char* line)
{
    // Reads in keys pressed form the user in real time using interrupts.
    // After reading, prints out what was typed also using interrupts.

    int index = 0;

    while (1) { // Stop when user enters 'enter' aka ascii code: 0xD
        // interrupt 0x16 does not display to the screen
        char userChar = interrupt(0x16, 0, 0, 0, 0);
        if (userChar == 0x08) // Don't save the backspace into the array, overwrite charcters instead
        {
            // Do backspace stuff
            if (index != 0) {
                index--;
                printChar(0x08);
                printChar(' ');
                printChar(0x08);
            }
        }
        else {
            line[index] = userChar;
            printChar(userChar); // Prints out what the user just entered
            index++;
        }
        // If user pressed enter
        if (userChar == 0xD)
        {
            printChar(0xA); // prints out a carriage return
            line[index] = 0x0; // Adds \0 to the end of the line
            interrupt(0x21, 0, "You entered: ", 0, 0);
            interrupt(0x21, 0, line, 0, 0); // calls printString(line) to print the line
            break;
        }

    }

}


void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    // Lets interrupt 21 call on 3 different interrupts.

    if (AX == 0)
    {
        printString(BX);
    }
    else if (AX == 1)
    {
        readString(BX);
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
