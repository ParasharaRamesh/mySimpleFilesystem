#include "sfs.h"
#include "utility.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/time.h>

//purpose of this file is to mimic the shell and support the following commands 
//and call the appropriate handlers
extern superblock sfssuperblock;
int main()
{
	char *name="hello";
	char * name2="worlds";
	mkfs();
	showFileTableContents();
	printf("testing the filesystem\n");
	if(sfscreate("Hello"))
	{
		printf("File \'Hello\' created\n\n");
	}
	if(sfsopen("Hello",getpid()))
	{
		printf("file hello opened!\n\n");
	}

	showFileTableContents();

	int result=sfswrite("Hello",getpid(),name);
	if(result==1)
	{
		printf("wrote into file \'Hello\' \n");
		inode *q=getInodeFromCurrDirectory("Hello","file");
		printf("the current size of this file is %d \n\n",getSizeOfFile(q));
		showFileTableContents();
	}
		inode *q=getInodeFromCurrDirectory("Hello","file");
		printf("From read data blocks %s\n\n",readDataBlocks(q));
		if(sfslseek("Hello",getpid(),2))
		{
			printf("lseeked !\n");
			showFileTableContents();
		}
		printf("From sfsread ---%s---\n",sfsread("Hello",getpid(),3));
/*
		if(sfslseek("Hello",getpid(),0))
		{
			printf("lseeked !\n");
			showFileTableContents();
		}
*/
		if(sfsread("Hello",getpid(),8)==NULL)
		{
			printf("test case passsed!\n\n");
		}
		printf("No of data blocks in this file %d\n\n",q->noOfDatablocks);
		showFileTableContents();
		printf("writing into same file again!\n");
		result=sfswrite("Hello",getpid(),name2);//was 1
  	if(result==1)
  	{
 	 		printf("wrote into file 2nd time \'Hello\' \n");
 	 		inode *q=getInodeFromCurrDirectory("Hello","file");
 	 		printf("the current size of this file is %d \n\n",getSizeOfFile(q));
 	 		showFileTableContents();
 	 	}
		printf("second time From read data blocks is---%s----\n\n",readDataBlocks(q));
		printf("second time From sfsread  %s\n",sfsread("Hello",getpid(),2));
		sfsclose("Hello",getpid());
		printf("closed the first time\n\n");
		showFileTableContents();

	 // printf("opening same file again\n");
	 // if(sfsopen("Hello",getpid()))
	 // {
		//  printf("file hello opened!\n\n");
		//  showFileTableContents();
	 // }
	 // result=sfswrite("Hello",getpid(),name2,1);
 		// if(result==1)
 		// {
	 // 		printf("wrote into file 2nd time \'Hello\' \n");
	 // 		inode *q=getInodeFromCurrDirectory("Hello","file");
	 // 		printf("the current size of this file is %d \n\n",getSizeOfFile(q));
	 // 		showFileTableContents();
	 // 	}
	return 0;
}
