/*============================================================================
 * Project #1 : Maze Solving
 * Task #3    : Coding
 * Group      : Kanchana H.A.G.M (E/10/170)
                Bandara W.G.N (E/10/049)
=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack{
    int count;
    int *x;
    int *y;
};

typedef struct stack *STACK;

/*----------------------------------------------------------------------------
 global variables
-----------------------------------------------------------------------------*/
int height,width; // dimensions of maze


/*----------------------------------------------------------------------------
 function prototypes
-----------------------------------------------------------------------------*/
void ReadMaze(FILE *file,char maze[height][width]);
void PrintMaze(char maze[height][width]);

void solvemaze(char maze[height][width]);
int check(char maze[height][width],int x,int y);

STACK create();
STACK push(STACK stack,int a,int b);
STACK pop(STACK stack);


/*----------------------------------------------------------------------------
 main function
-----------------------------------------------------------------------------*/
int main(int argc,char *argv[])
{
    FILE *file;
    file = fopen(argv[1],"r");

    /* scan the dimensions of array and assign them to variables */
    fscanf(file,"%d" "%d",&height,&width);

    /* array to store the maze */
    char maze[height][width];

    /* read the maze from text file and store it in a array*/
    ReadMaze(file,maze);

    PrintMaze(maze);

    /* solve the maze and print the solution */
    solvemaze(maze);
    PrintMaze(maze);

    /* close the opened file */
    fclose(file);

    return 0;
}


 /* --------------------------------------------------------------------------
 function to read a maze from a text file and store it in a array
 ----------------------------------------------------------------------------*/
void ReadMaze(FILE *file,char maze[height][width])
{
    int i;
    for(i=0;i<height;i++){
        fscanf(file,"%s",maze[i]);
        /* whole character line takes as a single string without
         taking characters one by one*/
    }
}


/*----------------------------------------------------------------------------
 function to print the maze
-----------------------------------------------------------------------------*/
void PrintMaze(char maze[height][width])
{
    int i,j;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            printf("%c",maze[i][j]);
        }
        printf("\n");
    }
}

/*----------------------------------------------------------------------------
This function returns the number of open spaces around the given position
----------------------------------------------------------------------------*/
int check(char maze[height][width],int x,int y)
{
    int n=0;
    if(maze[y+1][x]!='#'){n++;}
    if(maze[y][x+1]!='#'){n++;}
    if(maze[y-1][x]!='#'){n++;}
    if(maze[y][x-1]!='#'){n++;}
    return n;
}

/*---------------------------------------------------------------------------
function to solve the maze
----------------------------------------------------------------------------*/
void solvemaze(char maze[height][width])
{
    int x,y,i,j;
    char a;

    /* create a copy of maze */
    char mazecopy[height][width];
    for(i=0;i<height;i++){
        strcpy(mazecopy[i],maze[i]);
    }

    /*create two stacks to store the path of the robot and junctions*/
    STACK paths=create(),junctions=create();

    /*find the starting position of the robot*/
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            if(maze[i][j]=='o'){
                    x=j,y=i;
            }
        }
    }

    /*assign the starting point as the initial values for both path and junction stacks*/
    push(paths,x,y);
    push(junctions,x,y);

    /* move the robot until it finds the end point */
    while(mazecopy[y][x]!='*'){

        /* mark the current position as a wall so robot will not check this cell again*/
        mazecopy[y][x]='#';

        /*if the current poisition is a junction, add the coordinate to the junctions stack*/
        if(check(mazecopy,x,y)>1){push(junctions,x,y);}

        /* if atleast one path is free, move the robot to one of that direction*/
        if(check(mazecopy,x,y)>=1){
            if     (mazecopy[y-1][x]!='#'){y--;}
            else if(mazecopy[y][x+1]!='#'){x++;}
            else if(mazecopy[y+1][x]!='#'){y++;}
            else if(mazecopy[y][x-1]!='#'){x--;}

            /* add the new position to the path stack */
            push(paths,x,y);
        }

        /* conditions if all the paths are blocked */
        else{

            /*move the robot to the last junction*/
            x=junctions->x[0],y=junctions->y[0];

            /*remove the wrong path coordinates from the path stack*/
            while(paths->x[0]!=junctions->x[0] || paths->y[0]!=junctions->y[0]){
                pop(paths);
            }

            /* if last junction is also blocked, remove it from the junction stack*/
            if(check(mazecopy,x,y)==0){
                pop(junctions);
            }
        }
    }

    /*mark the correct path inside the maze and print the steps of correct path*/
    for(i=paths->count-2;i>=0;i--){
        maze[paths->y[i]][paths->x[i]]='x';
        printf("->(%d,%d)\n",paths->x[i],paths->y[i]);
    }
    printf("-end\n");
}

/*function to create a stack*/
STACK create()
{
    STACK mystack = malloc(sizeof(struct stack));
    mystack->count = 0;
    mystack->x=malloc(height*width*sizeof(int));
    mystack->y=malloc(height*width*sizeof(int));
    return mystack;
}

/*function to add data to the stack */
STACK push(STACK mystack,int a,int b)
{
    int n=mystack->count;
    for(n;n>0;n--){
        mystack->x[n]=mystack->x[n-1];
        mystack->y[n]=mystack->y[n-1];
    }
    mystack->x[0]=a;
    mystack->y[0]=b;
    mystack->count++;
    return mystack;
}

/*function to remove data from the stack*/
STACK pop(STACK mystack)
{
    int n;
    for(n=0;n< mystack->count-1;n++){
        mystack->x[n]=mystack->x[n+1];
        mystack->y[n]=mystack->y[n+1];
    }
    mystack->count--;
    return mystack;
}
