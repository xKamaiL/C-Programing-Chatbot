
#include <time.h>
#include "string.h"
#include "stdio.h"
#include "math.h"

char *dayText[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
char *monthText[12] = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
};

char *isDateTimeQuestion(char *);
// printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

char *isDateTimeQuestion(char *msg) {
    char *text = (char *) malloc(200);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (strstr(toLower(msg), "time") != NULL){
        snprintf( text, 50, "It is %d:%d.", tm.tm_hour,tm.tm_sec );
        return text;
    }

    if (strstr(toLower(msg), "date") != NULL) {
        snprintf( text, 50, "%d/%d/%d", tm.tm_year+1900, tm.tm_mon, tm.tm_mday );
        return text;
    }


    if (strstr(toLower(msg), "today") != NULL) {
        strcpy(text, "Today is ");
        strcat(text, dayText[tm.tm_wday]);
        return text;
    }

    if (strstr(toLower(msg), "month") != NULL) {
        strcpy(text, "Month is ");
        strcat(text, monthText[tm.tm_mon]);
        return text;
    }

    if (strstr(toLower(msg), "year") != NULL) {
        char * year = (char *) malloc(5);
        snprintf( year, 5, "%d", tm.tm_year+1900 );
        strcpy(text, "Month is ");
        strcat(text, year);
        return text;
    }

    free(text);
    return NULL;
}