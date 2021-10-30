#include <stdbool.h>

enum operator {
    Unknown, Plus, Minus, Multiple, Division
};

typedef struct {
    // result of scan
    float result;
    // next index
    int next;
    // previous scan
    int prev;
} scan_result;

scan_result scanNumber(char *text, int start);

// scanNumber
// scan หา แต่ตัวเลข
// return scan_result
scan_result scanNumber(char *text, int start) {
    int i = start;
    float result = 0;
    while (text[i] >= '0' && text[i] <= '9') {
        // add a new number
        // and shift digit of old number
        result = 10.0 * result + (text[i] - '0') * 1.0;
        // save how many index
        i++;
    }
    scan_result r;
    r.result = result;
    r.prev = start;
    // not found
    if (start == i) {
        r.next = start;
    } else {
        // add index of scanned
        // minus -1
        // keep parser i++ work
        r.next = i - 1;
    }
    return r;
}


float parser(char *);

float parser(char *text) {

    enum operator operator_state = Unknown;
    float result = 0;
    for (int i = 0; i < strlen(text); i++) {

        if (text[i] >= '0' && text[i] <= '9') {
            scan_result res = scanNumber(text, i);

            // if never operator before
            // set result to that number
            if (operator_state == Unknown) {
                result = res.result;
            }

            // update current i
            i = res.next;
            continue;
        }


        if (text[i] == '+') {
            operator_state = Plus;
            // scan a next number to operate
            scan_result res = scanNumber(text, i + 1);
            result += res.result;
            i = res.next;
            continue;
        }

        if (text[i] == '-') {
            operator_state = Minus;
            scan_result res = scanNumber(text, i + 1);
            result -= res.result;
            i = res.next;
            continue;
        }

        if (text[i] == '*') {
            operator_state = Multiple;
            scan_result res = scanNumber(text, i + 1);
            result *= res.result;
            i = res.next;
            continue;
        }

        if (text[i] == '/') {
            operator_state = Division;
            scan_result res = scanNumber(text, i + 1);
            result /= res.result;
            i = res.next;
            continue;
        }
    }

    return result;
}


char *doCalculate(char *);


char *doCalculate(char *text) {



    int digits = 0;

    float result = parser(text);

    float n = result;
    do {
        n /= 10;
        ++digits;
    } while (n != 0);

    // allocate number string
    char *numberInText = malloc(sizeof(char *) * (digits + 2 + 10));

    snprintf(numberInText, digits + 2, "%.2f", result);
    char *prefix = malloc(sizeof(char *) * (14));
    strcpy(prefix, "The answer is ");
    strcat(prefix, numberInText);

    return prefix;
}
