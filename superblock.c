
#include "globalConstants.h"
#include "utility.h"

//one for root
inode * root;
//curr directory
inode * currdirectory;//one for root
superblock *sfssuperblock;

int superblockInit()
{
    sfssuperblock=(superblock *)malloc(sizeof(superblock));
    sfssuperblock->no_of_free_datablocks = TOTAL_DATABLOCKS;
    sfssuperblock->no_of_free_inodes = TOTAL_INODES;
    sfssuperblock->inodes = (inode *)malloc(sizeof(inode)*TOTAL_INODES);
    sfssuperblock->datablocks = (datablock *)malloc(sizeof(datablock)*TOTAL_DATABLOCKS);
    sfssuperblock->inodelist=(int *)malloc(sizeof(int)*TOTAL_INODES);
    sfssuperblock->datablocklist=(int *)malloc(sizeof(int)*TOTAL_DATABLOCKS);
	if(sfssuperblock->inodes == NULL || sfssuperblock->datablocks==NULL)
    {
    	return 0;
	}
	for(int i=0;i<TOTAL_INODES;i++)
	{
		//0 if the inode is not used,1 if the inode is used.
		sfssuperblock->inodelist[i]=0;
		//Setting the id for each inode with its index in the list
		sfssuperblock->inodes[i].id=i;
	}

	for(int i=0;i<TOTAL_DATABLOCKS;i++)
	{
		//Marking each datablock as unused by setting it to 0
		//0 means unused 1 means used
		sfssuperblock->datablocklist[i]=0;
	}

  //Creating the root directory
  sfssuperblock->inodelist[0]=1;
  sfssuperblock->no_of_free_inodes--;
  root=&sfssuperblock->inodes[0];
  strcpy(root->name,"root");
  strcpy(root->type,"directory");
  root->n_link=2;
  root->createtime=time(0);
  root->datablocksarray=NULL;
  root->noOfDatablocks=0;
  root->inodeList=NULL;
  root->noOfInodes=0;
  root->parent=NULL;//Root directory has no parent
  root->fdcount=0;
  currdirectory=&sfssuperblock->inodes[0];//Setting the currentdirectory to root
  return 1;
}


inode* getInode()
{
	if(sfssuperblock->no_of_free_inodes > 0)
	{
		for(int i=0;i<TOTAL_INODES;i++)
		{
			if(sfssuperblock->inodelist[i]==0)
			{
				//Finding the smallest unused inode
				sfssuperblock->no_of_free_inodes--;
        		sfssuperblock->inodelist[i]=1;
        		sfssuperblock->inodes[i].nextdirentry=NULL;
        		sfssuperblock->inodes[i].noOfInodes=0;
        		sfssuperblock->inodes[i].inodeList=NULL;
        		sfssuperblock->inodes[i].datablocksarray=NULL;
        		sfssuperblock->inodes[i].noOfDatablocks=0;
				return &sfssuperblock->inodes[i];
			}
		}
	}
	//Returning NULL if you cant find an empty inode
	return NULL;
}



datablock* getDatablock()
{
	if( sfssuperblock->no_of_free_datablocks > 0)
	{
		for(int i=0;i<TOTAL_DATABLOCKS;i++)
		{


			if(sfssuperblock->datablocklist[i]==0)
			{
				//Finding the smallest unused datablock
				sfssuperblock->no_of_free_datablocks--;
        		sfssuperblock->datablocklist[i]=1;
        		sfssuperblock->datablocks[i].nextdatablock=NULL;
        		memset(sfssuperblock->datablocks[i].data,0,sizeof(sfssuperblock->datablocks[i].data));
        		sfssuperblock->datablocks[i].currsize=0;
				return &sfssuperblock->datablocks[i];
			}
		}
	}
	//Returning NULL if you cant find an empty inode
	return NULL;
}

int superblockDestroy()
{
	free(sfssuperblock->inodes);
	free(sfssuperblock->datablocks);
	free(sfssuperblock);
}
