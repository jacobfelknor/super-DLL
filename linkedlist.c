/*
 * linkedlist.c
 *
 *  Created on: May 13, 2019
 *      Author: jfelknor
 */

#include "linkedlist.h"

#define LIST_LENGTH 50

typedef struct{
	uint8_t num1;
	uint8_t num2;
	char message[50];
}testType;


void listInit(dlldesc_t *list, uint8_t NUM_ITEMS, uint32_t size){ //sets up memory space for linked list

	list->pbuffer = (struct Node*) malloc(NUM_ITEMS * (sizeof(struct Node) + size)); //allocate necessary memory
	list->length = NUM_ITEMS;	//set number of items for this list (length)
	list->item_size = size;		//set size of each item in this list
	list->head = NULL;	// initialize head of list to NULL

	for(uint8_t i = 0; i < NUM_ITEMS; i++){ //for each space in allocated memory, initialize next/prev pointers and indicate space
											// is free
		(list->pbuffer + i*(sizeof(struct Node) + size))->next = NULL;
		(list->pbuffer + i*(sizeof(struct Node) + size))->prev = NULL;
		(list->pbuffer + i*(sizeof(struct Node) + size))->free = true;
	}
}



struct Node* findFree(dlldesc_t *list){ //finds a free spot in previously allocated memory and returns a pointer to this location
										//if no free spots are available, return a NULL pointer
	struct Node* ret = NULL;
	for(uint8_t i = 0; i < list->length; i++){
		if((list->pbuffer + i*(sizeof(struct Node) + list->item_size))->free){
			ret = (list->pbuffer + i*(sizeof(struct Node) + list->item_size));
			break;
		}
	}
	return ret;
}

void append(dlldesc_t *list, uint8_t newItem[]){//appends a new item to the end of the linked list
	if((list)->head == NULL){//head case, list is empty
		(list)->head = findFree(list);	//get a pointer to free memory to use as the head
		(list)->head->next = NULL;
		(list)->head->prev = NULL;
		(list)->head->free = false;
		//(list)->head->timeStamp = xTaskGetTickCount();
		memcpy((list)->head->data, newItem, (list)->item_size); //use memcpy to store given data in the Node.
	}else{
		//list is not empty, append list with new data
		struct Node *newData;
		struct Node *temp = list->head;
		newData = findFree(list);		//find a free spot in list buffer to store new data
		memcpy(newData->data, newItem, list->item_size); //copy data to the free spot in buffer
		while(temp->next != NULL){
			temp = temp->next;  //traverse list to find end to append to
		}
		temp->next = newData; //once end of list is found, link up everything and mark memory as used.
		newData->next = NULL;
		newData->prev = temp;
		newData->free = false;
		//newData->timeStamp = xTaskGetTickCount();

	}
}

struct Node* search(dlldesc_t *list, uint8_t sKey[]){ //find given data in the list, return a pointer to this Node
													  // return NULL if not found
	struct Node *ret = NULL;
	struct Node *temp = list->head;

	bool found = false;

	while(!found && temp != NULL){//traverse linked list
		if(memcmp(temp->data, sKey, list->item_size) == 0){// compare memory to see if target has been found
			found = true;
			ret = temp;  //once found, set ret to the pointer of the targeted Node
		}else{
			temp = temp->next;
		}
	}

	return ret;
}

void deleteNode(dlldesc_t *list, uint8_t target[]){ //remove targeted node from LL and mark memory as free
	struct Node *found;
	struct Node *temp;

	found = search(list, target); //find Node in LL

	if(found == list->head){//head case. mark head->next as new head and fix linking, free memory
		temp = list->head;
		list->head = list->head->next;
		list->head->prev = NULL;
		temp->free = true;
		temp = NULL;
	}else{
		if(found->next == NULL){// tail case. set tail->prev to new tail and fix linking, free memory
			found->prev->next = NULL;
			found->free = true;
		}else{					//middle case. fix linking and free memory
			temp = found;
			found->prev->next = found->next;
			found->next->prev = found->prev;
			temp->free = true;
		}
	}


}

void dispTestTypeList(dlldesc_t *list){
	struct Node* walker = list->head;

	while(walker != NULL){
		printf("%s\n", ((testType*)walker->data)->message);
		printf("%d\n", ((testType*)walker->data)->num1);
		printf("%d\n", ((testType*)walker->data)->num2);
		walker = walker->next;
	}
}


int main(){

	dlldesc_t list0;

	testType t0[LIST_LENGTH];

	for(uint8_t i = 0; i < LIST_LENGTH; i++){
		t0[i].num1 = i;
		t0[i].num2 = 2*i + 1;
		strcpy(t0[i].message, "Hello");
	}


	listInit(&list0, LIST_LENGTH, sizeof(testType));

	for(uint8_t i = 0; i < LIST_LENGTH; i++){
		append(&list0, (uint8_t*)&t0[i]);
	}

	dispTestTypeList(&list0);
	//deleteNode(&list0, &t0);



	return 0;

}
