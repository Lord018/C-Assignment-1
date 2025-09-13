#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Conversion Functions
void div_convert(uint32_t n, int base, char *out) {
    char temp[33];
    int x = 0;

    if (n == 0) {
        strcpy(out, "0");
        return;
    }

    while (n > 0) {
        int rem = n % base;
        n = n / base;

        if (rem < 10)
            temp[x++] = '0' + rem;
        else
            temp[x++] = 'A' + (rem - 10);
    }

    temp[x] = '\0';

    for (int i = 0; i < x; i++) {
        out[i] = temp[x - 1 - i];
    }
    out[x] = '\0';
}

void sub_convert(uint32_t n, int base, char *out) {
    int exponent = 0;
    int pos = 0;

    while (pow(base, exponent) <= n) {
        exponent++;
    }
    exponent--;

    while (exponent >= 0) {
        int power = pow(base, exponent);
        int digit = 0;

        while (n >= power) {
            n -= power;
            digit++;
        }

        if (digit < 10)
            out[pos++] = '0' + digit;
        else
            out[pos++] = 'A' + digit - 10;

        exponent--;
    }

    out[pos] = '\0';
}

//Binary
void print_binary(uint32_t n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 8 == 0 && i != 0) printf(" ");
    }
}
//table
void print_tables(uint32_t n) {
    printf("Original: %u\n", n);
    printf("  Decimal: %u\n", n);
    printf("  Hex:     0x%X\n", n);
    printf("  Binary:  "); print_binary(n); printf("\n\n");

    uint32_t shifted = n << 3;
    printf("Left shift by 3:\n");
    printf("  Decimal: %u\n", shifted);
    printf("  Hex:     0x%X\n", shifted);
    printf("  Binary:  "); print_binary(shifted); printf("\n\n");

    uint32_t masked = n & 0xFF;
    printf("AND with 0xFF:\n");
    printf("  Decimal: %u\n", masked);
    printf("  Hex:     0x%X\n", masked);
    printf("  Binary:  "); print_binary(masked); printf("\n");
}


//do the thing
int main() {
    FILE *file = fopen("a1_test_file.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }

    char line[256];
    int total_tests = 0;
    int passed_tests = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        char func[20], expected[20];
        uint32_t input, base;

        if (sscanf(line, "%s %u %u %s", func, &input, &base, expected) != 4) {
            printf("Skipping invalid test line: %s\n", line);
            continue;
        }

        char result[20];
        if (strcmp(func, "div_convert") == 0) {
            div_convert(input, base, result);
        } else if (strcmp(func, "sub_convert") == 0) {
            sub_convert(input, base, result);
        } else {
            printf("Unknown function: %s\n", func);
            continue;
        }

        total_tests++;
        if (strcmp(result, expected) == 0) {
            printf("Test %d: %s(%u, %u) -> Expected: \"%s\", Got: \"%s\" [PASS]\n",
                   total_tests, func, input, base, expected, result);
            passed_tests++;
        } else {
            printf("Test %d: %s(%u, %u) -> Expected: \"%s\", Got: \"%s\" [FAIL]\n",
                   total_tests, func, input, base, expected, result);
        }
    }

    fclose(file);
    printf("Summary: %d/%d tests passed\n", passed_tests, total_tests);
    return 0;
}


