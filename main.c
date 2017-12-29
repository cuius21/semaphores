#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define INPUT "spr1.txt"
FILE *fw;
int semafor;
key_t key;

static void poczatek(void);
static void utworz_nowy_semafor(key_t);
static void ustaw_semafor(void);
static void semafor_v(int);
static void semafor_p(int);
static void usun_semafor(void);


int main(){
	int w;
	fw=fopen(INPUT,"w");
	if ((key = ftok(".", 'M')) == -1)
    	{
        	fprintf(stderr, "blad tworzenia klucza\\n");
        	exit(1);
    	}
    	utworz_nowy_semafor(key);
    	ustaw_semafor();
        int i, id, pid=getpid();
	char p[10];
	char q[10];
        //char cmd[30];
        //sprintf(cmd,"pstree -p %d",pid);
        for(i=1;i<4;i++)
	{
		sprintf(p,"./P%d",i);
		sprintf(q,"P%d",i);
		//printf(p);
		//printf(q);
		//printf("\n");
		switch (id = fork())
                {
                case -1:
                        //fprintf(stdout, "ERROR\n");
                        perror("fork error");
                        exit(1);
                        break;
                case 0:
                        //printf("Proces Potomny: \n");
                        if((execl(p,q, NULL))==-1)
                        {
                                perror("exec error");
                                exit(2);
                        }
                break;
                default:
                break;
                }
	}
	//system(cmd);
	for (i = 0; i < 3; ++i)
    	{
        	w = wait(&id);
        	//printf("%d\n",wait(&id[i]));
        	if(w==-1)
        	{
                	perror("wait error");
                	exit(3);
        	}
        	else
        	{
                	//printf("&id = %d\n",i,&id);
                	printf("PID procesu zakonczonego: %d\n",w);
                	printf("Kod powrotu procesu: %d\n",id);
        	}
    	}
	fclose(fw);
	usun_semafor();
        exit(0);
}

static void utworz_nowy_semafor(key_t key)
{
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
}

static void ustaw_semafor(void)
{
    	int ustaw_sem;
	short tab[5]={0,0,0,0,0};
	ustaw_sem=semctl(semafor,0,SETALL,tab);
	if (ustaw_sem==-1)
	{
      		printf("Nie mozna ustawic semafora.\n");
  		exit(EXIT_FAILURE);
	}
	else
	{
       		printf("Semafor zostal ustawiony.\n");
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
    if (zmien_sem==-1)
      	{
        	printf("Nie moglem zamknac semafora.\n");
        	exit(EXIT_FAILURE);
      	}
    else
      	{
        	printf("Semafor zostal zamkniety.\n");
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
        	printf("Semafor zostal otwarty.\n");
      	}
}

static void usun_semafor(void)
{
    int sem;
    sem=semctl(semafor,5,IPC_RMID);
    if (sem==-1)
      	{
        	printf("Nie mozna usunac semafora.\n");
        	exit(EXIT_FAILURE);
      	}
    else
      	{
        printf("Semafor zostal usuniety : %d\n",sem);
	}
}
