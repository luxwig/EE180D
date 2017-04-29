//
//  main.c
//  xcode_ranger
//
//  Created by Ludwig Zhao on 4/28/17.
//  Copyright (c) 2017 Ludwig Zhao. All rights reserved.
//


#include <stdio.h>
#include "ranger.h"
int main(int argc, char **argv) {
    char b[] = "Size TYPE\n1.1 10.1\n2.2 20.2\n3.3 30.3\n2.2 20.2\n1.1 10.1\n2.2 20.2\n1.1 10.1\n3.3 30.3";
    char a[] = "Size TYPE\n1.1 -1\n2.2 -1\n3.3 -1";
    const char* filename = "test";
    train_random_forest(b, 10, filename);
    double result[3];
    exec_random_forest(a, filename, result);
    printf("%lf\t%lf\t%lf\n",result[0],result[1],result[2]);
    return 0;
}
