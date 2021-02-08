#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define RES 60

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
double calculateStability (int iteration, struct Complex z, struct Complex c, double threshold) {
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
        return calculateStability(iteration + 1, zn, c, threshold);
    }
}

int main () {
    struct Complex c;
    double stability;
    
    double scale = 1;
    double pScale = 1;
    double re = -2.0;
    double pRe = -2.0;
    double im = 1;
    double pIm = 1;

    int height = RES * 2/3;
    int width =  RES * 2;
    int n = 0;

    for (;;) {
        for (int j = 0; j < height; j ++) {
            for (int k = 0; k < width; k ++) {
                c.r = re + (double)k * scale * (double)3 / (RES * 9/4);
                c.i = im - (double)j * scale * (double)3 / RES;
                /*
                printZ(c);
                printf("\n");
                */
                stability = calculateStability(1, f_z(origin, c), c, 0.1);
                // printf("Stability: %.3f\n", s);

                char shade = stability > 0 ? '%' : '.';
                printf("%c", shade);
                
            }
            printf("\n");
        }
        scanf("%d", &n);
        printf("\n");
        
        if (n <= 0) {
            scale = pScale;
            re = pRe;
            im = pIm;
        } else {
            pScale = scale;
            scale = scale / 2;
            pIm = im;
            pRe = re;

            if (n >= 7) {
                re += scale * (n - 7);
            } else if (n >= 4) {
                im -= scale;
                re += scale * (n - 4);
            } else if (n >= 1) {
                im -= scale * 2;
                re += scale * (n - 1);
            }
        }
        
       // printf("n = %d", n);
    }

    return 0;
}
