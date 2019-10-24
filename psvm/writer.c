#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
        int mgrow = 1024;
        int gigrow = 1048576;
        long int two_gigrow = 2*gigrow;
        long int three_gigrow = 3*gigrow;
        long int four_gigrow = 4*gigrow;
        long int eight_gigrow = 8*gigrow;
        int col = 1024;

        unsigned long int mgsize = mgrow * col;
        unsigned long int gigsize = gigrow * col;
        unsigned long int two_gigsize = two_gigrow * col;
        unsigned long int three_gigsize = three_gigrow * col;
        unsigned long int four_gigsize = four_gigrow * col;
        unsigned long int eight_gigsize = eight_gigrow * col;
        
        char *data = calloc(two_gigrow, col);
        memset(data, 'a', two_gigsize);
        printf("%d %p %lu \n", getpid(), data, two_gigsize);
        while (1);
        return 0;
}