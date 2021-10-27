#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#define CANNED_QUESTION_FILE "data/canned_question.txt"
#define CANNED_RESPONSE_FILE "data/canned_response.txt"
#define CONVERSATION_FILE "data/conversation.txt"
#define KEYWORD_FILE "data/keyword_response.tsv"

#define MAX_QUESTIONS 200
#define MAX_QUESTION_LENGTH 50
#define MAX_ANSWER 5

#define BUFFER_LENGTH 255

typedef struct {
    char* question[MAX_QUESTIONS];
    char* response[MAX_QUESTIONS];
    int size;
} response;

typedef struct {
    char* question[MAX_QUESTIONS];
    char* response[MAX_QUESTIONS][MAX_ANSWER];
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
response load_keywords_responses(void);
char* canned_response(char* question, response canned, response conversation);
char* keyword_response(char*  question, response keyword);
char* yes_no_response(char*  input_text);
char* reflecting(char*  input_text);
char* give_up(void);
char* select_response(char*  input_text, response canned, response conversation, response keyword);

char* banner[5] = {
        ".d88b.                          .d88b 8           w   8           w   ",
        "YPwww. 8   8 88b. .d88b 8d8b    8P    8d8b. .d88 w8ww 88b. .d8b. w8ww ",
        "    d8 8b d8 8  8 8.dP' 8P      8b    8P Y8 8  8  8   8  8 8' .8  8   ",
        "`Y88P' `Y8P8 88P' `Y88P 8       `Y88P 8   8 `Y88  Y8P 88P' `Y8P'  Y8P ",
        "             8                                                        "
};

// main function
int main(void) {

    response  canned,           // canned questions/responses
    conversation,     // conversation questions/responses
    keyword;          // keyword questions/responses

    // load questions/responses files
    canned = load_canned_questions_responses();
    conversation = load_conversations_responses();
    keyword = load_keywords_responses();
    char input_text[MAX_QUESTION_LENGTH];
    char* response_text;

    printf("\n");
    for(int i=0; i<5; i++){
        printf("%s\n",banner[i]);
    }

    //run chatbot
    while(1){
        printf("\n>>> ");
        fgets(input_text, MAX_QUESTION_LENGTH, stdin);
        input_text[strcspn(input_text, "\n")] = 0;  // trim newline
        if(strcmp(input_text,"bye")==0){
            printf("Good bye!\n");
            break;
        }else{
            printf("\n%s\n",select_response(input_text, canned, conversation, keyword));
        }
    }
    return 0;
}

response load_canned_questions_responses(void){

    FILE *fp;
    char buffer[BUFFER_LENGTH];
    response res = {.size =0 };

    fp = fopen(CANNED_QUESTION_FILE, "r");

    for(int i=0;fgets(buffer, BUFFER_LENGTH, fp);i++) {
        buffer[strcspn(buffer, "\n")] = 0;  // trim newline
        res.question[i] = (char*) malloc(strlen(buffer));
        strcpy(res.question[i],buffer);
        res.size++;
    }
    fclose(fp);

    fp = fopen(CANNED_RESPONSE_FILE, "r");

    for(int i=0;fgets(buffer, BUFFER_LENGTH, fp);i++) {
      buffer[strcspn(buffer, "\n")] = 0;
      res.response[i] = (char*) malloc(strlen(buffer));
      strcpy(res.response[i], buffer);
    }

    fclose(fp);

    return res;
}

response load_conversations_responses(void){
    FILE *fp;
    char buffer[BUFFER_LENGTH];
    
    response output = { .size =0 };
    
    fp = fopen(CONVERSATION_FILE, "r");

    for(int i=0; fgets(buffer, BUFFER_LENGTH, fp);i++) {

        buffer[strcspn(buffer, "\n")] = 0;

        if ((i) % 2 == 0) {
          output.question[output.size] = (char*) malloc(strlen(buffer));
          strcpy(output.question[output.size], buffer);
          // printf("Q: %s %d\n", output.question[output.size], output.size);
          output.size++;
        } else {
          output.response[output.size-1] = (char*) malloc(strlen(buffer));
          strcpy(output.response[output.size-1], buffer);
          // printf("A: %s %d\n", output.response[output.size-1], output.size-1);
        }
        
    }

    printf("[INFO]: load_conversations_responses size = %d\n", output.size);

    fclose(fp);

    return output;

}

response load_keywords_responses(void){
    FILE *fp;
    char buffer[BUFFER_LENGTH];
    response res;
    res.size = 0;
    fp = fopen(KEYWORD_FILE,"rw");
    for(int i=0; fgets(buffer, BUFFER_LENGTH, fp);i++) {
        buffer[strcspn(buffer, "\n")] = 0;
        char* token = strtok(buffer, "\t");
        if (token == NULL) {
            printf("wrong format of tsv");
            break;
        }
        res.question[i] = (char*) malloc(strlen(token));
        strcpy(res.question[i],token);
        char* split = strtok(NULL,"\t");
        if (split == NULL) {
            printf("some row in .tsv file is no value");
            break;
        }
        res.response[i] = (char*) malloc(strlen(split));
        strcpy(res.response[i],split);
        res.size++;
    }
    printf("[INFO]: load_keywords_responses.size = %d",res.size);
    return res;

}

char* canned_response(char* question, response canned, response conversation){
    for(int i=0; i < canned.size; i++){
        if(strcmp(question,canned.question[i])==0){
            return canned.response[i];
        }
    }

    for (int i =0; i < conversation.size; i++) {
      if(strcmp(question, conversation.question[i])==0){
        return conversation.response[i];
      }
    }

    return NULL;
}

char* keyword_response(char* question, response keyword){

    for (int i =0; i< keyword.size;i++) {
        if (strcmp(question, keyword.question[i]) == 0){
            return keyword.response[i];
        }
    }

    return NULL;
}

char* yes_no_response(char* input_text){
    // set seed
    srand(time(NULL));

    char *yes_or_no[] = {
            "Yes",
            "No",
    };

    char* verbs[] = {"Do", "Are", "Have", "Will", "Would", "Should", "Can", "Could"};

    size_t len =  sizeof(verbs) / sizeof(verbs[0]);
    // loop check
    for (int i =0; i <len ; i++) {
      // check string is start with verb[i] or not ?
      if (hasPrefix(verbs[i],input_text) && hasSuffix(input_text,"?")) {
        int r = rand()%(sizeof(yes_or_no)/sizeof(char*));
        // random answer yes or no 
        char *answer = yes_or_no[r];
        
        // split word
        char* token = strtok(input_text," ");

        // split next 2 word from input_text
        char* pronous = strtok(NULL," ");

        if (pronous == NULL) {
          return answer;
        }
        // if there are no nous just exit
        token = strtok(NULL," ");
        if (token == NULL) {
          return answer;
        }

        // approximate new message size
        int newSize = strlen(answer) +  strlen(token) + 50;
        // Allocate new message string
        char * message = (char *)malloc(newSize);

        strcpy(message, answer);

        strcat(message,", ");

        if (strcmp(toLower(pronous), "i") == 0) {
          strcat(message, "You");
        }else if (strcmp(toLower(pronous), "you") == 0) {
          strcat(message, "I");
        }else{
          strcat(message, pronous);
        }

        // append a word
        switch(i){
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
        }

        // answer is not 
        if (r == 1) {
          strcat(message,"n't ");
        }else {
          strcat(message," ");
        }


        // append nous        
        strcat(message, token);
        
        strcat(message, ".");


        return message;
      }
    }

    return NULL;
}

char* reflecting(char* input_text){
    return input_text;
}

char* give_up(void){

    srand(time(NULL));

    char* give_up_response[] = {
            "I heard you!",
            "So, you are talking to me.",
            "Continue, I'm listening.",
            "Very interesting conversation.",
            "Tell me more..."
    };

    return give_up_response[rand()%(sizeof(give_up_response)/sizeof(char*))];
}

char* select_response(char* input_text, response canned, response conversation, response keyword){

    // priority of functions:
    // 1.canned + conversation [1]
    // 2.yes or no
    // 3.keyword
    // 4.reflecting
    // 5.give up

    char* selected_response = NULL;

    selected_response = canned_response(input_text, canned, conversation);
    if (selected_response == NULL){
        
        selected_response = yes_no_response(input_text);
        if (selected_response == NULL){
       
            selected_response = keyword_response(input_text, keyword);
            if (selected_response == NULL){
                selected_response = reflecting(input_text);
                if (selected_response == NULL){
                    selected_response = give_up();
                }
            }
        }
    }
    
    return selected_response;
}


