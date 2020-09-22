#include <stdio.h>
#include <stdlib.h>
#include <opencv2\highgui\highgui_c.h>
#include <linkedList.h>
#include <view.h>
#include <string.h>
#include <Frame.h>

#define EXIT 0
#define ADD_NODE 1
#define REMOVE_NODE 2
#define SWITCH_NODE 3
#define FRAME_DURATION 4
#define FRAMES_DURATION 5
#define LIST_FRAMES 6
#define PLAY_MOVIE 7
#define MAX_LENGTH 256
#define NOT_FOUND -1
#define FOUND 1
#define WRONG_INPUT -1
#define SUCCESS 1

void addFrameToList(link_t** firstNode, frame_t* newNode);
frame_t* createFrame(char*** arr, int* size);
int removeFrame(char*** arr, int* size, link_t** list);
void fixString(char* str);
int printMenu();
int playMovie(link_t* list, int size);
void changeAllImageDuration(link_t** list);
void displayList(link_t* list);
int nameInArr(char** arr, int size, char* name);
void addToArray(char** arr, int size, char* name);
void freeall(char*** arr, int size, link_t** list);
int changeImageDuration(link_t** list, char** arr, int size);
int changeIndex(link_t** list, char** arr, int size);

int main()
{
	//declaring variables
	char name[MAX_LENGTH] = { 0 };
	link_t* list = NULL;
	frame_t* frame = NULL;
	int size = 0;
	char** arr = (char**)malloc(sizeof(char*) * 1);
	int answer = 0;
	int print_ans = 0;
	int function_ans = 0;
	int found = 0;
	do
	{
		//checking what function to call
		answer = printMenu();
		switch (answer)
		{
			case EXIT:
				freeall(&arr, size, &list);
				break;
			case ADD_NODE:
				frame = createFrame(&arr, &size);
				addFrameToList(&list, frame);
				break;
			case REMOVE_NODE:
				function_ans = removeFrame(&arr, &size, &list);
				if (function_ans == NOT_FOUND)
				{
					printf("The image wasn't found !");
				}
				break;
			case SWITCH_NODE:
				function_ans = changeIndex(&list, arr, size);
				if (function_ans == NOT_FOUND)
				{
					printf("The image wasn't found !");
				}
				break;
			case FRAME_DURATION:
				function_ans = changeImageDuration(&list, arr, size);
				if (function_ans == NOT_FOUND)
				{
					printf("The image wasn't found !");
				}
				break;
			case FRAMES_DURATION:
				changeAllImageDuration(&list);
				break;
			case LIST_FRAMES:
				displayList(list);
				break;
			case PLAY_MOVIE:
				function_ans = playMovie(list, size);
				if (function_ans == WRONG_INPUT)
				{
					printf("You need to enter number 1 or 2 !");
				}
				break;

			default:
				printf("You should type one of the options  - 0-7\n\n");
				break;
		}
	} while (answer != EXIT);
	system("pause");
	return 0;
}

/**
	Function will add a new frame to a new node in the list
	Input:
		list - the first node of the list
		frame - the new song to add to the playlist
	Output:
		None
*/
void addFrameToList(link_t** list, frame_t* frame)
{
	//declaring variables
	link_t* newNode = (link_t*)malloc(sizeof(link_t));
	newNode->frame = frame; // putting the frame in the new node
	link_t* currNode = *list;
	/* if the linked list is empty
	put the new node as the first */
	if (!currNode)
	{
		*list = newNode;
		newNode->next = NULL;
	}
	else
	{
		//else finding the last node in array
		while (currNode->next)
		{
			currNode = currNode->next; //if currNode didn't reach the last node, it will move forward to the next node
		}

		currNode->next = newNode; // putting newNode address as the next of the last node
		newNode->next = NULL; //making newNode the end of the linked list
	}
}

