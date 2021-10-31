#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "current_timestamp.h"
#include "calculate.h"

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
    int response_count[MAX_QUESTIONS];
    int size;
} multi_response;



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
        "======================================================================",
        "=                              Welcome to                            =",
        "=                               Chat bot                             =",
        "=                           By Pakorn Sangpeth                       =",
        "======================================================================"
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
        input_text[strcspn(input_text, "\r")] = 0; // trim new line
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
    printf("[INFO] load_canned_questions_responses.size = %d\n",res.size);
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
    // สร้าง string buffer * 5 เท่า
    char buffer[BUFFER_LENGTH * 5] ;
    // multi_response initial size to zero
    multi_response res = {.size = 0};
    // open file with read mode
    fp = fopen(KEYWORD_FILE, "r");

    // read file line by line until EOF
    for (int i = 0; fgets(buffer, BUFFER_LENGTH * 5, fp); i++) {
        // find length of string expect \n
        // remove \n out
        buffer[strcspn(buffer, "\n")] = 0;

        // split text by tab
        char *token = strtok(buffer, "\t");
        // prevent no first column of text
        if (token == NULL) {
            printf("wrong format of tsv");
            break;
        }

        // allocate question string
        res.question[i] = (char *) malloc(strlen(token));
        // copy question string (token) into res.question
        strcpy(res.question[i], token);
        // increase size
        res.size++;

        // split a first response string
        token = strtok(NULL, "\t");
        // prevent no response string ( 2 column )
        if (token == NULL) {
            printf("wrong format of tsv\n");
            break;
        }

        // split a response by limit at 5 response
        int j = 0;
        while (token != NULL && j < 5) {
            // re allocate memory to increase capacity of response array
            res.response[i] = realloc(res.response[i], (j + 1) * sizeof(char *));

            // allocate response number j by length of token
            res.response[i][j] = malloc((strlen(token)) * sizeof(char));
            // copy token ( response number j )
            strcpy(res.response[i][j], token);

            j++;
            // find the next response
            token = strtok(NULL, "\t");
            // increase counter
        }
        // set response_count into j + 1
        res.response_count[i] = j + 1;

    }
    // logging a res.size
    printf("[INFO]: load_keywords_responses.size = %d", res.size);
    return res;

}

char *canned_response(char *question, response canned, response conversation) {
    // loop all canned responses
    for (int i = 0; i < canned.size; i++) {
        // compare question with canned question
        if (strcmp(question, canned.question[i]) == 0) {
            // response back to user
            return canned.response[i];
        }
    }

    // loop all conversation responses
    for (int i = 0; i < conversation.size; i++) {
        // compare question with conversation question
        if (strcmp(question, conversation.question[i]) == 0) {
            // response back to user
            return conversation.response[i];
        }
    }

    return NULL;
}

char *findKeyword(char *, multi_response);

// findKeyword receive a word string
// and return with random response from multi_response
char *findKeyword(char *word, multi_response data) {
    // set seed
    srand(time(NULL));


    // loop all keywords
    for (int i = 0; i < data.size; i++) {
        // compare word vs question
        if (strcmp(word, data.question[i]) == 0) {
            // random a response in response_count
            int r = rand() % data.response_count[i];
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

    // split a question into word by space
    char *token = strtok(tempstr, " ");
    while (token != NULL) {
        // find a keyword
        char *r = findKeyword(token, keyword);
        // found keyword response
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

    // loop check all verbs array
    for (int i = 0; i < len; i++) {
        // check string is start with verb[i]
        // and string end with ? or not
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

char *reflecting(char *input_text) {
    // create a temp string for strtok
    char *str = calloc(strlen(input_text) + 1,1);
    // copy original input_text
    strcpy(str, input_text);

    // check string is start with "you"
    // if not exit this function
    if (!hasPrefix("you",toLower(str))) return NULL;

    // split the string by space into word
    char *token = strtok(str, " ");

    // malloc verb and object
    char * verb = malloc(sizeof (char*));
    char * object = malloc(sizeof (char*));

    // initial counter = 0
    int j = 0;
    // loop token ( split text until text is NULL )
    while (token != NULL) {
        // j == 1 is verb
        if (j == 1) {
            // re allocate verb string by length of token ( verb string )
            verb = realloc(verb, 1+strlen(token));
            // copy to verb string
            strcpy(verb,token);
        }
        // j == 3 is object
        if (j == 3) {
            // re allocate object string by length of token ( object string )
            object = realloc(object, 1+strlen(token));
            // copy to object string
            strcpy(object,token);
        }
        // exit if too much word
        if (j> 3) break;
        // increase counter
        j++;
        // split next word
        token = strtok(NULL," ");
    }

    // have verb or verb and subject
    if (j >= 3) {
        // allocate for new string
        char * message = malloc(30);
        // pattern
        strcpy(message,"Do you really think ");

        strcat(message,"I ");
        // re allocate for verb string
        message = realloc(message,sizeof (message) + strlen(verb));
        // append message with verb
        strcat(message,verb);

        // has object
        if (j > 3) {
            strcat(message," your ");
            message = realloc(message,sizeof (message) + strlen(object));
            strcat(message,object);
        }else {
            // no object
            strcat(message," you");
        }
        // end up with "?"
        strcat(message,"?");

        return message;
    }

    return NULL;
}

char *give_up(void) {
    // set seed for random
    srand(time(NULL));

    // pattern of give up response
    char *give_up_response[] = {
            "I heard you!",
            "So, you are talking to me.",
            "Continue, I'm listening.",
            "Very interesting conversation.",
            "Tell me more..."
    };
    // random by size of give_up_response array
    return give_up_response[rand() % (sizeof(give_up_response) / sizeof(char *))];
}


// priority of functions:
// 1.canned + conversation [1]
// 2.yes or no
// 3.keyword
// 4.reflecting
// 5.give up
char *select_response(char *input_text, response canned, response conversation, multi_response keyword) {
    // input_text contains (+ - / *)
    // we will go to doCalculate
    if (strstr(input_text, "+") != NULL || strstr(input_text, "-") != NULL || strchr(input_text, '/') != NULL ||
        strstr(input_text, "*") != NULL) {
        return doCalculate(input_text);
    }


    // check is  date time question
    char *time_response = isDateTimeQuestion(input_text);
    if (time_response != NULL) {
        return time_response;
    }
    char *selected_response = canned_response(input_text, canned, conversation);
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


