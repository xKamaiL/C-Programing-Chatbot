#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "current_timestamp.h"
#include "calculate.h"
#include "teach.h"

#define CANNED_QUESTION_FILE "data/canned_question.txt"
#define CANNED_RESPONSE_FILE "data/canned_response.txt"
#define CONVERSATION_FILE "data/conversation.txt"
#define KEYWORD_FILE "data/keyword_response.tsv"

#define MAX_QUESTIONS 200
#define MAX_QUESTION_LENGTH 50

#define BUFFER_LENGTH 255

typedef struct {
    char *question[MAX_QUESTIONS];
    char *response[MAX_QUESTIONS];
    int size;
} response;

typedef struct {
    char *question[MAX_QUESTIONS];
    char **response[MAX_QUESTIONS];
    int response_count;
    int size;
} multi_response;

//                        _oo0oo_
//                       o8888888o
//                       88" . "88
//                       (| -_- |)
//                       0\  =  /0
//                     ___/`---'\___
//                   .' \|     |// '.
//                  / \|||  :  |||// \
//                 / _||||| -:- |||||- \
//                |   | \\  -  /// |   |
//                | \_|  ''\---/''  |_/ |
//                \  .-\__  '-'  ___/-. /
//              ___'. .'  /--.--\  `. .'___
//           ."" '<  `.___\_<|>_/___.' >' "".
//          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//          \  \ `_.   \_ __\ /__ _/   .-` /  /
//      =====`-.____`.___ \_____/___.-`___.-'=====
//                        `=---='

// function prototypes
response load_canned_questions_responses(void);

response load_conversations_responses(void);

multi_response load_keywords_responses(void);

char *canned_response(char *question, response canned, response conversation);

char *keyword_response(char *question, multi_response keyword);

char *yes_no_response(char *input_text);

char *reflecting(char *input_text);

char *give_up(void);

char *select_response(char *input_text, response canned, response conversation, multi_response keyword);

char *banner[5] = {
        ".d88b.                          .d88b 8           w   8           w   ",
        "YPwww. 8   8 88b. .d88b 8d8b    8P    8d8b. .d88 w8ww 88b. .d8b. w8ww ",
        "    d8 8b d8 8  8 8.dP' 8P      8b    8P Y8 8  8  8   8  8 8' .8  8   ",
        "`Y88P' `Y8P8 88P' `Y88P 8       `Y88P 8   8 `Y88  Y8P 88P' `Y8P'  Y8P ",
        "             8                                                        "
};

// main function
int main(void) {

    response canned,           // canned questions/responses
    conversation;     // conversation questions/responses
    multi_response keyword;          // keyword questions/responses

    // load questions/responses files
    canned = load_canned_questions_responses();
    conversation = load_conversations_responses();
    keyword = load_keywords_responses();
    char input_text[MAX_QUESTION_LENGTH];

    printf("\n");
    for (int i = 0; i < 5; i++) {
        printf("%s\n", banner[i]);
    }

    //run chat bot
    while (1) {
        printf("\n>>> ");
        fgets(input_text, MAX_QUESTION_LENGTH, stdin);
        input_text[strcspn(input_text, "\n")] = 0;  // trim newline
        if (strcmp(input_text, "bye") == 0) {
            printf("Good bye!\n");
            break;
        } else {
            printf("%s\n", select_response(input_text, canned, conversation, keyword));
        }
    }
    return 0;
}

response load_canned_questions_responses(void) {

    FILE *fp;
    char buffer[BUFFER_LENGTH];
    response res = {.size =0};

    fp = fopen(CANNED_QUESTION_FILE, "r");

    for (int i = 0; fgets(buffer, BUFFER_LENGTH, fp); i++) {
        buffer[strcspn(buffer, "\n")] = 0;  // trim newline
        res.question[i] = (char *) malloc(strlen(buffer));
        strcpy(res.question[i], buffer);
        res.size++;
    }
    fclose(fp);

    fp = fopen(CANNED_RESPONSE_FILE, "r");

    for (int i = 0; fgets(buffer, BUFFER_LENGTH, fp); i++) {
        buffer[strcspn(buffer, "\n")] = 0;
        res.response[i] = (char *) malloc(strlen(buffer));
        strcpy(res.response[i], buffer);
    }

    fclose(fp);

    return res;
}

