#ifndef LEMONC_H
#define LEMONC_H

#ifdef __cplusplus
extern "C" {
#endif

void getMatching(int** plist1, int** plist2, int* length, const char* graph_str);
void printArray(char *name, int *a1, int l); 

#ifdef __cplusplus
}
#endif

#endif
