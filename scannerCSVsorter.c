#include "scannerCSVsorter.h"
#include "mergesort.h"

int isPath = 1;
int count=0;

//------------Functions-----------------------//
int commaCounter(char* firstLine, char* catCheck, int bigLineCount){
	int functionCommaCount=0;
	char *token;
	token = strtok(firstLine, ",");
	while(token!=NULL){
		if(strcmp(token,catCheck)==0){
			return functionCommaCount;
		}
		token = strtok(NULL, ",");	
		functionCommaCount++;
	}	
	return -1;
}
char* searchStartPath(char* startPathName, char* dot){
	//char* returnPath = ""
	//char* newStart;
	DIR * dir;
	struct dirent *entry;
	dir = opendir(dot);
	while((entry = readdir(dir)) != NULL){
		//char* returnPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(dot) + 2));
		char* returnPath = (char*)malloc(sizeof(char)*1025);
		//char* newStart = (char*)malloc(sizeof(char)*1025);
		if((entry->d_type == DT_DIR)){
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0||strcmp(entry->d_name, ".git") == 0){
				continue;
			}
			if((strcmp(entry->d_name, startPathName)) != 0){			
				strcpy(returnPath, dot);
				strcat(returnPath,"/");
				strcat(returnPath,entry->d_name);
				//strcpy(returnPath,searchStartPath(startPathName,returnPath));
				returnPath = searchStartPath(startPathName, returnPath);
			}
			else{
				strcpy(returnPath, dot);
				strcat(returnPath,"/");
				strcat(returnPath,entry->d_name);
				//newStart = returnPath;
				isPath = 0;
				return returnPath;
			}		
		}
		else{
			continue;			
		}
		if(isPath == 0){
			return returnPath;			
		}	
	}
	return dot;
}
void sorter(char* category, char* oldPath, char* globalPath, char* fileName,char* targetDir,int mode){
	//ta = fopen("jeremyandryan.txt","w");
	//oldpath is the path to the directory that will contain the csv
	//for example, if globalpath is ./zed/movie1.csv
	//oldpath will be ./zed
	if(globalPath[0] == '.' && globalPath[2]=='.'){
		return;
	}
	//printf("THE PATH IS: %s\n",globalPath);
	FILE* fp = fopen(globalPath, "r");
	//fclose(fp);
	char* categoryToSort=category;	
	char *buffer;
	size_t bufsize = 0;
	int len;
	int mainCommaCount;
	buffer = NULL;
	line* head = NULL;
	line* ptr = head;
	//fscanf(fp, "%s", buffer);
	//printf("35\n");
	if(fp==NULL)
		return;
	getline(&buffer, &bufsize, fp);
	//printf("37\n");
	len = strlen(buffer);
	int len2= strlen(fileName);
	char* heading = (char*)malloc(len*sizeof(char));
	strncpy(heading,buffer,len);
	mainCommaCount = commaCounter(buffer, categoryToSort, len);
	if(mainCommaCount == -1){
		return;
	}
	while(getline(&buffer, &bufsize, fp)!=-1){
		//getline(&buffer,&bufsize,stdin);
		len = strlen(buffer);
		char* tempBuff = (char*)malloc((len)*sizeof(char));
		strncpy(tempBuff, buffer, len);
		//printf("53\n");
		int i = 0;
		int currentComma = 0;
		while(currentComma!=mainCommaCount){
			if(tempBuff[i] == '"'){
				while(tempBuff[i]!='"'){
					i++;				
				}			
			}
			if(tempBuff[i] == ','){
				currentComma++;
			}
			i++;		
		}
		int j = i;
		while(tempBuff[j] != ','){
			j++;		
		}
		char *tempCompare = (char*)malloc((j-i)+2);//putting a big number 100 to replace (j-i) for this line
		if(tempBuff[i] == '"'){
			strncpy(tempCompare, tempBuff+(i+1), ((j-i)-1));		
		}
		else{
		strncpy(tempCompare, tempBuff+i, (j-i));
		}
		int length = strlen(tempCompare);
		j = length-1;
		i=0;
		if(strcmp(tempCompare,"")==0){
			j=0;
		}
		while(tempCompare[i]==' '||tempCompare[i]=='\t'){
			i++;
		}
		while(tempCompare[j]==' ' || tempCompare[j]=='\t'){
			j--;
		}
		j++;
		char *trimCompare = (char*)malloc(length+1);
		strncpy(trimCompare, tempCompare+i, (j-i));
		free(tempCompare);		
		line* newLine = (line*)malloc(sizeof(line));
		newLine->wholeLine =tempBuff;
		newLine->compareCat = trimCompare;
		if(head==NULL){
			head=newLine;
			head->next = NULL;
		}
		else{
			newLine->next = head;
			head = newLine;	
		}	
		//lineCount++;
	}//end while
	line* prev = NULL;
	line* current = head;
	line* next;
	while(current!=NULL){
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;	
	}
	head = prev;
//~~~~~~~~~~~~~~~~~~~~~~~list reversal ends here	
	//printf("LINE 111: %s\n",fileName);
	mergeSort(&head);
	ptr=head;
	//create a new csv file here
	//write to it.
	//do case check here, name of write-to csv may be given for 7 argument case
	//check for which 5 argument case it may be
	// for 3 argument place, drop the csv in place.
	if(mode==3){
		char* tempy= (char*)malloc(len*sizeof(char));
		strncpy(tempy,fileName,(len2-4));	
		char* final = (char*)malloc((len2+strlen(categoryToSort)+8+4)*sizeof(char));
		strcat(final,tempy);
		strcat(final,"-sorted-");
		strcat(final,categoryToSort);
		strcat(final,".csv");
		char* realFinal = (char*)malloc((strlen(oldPath)+strlen(final)+2)*sizeof(char));
		strncpy(realFinal,oldPath,strlen(oldPath));
		strcat(realFinal,"/");
	
		//printf("THE value of final IS: %s\n",final);
		strcat(realFinal,final);
		//printf("THE REALFINAL STRING IS: %s\n",realFinal);
		//printf("THE IS: %s\n",realFinal);
		FILE* new = fopen(realFinal,"w");
		fprintf(new,"%s",heading);
		while(ptr!=NULL){
			fprintf(new,ptr->wholeLine);
			//printf("compareCat: %s\n", ptr->compareCat);
			ptr=ptr->next;
		}
		fclose(fp);
	}//end mode 3
	else if (mode==7){	
		char* tempy= (char*)malloc(len*sizeof(char));
		strncpy(tempy,fileName,(len2-4));	
		char* final = (char*)malloc((len2+strlen(categoryToSort)+8+4)*sizeof(char));
		strcat(final,tempy);
		strcat(final,"-sorted-");
		strcat(final,categoryToSort);
		strcat(final,".csv");

		char* realFinal = (char*)malloc((strlen(targetDir)+strlen(final)+2)*sizeof(char));
		strncpy(realFinal,targetDir,strlen(targetDir));
		strcat(realFinal,"/");
		strcat(realFinal,final);
		//printf("THE VALUE OF REALFINAL: %s\n",realFinal);

		FILE* new = fopen(realFinal,"w");
		fprintf(new,"%s",heading);
		while(ptr!=NULL){
			fprintf(new,ptr->wholeLine);
			//printf("compareCat: %s\n", ptr->compareCat);
			ptr=ptr->next;
		}
		fclose(fp);
	}//end mode 7
	else if(mode==6){
		char* tempy= (char*)malloc(len*sizeof(char));
		strncpy(tempy,fileName,(len2-4));	
		char* final = (char*)malloc((len2+strlen(categoryToSort)+8+4)*sizeof(char));
		strcat(final,tempy);
		strcat(final,"-sorted-");
		strcat(final,categoryToSort);
		strcat(final,".csv");

		char* realFinal = (char*)malloc((strlen(targetDir)+strlen(final)+2)*sizeof(char));
		strncpy(realFinal,targetDir,strlen(targetDir));
		strcat(realFinal,"/");
		strcat(realFinal,final);
		//printf("THE VALUE OF REALFINAL: %s\n",realFinal);

		FILE* new = fopen(realFinal,"w");
		fprintf(new,"%s",heading);
		while(ptr!=NULL){
			fprintf(new,ptr->wholeLine);
			//printf("compareCat: %s\n", ptr->compareCat);
			ptr=ptr->next;
		}
		fclose(fp);
	}//end mode 6	
	else if(mode==5){
	char* tempy= (char*)malloc(len*sizeof(char));
		strncpy(tempy,fileName,(len2-4));	
		char* final = (char*)malloc((len2+strlen(categoryToSort)+8+4)*sizeof(char));
		strcat(final,tempy);
		strcat(final,"-sorted-");
		strcat(final,categoryToSort);
		strcat(final,".csv");
		char* realFinal = (char*)malloc((strlen(oldPath)+strlen(final)+2)*sizeof(char));
		strncpy(realFinal,oldPath,strlen(oldPath));
		strcat(realFinal,"/");
	
		//printf("THE value of final IS: %s\n",final);
		strcat(realFinal,final);
		//printf("THE REALFINAL STRING IS: %s\n",realFinal);
		//printf("THE IS: %s\n",realFinal);
		FILE* new = fopen(realFinal,"w");
		fprintf(new,"%s",heading);
		while(ptr!=NULL){
			fprintf(new,ptr->wholeLine);
			//printf("compareCat: %s\n", ptr->compareCat);
			ptr=ptr->next;
		}
		fclose(fp);
	}
	return;
}//end sorter

