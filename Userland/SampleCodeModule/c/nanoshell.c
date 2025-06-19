#include <nanoshell.h>
#include <standardlib.h>
#include <videolib.h>
#include <processes.h>
#include <stddef.h>

#define CMD_MAX_CHARS 1000
#define CMD_NAME_MAX_CHARS 100
#define PROMPT "NanoShell $> "

void startProcess(int argc, char* argv[]);
void testFunc(int argc, char* argv[]);

void bloqueadoFunc(int argc, char* argv[]);
void liberadorFunc(int argc, char* argv[]);
void hablaFunc(int argc, char* argv[]);
void escuchaFunc(int argc, char* argv[]);
void loopFunc(int argc, char* argv[]);
	
// add new command or useful here
static char *instructions[] = {"help", "registers", "time", "echo", "clear", "test_zero_division", \
"test_invalid_opcode", "test_malloc", "todo", "functions", "mini_process", "test_priority",\
"test_semaphore", "test_pipe", "sh", "mem", "ps", "loop", "kill", "nice", "block", "cat", "wc", \
"filther", "phylo", \
/*useful*/ "malloc", "realloc", "calloc", "free", "createProcess", "getPriority", "setPriority", 0,};

// add new command here
static char *help_text = "Here's a list of all available commands:\n\
- help --> Help display with all commands\n\
- registers --> Displays the lastest backup of registers\n\
- time --> Displays time and date\n\
- echo [string] --> Prints the [string] argument in the display\n\
- clear --> clears the display\n\
- test_zero_division --> Test for the Zero Division exception\n\
- test_invalid_opcode --> Test for the Invalid Opcode exception\n\
- test_malloc --> starts the malloc test\n\
- todo --> displays a random thing that has to be done\n\
- functions --> displays every page inside the manual\n\
- mini_process --> creates a new process according to simpleProcess.c\n\
- test_priority --> test that the priority system is working correctly\n\
- test_semaphore --> test that the semaphore system is working correctly NOT TESTED\n\
- test_pipe --> test that the pipe system is working correctly NOT TESTED\n\
- sh --> correctly executes what was asked from it NOT DONE\n\
- mem --> shows the memory state NOT DONE\n\
";

static char *help_text2 = "- ps --> prints a list of every running process with some data from each NOT DONE\n\
- loop [count] --> makes a process run and print its id along with a greeting every [count] of seconds NOT TESTED\n\
- kill [pid] --> kills a process based on its [pid] NOT TESTED\n\
- nice [pid] [new priority] --> changes the [pid] process to be of [new priority] priority NOT TESTED\n\
- block [pid] --> changes the [pid] process between blocked and unblocked NOT TESTED\n\
- cat --> prints the stdin NOT TESTED\n\
- wc --> counts the amount of lines in the input NOT TESTED\n\
- filther --> filthers the vowels from the input NOT TESTED\n\
- phylo --> starts running the phylosofers problem. \"a\" to add 1, \"r\" to remove one NOT DONE\n\
";

// add new command or useful here
static char *functions = "\
Commands: help, registers, time, echo, clear, test_zero_division\n\
test_invalid_opcode, test_malloc, todo, functions, mini_process, test_priority\n\n\n\
Useful: malloc, realloc, calloc, free, getPriority, setPriority\n";

// add new command or useful here
static char *todo[] = {
// help
"Check that they are on date",
// registers
"",
// time
"",
// echo
"Make it be able to print other things (echo test_malloc for example)",
// clear
"",
// test_zero_divition
"",
// test_invalid_opcode
"",
// test_malloc
"",
// todo
"write a lot of TODOs\n\
make it return a random todo",
// functions
"Not implemented",
// mini_process
"Make easier to understand",
// test_priority
"",

// Useful

// malloc
"",
// realloc
"Not implemented",
// calloc
"Not implemented",
// free
"",
// createProcess
"Not implemented",
// getPriority
"",
// setPriority
"",
};

static uint64_t readCommand(char *buff);
static int interpret(char *command);

void shell();

void startNanoShell(){
	char* argv[] = {""};
	pid_t pid = (pid_t)createProcess(&shell, 1, argv);
}

