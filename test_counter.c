#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int counter = 0;

void inc_counter(void){
    counter++;
}

int get_counter(void){
   return counter;
}


int main(int argc, char *argv[]){
   pid_t pid;
   pid = fork();

   if(pid < 0){
      fprintf(stderr,"Un problème est survenu lors de la création du processus\n");
      return 1;
   }

   if(pid==0){
     // Dans le fils
     inc_counter();
     inc_counter();
     int val = get_counter();
     printf("La valeur du compteur dans le fils: %d\n", val);
     return 0;
   }else{
     // Dans le père
     inc_counter();
     inc_counter();
     inc_counter();
     int val = get_counter();
     printf("La valeur du compteur dans le père: %d\n", val);
     return 0;
   }
}
