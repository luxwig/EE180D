#ifndef xcode_ranger_main_h
#define xcode_ranger_main_h

#ifdef __cplusplus
extern "C" {
#endif
int exec_random_forest(char*, const char*, double*);
int train_random_forest(char*, int, const char*);
#ifdef __cplusplus
}
#endif

#endif
