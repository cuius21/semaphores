#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define INPUT "spr.txt"
FILE *fw;
FILE *w;
int semafor;
key_t key;

static void semafor_v(int);
static void semafor_p(int);

int main(){
	if ((key = ftok(".", 'M')) == -1)
        {
                fprintf(stderr, "blad tworzenia klucza P3\\n");
                exit(1);
        }
	semafor=semget(key,5,IPC_CREAT|0777);
 	if (semafor==-1)
        {
                printf("Nie moglem utworzyc nowego semafora.\n");
                exit(EXIT_FAILURE);
        }
    	else
        {
                //printf("Semafor zostal utworzony : %d\n",semafor);
        }
	semafor_p(0);
	fw=fopen(INPUT,"a");
	printf("Sekcja t31 o PID=%d\n",getpid());
        fprintf(fw,"Sekcja t31 o PID=%d\n",getpid());
	sleep(1);
	printf("Sekcja t32 o PID=%d\n",getpid());
        fprintf(fw,"Sekcja t32 o PID=%d\n",getpid());
        fclose(fw);
	sleep(1);
	semafor_v(1);
	semafor_p(4);
	w=fopen(INPUT,"a");
	printf("Sekcja t33 o PID=%d\n",getpid());
        fprintf(w,"Sekcja t33 o PID=%d\n",getpid());
       	fclose(w);
	sleep(1);
        exit(0);
}

static void semafor_p(int n)
{
    int zmien_sem;
    struct sembuf bufor_sem;
    bufor_sem.sem_num=n;
    bufor_sem.sem_op=-1;
    bufor_sem.sem_flg=SEM_UNDO;
    zmien_sem=semop(semafor,&bufor_sem,1);
    if (zmien_sem==-1)
        {
                printf("Nie moglem zamknac semafora.\n");
                exit(EXIT_FAILURE);
        }
    else
        {
                printf("Semafor %d P3 zostal zamkniety.\n",n);
        }
}

static void semafor_v(int n)
{
    int zmien_sem;
    struct sembuf bufor_sem;
    bufor_sem.sem_num=n;
    bufor_sem.sem_op=1;
    bufor_sem.sem_flg=SEM_UNDO;
    zmien_sem=semop(semafor,&bufor_sem,1);
    if (zmien_sem==-1)
        {
                printf("Nie moglem otworzyc semafora.\n");
                exit(EXIT_FAILURE);
        }
    else
        {
                printf("Semafor %d P3 zostal otwarty.\n",n);
        }
}
