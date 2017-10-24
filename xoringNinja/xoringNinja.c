#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define D(n) 
#define Modulo 1000000007

int getNumberOfSetBits(int i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int main() 
{
    freopen("sample.txt", "r", stdin);
    int casesCount;
    scanf("%d", &casesCount);
    for(int i = 0; i < casesCount; i++)
    {    
        int n; 
        scanf("%d", &n);
        int sum = 0;
        for(int i = 0; i < n; i++)
        {
            int number;
            scanf("%d", &number);
            sum |= number;
        }
        int bits = getNumberOfSetBits(sum);
        D(printf("sum %d  bits %d\n", sum, bits));
        //int power = pow(2, n-1);
        //int result = (int)(sum*power) % Modulo;
        int result = sum % Modulo;
        for(int i=0; i<n-1; i++)
        {
            result = (result * 2) % Modulo;
        }
        D(printf("power %d  result %d\n", power, result));
        printf("%d\n", result);
    }
    return 0;
}