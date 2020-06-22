#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/wait.h>
#define KEY 1010
#define BUFFER_SIZE 10
int unlock1=0;
struct Bank {
int accountnum;
char name[50];
int balance;
}s[10];
int from,to,amount;
int i,m=1,choice,j=2,w,amount;
struct Bank *bank;
void* MakeTransactions()
{
printf("User is trying to transfer money\n");
dolock(m,0);
printf("enter account number from where you want to transfer\n");
scanf("%d",&from);
printf("enter account number where you want to transfer\n");
scanf("%d",&to);
printf("enter amount you want to transfer\n");
scanf("%d",&amount);
s[from-1].balance=s[from-1].balance-amount;
s[to-1].balance=s[to-1].balance+amount;
printf("\nBalances of account number %d ==> %d n",from,s[from-1].balance);
printf("\nBalances of account number %d ==> %d \n",to,s[to-1].balance);
unlock(m,0);
printf("USer has finished...\n");
return NULL;
}
void *deposit()
{
printf("User is trying to deposit\n");
dolock(m,0);
printf("enter your account number\n");
scanf("%d",&w);
if(w<=j){
printf("Enter amount to deposit\n");
scanf("%d",&amount);
s[w-1].balance=s[w-1].balance+amount;
printf("Balance after deposit: %d\n",s[w-1].balance);
}
else{
printf("Wrong account number");
unlock(m,0);
printf("USer has finished...\n");
}
}
void *withdraw()
{
printf("User is trying to withdraw\n");
dolock(m,0);
printf("enter your account number\n");
scanf("%d",&w);
if(w<=j){
printf("Enter amount to withdraw\n");
scanf("%d",&amount);
if(amount<=s[w-1].balance){
s[w-1].balance=s[w-1].balance-amount;
printf("Balance after withdrawl: %d\n",s[w-1].balance);
}
else{
s[w-1].balance=s[w-1].balance;
printf("Permission denied due to insufficient balance\n");
}
}
else
{printf("Wrong account number");}
unlock(m,0);
printf("User has finished...\n");
}
void unlock(int m,int index){
if(unlock1==0){
if(m==1)
m=0;
unlock1++;
}
}
void dolock(int m,int index)
{
if(m==1)
m=1;
int unlock1=0;
}
int main(int argc, char **argv) {
char ans[10];
char n[50];
char aname[50];
void* voidptr = NULL;
pthread_t cid[10],cid1;
pid_t pid;
int num;
srand(getpid());
bank = mmap(NULL, sizeof(struct Bank), PROT_READ | PROT_WRITE,
MAP_SHARED | MAP_ANONYMOUS, -1, 0);
s[0].balance = 200;
s[1].balance = 100;
s[0].accountnum=1;
s[1].accountnum=2;
s[0].name[50] = "muskan";
s[1].name[50]= "bhavana";
printf("--------------Welcome to ABM bank--------------\n");
do{
printf("\nPlease enter your choice of service\n");
printf("\n1.Create an account\t\t2.Cash withdrawl\n3.Cash deposit\t\t\t4.Check balance\n5.Money transfer\t\t6.exit\n");
scanf("%d",&choice);
switch(choice){
case 1:	
	printf("You have chosen to create an account\n");
	if(j<10){
	j++;
	printf("Enter your name\n");
	scanf("%s",&s[j-1].name);
	printf("Account number generated:%d\n",j);
	printf("Do you want to deposit some amount?(Yes/No)\n");
	scanf("%s",&ans);
	char str2[10]="yes";
	if(strcmp(ans, str2)==0){
	printf("Enter the amount \n");
	scanf("%d",&s[j-1].balance);
	}
	else{
	s[j-1].balance=0;
	printf("Balance= %d\n",s[j-1].balance);
	}
	}
	else
	printf("Cannot create account");
	break;
case 2:
	printf("You have chosen to withdraw\n");
	printf("Enter number of users\n");
	scanf("%d",&num);
	for(i=0;i<num;i++)
	{
	pthread_create(&cid[i],NULL,withdraw,NULL);
	pthread_join(cid[i],NULL);
	}
	break;
	case 3:
	printf("You have chosen to deposit\n");
	printf("Enter number of users\n");
	scanf("%d",&num);
	for(i=0;i<num;i++){
	pthread_create(&cid[i],NULL,deposit,NULL);
	pthread_join(cid[i],NULL);
	}
	break;
case 4:
	printf("Enter the account number\n ");
	scanf("%d",&w);
	if(w<=j){
	printf("Balance : %d\n",s[w-1].balance);
	}
	else
	printf("Wrong account number");
	break;
case 5:
	printf("Enter number of users\n");
	scanf("%d",&num);
	for(i=0;i<num;i++)
	{
	pthread_create(&cid[i],NULL,MakeTransactions,NULL);
	pthread_join(cid[i],NULL);
	}
	break;
case 6:
	printf("THANK YOU FOR USING ABM ONLINE BANKING\n");
	break;
}
}while(choice!=6);
return 0;
}

