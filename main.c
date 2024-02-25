#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "tested_declarations.h"
#include "rdebug.h"

int error(int err_code) {
    switch (err_code) {
        case 0:
            return 0;
        case 1:
            printf("Incorrect input"); return 1;
        case 2:
            printf("Incorrect input data"); return 2;
        case 3:
            return 3;
        case 4:
            printf("Couldn't open file"); return 4;
        case 5:
            printf("Couldn't create file"); return 5;
        case 6:
            printf("File corrupted"); return 6;
        case 7:
            printf("Unsupported file format"); return 7;
        case 8:
            printf("Failed to allocate memory"); return 8;
        case 9:
            printf("Not enough arguments"); return 9;
        default:
            return 0;
    }
}

int validate(const char *number);
int multiply(const char* number1, const char* number2, char** result);
int subtract(const char* number1, const char* number2, char** result);
int add(const char* number1, const char* number2, char** result);
int validate_expression(const char *expr);
int calculate(const char* expr, char **result);

void swap(const char** number1,const char** number2);
int smallerOrBigger(const char* number1, const char* number2);

int main(){
    char* expr;
    expr = malloc(sizeof(char) * 500);
    if(expr == NULL)
        return error(8);
    printf("Podaj wyrazenie: ");
    scanf("%499s",expr);
    char* res;
    int x = calculate(expr,&res);
    if(x == 3) {
        free(expr);
        return error(8);
    }
    if(x == 2){
        free(expr);
        return error(1);
    }
    printf("%s", res);
    free(res);
    free(expr);
    return 0;
}



void destroy(char **words){
    if(words == NULL)
        return;
    int i=0;
    while(*(words + i) != NULL) {
        free(*(words + i));
        i++;
    }
    free(words);
}

int validate(const char *number){
    if(number == NULL)
        return -1;
    int i=0;
    if(strlen(number) == 0)
        return 2;
    if((*number == '0' && *(number + 1) != '\0') || (strlen(number) == 1 && isdigit(*number) == 0))
        return 2;
    if(*number == '-')
        i++;
    while (*(number + i) != '\0'){
        if(isdigit(*(number + i)) == 0)
            return 2;
        i++;
    }
    return 0;
}

void swap(const char** number1,const char** number2){
    const char* temp;
    temp = *number1;
    *number1 = *number2;
    *number2 = temp;
}

int add(const char* number1, const char* number2, char** result){
    if(number1 == NULL || number2 == NULL || result == NULL)
        return 1;
    if(validate(number1) != 0 || validate(number2) != 0)
        return 2;
    if(*number1 == '-' && *number2 != '-')
        return subtract(number2,number1+1,result);
    if(*number1 != '-' && *number2 == '-')
        return subtract(number1, number2+1, result);
    int dl1 = strlen(number1);
    int dl2 = strlen(number2);
    if(dl1 < dl2)
        swap(&number1, &number2);
    dl1 = strlen(number1);
    dl2 = strlen(number2);
    *result = malloc(sizeof(char) * (dl1 + 1));
    if(*result == NULL)
        return 3;
    int i=dl1 - 1;
    int j=dl2 - 1;
    int memory=0;
    int wynik;
    *(*result + dl1) = '\0';
    // obie ujemne
    if(*number1 == '-' && *number2 == '-'){
        while (j>=1){
            wynik = *(number1 + i) + *(number2 + j) - 96 + memory;
            if(wynik>9){
                memory = 1;
                wynik = wynik - 10;
            }
            else
                memory = 0;
            *(*result + i) = wynik + 48;
            if(i == 1 && memory == 1){
                char* reall = realloc(*result, sizeof(char) * (dl1 + 2));
                if(reall == NULL)
                    return 3;
                *result = reall;
                for(int k=dl1;k>0;k--){
                    *(*result + k) = *(*result + k - 1);
                }
                *(*result + dl1 + 1) = '\0';
                *(*result + 1) = '1';
            }
            i--;
            j--;
        }
        while (i>=1){
            if(memory == 0){
                *(*result + i) = *(number1 + i);
            }
            else{
                wynik = *(number1 + i) + memory - 48;
                if(wynik>9){
                    memory = 1;
                    wynik = wynik - 10;
                }
                else
                    memory = 0;
                *(*result + i) = wynik + 48;
            }
            if(i == 1 && memory == 1){
                char* reall = realloc(*result, sizeof(char) * (dl1 + 2));
                if(reall == NULL)
                    return 3;
                *result = reall;
                for(int k=dl1;k>0;k--){
                    *(*result + k) = *(*result + k - 1);
                }
                *(*result + dl1 + 1) = '\0';
                *(*result + 1) = '1';
            }
            i--;
        }
        **result = '-';
    }
        // obie dodatnie
    else if(*number1 != '-' && *number2 != '-'){
        while (j >= 0) {
            wynik = *(number1 + i) + *(number2 + j) - 96 + memory;
            if (wynik > 9) {
                memory = 1;
                wynik = wynik - 10;
            }
            else
                memory = 0;
            *(*result + i) = wynik + 48;
            if (i == 0 && memory == 1) {
                char *reall = realloc(*result, sizeof(char) * (dl1 + 2));
                if (reall == NULL)
                    return 3;
                *result = reall;
                for(int k=dl1;k>0;k--){
                    *(*result + k) = *(*result + k - 1);
                }
                *(*result + dl1 + 1) = '\0';
                *(*result) = '1';
            }
            i--;
            j--;
        }
        while (i >= 0) {
            if (memory == 0) {
                *(*result + i) = *(number1 + i);
            }
            else {
                wynik = *(number1 + i) + memory - 48;
                if (wynik > 9) {
                    memory = 1;
                    wynik = wynik - 10;
                } else
                    memory = 0;
                *(*result + i) = wynik + 48;
            }
            if (i == 0 && memory == 1) {
                char *reall = realloc(*result, sizeof(char) * (dl1 + 2));
                if (reall == NULL)
                    return 3;
                *result = reall;

                for(int k=dl1;k>0;k--){
                    *(*result + k) = *(*result + k - 1);
                }
                *(*result + dl1 + 1) = '\0';
                *(*result) = '1';
            }
            i--;
        }
    }
    return 0;
}

