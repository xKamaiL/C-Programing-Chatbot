#include <stdbool.h>

#ifndef C_PROGRAMING_CHATBOT_CALCULATE_H
#define C_PROGRAMING_CHATBOT_CALCULATE_H

#endif //C_PROGRAMING_CHATBOT_CALCULATE_H

enum operator {Unknown, Plus,Minus,Multiple,Division};

float parser(char*);

float parser(char *text){

    bool negative_number = false;
    enum operator operator_state = Unknown;
    float result = 0,number_state = 0;
    for (int i =0; i < strlen(text);i++) {
        if (text[i] == ' ') continue;
        if(text[i] >= '0' &&  text[i] <= '9') {
            number_state = 10.00*number_state + (text[i] -'0') * 1.0;

            // if is the last one
            if (i == strlen(text) - 1) {
                switch (operator_state) {
                    case Plus:
                        result += number_state;
                        break;
                    case Minus:
                        result -= number_state;
                        break;
                    case Multiple:
                        result *= number_state;
                        break;
                    case Division:
                        // prevent case 0/0
                        if (result == 0 && number_state == 0) {
                            return -1;
                        }
                        result /= number_state;
                        break;
                    default:
                        // nothing
                        break;
                }
            }
            continue;
        }
        // negative number cases
        // 10 + -10
        // -9 * 29
        // then set a number state to minus
        // reset negative flag
        if ((text[i] == '+' || text[i] == '-' || text[i] == '/' || text[i] == '*') && negative_number) {
            number_state = -number_state;
            negative_number = false;
        }


        if (text[i] == '+') {
            operator_state = Plus;
            result += number_state;
            number_state = 0;
            continue;
        }
        if (text[i] == '-') {
            operator_state = Minus;
            // for negative case of number parsing
            // old number state is zero
            if (number_state == 0) {
                negative_number = true;
                // printf("found negative case\n");
                continue;
            }

            result -= number_state;
            number_state = 0;
            continue;
        }

        if (text[i] == '*') {
            operator_state = Multiple;
            if (result == 0) {
                result = number_state;
                number_state = 0;
                continue;
            }
            result *= number_state;
            number_state = 0;
            continue;
        }
        if (text[i] == '/') {
            operator_state = Division;
            if (result == 0) {
                result = number_state;
                number_state =0;
                continue;
            }
            result /= number_state;
            number_state =0;
            continue;
        }
    }

    return result;
}


char *doCalculate(char *);


char *doCalculate(char *text) {
    int digits =0;

    float result = parser(text);

    float n = result;
    do {
        n /= 10;
        ++digits;
    } while (n != 0);

    // allocate number string
    char * numberInText = malloc(sizeof (char *) *( digits + 2 + 10));

    snprintf(numberInText,digits + 2,"%.2f", result);
    char *prefix = malloc(sizeof (char*) * (14));
    strcpy(prefix,"The answer is ");
    strcat(prefix,numberInText);

    return prefix;
}