response load_conversations_responses(void) {
    FILE *fp;
    char buffer[BUFFER_LENGTH];

    response output = {.size =0};

    fp = fopen(CONVERSATION_FILE, "r");

    for (int i = 0; fgets(buffer, BUFFER_LENGTH, fp); i++) {

        buffer[strcspn(buffer, "\n")] = 0;

        if ((i) % 2 == 0) {
            output.question[output.size] = (char *) malloc(strlen(buffer));
            strcpy(output.question[output.size], buffer);
            // printf("Q: %s %d\n", output.question[output.size], output.size);
            output.size++;
        } else {
            output.response[output.size - 1] = (char *) malloc(strlen(buffer));
            strcpy(output.response[output.size - 1], buffer);
            // printf("A: %s %d\n", output.response[output.size-1], output.size-1);
        }

    }

    printf("[INFO]: load_conversations_responses size = %d\n", output.size);

    fclose(fp);

    return output;

}

multi_response load_keywords_responses(void) {
    FILE *fp;
    char buffer[BUFFER_LENGTH];
    multi_response res;
    res.size = 0;
    fp = fopen(KEYWORD_FILE, "rw");
    for (int i = 0; fgets(buffer, BUFFER_LENGTH, fp); i++) {
        buffer[strcspn(buffer, "\n")] = 0;
        char *token = strtok(buffer, "\t");
        if (token == NULL) {
            printf("wrong format of tsv");
            break;
        }

        res.question[i] = (char *) malloc(strlen(token));
        strcpy(res.question[i], token);
        res.size++;
        token = strtok(NULL, "\t");
        if (token == NULL) {
            printf("wrong format of tsv\n");
            break;
        }


        int j = 0;
        while (token != NULL) {
            res.response[i] = realloc(res.response[i], (j + 1) * sizeof(char *));
            res.response[i][j] = malloc((strlen(token)) * sizeof(char));
            strcpy(res.response[i][j], token);
            token = strtok(NULL, "\t");
            j++;
        }
        res.response_count = j + 1;

    }
    printf("[INFO]: load_keywords_responses.size = %d", res.size);
    return res;

}

char *canned_response(char *question, response canned, response conversation) {
    for (int i = 0; i < canned.size; i++) {
        if (strcmp(question, canned.question[i]) == 0) {
            return canned.response[i];
        }
    }

    for (int i = 0; i < conversation.size; i++) {
        if (strcmp(question, conversation.question[i]) == 0) {
            return conversation.response[i];
        }
    }

    return NULL;
}

char *findKeyword(char *, multi_response);

char *findKeyword(char *word, multi_response data) {
    // set seed
    srand(time(NULL));
    // loop all keywords
    for (int i = 0; i < data.size; i++) {
        // compare word vs question
        if (strcmp(word, data.question[i]) == 0) {
            // random a response in response_count
            int r = rand() % data.response_count;
            // return a response with random index
            return data.response[i][r];
        }
    }
    return NULL;
}

char *keyword_response(char *question, multi_response keyword) {
    // create a temp string for strtok will not modify an original string.
    char *tempstr = calloc(strlen(question) + 1, sizeof(char));
    strcpy(tempstr, question);



    // split a question into word

    char *token = strtok(tempstr, " ");
    while (token != NULL) {
        // find a keyword
        char *r = findKeyword(token, keyword);
        if (r != NULL) {
            return r;
        }
        // split next word
        token = strtok(NULL, " ");
    }

    return NULL;
}

