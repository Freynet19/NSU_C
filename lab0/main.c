#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 14

void separate (char* x, int len, char* x_int, char* x_frac) {
    int dot, j;
    dot = j = 0;
    for (int i = 0; i < len; ++i) {
        if (dot == 0) {
            if (x[i] == '.') {
                dot = 1;
                continue;
            }
            x_int[i] = x[i];
        }
        else {
            x_frac[j] = x[i];
            ++j;
        }
    }
}

int num2int (const char* nums, char num, int b1) {
    int i;
    for (i = 0; i < b1; ++i) {
        if (nums[i] == num) break;
    }
    return i;
}

long long int2dec (const char* nums, char* x_int, int len_int, int b1) {
    long long dec = 0;
    for (int i = 0; i < len_int; ++i) {
        dec += num2int(nums, x_int[i], b1);
        dec *= b1;
    }
    dec /= b1;
    return dec;
}

void frac2dec (const char* nums, char* x_frac, int len_frac, int b1, long long* nom, long long* denom) {
    for (int i = 0; i < len_frac; ++i) {
        *nom += num2int(nums, x_frac[i], b1);
        *nom *= b1;
        *denom *= b1;
    }
    *nom /= b1;
}

void int2b2 (const char* nums, long long intgr, char* str_int, int b2) {
    char tmp[128] = {0};
    int i = 0;
    while (intgr > 0) {
        tmp[i] = nums[intgr % b2];
        intgr /= b2;
        ++i;
    }
    i = 0;
    for (int j = 127; j >= 0; --j) {
        if (tmp[j] != '\0') {
            str_int[i] = tmp[j];
            ++i;
        }
    }
    if (strlen(str_int) == 0)
        str_int[0] = '0';
}

void frac2b2 (const char* nums, long long nom, long long denom, char* str_frac, int b2) {
    for (int i = 0; i < 12 && nom > 0; ++i) {
        str_frac[i] = nums[(nom * b2) / denom];
        nom = (nom * b2) % denom;
    }
}

void format_inp(char* inp) {
    int len = (int)strlen(inp);
    for (int i = 0; i < len; ++i) {
        inp[i] = tolower(inp[i]);
    }
}

// проверка на соответствие b1-ричной системе счисления
int numsys_ok(char* x, const char* nums, int b1, int len) {
    int dot = 0;
    for (int i = 0; i < len; ++i) {
        int ok = 0;
        for (int j = 0; j < b1; ++j) {
            if (x[i] == '.')
                dot += 1;
            if ((x[i] == '.' && i != 0 && i != len-1 && dot <= 1) || x[i] == nums[j]) {
                ok = 1;
                break;
            }
        }
        if (!ok) {
            printf("bad input\n");
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int b1, b2;
    if (scanf("%d%d", &b1, &b2) != 2 || b1 < 2 || b2 < 2 || b1 > 16 || b2 > 16) {
        printf("bad input\n");
        return 0;
    }

    const char nums[16] = "0123456789abcdef";

    char x[MAX_LEN];
    if (scanf("%13s", x) != 1) {
        printf("bad input\n");
        return 0;
    }
    format_inp(x);
    int len = (int)strlen(x);

    if (!numsys_ok(x, nums, b1, len))
        return 0;

    char x_int[MAX_LEN] = {0}, x_frac[MAX_LEN] = {0};
    separate(x, len, x_int, x_frac);
    int len_int = (int)strlen(x_int);
    int len_frac = (int)strlen(x_frac);

    long long intgr = int2dec(nums, x_int, len_int, b1);
    long long nom = 0, denom = 1;
    frac2dec(nums, x_frac, len_frac, b1, &nom, &denom);

    char str_int[128] = {0}, str_frac[13] = {0};
    int2b2(nums, intgr, str_int, b2);
    frac2b2(nums, nom, denom, str_frac, b2);
    int len_sf = (int)strlen(str_frac);

    printf("%s", str_int);
    if (len_sf > 0)
        printf(".%s", str_frac);
    printf("\n");

    return 0;
}
