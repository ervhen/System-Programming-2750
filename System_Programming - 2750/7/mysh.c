#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //for string manipulation (i use this a lot)
#include <unistd.h>  //for fork
#include <sys/wait.h> //for wait
#include <signal.h>

int ball;

char *cmdline();


int makeargv(char *s, const char *delimiters, char ***argvp){ //array never has to be returned, any changes made will be done to the real one
	int i;
	int numtokens;
	char *t;
	char *temp;

	*argvp = NULL;
   	t = malloc(strlen(s) + 1); //create space for t the size of the arg but add one for the \0
   	strcpy(t, s);//copy the string in s to t, so i can use strtok 
   	numtokens = 0; //set the counter to 0;
	temp = strtok(t,delimiters);
	while (temp != NULL){ //count tokens
		temp=strtok(NULL,delimiters);
		numtokens++;
	}
                       // create argument array for ptrs to the tokens
	*argvp = malloc((numtokens + 1)*sizeof(char *));
		//make sure there was actually something there
	if (numtokens == 0){ 
		free(t);
		return -1;
	}
	else {
		strcpy(t, s);
		**argvp = strtok(t, delimiters);
		
		for (i = 1; i < numtokens; i++){
			*((*argvp) + i) = strtok(NULL, delimiters);
		}
	} 
	*((*argvp) + numtokens) = NULL;             //now put in the last null at end
	return numtokens;
}     

void prompt(char * PS1, char *workdir){	//I just made it check constantly for the \W in this in the ps1 and is its set and finds it it shows the working directory otherwise its just the $
	char **dir;
	char *s;
	int tokens;
	dir=(char **)calloc(sizeof(workdir)+1,sizeof(char **));
	if(getenv("PS1") != NULL){
		if (strstr(getenv("PS1"), "\\W")){
			tokens=makeargv(workdir, "/",&dir);
			workdir = dir[tokens-1];
		}
	}
	else{
		workdir="";
	}
	printf("%s %s $ ",PS1, workdir);		
}

char *cmdline(){
	char *in = NULL;  // From man, "If *lineptr is set to NULL and *n is set 0 before the call, then getline() will allocate a buffer for storing the line." Make the computer do the work
	ssize_t buffer = 0; //ssize_t is basically a dynamic signed int, as in it goes as big as it needs to (where int has SIZE_MAX), so the size is generated when the user enters in their string
	getline(&in, &buffer, stdin);			// getline(char **lineptr, size_t *n, FILE *stream)
	strtok(in, "\n");		//getline includes the newline character, so it must be removed
	return in;
}

void handler(int signum){ //my signal handler
	if(signum == SIGINT){
		ball = 1;	
	}	
}


