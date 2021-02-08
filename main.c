#include <stdio.h>
#include <math.h>

#define WIDTH 50
#define HEIGHT 50
#define LIMIT 10

struct Complex {
    double r;
    double i;
};

struct Complex origin = {.r = (double)0, .i = (double)0};

void printZ (struct Complex z) {
    printf("(%.3f %c%.3fi)", z.r, z.i < 0 ? '-' : '+', fabs(z.i));
}

struct Complex square (struct Complex z) {
    struct Complex zSquared = {
        .r = (z.r * z.r) - (z.i * z.i),
        .i = 2 * z.r * z.i
    };

/*
    printZ(z);
    printf(" squared = ");
    printZ(zSquared);
    printf("\n");
*/
    return zSquared;
}

struct Complex add (struct Complex z1, struct Complex z2) {
    struct Complex sum = {
        .r = z1.r + z2.r,
        .i = z1.i + z2.i
    };
    return sum;
}

struct Complex f_z (struct Complex z, struct Complex c) {
    struct Complex z_nminus1 = add(square(z), c);
    return z_nminus1;
}

int isStableRecursive (struct Complex c, struct Complex zn, int limit) {
    if (limit == 0) {
        return zn.r + 2 > c.r || zn.i +2 > c.i || zn.r - 2 < c.r || zn.i - 2 < c.i
            ? 1
            : 0;
    }

    return isStableRecursive(c, f_z(zn, c), limit - 1);
}

int isStable (struct Complex c) {
    struct Complex origin = { .r = 0, .i = 0 };
    
    return isStableRecursive(c, f_z(origin, c), LIMIT);
}

double distance (struct Complex z1, struct Complex z2) {
    double dist = sqrt(pow(z1.r - z2.r, 2) + pow(z1.i - z2.i, 2));
    /*
    printf("distance(");
    printZ(z1);
    printf(", ");
    printZ(z2);
    printf(") = %.3f\n", dist);
    */
    return dist;
}

/**
 * Quanitifies the stability of a complex constant
 * @param iteration The current iteration
 * @param c The complex constant
 * @param threshold The threshold where, if the distance between f(c) and the origin is less than this, f(c) is considered stable
 */
double stability (int iteration, struct Complex z, struct Complex c, double threshold) {
    // Check if c is stable.

    /*
    printf("z = ");
    printZ(z);
    printf("\n");

    */
    if (distance(z, origin) > 2) {
        // printf("distance(z, origin) > 2.\n");
        return 0;
    }
    struct Complex zn =  f_z(z, c);
    if (distance(z, zn) < threshold || iteration > 200) {
        // printf("z is stable for iteration %d\n", iteration);
        return (double)1 / iteration;
    } else {
        /*
        printf("Checking stability for ");
        printZ(zn);
        printf("\n");
        */
        return stability(iteration + 1, zn, c, threshold);
    }
}

int main () {
    struct Complex c;
    int scaleX = 4;
    int div = WIDTH / scaleX;
    double s;


    for (int y = 0; y < HEIGHT; y ++) {
        for (int x = 0; x < WIDTH; x ++) {
            c.r = (double)(- scaleX / 2) + (double)x * (double)scaleX / WIDTH;
            c.i = (double)(scaleX / 2) - (double)y * (double)scaleX / HEIGHT;
            /*
            printZ(c);
            printf("\n");
            */
            s = stability(1, f_z(origin, c), c, 0.1);
            // printf("Stability: %.3f\n", s);

            char shade = s > 0 ? '%' : '.';
            printf("%c", shade);
            
        }
        printf("\n");
    }

    double r;
    double i;

        c.r = -0.16;
        c.i = 0.8;
        printZ(c);
        printf("\n");
        s = stability(1, origin, c, 0.05);
        printf("Stability: %.6f\n", s);


    
    return 0;
}