char *yes_no_response(char *input_text) {
    // set seed
    srand(time(NULL));

    // create a yes no array
    char *yes_or_no[] = {
            "Yes",
            "No",
    };

    // verbs for checking
    char *verbs[] = {"Do", "Are", "Have", "Will", "Would", "Should", "Can", "Could"};

    size_t len = sizeof(verbs) / sizeof(verbs[0]);

    // loop check
    for (int i = 0; i < len; i++) {
        // check string is start with verb[i] or not
        // and string end with ?
        if (hasPrefix(verbs[i], input_text) && hasSuffix(input_text, "?")) {
            // random yes no
            int r = rand() % (sizeof(yes_or_no) / sizeof(char *));
            // random answer yes or no
            char *answer = yes_or_no[r];

            // split a first word
            char *token = strtok(input_text, " ");

            // split next 2 word from input_text
            char *pronous = strtok(NULL, " ");

            // if no pro nous just answer only yes or no
            if (pronous == NULL) {
                return answer;
            }
            // split a nous
            // if there are no nous just exit
            token = strtok(NULL, " ");
            if (token == NULL) {
                return answer;
            }

            // approximate new message size
            int newSize = strlen(answer) + strlen(token) + 50;
            // Allocate new message string
            char *message = (char *) malloc(newSize);

            // copy Yes , No into message
            strcpy(message, answer);
            // add ,
            strcat(message, ", ");

            // compare pronouns i and you
            if (strcmp(toLower(pronous), "i") == 0) {
                strcat(message, "You");
            } else if (strcmp(toLower(pronous), "you") == 0) {
                strcat(message, "I");
            } else {
                strcat(message, pronous);
            }

            // append a word by i
            switch (i) {
                case 0:
                    strcat(message, " do");
                    break;
                case 1:
                    strcat(message, " are");
                    break;
                case 2:
                    strcat(message, " have");
                    break;
                case 3:
                    strcat(message, " will");
                    break;
                case 4:
                    strcat(message, " would");
                    break;
                case 5:
                    strcat(message, " should");
                    break;
                case 6:
                    strcat(message, " can");
                    break;
                case 7:
                    strcat(message, " could");
                    break;
                default:
                    break;
            }

            // answer is not then add "n't"
            if (r == 1) {
                strcat(message, "n't ");
            } else {
                strcat(message, " ");
            }


            // add a nous
            strcat(message, token);
            // full stop
            strcat(message, ".");
            return message;
        }
    }

    return NULL;
}

// reflective ต้องสังเกตจาก pattern ของคำถามครับที่แน่นอน
// เช่น คำถาม "you kill my dog" จับ pattern ได้ว่า you [verb] my [something].
// ตอบเป็น pattern เหมือนกันเช่น Do you really think I [verb] your [something] =
// "Do you really think I kill your dog?".
char *reflecting(char *input_text) {
    char *str = calloc(strlen(input_text) + 1,1);
    strcpy(str, input_text);
    int j = 0;

    if (!hasPrefix("you",toLower(str))) return NULL;

    char *token = strtok(str, " ");
    char * verb = malloc(sizeof (char*));
    char * object = malloc(sizeof (char*));
    while (token != NULL) {
        if (j == 1) {
            verb = realloc(verb, 1+strlen(token));
            strcpy(verb,token);
        }
        if (j == 3) {
            object = realloc(object, 1+strlen(token));
            strcpy(object,token);
        }
        if (j> 3) break;
        j++;
        token = strtok(NULL," ");
    }

    if (j >= 3) {
        char * message = malloc(20);
        strcpy(message,"Do you really think ");

        strcat(message,"I ");
        message = realloc(message,21 + strlen(verb));
        strcat(message,verb);

        if (j > 3) {
            strcat(message," your ");
            message = realloc(message,21 + strlen(object));
            strcat(message,object);
        }else {
            strcat(message," you");
        }
        strcat(message,"?");

        return message;
    }

    return NULL;
}

char *give_up(void) {

    srand(time(NULL));

    char *give_up_response[] = {
            "I heard you!",
            "So, you are talking to me.",
            "Continue, I'm listening.",
            "Very interesting conversation.",
            "Tell me more..."
    };

    return give_up_response[rand() % (sizeof(give_up_response) / sizeof(char *))];
}


// priority of functions:
// 1.canned + conversation [1]
// 2.yes or no
// 3.keyword
// 4.reflecting
// 5.give up
char *select_response(char *input_text, response canned, response conversation, multi_response keyword) {

    if (strstr(input_text, "+") != NULL || strstr(input_text, "-") != NULL || strchr(input_text, '/') != NULL ||
        strstr(input_text, "*") != NULL) {
        return doCalculate(input_text);
    }

    char *time_response = isDateTimeQuestion(input_text);
    if (time_response != NULL) {
        return time_response;
    }
    char *selected_response = NULL;

    selected_response = canned_response(input_text, canned, conversation);
    if (selected_response == NULL) {
        selected_response = yes_no_response(input_text);
        if (selected_response == NULL) {
            selected_response = keyword_response(input_text, keyword);
            if (selected_response == NULL) {
                selected_response = reflecting(input_text);
                if (selected_response == NULL) {
                    selected_response = give_up();
                }
            }
        }
    }

    return selected_response;
}


