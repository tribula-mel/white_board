

void swap(int *x, int *y) 
{ 
    int Temp = *x;
    
    *x = *y; 
    *y = Temp; 
} 
  
void sort_array(int A[], int N) 
{ 
   int i = 0;
   int j = 0;
   
   for (i = 0; i < N-1; i++)       
  
       for (j = 0; j < N-i-1; j++)  
           if (A[j] > A[j+1]) 
              swap(&A[j], &A[j+1]); 
} 

int solution(int A[], int N)
{
    int result = 1;
    int i = 0;
    
    if ((N == 1) && ((A[0] > 1) || (A[0] <= 0)))
    {
        return result;
    }
    else if (N == 1)
    {
        return A[0]+1;
    }

    sort_array(A, N);
    
    for (i = 0; i < N; i++)
    {
        if (A[i] <= 0)
            continue;
        
        if (result == A[i]) {
            result++;
            continue;
        }
        if (result > A[i])
            continue;
        if (result < A[i])
            break;
    }
    
    return result;
}
