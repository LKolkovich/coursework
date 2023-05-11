#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#define BUF_SIZE 50
#define AGENT "HiddenAgent"

typedef struct Word{
    char *chars;
    size_t len;
    char separator;
} Word;

typedef struct Sentence{
    Word *words;
    size_t wordCount;
} Sentence;

typedef struct Text{
    Sentence *sentence;
    size_t sentenceCount;
} Text;

void getText(Text *text);
int readSentence(Sentence *sentence);
int readWord(Word *word);
void freeSentence(Sentence sentence);
void printText(Text text);
void printSentence(Sentence sentence);
void freeText(Text text);
void deletePalindrom(Text *text);
int palindromInSentence(Sentence sentence);
int findHiddenAgent(Text text);
int isAgentInSentence(Sentence sentence);
void wordSort(Sentence sentence);
char* toString(Sentence sentence);
size_t sentenceLen(Sentence sentence);
void deleteSentence(Text *text, int num);
void deleteDouble(Text *text);
void sentenceToDeleteChars(Text *text);
int deleteStartAndEnd(Sentence *sentence, char **string);
Sentence parseStringToSentence(char **string);

int main() {
    setlocale(LC_ALL, "RUS");
    wprintf(L"Введите текст одной строкой:\n");
    Text text;
    getText(&text);
    deleteDouble(&text);
    wprintf(L"Повторяющиеся предложения удалены:\n");
    printText(text);
    int isHiddenAgentHere;
    int sentenceNum;
    int typeOfProgram = 0;
    while(typeOfProgram != 5){
        wprintf(L"Выберите режим работы программы\n");
        wprintf(L"1 - Удалить все символы в начале и конце строки так, чтобы в итоге первый и последний символ были различными (без учета регистра)\n");
        wprintf(L"2 - Отсортировать все слова в выбранном предложении в лексикографическом порядке\n");
        wprintf(L"3 - Удалить все предложения, в которых хотя бы одного слово является палиндромом\n");
        wprintf(L"4 - Вывести все предложения в которых есть слово “HiddenAgent” и которое не является первым словом\n");
        wprintf(L"5 - Закрыть программу\n");
        scanf("%d", &typeOfProgram);
        switch (typeOfProgram) {
            case 1:
                sentenceToDeleteChars(&text);
                printText(text);
                break;
            case 2:
                wprintf(L"Введите номер предложения для сортировки\n");
                scanf("%d", &sentenceNum);
                if(sentenceNum > text.sentenceCount || sentenceNum < 1){
                    wprintf(L"Введен номер несуществующего предложения\n");
                }
                else{
                    wordSort(text.sentence[sentenceNum - 1]);
                    printSentence(text.sentence[sentenceNum - 1]);
                }
                break;
            case 3:
                deletePalindrom(&text);
                if(text.sentenceCount == 0){
                    wprintf(L"В тексте не осталось предложений");
                }
                else {
                    printText(text);
                }
                break;
            case 4:
                isHiddenAgentHere = findHiddenAgent(text);
                if(isHiddenAgentHere == 0){
                    wprintf(L"В тексе не встречено ни одного слова “HiddenAgent”");
                }
                break;
            case 5:
                wprintf(L"Выход из программы\n");
                break;
            default:
                wprintf(L"Введено неверное число\n");
                break;
        }
        printf("\n\n");
    }
    freeText(text);
    return 0;
}

void deleteSentence(Text *text, int num){
    freeSentence(text->sentence[num]);
    memmove(text->sentence + num, text->sentence + num + 1, (text->sentenceCount - num - 1) * sizeof(Sentence));
    text->sentenceCount--;
}

void deleteDouble(Text *text){
    for(int i1 = 0; i1 < text->sentenceCount - 1; i1++){
        for(int i2 = i1 + 1; i2 < text->sentenceCount; i2++){
            if(text->sentence[i1].wordCount != text->sentence[i2].wordCount){
                continue;
            }
            int flag = 1;
            for(int i = 0; i < text->sentence[i1].wordCount; i++){
                if((strcasecmp(text->sentence[i1].words[i].chars, text->sentence[i2].words[i].chars)) || (text->sentence[i1].words[i].separator != text->sentence[i2].words[i].separator)){
                    flag = 0;
                    break;
                }
            }
            if(flag){
                deleteSentence(text, i2);
            }

        }
    }
}

