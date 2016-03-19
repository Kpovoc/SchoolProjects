/* Multithreaded sorting program that works as follows:
 * A list of integers is divided into two smaller lists
 * of equal size. Two seperate threads (which we will 
 * term sorting threads) sort each sublist using a 
 * sorting algorithm of your choice. The two sublists
 * are then merged by a third thread - a merging thread
 * - which merges the two sublists into a single sorted
 * list.
 * Requirements:
 * 
 * 1. This Program will work by multiple thread generation
 * 2. Input is integer and should be kept in a file as one value 
 *    per line. The file name will be read from command line
 * 3. The list must be divided into two smaller lists of equal size
 * 4. Two different sorting threads will be created to sort the 
 *    two different lists using any sorting algorithm
 * 5. The two sublists shoud be merged into a single sorted list 
 *    by a merging thread
 * 6. The parent thread will output the sorted array once all 
 *    sorted threads have exited
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int *array;
int *sorted;
int ARRAYSIZE;

/*****************************************/
/*PASSABLE ARRAY STRUCT FUNCTIONS - BEGIN*/
/*****************************************/
struct sortpoints
{
	int pasize;
	int *pa;
};
struct sortpoints *point_create(int size, int start)
{
	struct sortpoints *point = malloc(sizeof(struct sortpoints));

	point->pasize = size;
	point->pa = malloc(size * sizeof(int));
	for(int i=0;i<size;i++)
		point->pa[i]=array[i+start];

	return point;
}
void point_destroy(struct sortpoints *point)
{
	free(point->pa);
	free(point);
}
void point_print(struct sortpoints *point,int start)
{
	for(int i=0;i<point->pasize;i++)
		sorted[i+start]=point->pa[i];
}
/*****************************************/
/*PASSABLE ARRAY STRUCT FUNCTIONS - END  */
/*****************************************/

/*****************************************/
/*HEAP SORTING ALG AND FUNCTIONS - BEGIN */
/*****************************************/
int iParent(int i)
{
	if(i-1==0 || i-2==0)
		return 0;
	else if((i-1)%2==0)
		return (i-1)/2;
	else //(i-2)%2==0
		return (i-2)/2;
}

int iLeftChild(int i)
{
	return (i*2)+1;
}

void aswap(int *p,int a, int b)
{
	int h;
	h=p[a];
	p[a]=p[b];
	p[b]=h;
}
void siftDown(int *p,int root,int end)
{
	int child;
	int swap;
	while(iLeftChild(root) <= end)
	{
		child = iLeftChild(root);
		swap = root;

		if(p[swap] < p[child])
			swap = child;
		if (child+1 <= end && p[swap] < p[child+1])
			swap = child + 1;
		if (swap == root)
			break;
		else
		{
			aswap(p,root, swap);
			root = swap;
		}
	}
}

void heapify(int *p, int size)
{
	int start = iParent(size-1);
	while(start >= 0)
	{
		siftDown(p,start, size-1);
		start-=1;
	}
}

void heapSort(struct sortpoints *point)
{
	int *p=point->pa;
	int size=point->pasize;
	int end=size-1;
	// Build the heap in array so that the largest value is at the root)
	heapify(p,size);

	while(end>0)
	{
		aswap(p,end, 0);
		end-=1;
		siftDown(p,0, end);
	}
}
int cmpfunc (const void *a, const void *b)
{
	return (*(int*)a - *(int*)b);
}
/*****************************************/
/*HEAP SORTING ALG AND FUNCTIONS - END   */
/*****************************************/

/*******************************************/
/*THREAD DECLARATIONS AND FUNCTIONS - BEGIN*/
/*******************************************/
pthread_t tid[3];
struct sortpoints *point[2];
void *threadSort(void *param)
{
	heapSort(param);
	return 0;
}
void *threadCombine(void *param)
{
	point_print(point[0],0);
	point_print(point[1],(ARRAYSIZE/2)+(ARRAYSIZE%2));
	qsort(sorted,ARRAYSIZE,sizeof(int),cmpfunc);
	return 0;
}

void threadUp(){
	struct sortpoints *p1 = point_create((ARRAYSIZE/2)+(ARRAYSIZE%2),0);
	struct sortpoints *p2 = point_create((ARRAYSIZE/2),
		(ARRAYSIZE/2)+(ARRAYSIZE%2));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid[0],&attr,threadSort,(void*) p1);
	pthread_create(&tid[1],&attr,threadSort,(void*) p2);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	point[0]=p1;
	point[1]=p2;
	pthread_create(&tid[2],&attr,threadCombine,NULL);
	pthread_join(tid[2],NULL);
	point_destroy(p1);
	point_destroy(p2);
}
/*******************************************/
/*THREAD DECLARATIONS AND FUNCTIONS - END  */
/*******************************************/

void fileToArray(char fileName[])
{
	int number;
	int ch;
	int count=0;

	FILE *input = fopen(fileName,"r");
	if (input == NULL)
	   {
	       printf("File Read Error.\n");
	       exit (0);
	   }
	ch = fscanf(input,"%d",&number);
	array = malloc(ARRAYSIZE*sizeof(int));
	while(ch!=EOF)
	{
		ARRAYSIZE++;
		array = realloc(array, ARRAYSIZE*sizeof(int));
		array[count]=number;
		count++;
		ch = fscanf(input,"%d",&number);
	}
	sorted = malloc(ARRAYSIZE*sizeof(int));
}
void printSortedArray()
{
	for(int i=0; i<ARRAYSIZE; i++)
		printf("%d ",sorted[i]);
	printf("\n");
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	char fileName[sizeof(argv[1])];
	strcpy(fileName,argv[1]);
	fileToArray(fileName);
	threadUp();
	printSortedArray();
}
