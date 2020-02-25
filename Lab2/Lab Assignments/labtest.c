#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
// Just CPU-bounded instructions which take some time to execute.
// The information computed is not important.
double delay(int loops)
{
    printf("Child");
int i, j;
double z = i, y;
z = i; // use an uninitialized value
for (i = 0; i < loops ; i++) {
// just some random computation to take CPU time
for (j = 0; j < 1000; j++) {
y = ((double) loops)/((double) (loops - i + 1));
z = sqrt((y*y + z)*i/((double) loops));

}
}
return z; // reduce the possibility of loop being optimized away
}

int main(int argc, char *argv[]) {
double z;
int D = atoi(argv[1]);
if (D > 0) z = delay(D);
exit(z);
}
