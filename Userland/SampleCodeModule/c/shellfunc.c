#include <nanoshell.h>
#include <standardlib.h>

#define REGISTERS 18
#define NULL_PARAM 0

uint64_t syscall(uint64_t rax, uint64_t rbx, uint64_t rdx, uint64_t rcx);
void throw_zero_division();
void throw_invalid_opcode();

void getRegisters()
{
    char *regName[] = {
        "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ",
        "RBP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ",
        "R13: ", "R14: ", "R15: ", "RSP: ", "RIP: ", "RFLAGS: "};
    printf("Register Status: ");
    uint64_t regsStatus[REGISTERS];
    uint64_t call = syscall(11, regsStatus, NULL_PARAM, NULL_PARAM);
    if (call == 0)
        printf("Register backup not done. Press ESC to save register status.");
    else
    {
        uint64_t reg;
        for (int i = 0; i < REGISTERS; i++)
        {
            reg = regsStatus[i];
            printf("\n%s %x", regName[i], reg);
        }
    }
}

void changeFontSize()
{
    syscall(8, NULL_PARAM, NULL_PARAM, NULL_PARAM);  // Syscall clear
    syscall(10, NULL_PARAM, NULL_PARAM, NULL_PARAM); // Syscall change_font_size
}

void printCurrentTime()
{
    TimeStamp ts = {0};
    syscall(6, &ts, NULL_PARAM, NULL_PARAM);
    printf("Current date: %d-%d-%d\nCurrent time: %d:%d:%d hs", ts.day, ts.month, ts.year, ts.hours, ts.minutes, ts.seconds);
}

void testZeroDivision()
{
    throw_zero_division();
}

void testInvalidOpcode()
{
    throw_invalid_opcode();
}
/*
#define C4  261
#define Cs4 277
#define D4  294
#define Ds4 311
#define E4  329
#define F4  349
#define Fs4 370
#define G4  392
#define Gs4 415
#define A4  440
#define As4 466
#define B4  494

#define C5  523
#define Cs5 554
#define D5  587
#define Ds5 622
#define E5  659
#define F5  698
#define Fs5 740
#define G5  784
#define Gs5 831
#define A5  880
#define As5 932
#define B5  988

#define C6  1047
#define Cs6 1109
#define D6  1175
#define Ds6 1245
#define E6  1319
#define F6  1397
#define Fs6 1480
#define G6  1568
#define Gs6 1661
#define A6  1760
#define As6 1865
#define B6  1976

#define WHOLEN 288
#define HALFN 144
#define QUARTERN 72
#define THREEQUARTERSN 54
#define EIGHTHN 36
#define SIXTEENTHN 18
*/
void nanoAnthem(){
    /*syscall(9,SIXTEENTHN,0,C4);
    syscall(9,SIXTEENTHN,0,F4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,THREEQUARTERSN,0,C4);
    syscall(9,SIXTEENTHN,0,C4);
    syscall(9,SIXTEENTHN,0,F4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,D4);
    syscall(9,THREEQUARTERSN,0,C4);
    syscall(9,SIXTEENTHN,0,C4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,SIXTEENTHN,0,F4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,QUARTERN,0,A4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,SIXTEENTHN,0,D4);
    syscall(9,QUARTERN,0,F4);

    syscall(9,SIXTEENTHN,0,D4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,D4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,SIXTEENTHN,0,D4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,D4);
    syscall(9,EIGHTHN,0,D4);
    syscall(9,SIXTEENTHN,0,C4);
    syscall(9,EIGHTHN,0,D4);
    syscall(9,THREEQUARTERSN,0,C4);
    syscall(9,SIXTEENTHN,0,D4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,E4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);

    syscall(9,QUARTERN,0,A4);
    syscall(9,QUARTERN + EIGHTHN,0,1);    //SILENCIO
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,HALFN,0,G4);
    syscall(9,SIXTEENTHN,0,C4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,SIXTEENTHN,0,G4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,SIXTEENTHN,0,D4);
    syscall(9,QUARTERN,0,C4);
    syscall(9,SIXTEENTHN,0,1);  //SILENCIO
    syscall(9,SIXTEENTHN,0,C4);
    syscall(9,SIXTEENTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,C5);
    syscall(9,EIGHTHN,0,D5);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,QUARTERN,0,A4);
    syscall(9,EIGHTHN,0,1);  //SILENCIO
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,C5);
    syscall(9,EIGHTHN,0,D5);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,SIXTEENTHN,0,F4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,EIGHTHN,0,A4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,G4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,SIXTEENTHN,0,D4);
    syscall(9,EIGHTHN,0,F4);
    syscall(9,EIGHTHN,0,F5);
    syscall(9,EIGHTHN,0,F5); 
    syscall(9,SIXTEENTHN,0,D5);
    syscall(9,EIGHTHN,0,F5); 
    syscall(9,SIXTEENTHN,0,D5);
    syscall(9,QUARTERN,0,C5); */
}