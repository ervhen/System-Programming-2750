#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct node {
	int serve;
	int arrival;
	struct node * next;
};

struct Queue{
	struct node * head, *tail;
};

static struct Queue arr[10];
static int custserviced;
static int servarr[600];
static int waiting[600];
static int waitcount;
static int idle;
static int idflag[10] = { 0 }; 

struct node * newnd(int service, int arrive){
	struct node * temp = (struct node*)malloc(sizeof(struct node));
	temp->serve = service;
	temp->arrival = arrive;
	return temp;
}

struct Queue *create(){
	struct Queue *temp = (struct Queue*)malloc(sizeof(struct Queue));
	temp->head = temp->tail = NULL;
	return temp;
}

void enqueue(int count , int  arrv, int ser){
	int hold;
	struct node *temp = newnd(arrv, ser);
	temp->arrival=arrv;
	temp->serve=ser;
	temp->next=NULL;
	if (arr[count].head == NULL)
	{
		arr[count].head = arr[count].tail = temp;
		hold = arrv - idflag[count];
		idle = hold + idle;
		
	}
	else{
		arr[count].tail->next = temp;
		arr[count].tail = temp;
	}
}

void printar(int count)
{	
	struct node *temp = arr[count].head;
	printf("-----------------\n");
	while (temp->next != NULL)
	{
		printf(" x ");
		temp=temp->next;
	}
	printf("\n-----------------");
	printf("\n\n");
}

void  dequeue(int count)
{
	int holdserv;
	int holdarrive;
	if (arr[count].head == NULL)
	{
		return;
	}
	else {
		struct node * temp;
		struct node * holder;
	if (arr[count].head->next == NULL){ 		//for idle time
		idflag[count] == arr[count].head->arrival;
	}

	holdserv = arr[count].head->serve;
	holdarrive= arr[count].head->arrival;
	servarr[custserviced] = holdserv;
	holder = arr[count].head;
	temp = holder->next;
	free(holder);
	arr[count].head = temp;
	custserviced++; 
	}
}

int helper(int x){
	int hold =0;
	struct node *temp = arr[x].head;
	while (temp->next != NULL)
	{
		hold++;
		temp=temp->next;
	}
	return hold;
}

int getsize(){
	int count = 0;
	int hold = 0;
	int min;
	int position=0;
	int i,k,j;
	int minarr[10];
	for (count = 0; count <10; count++)
	{
		if (arr[count].head == NULL){
			return count;
		}
		else{
			minarr[count]=helper(count);
		}
	}
	min=minarr[0];
	for (j=1; j<10; j++)
	{
		if (min > minarr[j])
		{
			min=minarr[j];
			position=j;
		}
	}

	return position;
}

void timer(int time)
{
	int y=0;
	int test;
	for (y=0;y<10;y++)
	{
		if (arr[y].head == NULL){
			return;
		}
		test = arr[y].head->arrival + arr[y].head->serve;
		if (time > test) 
		{
			dequeue(y);
		}
	}	
}

void formtime(int time)
{
	int hr, min, sec;
	if(time>3600)
	{
		min = time/60;
		sec = time%60;
		hr = min/60;
		min = min % 60;
		printf("This simulation ran for, %d hours %d mins %d seconds\n", hr,min,sec);
	}
	else{
		min = time/60;
		sec = time%60;
		printf("%d mins %d seconds\n",min,sec);
	}
}

void helpwaiting(int count){
	int hold =0;
	int later;
	struct node *temp = arr[count].head;
//	printf("%d", temp->arrival);
	while (temp->next != NULL)
	{
		hold = temp->serve + temp->arrival;
		later = temp->next->arrival;
		waiting[waitcount] = hold - later;
		temp=temp->next;
		waitcount++;
	}
}



void getwaiting(){
	int x,p;
	for(x=0;x<10;x++)
	{
		if(arr[x].head == NULL){
			idle++;
			return;
		}
		p=helper(x);
		if(p>1){
			helpwaiting(x);
		}
	}
}

void release(){
	int x;
	for (x=0; x<10; x++){
			while(arr[x].head != NULL){
			struct node * temp = arr[x].head->next;
			free(arr[x].head);
			arr[x].head = temp;
		}
	}
}


int main(){
	line();
	int ser;
	int arrv;
	int waiter;
	int time;
	int i,k;
	int f=0;	
	int t=0;
	int athing= 0;
	int avgwait;
	int totservtime;
	int totaltime =0;
	int hold = 0; 
	FILE * fptr = fopen("customers", "r");
	if (fptr == NULL){
		exit(0);
	}
	for (k=0; k<10; k++)
	{
		arr[k].head = NULL;
		arr[k].tail = NULL;
	}
	while(fscanf(fptr, "%d\t%d", &arrv, &ser) == 2){
		i=getsize();
		enqueue(i, arrv, ser);
		time=arrv;
		if (f > 10){
			timer(time);
		}
		getwaiting();
		f++;
		athing++;
	}
	for (t = 0; t<f+1;t++){
		hold= hold + servarr[t];
		waiter = waiter + waiting[t];	
	}
	avgwait=waiter/f;
	totservtime = hold/athing;
	printar(0);
	printar(1);
	printar(2);
	printar(3);
	printar(4);
	printar(5);	
	printar(6);
	printar(7);
	printar(8);
	printar(9);
	formtime(time);
	printf("\n%d Customers arrived\n",f);
	printf("\n%d Customers were serviced\n",custserviced + 10);
	printf("\n%d Customers are still in line\n", f - custserviced -10);
	printf("The average service time for each customer was ");
	formtime(totservtime);
	printf("The average waiting time was ");
	formtime(avgwait);
	printf("The idle time was ");
	formtime(idle);
	fclose(fptr);
	release();
}