int wordCmp(const void *a, const void *b){
    const Word *word1 = (Word*) a;
    const Word *word2 = (Word*) b;
    int i = 0;
    while (tolower(*(word1->chars + i)) == tolower(*(word2->chars + i))){
        if(i == (word1->len - 1) && (word1->len == word2->len)){
            return 0;
        }
        if(i == word1->len - 1){
            return -1;
        }
        if(i == word2->len - 1){
            return 1;
        }
        i++;
    }
    return (tolower(word1->chars[i]) - tolower(word2->chars[i]));
}

void wordSort(Sentence sentence){
    qsort(sentence.words, sentence.wordCount, sizeof(Word), wordCmp);
}

int findHiddenAgent(Text text){
    int flag = 0;
    for(int i = 0; i < text.sentenceCount; i++){
        if(isAgentInSentence(text.sentence[i])){
            flag = 1;
            printSentence(text.sentence[i]);
            printf("\n");
        }
    }
    return flag;
}

int isAgentInSentence(Sentence sentence){
    for(int i = 0; i < sentence.wordCount; i++){
        if(strcmp(sentence.words[i].chars,AGENT) == 0){
            return 1;
        }
    }
    return 0;
}

int isPalindrom(Word word){
    for(int i = 0; i < word.len / 2 + 1; i++){
        if(!(word.chars[i] == word.chars[word.len - i - 1] || (i == word.len - i - 1))) return 0;
    }
    return 1;
}

int palindromInSentence(Sentence sentence){
    for(int i = 0; i < sentence.wordCount; i++){
        if(isPalindrom(sentence.words[i])) {
            return 1;
        }
    }
    return 0;
}

void deletePalindrom(Text *text){
    for(int i = 0; i < text->sentenceCount;){
        if(palindromInSentence(text->sentence[i])){
            deleteSentence(text, i);
        }
        else{
            i++;
        }
    }
}

void getText(Text *text){
    int size = BUF_SIZE;
    int sentenceStatus; // 2 = конец текста, 3 = конец текста, но последнее предложение введено без точки
    text->sentence = malloc(BUF_SIZE * sizeof(Sentence));
    text->sentenceCount = 0;
    do{
        if(text->sentenceCount == BUF_SIZE - 1){
            size += BUF_SIZE;
            text->sentence = realloc(text->sentence, size * sizeof(Sentence));
        }
        sentenceStatus = readSentence(&text->sentence[text->sentenceCount]);
        text->sentenceCount++;
    }while(sentenceStatus != 2 && sentenceStatus != 3);
    if(sentenceStatus == 2) text->sentenceCount--;
}

int readSentence(Sentence *sentence){
    int wordStatus; // 0 = конец предложения, 2 = конец текста
    int size = BUF_SIZE;
    sentence->words = malloc(size * sizeof(Word));
    sentence->wordCount = 0;
    do{
        if(sentence->wordCount == BUF_SIZE - 1){
            size += BUF_SIZE;
            sentence->words = realloc(sentence->words, size * sizeof(Word));
        }
        wordStatus = readWord(&sentence->words[sentence->wordCount]);
        sentence->wordCount++;
    }while(wordStatus == 1);
    return wordStatus;
}

int readWord(Word *word){
    int result = 1;
    int size = BUF_SIZE;
    int keepGoing = 1;
    word->chars = malloc(size * sizeof(char));
    word->len = 0;
    char ch;

    do{
        ch = (char)getchar();
        if(ch == '\n') return 2;
    }while(ch == ' ');

    do{
        if(word->len == BUF_SIZE - 1){
            size += BUF_SIZE;
            word->chars = realloc(word->chars, size * sizeof(char));
        }
        word->chars[word->len] = ch;
        word->len++;
        ch = (char) getchar();
        if(ch == '.') {
            result = 0;
            keepGoing = 0;
        }
        if(ch == ' '  || ch == ','){
            word->separator = ch;
            keepGoing = 0;
        }
        if(ch == '\n'){
            keepGoing = 0;
            result = 3;
        }
    }while(keepGoing);
    word->chars[word->len] = '\0';
    return result;
}

