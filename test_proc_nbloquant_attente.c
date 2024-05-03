#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){

   pid_t pid, ret;
   int pipefd[2];   
   ret = pipe2(pipefd, O_NONBLOCK); // rendons le pipe non bloquant
   if(ret < 0){
      fprintf(stderr, "Il ya eu un problème à la création du pipe\n");
      return 2;
   }
   pid = fork();


   if(pid < 0){
      fprintf(stderr, "Une erreur s'est produite lors de la création du processus\n");
      return 1;
   }

   if(pid == 0){
      int fd;
      // le fils écrit dans le pipe
      close(pipefd[0]);     
      printf("Moi: %d  Mon Père: %d\n", getpid(), getppid());
      // j'envoie le contenu d'un fichier secret à mon père
      char nomFichier[100];
      printf("Donnez svp le nom du fichier secret, très très secret\n");
      scanf("%s", nomFichier);

      fd = open(nomFichier, O_RDONLY);

      if(ret < 0){
         printf("Un problème est survenu lors de l'ouverture du fichier\n");
         return 1;
      }
      char buffer[2048];
      while((ret = read(fd, buffer, 2048)) > 0){
          write(pipefd[1], buffer, ret);
      }
      return 0;
   }else{

      // le père lit dans le pipe
      close(pipefd[1]);
      printf("Moi: %d Mon fils: %d Mon père: %d\n", getpid(), pid, getppid());
      // je lis les informations envoyées par mon fils et je les affiche à l'écran
      char buffer[2048];
      int count = 0; // compteur pour verifier l'exécution du read
      do{
          ret = read(pipefd[0], buffer, 20);
      }while(ret < 0 && errno == EAGAIN);
      
      if(ret < 0){
          fprintf(stderr, "Problème de lecture dans le pipe\n");
          return 3;
      }
      printf("%s", buffer);
      count++;
      
      // obligeons le père à attendre la fin de son fils avant de se terminer
      int status;
      pid = wait(&status);
      if(pid < 0)
          fprintf(stderr, "Une erreur s'est produite dans l'attente du fils\n");
      // Quelle est la valeur du compteur

      printf("Nous sommes à la fin compteur= %d\n", count);
      return 0;
   }

}
