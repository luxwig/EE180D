#ifndef RANGER_H
#define RANGER_H

#ifdef __cplusplus
extern "C" {
#endif
    int exec_random_forest(double*, int, int, const char*, double*);
    int train_random_forest(double*, int, int, int, const char*);
#ifdef __cplusplus
}
#endif

#endif