

int hasPrefix(const char *pre, const char *str);

// ตรวจสอบ string ว่า นำหน้าด้วย pre หรือป่าว
int hasPrefix(const char *pre, const char *str)
{
    // compare only first length of prefix string
    return strncmp(pre, str, strlen(pre)) == 0;
}

int hasSuffix(const char *str, const char *suffix);

// ตรวจสอบ string ว่าลงท้ายด้วย suffix หรือป่าว
int hasSuffix(const char *str, const char *suffix)
{

    if (!str || !suffix){
      return 0;
    }

    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);

    // prefix suffix more than string compare
    if (len_suffix >  len_str){
      return 0;
    }

    // ขยับ ไป ตัวสุดท้าย แล้ว ถอยหลังกับมา len_suffix ตัวอักษร
    // จะได้ คำหลังสุดของ string
    size_t offset = len_str - len_suffix;

    // compare ตั้งแต่ str ขยับไป offset ตัว
    return strncmp(str + offset, suffix, len_suffix) == 0;
}

// int hasSuffix(const char *str) { return EndsWith(str, ".foo"); }

char* toLower(char* string);

char* toLower(char* s)
{
    // copy an original string
    // this function will not modify original string
    char* string = malloc(strlen(s));
    strcpy(string,s);
    // set counter to 0
    int i=0;
    // loop value of string pointer + i until \0
    while(*(string+i)!= '\0'){
        // check is uppercase
        if (*(string+i) >= 'A' && *(string+i)<='Z'){
            // convert uppercase to lowercase
            *(string+i)= *(string+i)+32;
        }
        i++;
    }

    // return a new string
    return string;
}