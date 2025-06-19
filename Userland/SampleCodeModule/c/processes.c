#include <standardlib.h>

void startProcess(int argc, char* argv[]){
	while(1){
		printf("Simple process running.\nargc: %d\n", argc);
	}
}

void testFunc(int argc, char* argv[]){
	while(1){
		printf("Function %d running!\n", argc);
	}
}

void bloqueadoFunc(int argc, char* argv[]){
	uint8_t sem = sem_open("test", 1);
	int i;
	while(1){
		printf("Bloqueado sem_wait value: %d\n", sem_wait(sem));
		i = 0;
		sleep(500);
	}
}

void liberadorFunc(int argc, char* argv[]){
	uint8_t sem = sem_open("test", 1);
	int i;
	while(1){
		printf("Liberador sem_post value: %d\n", sem_post(sem));
		i = 0;
		sleep(800);
	}
}

void hablaFunc(int argc, char* argv[]){
	int fds[2];
	int i;
	uint8_t pipeId = pipe_open("hola", fds);
	while(1){
		printf("Hablador habla %d\n", pipeId);
		//char* msg = "Hello world\0\n";
		//(void)pipe_write(fds[1], msg, 12);
		i = 0;		
		//sleep(1000);
	}
}

void escuchaFunc(int argc, char* argv[]){
	int fds[2];
	int i;
	//uint8_t pipeId = pipe_open("hola", fds);
	while(1){
		//printf("Escuchador escucha %d\n", pipeId);
		//char buffer[64];
		//(void)pipe_read(fds[0], buffer, 12);
		i = 0;
		//printf("Escuchador received: %s\n", buffer);
		//sleep(1000);
	}
}

void loopFunc(int argc, char* argv[]){
	int fd[2];
	pipe_open("loopPipe", fd);
	char buf[10];
	pipe_read(fd[0], buf, 10);
	char seconds[10];
	int it = 0;
	uint32_t time = unsigned_str_to_num(&it, 10, seconds);
	while(1){
		sleep(18*time);
		printf("Pid from loop function: %d\n", buf);
	}
}