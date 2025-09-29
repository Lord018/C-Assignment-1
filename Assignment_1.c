#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Conversion Functions
/*void div_convert(uint32_t n, int base, char *out) {
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
}*/

/*void sub_convert(uint32_t n, int base, char *out) {
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
}*/

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

void oct_to_bin (const char *oct, char *out) { //the second value is just what it's using to check your stuff
    //printf("%s", oct);
    int i=0;
    int k = 0;
    char buff[33];
        while (oct[i]!='\0') {
            int digit = oct[i]-'0';
            for (int j = 2; j >= 0; j--) {
                out[k++] = ((digit >> j) & 1) + '0';
            }
            i++;
        }
    out[k] = '\0';
    };

/*
 Get the octal ----------------- char *oct, * is a pointer which makes sure the function doesn't just pick 1 (I think)
 Divide the digits evenly into an array (seemingly already done)
    Use number's length to determine how big the array is and the loop count
        Do the division method to check length
    Use the mod|division trick loop to chop the int into the array digit by digit (It'll be reversed)
        Make sure to do division by 8 instead of 10

 check the octal-binary division chart to turn each number into it's binary equivalent


 */

void hex_to_bin(const char *hex, char *out) {
    int i = 0, k = 0;

    while (hex[i] != '\0') {
        int digit;

        if (hex[i] >= '0' && hex[i] <= '9')
            digit = hex[i] - '0';
        else if (hex[i] >= 'A' && hex[i] <= 'F')
            digit = hex[i] - 'A' + 10;
        else if (hex[i] >= 'a' && hex[i] <= 'f')
            digit = hex[i] - 'a' + 10;
        else
            digit = 0;

        for (int j = 3; j >= 0; j--)
            out[k++] = ((digit >> j) & 1) + '0';

        i++;
    }

    out[k] = '\0';
}

void oct_to_hex(const char *oct, char *out) {
    char bin[65];
    oct_to_bin(oct, bin);

    int len = strlen(bin);
    int pad = (4 - (len % 4)) % 4;

    char bin_padded[69];
    int k = 0;


    for (int i = 0; i < pad; i++)
        bin_padded[k++] = '0';


    for (int i = 0; i < len; i++)
        bin_padded[k++] = bin[i];

    bin_padded[k] = '\0';


    int out_index = 0;
    for (int i = 0; i < k; i += 4) {
        int val = 0;
        for (int j = 0; j < 4; j++)
            val = val * 2 + (bin_padded[i + j] - '0');
        out[out_index++] = (val < 10) ? ('0' + val) : ('A' + val - 10);
    }
    out[out_index] = '\0';
}


void to_sign_magnitude(int num1, char *out) {
    unsigned int magnitude = (num1 < 0) ? -num1 : num1;

    for (int i = 31; i >= 1; i--) {
        out[i] = (magnitude & 1) ? '1' : '0';
        magnitude >>= 1;
    }

    out[0] = (num1 < 0) ? '1' : '0';


    out[32] = '\0';
}
//AAAAAA
void to_ones_complement(int num1, char *out) {
    unsigned int val;

    if (num1 >= 0) {
        val = (unsigned int)num1;
    } else {

        val = ~((unsigned int)(-num1));
    }


    for (int i = 31; i >= 0; i--) {
        out[i] = (val & 1) ? '1' : '0';
        val >>= 1;
    }
    out[32] = '\0';
}

//do the thing
int main() {
    FILE *file = fopen("a2_test.txt", "r");
    if (!file) {
        printf("Error: Could not open file\n");
        return 1;
    }

    FILE *outfile = fopen("output.txt", "w");
    if (!outfile) {
        printf("Error: Could not open output.txt for writing\n");
        fclose(file);
        return 1;
    }

    char line[256];
    int total_tests = 0;
    int passed_tests = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // remove newline

        char func[50], expected[65];
        char input_str[65];  // for string-based inputs
        int num_input;       // for integer-based inputs
        int is_integer_test = 0;

        // Determine function type and parse accordingly
        if (strncmp(line, "oct_to_bin", 10) == 0 ||
            strncmp(line, "hex_to_bin", 10) == 0 ||
            strncmp(line, "oct_to_hex", 10) == 0) {
            // String-based functions
            if (sscanf(line, "%s %s %s", func, input_str, expected) != 3) {
                printf("Skipping invalid line: %s\n", line);
                fprintf(outfile, "Skipping invalid line: %s\n", line);
                continue;
            }
        } else {
            // Integer-based functions
            if (sscanf(line, "%s %d %s", func, &num_input, expected) != 3) {
                printf("Skipping invalid line: %s\n", line);
                fprintf(outfile, "Skipping invalid line: %s\n", line);
                continue;
            }
            is_integer_test = 1;
        }

        char result[65]; // buffer for output
        result[0] = '\0';

        // Call the correct function
        if (!is_integer_test) {
            if (strcmp(func, "oct_to_bin") == 0) {
                oct_to_bin(input_str, result);
            } else if (strcmp(func, "hex_to_bin") == 0) {
                hex_to_bin(input_str, result);
            } else if (strcmp(func, "oct_to_hex") == 0) {
                oct_to_hex(input_str, result);
            } else {
                printf("Unknown function: %s\n", func);
                fprintf(outfile, "Unknown function: %s\n", func);
                continue;
            }
        } else {
            if (strcmp(func, "to_sign_magnitude") == 0) {
                to_sign_magnitude(num_input, result);
            }
            else if (strcmp(func, "to_ones_complement") == 0) {
                to_ones_complement(num_input, result);
            }

            else {
                printf("Unknown function: %s\n", func);
                fprintf(outfile, "Unknown function: %s\n", func);
                continue;
            }
        }

        // Compare result
        int pass = strcmp(result, expected) == 0;
        total_tests++;
        if (pass) passed_tests++;

        // Print output
        if (is_integer_test) {
            printf("Test %d: %s(%d) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   total_tests, func, num_input, expected, result, pass ? "PASS" : "FAIL");
            fprintf(outfile, "Test %d: %s(%d) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   total_tests, func, num_input, expected, result, pass ? "PASS" : "FAIL");
        } else {
            printf("Test %d: %s(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   total_tests, func, input_str, expected, result, pass ? "PASS" : "FAIL");
            fprintf(outfile, "Test %d: %s(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   total_tests, func, input_str, expected, result, pass ? "PASS" : "FAIL");
        }
    }

    // Summary
    printf("Summary: %d/%d tests passed\n", passed_tests, total_tests);
    fprintf(outfile, "Summary: %d/%d tests passed\n", passed_tests, total_tests);

    fclose(file);
    fclose(outfile);
    return 0;
}