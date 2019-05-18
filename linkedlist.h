/*
 * linkedlist.h
 *
 *  Created on: May 14, 2019
 *      Author: jfelknor
 */

#ifndef INC_LINKEDLIST_H_
#define INC_LINKEDLIST_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LIST_LENGTH 50 //capacity of linkedlist. only allocated
					   // memory for this many custom items 

typedef struct{  //test data type to fill list with
	uint8_t num1;
	uint8_t num2;
	char message[50];
}testType;


struct Node{
	bool free;
	struct Node *next;
	struct Node *prev;
	uint32_t timeStamp;
	uint8_t data[]; //array of bytes(uint8_t), can hold custom size data
};

typedef struct {
	struct Node *pbuffer;	///< Buffer to hold items
	struct Node *head;		///< Pointer to head of LL
	uint32_t length;		///< Max number of items
	uint32_t item_size;		///< Size of an item in bytes
}dlldesc_t;


void listInit(dlldesc_t *list, uint8_t NUM_ITEMS, uint32_t size);

struct Node* findFree(dlldesc_t *list);

void append(dlldesc_t *list, uint8_t newItem[]);

struct Node* search(dlldesc_t *list, uint8_t sKey[]);

void deleteNode(dlldesc_t *list, uint8_t target[]);

void displayList(dlldesc_t *list);

#endif /* INC_LINKEDLIST_H_ */