//the execution function this one uses execv().
int doer(char ** workpath, int ptok, char **args, char *in){
	pid_t pid;
	int i;
	char addit[] = "/";
	char *temp;
//creates the path to command (the first one)
	temp = (char *)malloc(sizeof(workpath[0]) +sizeof(args[0] + 25));
	strcpy(temp, workpath[0]);
	strcat(temp, addit);
	strcat(temp,args[0]);

	struct sigaction sa; //setup the signal handling function
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

	pid = fork();
	if(pid == -1){
		printf("\nFailure to Fork");
		return 0;
	}
	else if (pid == 0) { //the child process 
		if(execv(temp,args) < 0){ //if this doesnt work
			for(i=1; i<ptok; i++){  //goes into this and goes through the path untill something works
				temp = (char *)malloc(sizeof(workpath[i]) +sizeof(args[0] + 6));
				strcpy(temp, workpath[i]);
				strcat(temp, addit);
				strcat(temp,args[0]);
				execv(temp, args);
			}
			printf("\nCould not complete command\n");
		}
		exit(0);
	}
	else{ //for the parent
		wait(NULL); //wait for the child to be done
		if(ball == 1){ //if there is a sigint signal ball is set to 1 and triggers this
			kill(pid,SIGINT); //send a signal from the parent to the child to die 
			ball =0;
			return 0;
		}
		return 0;
	}
}
//the easy version, this one also works. execvp() is really nice since it does all of the work for you.
/*
int doer(char **args, char *in){
	pid_t pid;
	
	pid = fork();
	if(pid == -1){
		printf("\nFailure to Fork");
		return;
	}
	else if (pid == 0) {
		if(execvp(args[0],args) < 0){
			printf("\nCould not complete command");
		}
		exit(0);
	}
	else{
		wait(NULL);
		return;
	}
}
*/
int main(){
	char workdir[1024];		//if its super long, i shouldnt use all of it
	char **workpath;  //hold parsed path var
	char *temp;	//holds the PS1
	char *hold;	//used as temp for finding PS1 values
	char *sony;	//same as above
	char *sonyholder;	//same as above
	char *work;	//for the path var
	char *PS1;	//working PS1 value
	int ptok;	//the number of tokens for path
	int tokens;	//number of tokens for command
	char **myargv;	//parsed command
	char *cmdin;	//holds command
	int i;		//used to count
	char getnew[] = "\n";
	char backu[] = "\\u"; //need to escape the backslash
	const char pathdelim[] = ":";
	const char readdelim[] = "=";
	const char delim[] = " ";
	getcwd(workdir, sizeof(workdir)); //get starting directory
	if(!getenv("PS1")){	//set PS1 if its not set to nothing
		PS1="";
	}
	else{		//get PS1 variables if set the \W is in the prompt function
		temp=getenv("PS1");
		PS1 = (char *)malloc(sizeof(temp) +10);
		strcpy(PS1, temp); //need to handle \u, \h
		hold = strstr(PS1, backu);
		if (hold != NULL){
			sony=getenv("USER");
			i++;
		}
		hold = (char *)calloc(sizeof(sony) + 30, sizeof(char *));
		hold = strstr(PS1, "\\h"); //need to escape the backslash
		if (hold != NULL){
			sonyholder = (char *)malloc(sizeof(sony)+1);
			strcpy(sonyholder, sony);
			sony=getenv("HOSTNAME");
			if (i=1){
				strcat(sonyholder, "@"); //cheating here
				strcat(sonyholder, sony);
				free(PS1);
				PS1= (char *)malloc(sizeof(sony)+sizeof(sonyholder)+3);
				strcpy(PS1,sonyholder);
			}
			else{
				free(PS1);
				PS1= (char *)malloc(sizeof(sonyholder)+1);
				strcpy(PS1, sonyholder);
			}
		}
		else{
			free(PS1);
			PS1= (char *)malloc(sizeof(sony)+1);
			strcpy(PS1, sony);
		}
		if ((!sony) && (!sonyholder) && (temp != NULL)){ //if sony and sonyholder are null then that means it is a string literal (or contains symbols i dont handle)
			PS1=(char *)calloc(sizeof(temp), sizeof(char *));
			strcpy(PS1, temp);
		}
		
	}
	if (!getenv("MYPATH")){ //check if mypath is null if it is use path
		char *path = getenv("PATH");
		work = (char *)malloc(sizeof(path) +1);
		strcpy(work, path);
		ptok =makeargv(work,pathdelim,&workpath);
	}
	else{ //mypath isnt null so use that instead
		char *path = getenv("MYPATH");
		work = (char *)malloc(sizeof(path) +1);
		strcpy(work, path);
		ptok =makeargv(work,pathdelim,&workpath);
	}	
	while(1){ //the actual start
		prompt(PS1, workdir); //show the prompt
		cmdin = cmdline();	//get the command
		tokens = makeargv(cmdin, delim, &myargv); //parse the command
		if(feof(stdin)){ //for EOF (ctrl+d)
			exit(0);
		} //check if the command is cd if so then change direct, also update workdir
		else if (strcmp(myargv[0],"cd")==0){ 
			if (myargv[1] == NULL){
				chdir(getenv("HOME"));
				getcwd(workdir, sizeof(workdir));
			}
			else{	//check to make sure permissions are ok
				if (access(myargv[1], R_OK)== 0){
					if (chdir(myargv[1]) == 0){
						getcwd(workdir, sizeof(workdir));
					}
					else{
						printf("%s: No such directory\n", myargv[1]);
					}
				}
				else{
					printf("You do not have permission to enter this directory\n");
				}
			}
		}//if user types exit, it exits
		else if (strcmp(myargv[0],"exit")==0){
				return 0;
		}
		else{ //if its none of the above then check if it is a bin command
			doer(workpath, ptok,myargv,cmdin);
		}
	}
	return 0;
}