/*
	The function takes the new frame details and makes a new Frame
	Input:
		a pointer to an arr of names, that saves all the images names
		a pointer to a size variable. that contains the number of names in the array
	Output:
		a pointer to the new frame
*/
frame_t* createFrame(char*** arr, int* size)
{
	//declaring variables
	int ans = 1;
	//allocates memory to the name and the frame
	frame_t* frame = (frame_t*)malloc(sizeof(frame_t)); 
	frame->path = (char*)malloc(sizeof(char) * 256);
	frame->name = (char*)malloc(sizeof(char) * 256);
	//getting the frame details from the user
	printf("     ***Creating New Frame***      \n");
	printf("Please enter Frame Path:\n");
	fgets(frame->path, MAX_LENGTH, stdin);
	frame->path[strlen(frame->path) - 1] = 0; //deletes the \n char
	printf("Please enter frame duration(in miliseconds):\n");
	scanf("%d", &(frame->duration));
	getchar(); //cleans the buffer
	while (ans >= 0) //getting the user input while the image name exist
	{
		printf("Please choose a name for this frame:\n");
		fgets(frame->name, MAX_LENGTH, stdin);
		frame->name[strlen(frame->name) - 1] = 0; //deletes the \n char
		ans = nameInArr(*arr, *size, frame->name); 
		if (ans >= 0)
		{
			printf("The name is already taken\n");
		}
	}
	*size = *size + 1; //increase the number of images's names in arr by one
	*arr = realloc(*arr, sizeof(char*)* (*size)); //allocates a bigger memory to save also the new image's name
	*(*arr + *size - 1) = (char*)malloc(sizeof(char) * MAX_LENGTH); //allocates memory to the new in the array name
	addToArray(*arr, *size, frame->name); //The function adds the name to the array
	return frame;
}


/*
	The function prints the menu, getting the user input and returns it
	Input:
		None
	Output:
		the choice's number of the user
*/
int printMenu()
{
	int answer = 0;
	printf("\n\nEditing Project! what would you like to do ?\n");
	printf("[0] - Exit\n");
	printf("[1] - Add new Frame\n");
	printf("[2] - Remove a Frame\n");
	printf("[3] - Change Frame index\n");
	printf("[4] - Change Frame duration\n");
	printf("[5] - Change duration of all Frames\n");
	printf("[6] - List frames\n");
	printf("[7] - Play movie!\n");
	printf("Please enter your choice:");
	scanf("%d", &answer); //getting the user's choice
	getchar(); //cleans the buffer
	return answer; 
}

/*
	The Function changes all the durations in all the images
	Input:
		 a pointer to the linked list
	Output:
		None
*/
void changeAllImageDuration(link_t** list)
{
	//declaring variables
	unsigned int duration = 0;
	link_t* currNode = NULL;
	//getting the user's new duration
	printf("Enter the duration for all Frames:\n");
	scanf("%d", &duration);
	getchar(); //cleans the buffer
	currNode = *(list);
	while (currNode) // moving forward to the next node in the linked list, until it reach a node that is NULL
	{
		currNode->frame->duration = duration; //changes every node's frame duration to the new duration that was got by the user
		currNode = currNode->next; //moving currNode to the next one
	}
}

/*
	The function prints every node's frame details
	Input:
		a linked list of the images
	Output:
		None
*/
void displayList(link_t* list)
{
	link_t* currNode = list;
	printf("Name               Duration           Path\n");
	while (currNode)
	{
		printf("%s               %d ms           %s\n", currNode->frame->name, currNode->frame->duration, currNode->frame->path); //print s the currNode details
		currNode = currNode->next; // moving currNode to the next  Node
	}
}

/*
	The function checks if the name that was given is in the list of image's, to see if it's in the linked list
	Input:
		an array of strings, the names of the image's in the linked list
	Output:
		-1 - if the image wasn't found
		or
		the number of the name in arr
*/
int nameInArr(char** arr, int size, char* name)
{
	//declaring variables
	int ans = -1;
	int i = 0;
	for (i = 0; i < size; i++) //scaning the array
	{
		if (strcmp(*(arr + i), name) == 0) //when finding name in the array
		{
			ans = FOUND; //changing ans to 1
			break;
		}
		if (strcmp(*(arr + i), name) > 0) //if the loop reach words that are "bigger" then name it stops
		{
			break; 
		}
	}
	if (ans == FOUND) //if name was found in arr
	{
		ans = i;
	}
	return ans;
}

