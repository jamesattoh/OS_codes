 #include <sys/types.h>
 #include <unistd.h>
 #include <stdio.h>



int main(int argc, char *argv[]){

	pid_t pid;
	pid = fork();
	
	if(pid < 0){
	fprintf(stderr, "Une erreur s'est produite\n");
	return 1;
	}
	if(pid == 0){
		printf("Moi: %d Mon pere: %d\n",getpid(),getppid());
		return 0;
	}else{
		printf("Moi : %d Mon fils: %d Mon pere: %d\n",getpid(),pid,getppid());
		return 0;
		}
}

