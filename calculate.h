#ifndef C_PROGRAMING_CHATBOT_CALCULATE_H
#define C_PROGRAMING_CHATBOT_CALCULATE_H

#endif //C_PROGRAMING_CHATBOT_CALCULATE_H

enum operator {Unknown, Plus,Minus,Multiple,Division};

char *doCalculate(char *);


char *doCalculate(char *text) {
    int digits =0;
    enum operator operator_state = Unknown;
    float result = 0,number_state = 0;

    for (int i =0; i < strlen(text);i++) {
        if (text[i] == ' ') continue;
        if(text[i] >= '0' &&  text[i] <= '9') {
            number_state = 10.00*number_state + (text[i] -'0') * 1.0;

            // if is the last one
            if (i == strlen(text) - 1) {
                printf("process last one\n");
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
                        result /= number_state;
                        break;
                    default:
                        // nothing
                        break;
                }
            }
            continue;
        }

        if (text[i] == '+') {
            operator_state = Plus;
            result += number_state;
            printf("do plus: %2.f %2.f\n",result,number_state);
            number_state = 0;
            continue;
        }
        if (text[i] == '-') {
            printf("do minus\n");
            operator_state = Minus;
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