/*
	Adds the name to the array of image's name
	Input:
		arr - The array of names
		size - The number of image's names in the array
		name - The name to add to the array
	Output:
		None
		
*/
void addToArray(char** arr, int size, char* name)
{
	char temp[MAX_LENGTH];
	int i = 0;
	int j = 0;
	for (i = 0; i < size; i++)
	{
		if (i == size - 1) //when reaches the end of the list, puts name at the end of the list
		{
			strcpy(*(arr + i), name); //puts name at the end of the list
			break;
		}
		else if (strcmp(*(arr + i), name) > 0) //moving forward on the array until name is "bigger" than some name
		{
			for (j = size - 1; j > i; j--)
			{
				strcpy(*(arr + j), *(arr + j - 1)); // moving every cell in the array right
			}
			strcpy(*(arr + i), name); //copying the name to the right place to keep the array sorted 
			break;
		}
	}
}

/*
	The function frees all the memory was allocated
	Input:
		arr - a pointer to an array of string, that are the image's names
		size - the size of the array
	Output:
		
	*/
void freeall(char*** arr, int size, link_t** list)
{
	//declaring variables
	link_t* currNode = *(list);
	link_t* temp = *(list);
	int i = 0;
	//frees the array of names
	for (i = 0; i < size; i++)
	{
		free(*(*arr + i)); 
	}
	free(*arr); //frees the head of the array
	while (currNode)
	{
		//The function free the frame memory
		free(currNode->frame->path);
		free(currNode->frame->name);
		free(currNode->frame);
		temp = currNode->next;
		free(currNode);
		currNode = temp;
	}
}
/*
	Changes an spesific image duration by her name
	Input:
		list - a pointer to the linked list
		arr - The arr of images's names.
		size - the number of names
	Output:
		-1 if the name wasn't found
		 1 if the name was found and the duration changed
*/
int changeImageDuration(link_t** list, char** arr, int size)
{
	//declaring variables
	int found = 0;
	unsigned int duration = 0;
	link_t* first = *(list);
	char name[MAX_LENGTH] = { 0 };
	printf("Please enter picture's name:"); 
	fgets(name, MAX_LENGTH, stdin); 
	name[strlen(name) - 1] = 0; //delete the \n char
	found = nameInArr(arr, size, name); //checking if the name is in arr
	if (found == NOT_FOUND)
	{
		return NOT_FOUND;
	}
	printf("Please enter picture's new duration:");
	scanf("%d", &duration); //getting the new duration
	while (first)
	{
		if (strcmp(first->frame->name, name) == 0) //finds the image
		{
			first->frame->duration = duration; //puts the new duration in the duration field
			break;
		}
		first = first->next; //moving to the net node
	}
	return FOUND;
}
/*
	The function remove an image Node and from arr by names
	Input:
		arr - a pointer to a n array of strings
		size - a pointer to size 
		list - a pointer to the linked list
	Output:
		1 - if the image was found
	   -1 - if the image wasn't found	  
*/
int removeFrame(char*** arr, int* size, link_t** list)
{
	//declaring variables
	link_t* pastNode = *(list);
	link_t* currNode = pastNode->next; 
	int ans = NOT_FOUND;
	int i = 0;
	char name[MAX_LENGTH] = {0};
	printf("Please choose a name of frame to remove:\n");
	fgets(name, 256, stdin);
	name[strlen(name) - 1] = 0; //getting the image name to delete
	ans = nameInArr(*arr, *size, name);
	if (ans == NOT_FOUND) //checks if the name exist in the array
	{
		return NOT_FOUND;
	}
	//keeping the array sorted without the image's name to remove
	for (i = ans; i < *size - 1; i++)
	{
		strcpy(*(*arr + i), *(*arr + i + 1));
	}
	*size = *size - 1;
	free(*(*arr + *size));
	*arr = realloc(*arr, sizeof(char*)* (*size)); //making the arr smaller
	//finding the image by name and free her memory
	if (strcmp(pastNode->frame->name, name) == 0) 
	{
		*list = currNode;
		free(pastNode->frame->path);
		free(pastNode->frame->name);
		free(pastNode->frame);
		free(pastNode);
	}
	else
	{
		//finds the image and free her memory
		while (currNode)
		{
			if (strcmp(currNode->frame->name, name) == 0)
			{
				pastNode->next = currNode->next;
				free(currNode->frame->path);
				free(currNode->frame->name);
				free(currNode->frame);
				free(currNode);
				break;
			}
			pastNode = currNode;
			currNode = currNode->next;
		}
	}
	return FOUND;
}

