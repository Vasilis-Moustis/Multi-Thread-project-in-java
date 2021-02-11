/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#include <time.h>
 
//#define SIZE    8;

int matrix_sum(int m[], int n)
{
    int sum=0;
    for (int i = 0; i < n; i++)
    {
        //printf("%d",m[i]);
        sum += m[i];
    }
    return sum;
}


int matrix_multiplication(int m[], int n)
{
    int product=1;
    for (int i = 0; i < n; i++)
    {
        product *= m[i];
    }
    return product;
}


int RandomNumber()
{
    int x = (rand() % 2000) - 1000;
    return x;
}


int main()
{
   
    int retval;     /* child process: user-provided return code */
    int status;     /* parent process: child's exit status */
    int fd[2 * 4];  //4 = number of processes
    for (int i = 0; i < 4; i++) 
    {
        pipe(&fd[2*i]);
    }
	
    printf ("Please give me the size of the matrix. \n");
    int N; 
    scanf("%d", &N);

    int matrix[N];
    for (int i = 0; i < N; i++)
    {
        matrix[i] = (rand() % 200) - 100;
        //printf("%d",matrix[i]);
        //printf("\n");
    }
    
	long long int T1, T2, T3;
	int i;
    int pid[4];
    //pipe(fd);
    pid[0]=fork();
    //pipe(fd);
    pid[1]=fork();
    pipe(fd);



    if (pid[0] == 0 && pid[1] == 0) 
    {
        T1 = matrix_sum(matrix ,N);
        printf("T1 is : ");
        printf("%lld", T1);
        printf("\n");
        write(fd[2*3 + 1], &T1, 16);
        //close(fd[2*0 + 1]);
    } 
    else if (pid[0] == 0 && pid[1] != 0) 
    {
        T2 = matrix_multiplication(matrix,N);
        printf("T2 is : ");
        printf("%lld", T2);
        printf("\n");
        write(fd[2*1 + 1], &T2, 16);
        //close(fd[2*1 +1]);
    } 
    else if (pid[0] != 0 && pid[1] == 0) 
    {
        T3 = RandomNumber();
        printf("T3 is : ");
        printf("%lld", T3);
        printf("\n");
        write(fd[2*2 + 1], &T3, 16);
        //close(fd[2*2 + 1]);
    } 
    else if (pid[0] != 0 && pid[1] != 0) 
    {
        sleep(4);
        read(fd[2*3], &T1, 16);
        read(fd[2*1], &T2, 16);
        read(fd[2*2], &T3, 16);
        close(fd[2*0]);
        close(fd[2*1]);
        close(fd[2*2]);
        close(fd[2*3]);
        
        int result[3] = {T1, T2, T3};
        for (int i = 0; i < 3; i++)                     //Loop for ascending ordering
    	{
		    for (int j = 0; j < 3; j++)             //Loop for comparing other values
	    	{
		    	if (result[j] < result[i])                //Comparing other array elements
	    		{
	    			int tmp = result[i];         //Using temporary variable for storing last value
	    			result[i] = result[j];            //replacing value
	    			result[j] = tmp;             //storing last value
	    		}  
	    	}
    	}
        for (int i = 0; i < 3; i++)
        {
            printf("%d", result[i]);
            printf("\n");
        }
        wait(&i);
        wait(&i);
        close(fd[2*3 + 1]);
    }
	else // fork returns -1 on failure 
    {
        perror("fork"); // display error message 
        exit(0); 
    }

    return 0;
}