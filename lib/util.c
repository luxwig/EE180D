#include "util.h"

typedef float ftype
#define MAX(X,Y) ((X<Y)?Y:X)

ftype* mean(ftype** x, int n, int p)
{
    int i, j;
    ftype sum;
    ftype* r = (ftype*)malloc(sizeof(ftype)*p);
    for(i = 0; i < p; i++){
        sum = 0;
        for (j = 0; j < n; j++)
            sum = sum + ftype[j][i];
        r[i] = sum/n;
    }
    return r;
}

void pca(ftype** x, int n, int p)
{
    int     i, j;
    int     vCentered = 1;
    int     DOF = MAX(0, n-vCentered);
    int*    maxind;
    ftype   tmp;
    ftype*  mu;
    ftype** s, coeff, u;
    ftype*  sigma;
    // mu = (ftype*)malloc(sizeof(ftype)*p);
    mu = mean(x, n, p); 
    svd(x, n, p, u, s ,coeff);
    sigma = diag(s);
    score = s;
    latent = sigma;
    for (i = 0; i < n; i++)
        latent[i] = sigma * sigma / DOF;
    maxind = (int*)malloc(sizeof(int)*p);

    for (i = 0; i < p; i++) {
        tmp = INT_MIN;
        for (j = 0; j < p; j++)
            tmp = MAX(tmp, coeff[j][i]);
        maxind[i] = tmp;
    }
    for (i = 0; i < p; i++)
        if (coeff[i][maxind[i]] < 0)
            for (j = 0; j < p; j++)
                coeff[i][j] = -coeff[i][j];
}
