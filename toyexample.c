//can you SEE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

#define L_DATE 0
#define L_SOMETHING 50

typedef struct something{
  int blah1;
  int blah2;
  char blah3[10];
}something;

typedef struct date {
    char day[80];
    int month;
    int year;
    something *x;
}date;

//r+b only works for already existing files
int DiskWrite(date * dateobj,char * disk)
{
  FILE *file = fopen(disk, "w+b");
  if( file == NULL)
  {
      printf("failed to create/open file\n");
      return 0;
  }
  if( fseek( file , L_DATE , SEEK_SET) != 0 )
  {
      printf("failed to lseek\n");
      return 0;
  }
  if( fwrite(dateobj, sizeof(date), 1 , file) != 1 )
  {
      printf("failed to write using fwrite\n");
      return 0;
  }
  fclose(file);
  return 1;
}

date * DiskRead(char* disk)
{
  date * dateobj = (date *)malloc(sizeof(date));
  FILE *file = fopen(disk, "rb");
  if(file == NULL)
  {
      printf("failed to create/open in read\n");
      return NULL;
  }
  if(fseek( file , L_DATE , SEEK_SET) != 0 )
  {
      printf("failed to fseek in read\n");
      return NULL;
  }
  if(fread(dateobj , sizeof(date), 1 , file) != 1)
  {
      printf("failed to read using fread\n");
      return NULL;
  }
  fclose(file);
  return dateobj;
}

int test123()
{
	int a = 123;
	return a;
}

int main()
{

  something *s1 = (something *)malloc(sizeof(something));
  date *d1 = (date *)malloc(sizeof(date));
  char *disk = "diskfile.txt";
  s1->blah1 = 1;
  s1->blah2 = 2;
  strcpy(s1->blah3,"hello world");
  d1->month = 3;
  d1->year = 2018;
  d1->x = s1;
  strcpy(d1->day,"Saturday");
  printf("going to write !!....\n");
  if(DiskWrite(d1,disk))
  {
    printf("write was successful!\n");
  }
  else
  {
    printf("write failed!\n");
    exit(0);
  }
  date *d2=DiskRead(disk);
  if( (d2->month == d1->month) && (d2->year == d1 ->year) && (strcmp(d1->day,d2->day) == 0))
  {
    printf("so far so good\n");
    if((d1->x->blah1==d2->x->blah1) && (d1->x->blah2==d2->x->blah2) && (strcmp(d1->x->blah3,d2->x->blah3)==0))
    {
      printf("wrote and read from file Successfully!\n");
    }
    else
    {
      printf("didnt work:(\n");
      exit(0);
    }
  }
  else
  {
    printf("didnt work:(\n");
    exit(0);
  }
  d1 -> month = 100;
  if(DiskWrite(d1,disk))
  {
    printf("write was successful!\n");
  }
  date *d3 = DiskRead(disk);
  printf("d3 month value should be 100 : %d\n",d3->month);
  int test;
  printf("enter the vlaue of test:\n");
  scanf("%d",&test);
  printf("test : %d\n",test);
  printf("finished executing\n");
  printf("How are you are you going to run ?? \n");
  test = test123();
  printf("value of test after test123 : %d\n",test);

  return 1;
}
