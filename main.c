#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define RES 60

struct Complex {
    double r;
    double i;
};

struct Complex origin = { .r = 0.0, .i = 0.0};

struct Complex square (struct Complex z) {
    struct Complex zSquared = {
        .r = (z.r * z.r) - (z.i * z.i),
        .i = 2 * z.r * z.i
    };

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
    return sqrt(pow(z1.r - z2.r, 2) + pow(z1.i - z2.i, 2));
}

double calculateStability (int iteration, struct Complex z, struct Complex c, double threshold) {
    if (distance(z, origin) > 2) {
        return 0;
    }
    struct Complex zn =  f_z(z, c);
    if (distance(z, zn) < threshold || iteration > 200) {
        return (double)1 / iteration;
    } else {
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

                stability = calculateStability(1, f_z(origin, c), c, 0.1);

                char shade = stability > 0 ? '%' : ' ';
                printf("%c", shade);
                
            }
            printf("\n");
        }
        scanf("%d", &n);
        printf("\n");
        
        if (n < 0)
            return 0;
        else if (n == 0) {
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
    }

    return 0;
}
