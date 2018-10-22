#include "scannerCSVsorter.h"
#include "mergesort.h"

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
int duplicateChecker(char*testFile, char*testDir){
	//problem: not sorting files already in the target dir
	DIR *dir;
	struct dirent *entry;
	dir = opendir(testDir);
	while ((entry = readdir(dir)) != NULL) {
		if(strcmp(entry->d_name,testFile)==0){//if there is a match
			closedir(dir);
			return 1;
		}
	}
	closedir(dir);
	return 0;
}
void sorter(char* category, char* oldPath, char* globalPath, char* fileName,char* targetDir,int mode){
	//oldpath is the path to the directory that will contain the csv
	//for example, if globalpath is ./zed/movie1.csv
	//oldpath will be ./zed
	if(globalPath[0] == '.' && globalPath[2]=='.'){
		return;
	}
	printf("THE PATH IS: %s\n",globalPath);
	FILE* fp = fopen(globalPath, "r");
	//fclose(fp);
	char* categoryToSort=category;	
	char *buffer;
	size_t bufsize = 0;
	int len;
	//int catLen = strlen(categoryToSort);
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

		FILE* new = fopen(realFinal,"w");
		fprintf(new,"%s",heading);
		while(ptr!=NULL){
			fprintf(new,ptr->wholeLine);
			//printf("compareCat: %s\n", ptr->compareCat);
			ptr=ptr->next;
		}
		fclose(fp);
	}//end mode 7	
	
}//end sorter

void listdir( char *name,char*subCategory,char* targetDir, int mode){//name will contain the path that leading to and including the current directory, if we are in zed file: name == ./zed
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
				printf("166 FORKING\n");
				childFlag = fork();
				if(childFlag==0){//if child
					//if its a child, we should traverse the current level of directory
					//entering other directories
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN DIRECTORY BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					listdir(newPath,subCategory,"",3);
					exit(EXIT_SUCCESS);
				
				}//end childflag 0 check
				else{
				wait(NULL);
				}//end else
        		}//end of directory condition
			else if((entry->d_name[len-1]=='v') && (entry->d_name[len-2]=='s') && (entry->d_name[len-3]=='c') && (entry->d_name[len-4]=='.')){					
		
				//else fork
				printf("186 FORKING\n");
				//we fork too many times here, its handled through a condition in sorter, but a better solution would be to check if the csv contains the word sorted-category.csv
				//possibly use conditional in sorter before fork on newpath.			
				childFlag = fork();
				if(childFlag==0){//if its a child
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
				continue;
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
				printf("166 FORKING\n");
				childFlag = fork();
				if(childFlag==0){//if child
					//if its a child, we should traverse the current level of directory
					//entering other directories
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					//printf("THE VALUE OF ENTRY->D_NAME IN DIRECTORY BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					listdir(newPath,subCategory,targetDir,7);
					exit(EXIT_SUCCESS);
				
				}//end childflag 0 check
				else{
				wait(NULL);
				}//end else
        		}//end of directory condition
			else if((entry->d_name[len-1]=='v') && (entry->d_name[len-2]=='s') && (entry->d_name[len-3]=='c') && (entry->d_name[len-4]=='.')){					
		
				//else fork
				printf("186 FORKING\n");
				//we fork too many times here, its handled through a condition in sorter, but a better solution would be to check if the csv contains the word sorted-category.csv
				//possibly use conditional in sorter before fork on newpath.			
				childFlag = fork();
				int x = duplicateChecker(entry->d_name,targetDir);
				if(childFlag==0 && x==0){//if its a child
					char* newPath = (char*)malloc(sizeof(char)*(strlen(entry->d_name) + strlen(name) + 2));
					strcpy(newPath,name);
					strcat(newPath,"/");
					printf("THE VALUE OF ENTRY->D_NAME IN CSV BLOCK IS: %s\n",entry->d_name);
					strcat(newPath,entry->d_name);
					sorter(subCategory, name, newPath, entry->d_name,targetDir,7);
					exit(EXIT_SUCCESS);
				}//end childflag 0
				else{
					wait(NULL);	
				}// end else
			
			}//end else ifcsv condition	
			else{
				continue;
			}
		}//endwhile
    	closedir(dir);	
	}
}//end listdir





/*
int main(int argc,char**argv) {
    listdir(".", 0,argv[2]);
    return 0;
}
*/
//------------Main-----------------------------//
int main(int argc, char** argv){
	//int searchForStart;
	//int createDestination;
	char * startDir;
	char *outputDir;
	char* univCategory;
	if(argc < 3){
		printf("Error: Not Enough Arguments\n");
		return -1;	
	}
	else{
		// CASE WHERE WE ARE ONLY GIVEN CATEGORY. WE WILL SORT EACH CSV AND DROP THE SORTED-CSV IN PLACE
		if(argc == 3){
			if((strcmp(argv[1], "-c")) != 0){
				printf("argument -c error\n");
				return -1;			
			}
			else{
				univCategory = argv[2];
			//flip boolean switch for searchForStart and createDestination to 1 = NO
			//searchForStart = 1;
			//createDestination = 1;
			//printf("THE UNIVERSAL CATEGORY IS :%s\n",univCategory);
			//printf("THE ARGV[2] IS :%s\n",argv[2]);
			listdir(".", univCategory,"",3);
			}
		}
		//only given category and start directory or target directory
		else if(argc == 5){
			if((strcmp(argv[1], "-c")) != 0){
				printf("argument -c or error\n");
				return -1;
			}
			else{
				if((strcmp(argv[3], "-d")) == 0){
					univCategory = argv[2];
					//searchForStart = 0;
					//createDestination = 1;
					startDir = argv[4];   //make start directory, dont know if correct
					listdir(startDir, univCategory,"",5);
					
				}
				else if((strcmp(argv[3], "-o")) == 0){
					univCategory = argv[2];
					//searchForStart = 1;
					//createDestination = 0;
					outputDir = argv[4];
					listdir(".", univCategory,outputDir,6); 			
				}
				else{
					printf("2nd argument not correct\n");
					return -1;
				}	
			}
		}
		// given category, start directory, and target directory
		else if(argc == 7){
			if(((strcmp(argv[1], "-c")) != 0) || ((strcmp(argv[3], "-d")) != 0) || ((strcmp(argv[5], "-o")) != 0)){
				printf("argument -c or -d or -o error\n");
				return -1;
			}
			else{
				//searchForStart = 0;
				//createDestination = 0;
				univCategory = argv[2];
				startDir = argv[4];
				outputDir = argv[6];
				listdir(startDir, univCategory,outputDir,7);
			}			
		}
	}
	return 0;
}

