#pragma once
#include <stdio.h>
#include "huffman_core.h"

typedef struct DecNode DecNode;

void Decode(FILE *in, FILE *out);
