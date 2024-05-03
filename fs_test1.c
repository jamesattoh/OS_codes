#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFF_SIZE 2048

int main(int argc, char *argv[]){
   int fd, ret;
    char buffer[BUFF_SIZE];
   if(argc != 2){
       fprintf(stderr,"usage %s  <nom_fichier>\n", argv[0]);
        return 1;
   }
   fd  = open(argv[1], O_CREAT| O_WRONLY ,  S_IRUSR| S_IWUSR);
   if(fd == -1){
        fprintf(stderr, "Une erreur vient de se produire à l'ouverture du fichier\n");
        return 2;
   }
   printf("Pouvez svp nous saisi un texte ?\n");
   scanf("%s", buffer);
   ret = write(fd, buffer, strlen(buffer));
   if( ret < 0){
      fprintf(stderr, "Il ya eu un problème lors de l'écriture dans le fichier");
      fprintf(stderr, "%d\n", errno);
      return 3;
   }
   close(fd);
   return 0;
}
