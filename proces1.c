#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define INPUT "spr1.txt"
FILE *fw;
int semafor;
key_t key;

static void utworz_nowy_semafor(void);
static void semafor_v(int);
static void semafor_p(int);

int main()
{
	if ((key=ftok(".",'M'))==-1)
	{
		printf(stderr, "blad tworzenia klucza\n");
		exit(1);
	}
	utworz_nowy_semafor();
	semafor_p(2);
	fw=fopen(INPUT, "a");
	printf("Sekcja t11 o PID=%d\n", getpid());
	fprintf(fw,"Sekcja t11 o PID=%d\n",getpid());
	sleep(1);
	printf("Sekcja t12 o PID=%d\n",getpid());
	fprintf(fw,"Sekcja t12 o PID=%d\n",getpid());
	fclose(fw);
	sleep(1);
	semafor_v(3);
	exit(0);
}

static void utworz_nowy_semafor(void)
{
	semafor=semget(key,5,IPC_CREAT|0777);
	if(semafor==-1)
	{
		printf("Nie moglem utworzyc nowego semafora.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Semafor zostal utworzony: %d\n",semafor);
	}
}
static void semafor_p(int n)
{
	int zmien_sem;
	struct sembuf bufor_sem;
	bufor_sem.sem_num=n;
	bufor_sem.sem_op=-1;
	bufor_sem.sem_flg=SEM_UNDO;
	zmien_sem=semop(semafor,&bufor_sem,1);
	if(zmien_sem==-1)
	{
		printf("Nie moglem zamknac semafora.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Semafor %d P1 zostal zamkniety.\n",n);
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
	if(zmien_sem==-1)
	{
		printf("Nie moglem otworzyc semafora.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Semafor %d P1 zostal otwarty.\n",n);
	}
}
