#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define INPUT "spr1.txt"
FILE *fw;
FILE *w;
FILE *k;
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
                printf("Semafor zostal utworzony : %d\n",semafor);
        }
	//semafor_v(0);
	fw=fopen(INPUT,"a");
	printf("Sekcja t21 o PID=%d\n",getpid());
        fprintf(fw,"Sekcja t21 o PID=%d\n",getpid());
	fclose(fw);
        sleep(1);
	semafor_v(0);
	semafor_p(1);
	w=fopen(INPUT,"a");
	printf("Sekcja t22 o PID=%d\n",getpid());
        fprintf(w,"Sekcja t22 o PID=%d\n",getpid());
	fclose(w);
        sleep(1);
	semafor_v(2);
	semafor_p(3);
	k=fopen(INPUT,"a");
	printf("Sekcja t23 o PID=%d\n",getpid());
        fprintf(k,"Sekcja t23 o PID=%d\n",getpid());
	fclose(k);
        sleep(1);
	semafor_v(4);
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
                printf("Semafor %d P2 zostal zamkniety.\n",n);
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
                printf("Semafor %d P2 zostal otwarty.\n",n);
        }
}