int smallerOrBigger(const char* number1, const char* number2){
    int len1 = strlen(number1);
    int len2 = strlen(number2);
    if(len1 < len2)
        return 1;
    if(len1 > len2)
        return 0;

    for (int i = 0; i < len1; i++){
        if(*(number1 + i) > *(number2 + i))
            return 0;
        else if(*(number1 + i) < *(number2 + i))
            return 1;
    }
    return 3;
}

int subtract(const char* number1, const char* number2, char** result){
    if(number1 == NULL || number2 == NULL || result == NULL)
        return 1;
    if(validate(number1) != 0 || validate(number2) != 0)
        return 2;
    if(*number1 != '-' && *number2 == '-')
        return add(number1,number2+1,result);
    if(*number1 == '-' && *number2 != '-'){
        int x = add(number1+1,number2,result);
        if(x == 3)
            return 3;
        char* temp;
        temp = realloc(*result, sizeof(char) * (strlen(*result) + 2));
        if(temp == NULL)
            return 3;
        *result = temp;
        for(int i=strlen(*result);i>=0;i--){
            *(*result + i + 1) = *(*result + i);
        }
        **result = '-';
        return 0;
    }
    if(*number1 == '-' && *number2 == '-') {
        return subtract(number2+1,number1+1,result);
    }
    int x = smallerOrBigger(number1, number2);
    if(x == 3){
        *result = malloc(sizeof(char) * 2);
        if(*result == NULL)
            return 3;
        *(*result) = '0';
        *(*result + 1) = '\0';
        return 0;
    }
    int minus=0;
    if(x == 1) {
        swap(&number1, &number2);
        minus = 1;
    }
    int len1 = strlen(number1);
    int len2 = strlen(number2);
    *result = malloc(sizeof(char) * (len1+2));
    if(*result == NULL)
        return 3;
    int i,j;
    for(i=0, j=len1-1;j>=0;i++,j--)
        *(*result + i) = *(number1 + j);
    *(*result + len1) = '\0';
    int memory=0;
    for(i=0,j=len2-1;i<len2;i++,j--){
        int wynik = *(*result + i) - *(number2 + j) - memory;
        if(wynik < 0){
            wynik += 10;
            memory = 1;
        }
        else
            memory = 0;
        *(*result + i) = wynik + '0';
    }
    for(i=len2;i<len1;i++){
        int wynik = *(*result + i) - '0' - memory;
        if(wynik < 0){
            wynik += 10;
            memory = 1;
        }
        else
            memory = 0;
        *(*result + i) = wynik + '0';
    }
    for(i=0,j=len1-1;i<len1/2;i++,j--){
        char ch = *(*result + i);
        *(*result + i) = *(*result + j);
        *(*result + j) = ch;
    }
    if(strlen(*result) != 1) {
        i = 0;
        while (*(*result + i) == '0') {
            i++;
        }
        for(j=0;j<len1;j++){
            *(*result + j) = *(*result + j + i);
        }
    }
    if(minus == 1){
        for(i=strlen(*result);i>=0;i--){
            *(*result + i + 1) = *(*result + i);
        }
        **result = '-';
    }
    return 0;
}