void freeText(Text text){
    for(int i = 0; i < text.sentenceCount; i++){
        freeSentence(text.sentence[i]);
    }
}

void freeSentence(Sentence sentence){
    for(int i = 0; i < sentence.wordCount; i++){
        free( sentence.words[i].chars);
    }
}

void printText(Text text){
    for(int i = 0; i < text.sentenceCount; i++){
        printSentence(text.sentence[i]);
        if(i != text.sentenceCount - 1) printf(" ");
    }
    printf("\n");
}

void printSentence(Sentence sentence){
    for(int i = 0; i < sentence.wordCount; i++){
        if(i != sentence.wordCount - 1){
            if(sentence.words)
                printf("%s%c", sentence.words[i].chars, sentence.words[i].separator);
        }
        else{
            printf("%s", sentence.words[i].chars);
        }
    }
    printf(".");
}

size_t sentenceLen(Sentence sentence){
    size_t len = 0;
    for(int i = 0; i < sentence.wordCount; i++){
        len += (sentence.words[i].len + 1);
    }
    return len;
}

char* toString(Sentence sentence){
    size_t len = sentenceLen(sentence);
    size_t index = 0;
    char *string = malloc(sizeof(char) * len);
    for(int i = 0; i < sentence.wordCount; i++){
        strcpy(string + index, sentence.words[i].chars);
        index += sentence.words[i].len;
        string[index] = sentence.words[i].separator;
        index++;
    }
    string[index - 1] = '\0';
    return string;
}



void sentenceToDeleteChars(Text *text){
    char **string = malloc(sizeof(char*));
    int status;
    for(int i = 0; i < text->sentenceCount;){
        status = deleteStartAndEnd(&(text->sentence[i]), string);
        if(status == -1){
            deleteSentence(text, i);
        }
        else{
            freeSentence(text->sentence[i]);
            text->sentence[i] = parseStringToSentence(string);
            i++;
        }

    }
}

Sentence parseStringToSentence(char **string){
    Sentence newSentence;
    newSentence.wordCount = 0;
    int size = BUF_SIZE;
    newSentence.words = malloc(sizeof(Word) * size);
    size_t len = strlen(*string);
    int j;
    int i = 0;
    while(**(string + i) == ' ' || **(string + i) == ',') i++;
    for(; i < len; i++) {
        if (newSentence.wordCount == BUF_SIZE - 1) {
            size += BUF_SIZE;
            newSentence.words = realloc(newSentence.words, size * sizeof(Word));
        }
        newSentence.words[newSentence.wordCount].len = 0;
        int wordSize = BUF_SIZE;
        newSentence.words[newSentence.wordCount].chars = malloc(wordSize * sizeof(char));
        for (j = 0; *(*string + i + j) != ' ' && *(*string + i + j) != ',' && *(*string + i + j) != '.' &&
                    *(*string + i + j) != '\0'; j++) {
            if (newSentence.words[newSentence.wordCount].len == size - 2) {
                wordSize += BUF_SIZE;
                newSentence.words[newSentence.wordCount].chars = realloc(newSentence.words[newSentence.wordCount].chars,
                                                                         wordSize * sizeof(char));
            }
            newSentence.words[newSentence.wordCount].chars[newSentence.words[newSentence.wordCount].len] = *(*string +
                                                                                                             i + j);
            newSentence.words[newSentence.wordCount].len++;
        }
        if (*(*string + i + j) == ' ' || *(*string + i + j) == ',')
            newSentence.words[newSentence.wordCount].separator = *(*string + i + j);
        newSentence.words[newSentence.wordCount].chars[newSentence.words[newSentence.wordCount].len] = '\0';
        newSentence.wordCount++;
        i += j;
    }
    return newSentence;
}

int deleteStartAndEnd(Sentence *sentence, char **string){
    *string = toString(*sentence);
    size_t len = strlen(*string);
    while((toupper((*string)[0]) == toupper((*string)[len - 1])) && (len > 0)){
        memmove(*string, *string + sizeof(char), len * sizeof(char));
        len--;
        memmove(*string + sizeof(char) * (len - 1), sentence + sizeof(char) * len, 2);
        len--;
        if(len <= 0) {
            free(*string);
            return -1;
        }
    }
    return 0;
}