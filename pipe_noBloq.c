#include <sys/wait.h> //le pere ne se termine pas avant sonfils
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main (int argc , char *argv[]){
	pid_t pid, ret;
	int pipefd[2];

	ret = pipe2(pipefd,O_NONBLOCK);

	if (ret < 0){
	fprintf(stderr , "Il y a eu un probleme a la creation de la pile\n");
	return 2;
	}

	pid = fork();

	if (pid < 0) {
	fprintf(stderr, "Une erreur s'est produite lors de la creation du processus\n");
	return 1;
	}

	if (pid == 0){
	int fd;
	//le fils ecrit dnas le pipe
	close(pipefd[0]);

	printf("Moi: %d Mon pere: %d\n",getpid(),getppid());
	//j'envoi le contenu secret d'un fichier a papa
	char nomFichier[100];
	printf("Donnez svp le nom du fichier secret, tres tres secret:\n");
	scanf("%s", nomFichier);

	fd = open (nomFichier, O_RDONLY);
	 if(ret < 0 ){
	  printf("Erreur\n");
	  return 1;
	             }

	       char buffer[2048];
        	while ((ret = read(fd, buffer, 2048))> 0){
                	write (pipefd[1], buffer, ret);
                }
	        return 0;
	}
         else {
	//le  pere lit danas le pipe
	close(pipefd[1]);
	printf("Moi: %d Mon fils: %d Mon pere: %d\n",getpid(),pid,getppid());
	//je lis les nformations envoyées par mon fils et je les affiche à l'ecran
	char buffer [2048];
	while ((ret = read(pipefd[0],buffer, 2048)>0)){
		 printf("%s",buffer);
		  int status;
			pid = wait(&status);
			if(pid < 0)
				fprintf(stderr,"une erreur s'est produite dans l'attente du fils\n");
		  return 0;
		 }
	}
}