/*
	The function check if the the user want to play from a certain point and change the list
	Input:
		a linked list
		the size of the list
*/
int playMovie(link_t* list, int size)
{
	int i = 0;
	int answer = 0;
	int startFrom = 0;
	link_t* first = list;
	printf("\n\nIf you want to play the whole movie enter 1\n");
	printf("If you want to play the movie from a certain point enter 2\n");
	printf("Please enter your choice:");
	scanf("%d", &answer);
	if (answer == 2) //if the user want to play from a certain point
	{
		printf("Where do you want to start the movie from?");
		scanf("%d", &startFrom);
		if (startFrom > size) //checking if the list isn't long enough
		{
			printf("The movie has only %d frames!", size);
		}
		else
		{
			for (i = 1; i < startFrom; i++) //making the list start from the certain point
			{
				first = first->next;
			}
		}
	}
	else if (answer != 1)
	{
		return WRONG_INPUT;
	}
	play(first); //play the new list, if the user wants to play it all, first won't change
	return SUCCESS;
}


/*
	The function changes an image index
	Input:
		list - a pointer to the linked list
		arr - the names of the images in the linked list
		size - the size of the array
	Output:
	   -1 - if the image wasn't found
		1 - if the image was found
*/
int changeIndex(link_t** list, char** arr, int size)
{
	//declaring varaibles
	link_t* switchNode = *(list);
	link_t* switchWithNode = *(list);
	link_t* pastSwitchNode = *(list);
	link_t* pastSwitchWithNode = *(list);
	link_t* currNode = *(list);
	int counter = 1;
	int index = 0;
	int found = 0;
	int saveCounter = 1;
	char firstname[MAX_LENGTH] = { 0 };
	char name[MAX_LENGTH] = { 0 };
	strcpy(firstname, currNode->frame->name);
	//getting from the user the image's name
	printf("Please enter picture's name:");
	fgets(name, MAX_LENGTH, stdin);
	name[strlen(name) - 1] = 0;
	//getting the image new index
	if (NOT_FOUND == nameInArr(arr, size, name)) //checking if the name of the image is in arr
	{
		return NOT_FOUND;
	}
	do
	{
		printf("Please enter number of index:");
		scanf("%d", &index);
		if (index > size || index < 1)
		{
			printf("The film has only %d images!\n", size);
			printf("The index is out of range!\n");
		}
	} while (index > size || index < 1);
	while (currNode) //until currNode reach NULL
	{
		if (currNode->next != NULL && strcmp(currNode->next->frame->name, name) == 0) //searching for the image node and the node before her
		{
			switchNode = currNode->next; //saving the image node
			pastSwitchNode = currNode; //saving the previous node to the image
			saveCounter = counter + 1; //saving the picture index
		}
		if (counter + 1 == index) //searching the image node and the node befoe her by the index 
		{
			switchWithNode = currNode->next; //saving the node of the index
			pastSwitchWithNode = currNode; //saving the previous node to the index
		}
		currNode = currNode->next; //moving currNode to the next node
		counter++;
	}
	if (index != saveCounter) //if the image isn't replaced by herself
	{
		if (strcmp(firstname, name) == 0) //if the first image is to be replaced
		{
			*(list) = switchNode->next;
			switchNode->next = switchWithNode->next;
			switchWithNode->next = switchNode;
		}
		else if (index == 1) //if a new image nned to be first
		{
			pastSwitchNode->next = switchNode->next;
			switchNode->next = *(list);
			*(list) = switchNode;
		}
		else
		{
			if (index < saveCounter) //if an image moves backwards
			{
				pastSwitchNode->next = switchNode->next;
				switchNode->next = switchWithNode;
				pastSwitchWithNode->next = switchNode;
			}
			else if (index > saveCounter) //if an image moves forward
			{
				pastSwitchNode->next = switchNode->next;
				switchNode->next = switchWithNode->next;
				switchWithNode->next = switchNode;
			}
		}
	}
	return FOUND;
}