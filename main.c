#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int *prob;
int *aliases;
int prob_size;
int ratio = 10000;

void make_table(int *orgin_prob, int size)
{
    prob_size = size;
    prob = malloc(sizeof(int)*size);
    aliases = malloc(sizeof(int)*size);
    int *small = malloc(sizeof(int)*size);
    int *large = malloc(sizeof(int)*size);

    int small_num = 0, large_num = 0;

    for (int i = 0; i < size; ++i) {
        prob[i] = orgin_prob[i] * size;
        if (prob[i] > ratio) {
             large[large_num++] = i;
         } else if (prob[i] < ratio) {
             small[small_num++] = i;
         }
    }

    while (small_num > 0 && large_num > 0) {
        int p = prob[small[--small_num]] + prob[large[--large_num]] - ratio;
        prob[large[large_num]] = p;
        aliases[small[small_num]] = large[large_num];

        if (p >= ratio) {
            large_num++;
        } else if (p < ratio) {
            small[small_num++] = large[large_num];
        }
    }

    for (int i = 0; i < large_num; ++i) {
        aliases[large[i]] = large[i];
        prob[large[i]] = ratio;
    }
    for (int i = 0; i < small_num; ++i) {
        aliases[small[i]] = small[i];
        prob[small[i]] = ratio;
    }

    free(small);
    free(large);

//    for (int l = 0; l < size; ++l) {
//        printf("prob : %f\n", prob[l]);
//    }
//    for (int l = 0; l < size; ++l) {
//        printf("aliases : %d\n", aliases[l]);
//    }
}

int lottery()
{
    int idx = rand() % prob_size;
    if ((rand() % ratio + 1) > prob[idx]) {
        idx = aliases[idx];
    }
    return idx;
}

void lottery_free()
{
    free(prob);
    free(aliases);
}

int main()
{
    srand ((unsigned)time(NULL));
    int size = 4;
    int *prob = malloc(sizeof(int)*size);
    int *aliases;
    prob[0] = 10;
    prob[1] = 3000;
    prob[2] = 5000;
    prob[3] = 1500;

    make_table(prob, size);
    int *get = calloc(sizeof(int), size);

    for (int i = 0; i < 10000; ++i) {
        int idx = lottery(size);
        get[idx]++;
    }

    for (int j = 0; j < size; ++j) {
        printf("%d -> %d\n", j, get[j]);
    }
}