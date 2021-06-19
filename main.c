#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
 
int main() {
 
  struct timeval start, end;
  gettimeofday(&start, NULL);
 
 	sleep(1);
 
  gettimeofday(&end, NULL);
  printf("Time taken to count to 10^5 is : %d milli seconds\n", 
    ((int)((end.tv_sec * 1000) + (end.tv_usec/1000)) - (int)((start.tv_sec * 1000) + (start.tv_usec/1000)))/1000);

  return 0;
}