
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

// if message contains time , date , today, month , year
// bot will answer from system time.
char *isDateTimeQuestion(char *msg) {

    char *text = (char *) malloc(200);

    // get a current time
    time_t t = time(NULL);
    // use a struct from tm.

    //   int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
    //  int tm_min;			/* Minutes.	[0-59] */
    //  int tm_hour;			/* Hours.	[0-23] */
    //  int tm_mday;			/* Day.		[1-31] */
    //  int tm_mon;			/* Month.	[0-11] */
    //  int tm_year;			/* Year	- 1900.  */
    //  int tm_wday;			/* Day of week.	[0-6] */
    //  int tm_yday;			/* Days in year.[0-365]	*/
    //  int tm_isdst;			/* DST.		[-1/0/1]*/
    struct tm tm = *localtime(&t);

    // message contains time
    if (strstr(toLower(msg), "time") != NULL){
        // HH:mm
        snprintf( text, 50, "It is %d:%d.", tm.tm_hour,tm.tm_sec );
        return text;
    }

    // message contain date
    if (strstr(toLower(msg), "date") != NULL) {
        // convert string format YYYY/MM/DD
        snprintf( text, 50, "%d/%d/%d", tm.tm_year+1900, tm.tm_mon, tm.tm_mday );
        return text;
    }

    // message contain today
    if (strstr(toLower(msg), "today") != NULL) {
        strcpy(text, "Today is ");
        strcat(text, dayText[tm.tm_wday]);
        return text;
    }

    // message contain month
    if (strstr(toLower(msg), "month") != NULL) {
        strcpy(text, "Month is ");
        // return month
        strcat(text, monthText[tm.tm_mon]);
        return text;
    }

    // message contain year
    if (strstr(toLower(msg), "year") != NULL) {
        char * year = (char *) malloc(5);
        // convert year to string
        snprintf( year, 5, "%d", tm.tm_year+1900 );
        strcpy(text, "Month is ");
        strcat(text, year);
        return text;
    }

    // free a text memory, no used.
    free(text);
    return NULL;
}