// add new command here
void shell()
{
    char cmdBuff[CMD_MAX_CHARS + 1] = {0};
    int exit = 0;
    while (!exit)
    {
        fdprintf(STDMARK, PROMPT);
        int command_length = readLine(cmdBuff, CMD_MAX_CHARS);

        int interpretation = interpret(cmdBuff);
		
		TimeStamp ts = {0};

        char toPrint[100];
        int i = 0;
		int j = 0;


		char* argv[] = {""};

		pid_t pid;

        switch (interpretation)
        {
        case HELP:
            printf(help_text);
			printf(help_text2);
            break;

        case REGISTERS:
            getRegisters();
            break;

        case TIME:
            printCurrentTime();
            break;

        case ECHO:
            while (cmdBuff[i] && cmdBuff[i] != ' ' && cmdBuff != '\t')
            {
                i++;
            }
            i++;
            for (j = 0; cmdBuff[i]; i++, j++)
            {
                toPrint[j] = cmdBuff[i];
            }
            toPrint[j] = 0;
            printf(toPrint);
            break;
            
        case CLEAR:
            clearScreen();
            break;

        case TEST_ZERO_DIVISION:
            testZeroDivision();
            break;

        case TEST_INVALID_OPCODE:
            testInvalidOpcode();
            break;

		case TEST_MALLOC:
			printf("%d", testMalloc());
			break;

		case TODO:
    		syscall(6, &ts, 0, 0);
			while(todo[ts.seconds%INSTRUCTION_COUNT] == ""){
				ts.seconds++;
			}
			printf("%d\n", ts.seconds);
			printf("%s\n", todo[ts.seconds%INSTRUCTION_COUNT]);
			break;

		case FUNCTIONS:
			printf("%s\n", functions);
			break;

		case MINI_PROCESS:
			pid = (pid_t)createProcess(&startProcess, 1, argv);
			break;
		
		case TEST_PRIORITY:
			;
			pid_t pid1, pid2, pid3;
			pid1 = (pid_t)createProcess(&testFunc, 1, argv);
			pid2 = (pid_t)createProcess(&testFunc, 2, argv);
			pid3 = (pid_t)createProcess(&testFunc, 3, argv);
			printf("priorities: %d: %d, %d: %d, %d: %d\n\n", pid1, getPriority(pid1), pid2, getPriority(pid2), pid3, getPriority(pid3));
			setPriority(pid1, 1);
			setPriority(pid2, 7);
			setPriority(pid3, 7);
			printf("priorities: %d: %d, %d: %d, %d: %d\n\n", pid1, getPriority(pid1), pid2, getPriority(pid2), pid3, getPriority(pid3));
			break;

		case TEST_SEMAPHORE:;
			pid_t bloqueado, liberador;
			bloqueado = (pid_t)createProcess(&bloqueadoFunc, 1, argv);
			liberador = (pid_t)createProcess(&liberadorFunc, 1, argv);
			break;

		case TEST_PIPE:;
			pid_t habla, escucha;
			escucha = (pid_t)createProcess(&escuchaFunc, 1, argv);
			habla = (pid_t)createProcess(&hablaFunc, 1, argv);
			setPriority(habla, 5);
			break;

		case SH:;
			
			break;

		case MEM:;
	
			break;

		case PS:;
	
			break;

		case LOOP:;
			int fd[2];
			pipe_open("loopPipe", fd);
			pid_t pid = (pid_t)createProcess(&loopFunc, 1, argv);
			char bufLoop[100];
			unsigned_num_to_str(pid, 0, bufLoop);
			pipe_write(fd[1], bufLoop, 10);
			
			while (cmdBuff[i] && cmdBuff[i] != ' ' && cmdBuff != '\t')
            {
                i++;
            }
            i++;
            for (j = 0; cmdBuff[i]; i++, j++)
            {
                toPrint[j] = cmdBuff[i];
            }
            toPrint[j] = 0;
			pipe_write(fd[1], toPrint, 10);
			break;

		case KILL:;
			while (cmdBuff[i] && cmdBuff[i] != ' ' && cmdBuff != '\t')
            {
                i++;
            }
            i++;
            for (j = 0; cmdBuff[i]; i++, j++)
            {
                toPrint[j] = cmdBuff[i];
            }
            toPrint[j] = 0;
			uint64_t itKill = 0;
			uint32_t pidKill = unsigned_str_to_num(&itKill, 100, toPrint);
			killProcess(pidKill);
			break;

		case NICE:;
			while (cmdBuff[i] && cmdBuff[i] != ' ' && cmdBuff != '\t')
            {
                i++;
            }
            i++;
            for (j = 0; cmdBuff[i]; i++, j++)
            {
                toPrint[j] = cmdBuff[i];
            }
            toPrint[j] = 0;
			char newPriority[100];
			while (cmdBuff[i] && cmdBuff[i] != ' ' && cmdBuff != '\t')
            {
                i++;
            }
            i++;
            for (j = 0; cmdBuff[i]; i++, j++)
            {
                newPriority[j] = cmdBuff[i];
            }
            newPriority[j] = 0;
			uint64_t itNice = 0;
			uint32_t pidNice = unsigned_str_to_num(&itNice, 100, toPrint);
			uint32_t priorityNice = unsigned_str_to_num(&itNice, 100, newPriority);
			setPriority(pidNice, priorityNice);	
			break;

		case BLOCK:;
			while (cmdBuff[i] && cmdBuff[i] != ' ' && cmdBuff != '\t')
            {
                i++;
            }
            i++;
            for (j = 0; cmdBuff[i]; i++, j++)
            {
                toPrint[j] = cmdBuff[i];
            }
            toPrint[j] = 0;
			uint64_t it = 0;
			uint32_t pidBlock = unsigned_str_to_num(&it, 100, toPrint);
			blockProcess(pidBlock); // doesnt work
			break;

		case CAT:;
			 int c;
			/* leemos hasta Enter (simple) */
			while ((c = getc()) != '\n')      // getc() bloquea hasta que llega algo
				putChar(c);
			putChar('\n');
			break;

		case WC:;
			int lines = 0;
			while(*cmdBuff != '\n'){
				if (*cmdBuff == '\n'){
					lines++;
				}
			}
			printf("lineas: %d\n", lines);
			break;

		case FILTER:;
			while(*cmdBuff != '\n'){
				if (!strchr("aeiouAEIOU", *cmdBuff)){
				    putChar(*cmdBuff);
				}
			}
			putChar('\n');
			break;

		case PHYLO:;
	
			break;
			

        case -1:
            printf("Command not found: '%s'", cmdBuff);
            break;
        }

        if (interpretation != CLEAR)
        {
            printf("\n");
        }
    }
}

static int interpret(char *command)
{
    char actualCommand[CMD_MAX_CHARS] = {0};
    int i;
    for (i = 0; i < CMD_MAX_CHARS && command[i] != 0 && command[i] != ' ' && command[i] != '\t'; i++)
    {
        actualCommand[i] = command[i];
        toMinus(actualCommand);
    }
    if (i == CMD_MAX_CHARS && command[i] != 0)
        return -1;
    for (i = 0; instructions[i]!=0; i++)
    {
        if (strcmp(actualCommand, instructions[i]) == 0)
            return i;
    }
    return -1;
}