void listdir( char *name,char*subCategory,char* targetDir, int mode, int *fd){
	if(mode==3){
		DIR *dir;
		struct dirent *entry;
		int len;
		int childFlag;
		if (!(dir = opendir(name)))
       			return;
		while ((entry = readdir(dir)) != NULL) {
			len = strlen(entry->d_name);
			if (entry->d_type == DT_DIR){
				//char path[1024];
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0||strcmp(entry->d_name, ".git") == 0){
               				continue;
				}//end . .. git check
				//printf("166 FORKING\n");
				childFlag = fork();
				if(childFlag==0){//if child
					//if its a child, we should traverse the current level of directory
					//entering other directories
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN DIRECTORY BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					listdir(newPath,subCategory,"",3,fd);
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				
				}//end childflag 0 check
				else{
				wait(NULL);
				}//end else
        		}//end of directory condition
			else if((entry->d_name[len-1]=='v') && (entry->d_name[len-2]=='s') && (entry->d_name[len-3]=='c') && (entry->d_name[len-4]=='.')){								
				childFlag = fork();
				if(childFlag==0){//if its a child
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					char* entryName = entry->d_name;
					char * sorted = "-sorted-";
					char * s;
					char * start;
					start = entryName;
    
    					s = strstr(start, sorted);
					//printf("The substring is: %s\n", s);
        				if(s != NULL){
						//count++;
						//printf("%d,",getpid());
						exit(EXIT_SUCCESS);
					}
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN CSV BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					sorter(subCategory, name, newPath, entry->d_name,"",3);
					exit(EXIT_SUCCESS);
				}//end childflag 0
				else{
					wait(NULL);	
				}// end else
			
			}//end else ifcsv condition	
			else{
				childFlag = fork();
				if(childFlag == 0){
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				}
				else{
					wait(NULL);
				}
			}
		}//endwhile
    	closedir(dir);
	}//end mode 3
	else if(mode==7){
		DIR *dir;
		struct dirent *entry;
		int len;
		int childFlag;
		if (!(dir = opendir(name)))
       			return;
		while ((entry = readdir(dir)) != NULL) {
			len = strlen(entry->d_name);
			if (entry->d_type == DT_DIR){
				//char path[1024];
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0||strcmp(entry->d_name, ".git") == 0){
               				continue;
				}//end . .. git check
				//printf("166 FORKING\n");
				childFlag = fork();
				if(childFlag==0){//if child
					//if its a child, we should traverse the current level of directory
					//entering other directories

					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));

					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN DIRECTORY BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					listdir(newPath,subCategory,targetDir,7,fd);
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				
				}//end childflag 0 check
				else{
				wait(NULL);
				}//end else
        		}//end of directory condition
			else if((entry->d_name[len-1]=='v') && (entry->d_name[len-2]=='s') && (entry->d_name[len-3]=='c') && (entry->d_name[len-4]=='.')){								
				childFlag = fork();
				if(childFlag==0){//if its a child
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					char* entryName = entry->d_name;
					char * sorted = "-sorted-";
					char * s;
					char * start;
					start = entryName;
    
    					s = strstr(start, sorted);
					//printf("The substring is: %s\n", s);
        				if(s != NULL){
						//count++;
						//printf("%d",getpid());
						exit(EXIT_SUCCESS);
					}
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN CSV BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					sorter(subCategory, name, newPath, entry->d_name,targetDir,7);
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				}//end childflag 0
				else{
					wait(NULL);	
				}// end else
			
			}//end else ifcsv condition	
			else{
				childFlag = fork();
				//printf("ENTRY SHOULD BE TXTFILE: %s\n", entry->d_name);
				//printf("SHOULD ONLY FORK ONCE\n");
				if(childFlag == 0){
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				}
				else{
					wait(NULL);
				}
			}
		}//endwhile
    	closedir(dir);	
	}
	else if(mode==6){
		DIR *dir;
		struct dirent *entry;
		int len;
		int childFlag;
		if (!(dir = opendir(name)))
       			return;
		while ((entry = readdir(dir)) != NULL) {
			len = strlen(entry->d_name);
			if (entry->d_type == DT_DIR){
				//char path[1024];
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0||strcmp(entry->d_name, ".git") == 0){
               				continue;
				}//end . .. git check
				//printf("166 FORKING\n");
				childFlag = fork();
				if(childFlag==0){//if child
					//if its a child, we should traverse the current level of directory
					//entering other directories
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN DIRECTORY BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
				//	printf("THE VALUE OF NEWPATH IS :%s\n",newPath);
					listdir(newPath,subCategory,targetDir,6,fd);
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				
				}//end childflag 0 check
				else{
				wait(NULL);
				}//end else
        		}//end of directory condition
			else if((entry->d_name[len-1]=='v') && (entry->d_name[len-2]=='s') && (entry->d_name[len-3]=='c') && (entry->d_name[len-4]=='.')){								
				childFlag = fork();
				if(childFlag==0){//if its a child
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					char* entryName = entry->d_name;
					char * sorted = "-sorted-";
					char * s;
					char * start;
					start = entryName;
    
    					s = strstr(start, sorted);
					//printf("The substring is: %s\n", s);
        				if(s != NULL){
						//count++;
						//printf("%d,",getpid());
						exit(EXIT_SUCCESS);
					}
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN CSV BLOCK IS: %s\n",newPath);
					strcat(newPath,entry->d_name);
					sorter(subCategory, name, newPath, entry->d_name,targetDir,6);
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				}//end childflag 0
				else{
					wait(NULL);	
				}// end else
			
			}//end else ifcsv condition	
			else{
				childFlag = fork();
				if(childFlag == 0){
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				}
				else{
					wait(NULL);
				}
			}
		}//endwhile
    	closedir(dir);
	}//end mode 6
	else if(mode==5){
		DIR *dir;
		struct dirent *entry;
		int len;
		int childFlag;
		if (!(dir = opendir(name)))
       			return;
		while ((entry = readdir(dir)) != NULL) {
			len = strlen(entry->d_name);
			if (entry->d_type == DT_DIR){
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0||strcmp(entry->d_name, ".git") == 0){
               				continue;
				}//end . .. git check
				//printf("166 FORKING\n");
				childFlag = fork();
				if(childFlag==0){//if child
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					//if its a child, we should traverse the current level of directory
					//entering other directories
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN DIRECTORY BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					listdir(newPath,subCategory,"",5,fd);
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				
				}//end childflag 0 check
				else{
				wait(NULL);
				}//end else
        		}//end of directory condition
			else if((entry->d_name[len-1]=='v') && (entry->d_name[len-2]=='s') && (entry->d_name[len-3]=='c') && (entry->d_name[len-4]=='.')){								
				childFlag = fork();
				if(childFlag==0){//if its a child
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					char* entryName = entry->d_name;
					char * sorted = "-sorted-";
					char * s;
					char * start;
					start = entryName;
    
    					s = strstr(start, sorted);
					//printf("The substring is: %s\n", s);
        				if(s != NULL){
						//count++;
						//printf("%d,",getpid());
						exit(EXIT_SUCCESS);
					}
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN CSV BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					sorter(subCategory, name, newPath, entry->d_name,"",5);
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				}//end childflag 0
				else{
					wait(NULL);	
				}// end else
			
			}//end else ifcsv condition	
			else{
				childFlag = fork();
				if(childFlag == 0){
					char buffer[1024];
					sprintf(buffer,"%d,",getpid());
					write(fd[1],buffer,strlen(buffer));
					//count++;
					//printf("%d,",getpid());
					exit(EXIT_SUCCESS);
				}
				else{
					wait(NULL);
				}
			}
		}//endwhile
    	closedir(dir);
	}//end mode 5
	//return;
}//end listdir

