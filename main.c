#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CANNED_QUESTION_FILE "data/canned_question.txt"
#define CANNED_RESPONSE_FILE "data/canned_response.txt"
#define CONVERSATION_FILE "data/conversation.txt"
#define KEYWORD_FILE "data/keyword_response.tsv"

#define MAX_QUESTIONS 200
#define MAX_QUESTION_LENGTH 50

#define BUFFER_LENGTH 255

typedef struct {
  char* question[MAX_QUESTIONS];
  char* response[MAX_QUESTIONS];
  int size;
} response;

// function prototypes
response load_canned_questions_responses(void);
response load_conversations_responses(void);
response load_keywords_responses(void);
char* canned_response(char* input_text, response canned, response conversation);
char* keyword_response(char*  input_text, response keyword);
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
        printf("%s\n",select_response(input_text, canned, conversation, keyword));
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

  // TODO: read the response file and keep line by line text the in res.response[i].
  // YOUR CODE HERE

  return res;

}

response load_conversations_responses(void){

  FILE *fp;
  char buffer[BUFFER_LENGTH];
  response res;
  // TODO: read conversation file and return in response
  // YOUR CODE HERE

  return res;

}

response load_keywords_responses(void){

  FILE *fp;
  char buffer[BUFFER_LENGTH];
  response res;

  // TODO: read keyword file and return in response
  // YOUR CODE HERE

  return res;
}

char* canned_response(char* question, response canned, response conversation){
  for(int i=0; i<canned.size;i++){
    if(strcmp(question,canned.question[i])==0){
      return canned.response[i];
    }
  }

  return NULL;
}

char* keyword_response(char* question, response keyword){

  // TODO: response the question with a keyword found in input text.
  // YOUR CODE HERE

  return NULL;
}

char* yes_no_response(char* input_text){
  char* verbs[] = {"Do", "Are", "Have", "Will", "Would", "Should", "Can", "Could"};

  // TODO: response the question with a Yes or No based on the verbs of closed questions.
  // YOUR CODE HERE
  
  return NULL;
}

char* reflecting(char* input_text){
  return NULL;
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
  // 1.canned + conversation
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


