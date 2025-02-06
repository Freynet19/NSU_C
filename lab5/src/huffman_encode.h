#pragma once
#include <stdio.h>
#include "huffman_core.h"

typedef struct HTNode HTNode;
typedef struct QNode QNode;
typedef struct PriorQueue PriorQueue;
typedef struct HCode HCode;

void Encode(FILE *in, FILE *out);