//------------Main-----------------------------//
int main(int argc, char** argv){

	//FILE * meta = fopen("jeremyandryan.txt","w");
	//fprintf(meta,"%d\n",getpid());
	
	//fclose(meta);
	//int searchForStart;
	//int createDestination;
	char * startDir;
	char *outputDir;
	//char* startPath;
	char* univCategory;
	int fd[2];
	int rc = pipe(fd);

	if(rc < 0) {
		fprintf(stderr, "Pipe not set up\n");
		exit(EXIT_FAILURE);
	}
	if(argc < 3){
		fprintf(stderr, "Error: Not Enough Arguments\n");
		return -1;	
	}
	else{
		// CASE WHERE WE ARE ONLY GIVEN CATEGORY. WE WILL SORT EACH CSV AND DROP THE SORTED-CSV IN PLACE
		if(argc == 3){
			if((strcmp(argv[1], "-c")) != 0){
				fprintf(stderr, "argument -c error\n");
				return -1;			
			}
			else{
				univCategory = argv[2];
			//flip boolean switch for searchForStart and createDestination to 1 = NO
			//searchForStart = 1;
			//createDestination = 1;
			listdir(".", univCategory,"",3, fd);
			}
		}
		//only given category and start directory or target directory
		else if(argc == 5){
			if((strcmp(argv[1], "-c")) != 0){
				fprintf(stderr, "argument -c or error\n");
				return -1;
			}
			else if(strcmp(argv[2],"-d")==0 || strcmp(argv[2],"-o")==0){
				fprintf(stderr, "input start Directory or output Directory\n");
				return -1;
			}
			else{
				if((strcmp(argv[3], "-d")) == 0){
					univCategory = argv[2];
					startDir = argv[4];  
					char* startPath = (char*)malloc(sizeof(char)*1025);
					strcpy(startPath, searchStartPath(startDir, "."));
					//printf("START DIR: %s\n", startPath);
					isPath = 1;
					listdir(startPath, univCategory,"",5, fd);
					
				}
				else if((strcmp(argv[3], "-o")) == 0){
					univCategory = argv[2];
					outputDir = argv[4];
					char* revisedOutput = (char*)malloc(sizeof(char)*1025);
					strcpy(revisedOutput, searchStartPath(outputDir, "."));
					isPath = 1;
					listdir(".", univCategory,revisedOutput,6, fd); 			
				}
				else{
					fprintf(stderr, "2nd argument not correct\n");
					return -1;
				}	
			}
		}
		// given category, start directory, and target directory
		else if(argc == 7){
			if(((strcmp(argv[1], "-c")) != 0) || ((strcmp(argv[3], "-d")) != 0) || ((strcmp(argv[5], "-o")) != 0)){
				fprintf(stderr, "argument -c or -d or -o error\n");
				return -1;
			}
			else if(strcmp(argv[2], "-d") == 0){
				fprintf(stderr, "input start directory missing\n");
				return -1;
			}
			else if(strcmp(argv[4], "-o") == 0){
				fprintf(stderr, "input output directory missing\n");
				return -1;
			}
			else{
				//searchForStart = 0;
				//createDestination = 0;
				univCategory = argv[2];
				startDir = argv[4];
				outputDir = argv[6];
				//printf("hello\n");

				char* startPath = (char*)malloc(sizeof(char)*1025);
				strcpy(startPath, searchStartPath(startDir, "."));
				//printf("START DIR: %s\n", startPath);
				isPath = 1;
				char* revisedOutput = (char*)malloc(sizeof(char)*1025);
				//printf("$$$$$ targetDir: %s $$$$$\n", outputDir);
				strcpy(revisedOutput, searchStartPath(outputDir, "."));
				isPath = 1;
				//printf("$$$$$ REVISED OUTPUT: %s $$$$$\n", revisedOutput);

				listdir(startPath, univCategory,revisedOutput,7, fd);
			}			
		}
		else{
			fprintf(stderr, "Invalid number of arguments\n");
			return -1;		
		}
	}
	close(fd[1]);
	char tbuff;
	//char com[] = ",";
	int k=0;
	int initial = getpid();
	printf("Initial PID: %d\n",initial);
	write(1,"PIDS of all child processes: ",29); 
	while(read(fd[0],&tbuff,1)){
		//printf("BUFFER AT 740: %c\n",tbuff);
		write(1,&tbuff,1);
		//printf("742\n");
		if(tbuff==','){
		//	printf("744\n");
			k++;	
		}
	}
	//printf("BELOW\n");
	printf("\n");
	printf("Total Processes: %d\n",k);
/*
char** catArray = (char**)malloc(sizeof(char*)*28);
catArray[0] = "color";
catArray[1] = "director_name";
catArray[2] = "num_critic_for_reviews";
catArray[3] = "duration";
catArray[4] = "director_facebook_likes";
catArray[5] = "actor_3_facebook_likes";
catArray[6] =  "actor_2_name";
catArray[7] = "actor_1_facebook_likes";
catArray[8] = "gross";
catArray[9] = "genres";
catArray[10] = "actor_1_name";
catArray[11] = "movie_title";
catArray[12] = "num_voted_users";
catArray[13] = "cast_total_facebook_likes";
catArray[14] = "actor_3_name";
catArray[15] = "facenumber_in_poster";
catArray[16] = "plot_keywords";
catArray[17] = "movie_imdb_link";
catArray[18] = "num_user_for_reviews";
catArray[19] = "language";
catArray[20] = "country";
catArray[21] = "content_rating";
catArray[22] = "budget";
catArray[23] = "title_year";
catArray[24] = "actor_2_facebook_likes";
catArray[25] = "imdb_score";
catArray[26] = "aspect_ratio";
catArray[27] = "movie_facebook_likes";

int i = 0;
while(i<28){
	printf("category: %s\n", catArray[i]);
	i++;
}*/
	return 0;
}

