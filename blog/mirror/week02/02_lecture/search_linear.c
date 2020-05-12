#include <stdio.h>
#include <stdlib.h>

int linearSearch(int a[],int n,int key);

int main(void){
	int size;
	int key;
	int i;
	printf("Enter number of elements:");
	scanf("%d",&size);
//printf("Enter key to search for:");
//scanf("%d",&key);
	key = -99;
	int * a = malloc(size * sizeof(int));
	printf("Enter elements:\n");
	for(i=0;i<size;i++){
		scanf("%d",&a[i]);
	}

	int result = linearSearch(a,size,key);
	if(result != -1){
		printf ("Found at index %d\n",result);
	}else{
		printf ("Did not find element %d\n",key);
	}
	return 0;
}

int linearSearch(int a[],int n,int key){
	int i;
	for(i=0;i<n;i++){
		if(key == a[i]){
			printf("%d %d\n",key,a[i]);
			return i;   //return the index
		}
	}
	return -1;        //return -1 if not found
}
