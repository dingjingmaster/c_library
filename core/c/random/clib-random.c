/*************************************************************************
> FileName: clib-random.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 14时38分57秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "clib-random.h"

uint64_t _random(uint64_t seed) 
{
  return seed * 3877 + 29573;
}

void random_shuffle(uint64_t seed, uint32_t *array, uint32_t size) 
{
  int i, j;
  uint32_t v;
  uint64_t d = seed;

  for (i = 0; i < size; ++i) {
    d = _random(d);
    j = d % size;
    if (i != j) {
      v = array[i];
      array[i] = array[j];
      array[j] = v;
    }
  }
}