int multiply(const char* number1, const char* number2, char** result){
    if(number1 == NULL || number2 == NULL || result == NULL)
        return 1;
    if(validate(number1) != 0 || validate(number2) != 0)
        return 2;
    if(*number1 == '-' && *number2 == '-')
        return multiply(number1+1,number2+1,result);
    *result = malloc(sizeof(char) * (strlen(number1) + strlen(number2) + 1));
    if(*result == NULL)
        return 3;
    if(*number1 == '0' || *number2 == '0'){
        *(*result + 0) = '0';
        *(*result + 1) = '\0';
        return 0;
    }
    int minus=0;
    if(*number1 == '-'){
        minus = 1;
        number1 = number1+1;
    }
    if(*number2 == '-'){
        minus = 1;
        number2 = number2+1;
    }
    for(int i=0;i<(int)(strlen(number1) + strlen(number2) + 1);i++){
        *(*result + i) = '0';
    }
    if(smallerOrBigger(number1,number2) == 1)
        swap(&number1, &number2);
    int len1 = strlen(number1);
    int len2 = strlen(number2);
    int wynik;
    int carry;
    int pom;
    int i1,i2;
    int index = 0;
    for(i2 = len2 - 1;i2>=0;i2--){
        index = len2 - i2 - 1;
        carry = 0;
        for(i1 = len1 - 1;i1>=0;i1--){
            wynik = (*(number1 + i1)- '0') * (*(number2 + i2)- '0') + carry;
            if(wynik > 9){
                carry = wynik/10;
                wynik = wynik%10;
            }
            else
                carry = 0;
            pom = *(*result + index) - '0' + wynik;
            if(pom > 9){
                carry+=1;
                pom = pom%10;
            }
            *(*result + index)=pom + '0';
            index++;
        }
        if(carry != 0){
            *(*result + index)+=carry;
            index++;
        }
    }
    int len = index+1;
    int i,j;
    if(minus == 1){
        *(*result + len-1) = '-';
        len++;
        index++;
    }
    for(i=0, j=index;i<len/2;i++, j--){
        char temp = *(*result + i);
        *(*result + i) = *(*result + j);
        *(*result + j) = temp;
    }
    if(minus == 0)
        i=0;
    else
        i=1;
    while(*(*result + i) == '0')
        i++;
    for(j=0;j<len;j++){
        *(*result + j) = *(*result + j + i);
    }
    *(*result + index) = '\0';
    return 0;
}

int validate_expression(const char *expr){
    if(expr == NULL)
        return 2;
    if(isdigit(*(expr + strlen(expr) -1)) == 0)
        return 1;
    if(isdigit(*expr) == 0 && *expr != '-')
        return 1;
    int i=0;
    while (*(expr + i) != '\0') {
        if(*(expr + i) == '-' || *(expr + i) == '+' || *(expr + i) == '*') {
            i++;
            continue;
        }
        if(isdigit(*(expr + i)) == 0)
            return 1;
        i++;
    }

    i=0;
    if(*(expr + i) == '-')
        i++;
    while(isdigit(*(expr + i)) != 0)
        i++;
    while(*(expr + i) != '\0'){
        if(*(expr + i) == '+' && (*(expr + i - 1) == '-' || *(expr + i - 1) == '*' || *(expr + i - 1) == '+'))
            return 1;
        if(*(expr + i) == '*' && (*(expr + i - 1) == '+' || *(expr + i - 1) == '-' || *(expr + i - 1) == '*'))
            return 1;
        i++;
    }
    return 0;
}

int calculate(const char* expr, char **result){
    if(expr == NULL || result == NULL)
        return 1;
    if(validate_expression(expr) != 0)
        return 2;
    *result = malloc(sizeof(char) * (strlen(expr)+2));
    if(*result == NULL)
        return 3;
    int i=0;
    if(*(expr + i) == '-')
        i++;
    while(isdigit(*(expr + i)) != 0)
        i++;
    strncpy(*result,expr,i);
    *(*result + i) = '\0';
    char* number1;
    number1 = malloc(sizeof(char));
    if(number1 == NULL){
        free(*result);
        return 3;
    }
    int start, end , len1, x, znak;
    while (*(expr + i) != '\0') {
        // odczyt znaku
       if(*(expr + i) == '+')
           znak = 1;
       else if(*(expr + i) == '-')
           znak = 2;
       else if(*(expr + i) == '*')
           znak = 3;
       i++;
       // odczyt liczby
       start = i;
       if(*(expr + i) == '-')
           i++;
       while (isdigit(*(expr + i)) != 0 && *(expr + i) != '\0')
           i++;
       end = i;
       len1 = end - start;
        // alokacja liczby
        char* temp1;
        temp1 = realloc(number1, sizeof(char) * (len1+1));
        if(temp1 == NULL){
            free(*result);
            return 3;
        }
        number1 = temp1;
        for(int j=0;j<len1;j++){
            *(number1 + j) = *(expr + start + j);
        }
        *(number1 + len1) = '\0';
        // dzialanie
        char *wynik;
        if(znak == 1)
            x = add(*result,number1,&wynik);
        else if(znak == 2)
            x = subtract(*result,number1,&wynik);
        else if(znak == 3)
            x = multiply(*result,number1,&wynik);
        if(x == 3){
            free(number1);
            free(*result);
            return 3;
        }
        strcpy(*result, wynik);
        free(wynik);
   }
    free(number1);
    return 0;
}