//TJ Bourget

void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int,int,int,int);

void main()
{
    char userInput[80];
    char fileInput[512];
    
    makeInterrupt21();
    interrupt(0x21,1,userInput,0,0);
    interrupt(0x21,0,userInput,0,0);
    interrupt(0x21,2,fileInput,30,0);
    interrupt(0x21,0,fileInput,0,0);

	while(1);
}

void printChar(char c)
{
    interrupt(0x10, 0xe*256+c,0,0,0);
}

void printString(char* chars)
{
    int increment = 0;
    while(chars[increment] != 0x0)
    {
        interrupt(0x10, 0xe*256+chars[increment],0,0,0);
        increment++;
    }
}

void readString(char* chars)
{
    int currIndex = 0;
    chars[currIndex] = interrupt(0x16, 0,0,0,0);
    printChar(chars[currIndex]);
    
    while(chars[currIndex] != 0xd && currIndex < 80)
    {
        char input = interrupt(0x16, 0,0,0,0);
        if(input == 0x8)
        {
            if(currIndex >= 0)
            {
                currIndex--;
                printChar(0x8);  
                printChar(' ');
                printChar(0x8);            
            }
        }
        else
        {
            currIndex ++;  
            chars[currIndex] = input;
            printChar(input);       
        }
    }
    printChar(0xa);    
    
    chars[currIndex+1] = 0xa;
    chars[currIndex+2] = 0x0;
}

void readSector(char* buffer, int sector)
{
    interrupt(0x13, 2*256+1, buffer, sector+1, 0x80);
}

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

