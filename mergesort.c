#include "scannerCSVsorter.h"

line* merge(line* a, line* b); 
void split(line* source, line** lead, line** back); 
//--------------mergesort------------//
void mergeSort(line** ptrHead) { 
	line* head = *ptrHead; 
	line* a; 
	line* b; 
	if ((head == NULL) || (head->next == NULL)) { 
    		return; 
	} 
	split(head, &a, &b);  
	mergeSort(&a); 
	mergeSort(&b); 
	*ptrHead = merge(a, b); 
} 
//---------------merge-------------//
line* merge(line* first, line* second){ 
	int i=0;
	int flag=0;
	if (first == NULL){ 
   		return(second); 
	}
	else if (second==NULL){ 
    		return(first); 
	}
	line* newGuy = NULL; 
//~~~~~~~~~~~~~~~~~~~~~~if negative~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//if both negative
	if((first->compareCat[0] == '-') && (second->compareCat[0] == '-')){
		for(i=1;i<strlen(first->compareCat);i++){
			if((isdigit(first->compareCat[i])==0)){
				flag=1;
			}
		}
		for(i=1; i<strlen(second->compareCat); i++){
			if((isdigit(second->compareCat[i])==0)){
				flag=1;
			}	
		}
	}
	//if first negative
	else if((first->compareCat[0] == '-') && (second->compareCat[0] != '-')){
		for(i=1;i<strlen(first->compareCat);i++){
			if((isdigit(first->compareCat[i])==0)){
				flag=1;
			}
		}
		for(i=0; i<strlen(second->compareCat); i++){
			if((isdigit(second->compareCat[i])==0)){
				flag=1;
			}	
		}			
	}
	//if second negative
	else if((first->compareCat[0] != '-') && (second->compareCat[0] == '-')){
		for(i=0;i<strlen(first->compareCat);i++){
			if((isdigit(first->compareCat[i])==0)){
				flag=1;
			}
		}
		for(i=1; i<strlen(second->compareCat); i++){
			if((isdigit(second->compareCat[i])==0)){
				flag=1;
			}	
		}	
	}
	//not negative, then just do regular
	else{


		for(i=0;i<strlen(first->compareCat);i++){
			if((isdigit(first->compareCat[i])==0)){
				flag=1;
			}
		}
		for(i=0; i<strlen(second->compareCat); i++){
			if((isdigit(second->compareCat[i])==0)){
				flag=1;
			}	
		}
	}
	if((strcmp(first->compareCat, "") == 0) && (strcmp(second->compareCat, "") == 0)){
		newGuy = first; 
		newGuy->next = merge(first->next, second);	
	}
	else if((strcmp(first->compareCat, "")!=0) && (strcmp(second->compareCat, "")==0)){
		newGuy = second; 
		newGuy->next = merge(first, second->next); 	
	}
	else if((strcmp(first->compareCat, "")==0) && (strcmp(second->compareCat, "")!=0)){
		newGuy = first; 
		newGuy->next = merge(first->next, second); 	
	}
	else{
	//if the arguments are both digits we can atoi
	if(flag == 0){
		if (atoi(first->compareCat) <= atoi(second->compareCat)) { 
			newGuy = first; 
			newGuy->next = merge(first->next, second); 
		} 
		else{ 
			newGuy = second; 
			newGuy->next = merge(first, second->next); 
		}
	}
 	//end of if theyre both digits
	//at this point we conclude they are not both digits, we strcmp
	else{
		if((strcmp(first->compareCat, second->compareCat) <= 0)){
			newGuy = first;
			newGuy->next = merge(first->next, second);		
		}
		else{
			newGuy = second;
			newGuy->next = merge(first, second->next);		
		}
	}
	}	
	return(newGuy); 
} 
//---------------split-----------------//
void split(line* head, line** front, line** back) { 
	line* fastPtr; 
        line* slowPtr; 
	slowPtr = head; 
	fastPtr = head->next; 
	while (fastPtr != NULL){ 
		fastPtr = fastPtr->next; 
		if (fastPtr != NULL){ 
			slowPtr = slowPtr->next; 
			fastPtr = fastPtr->next; 
		} 
	} 
	*front = head; 
	*back = slowPtr->next; 
	slowPtr->next = NULL; 
} 

//try while(fast!=NULL&&fast.next!=null)
		//slow=slow.next
		//fast=fast.next.next
/*
void split(line* source,line**headRef,line**backRef){
	printf("reached split\n");
	line* fastPtr;
	line* slowPtr;
	slowPtr=source;
	fastPtr=source->next;
	while(fastPtr!=NULL){
		fastPtr=fastPtr->next;
		if(fastPtr!=NULL){
			slowPtr=slowPtr->next;
			fastPtr=fastPtr->next;
		}
	}
	*headRef = source;
	*backRef = slowPtr->next;
	slowPtr->next=NULL;
}//end of getmiddle function


int compareLines(char* firstArg, char* secondArg){
	//fflush(stdout);
	printf("reached comparelines\n");
	int value1;
	int value2;

	if((strcmp(firstArg, "") == 0) && (strcmp(secondArg, "") == 0)){
		return 0;	
	}
	else if((strcmp(firstArg, "")!=0) && (strcmp(secondArg, "")==0)){
		return 1;	
	}
	else if((strcmp(firstArg, "")==0) && (strcmp(secondArg, "")!=0)){
		return 0;	
	}
	printf("LINE 36\n");
	if((isdigit(firstArg)!=0) && (isdigit(secondArg)!=0)){
printf("LINE 38\n");
printf("The value of firstArg is %s\n",firstArg);
printf("The value of secondArg is %s\n",secondArg);
		value1 = atoi(firstArg);
printf("LINE 40\n");
		printf("integers 1, 2: %s %s\n", firstArg, secondArg);
		value2 = atoi(secondArg);
printf("LINE 43\n");
		if(value1 <= value2){
printf("LINE 45\n");
			return 0;
		}
		else{
printf("LINE 49\n");
			return 1;
		}	
	}
	else{
printf("LINE 54\n");
		printf("strings 1, 2: %s %s\n", firstArg, secondArg);
printf("LINE 56\n");
		if(strcmp(firstArg, secondArg)<=0){
printf("LINE 68\n");
			return 0;		
		}
		else{
printf("LINE 62\n");
			return 1;
		}
	}
printf("LINE 66\n");
	return -1;	

}

line* merge(line* a, line* b){
	printf("reached merge\n");
	int compareValue;
	line* result =NULL;
	if(a==NULL){
		return (b);	
	}
	else if(b==NULL){
		return (a);
	}

	compareValue = compareLines(a->compareCat, b->compareCat);
	
	if(compareValue == 0){
		result = a;
		result->next = merge(a->next,b);
	}
	else if(compareValue == 1){
		result = b;
		result->next = merge(a,b->next);	
	}

	return (result);
}//end of merge function


void mergeSort(line** head){
	printf("reached mergesort\n");
	line* tempHead=*head;
	line* a;
	line*b;
	if((tempHead==NULL)||(tempHead->next==NULL)){
		return;
	}
	split(tempHead,&a,&b);
	mergeSort(&a);
	mergeSort(&b);
	*head=merge(a,b);
}//end mergeSort function
*/
