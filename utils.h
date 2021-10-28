

int hasPrefix(const char *pre, const char *str);

int hasPrefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int hasSuffix(const char *str, const char *suffix);

int hasSuffix(const char *str, const char *suffix)
{
    if (!str || !suffix){
      return 0;
    }
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    
    if (lensuffix >  lenstr){
      return 0;
    }
    // ขยับ ไป ตัวสุดท้าย แล้ว ถอยหลังกับมา lensuffix 
    // จะได้ คำหลังสุดของ string
    size_t offset = lenstr - lensuffix;

    return strncmp(str + offset, suffix, lensuffix) == 0;
}

// int hasSuffix(const char *str) { return EndsWith(str, ".foo"); }

char* toLower(char* string);

char* toLower(char* s)
{
    char* string = malloc(strlen(s));
    strcpy(string,s);
    int i=0;
    while(*(string+i)!= '\0'){
        if (*(string+i) >= 'A' && *(string+i)<='Z'){
            *(string+i)= *(string+i)+32;
        }
        i++;
    }
    
    return string;
}