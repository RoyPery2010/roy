#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "rasmlexer.h"

char *open_file(char *file_path,  int *length) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        exit(1);
    }
    char *current = {0};

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    current = malloc(sizeof(char) * *length);
    fread(current, 1, *length, file);
    if (!current) {
        fprintf(stderr, "ERROR: Could not read from file\n");
    }
    fclose(file);
    return current;
}
void print_token(Token token) {
    switch (token.type) {
        case TYPE_NONE:
            printf("TYPE NONE\n");
            break;
        case TYPE_NOP:
            printf("TYPE NOP\n");
            break;
        case TYPE_PUSH:
            printf("TYPE PUSH\n");
            break;
        case TYPE_POP:
            printf("TYPE POP\n");
            break;
        case TYPE_DUP:
            printf("TYPE DUP\n");
            break;
        case TYPE_INDUP:
            printf("TYPE INDOP\n");
            break;
        case TYPE_SWAP:
            printf("TYPE SWAP\n");
            break;
        case TYPE_INSWAP:
            printf("TYPE INSWAP\n");
            break;
        case TYPE_ADD:
            printf("TYPE ADD\n");
            break;
        case TYPE_SUB:
            printf("TYPE SUB\n");
            break;
        case TYPE_MUL:
            printf("TYPE MUL\n");
            break;
        case TYPE_DIV:
            printf("TYPE DIV\n");
            break;
        case TYPE_MOD:
            printf("TYPE MOD\n");
            break;
        case TYPE_CMPE:
            printf("TYPE CMPE\n");
            break;
        case TYPE_CMPNE:
            printf("TYPE CMPNE\n");
            break;
        case TYPE_CMPG:
            printf("TYPE CMPG\n");
            break;
        case TYPE_CMPL:
            printf("TYPE CMPL\n");
            break;
        case TYPE_CMPGE:
            printf("TYPE CMPGE\n");
            break;
        case TYPE_CMPLE:
            printf("TYPE CMPLE\n");
            break;
        case TYPE_JMP:
            printf("TYPE JMP\n");
            break;
        case TYPE_ZJMP:
            printf("TYPE ZJMP\n");
            break;
        case TYPE_NZJMP:
            printf("TYPE NZJMP\n");
            break;
        case TYPE_PRINT:
            printf("TYPE PRINT\n");
            break;
        case TYPE_HALT:
            printf("TYPE HALT\n");
            break;
    }
    printf("text: %s, line %d, character: %d\n", token.text, token.line, token.character);
}
Token init_token(TokenType type, char *text, int line, int character){
    Token token = {.type = type, .text = text, .line = line, .character = character};
    return token;
}
TokenType check_builtin_keywords(char *name) {
    if (strcmp(name, "nop") == 0) {
        return TYPE_NOP;
    } else if (strcmp(name, "push") == 0) {
        return TYPE_PUSH;
    } else if (strcmp(name, "pop") == 0) {
        return TYPE_POP;
    } else if (strcmp(name, "dup") == 0) {
        return TYPE_DUP;
    } else if (strcmp(name, "indup") == 0) {
        return TYPE_INDUP;
    } else if (strcmp(name, "swap") == 0) {
        return TYPE_SWAP;
    } else if (strcmp(name, "inswap") == 0) {
        return TYPE_INSWAP;
    } else if (strcmp(name, "add") == 0) {
        return TYPE_ADD;
    } else if (strcmp(name, "sub") == 0) {
        return TYPE_SUB;
    } else if (strcmp(name, "mul") == 0) {
        return TYPE_MUL;
    } else if (strcmp(name, "div") == 0) {
        return TYPE_DIV;
    } else if (strcmp(name, "mod") == 0) {
        return TYPE_MOD;
    } else if (strcmp(name, "cmpe") == 0) {
        return TYPE_CMPE;
    } else if (strcmp(name, "cmpne") == 0) {
        return TYPE_CMPNE;
    } else if (strcmp(name, "cmpg") == 0) {
        return TYPE_CMPG;
    } else if (strcmp(name, "cmpl") == 0) {
        return TYPE_CMPL;
    } else if (strcmp(name, "cmpge") == 0) {
        return TYPE_CMPGE;
    } else if (strcmp(name, "cmple") == 0) {
        return TYPE_CMPLE;
    } else if (strcmp(name, "jmp") == 0) {
        return TYPE_JMP;
    } else if (strcmp(name, "zjmp") == 0) {
        return TYPE_ZJMP;
    } else if (strcmp(name, "nzjmp") == 0) {
        return TYPE_NZJMP;
    } else if (strcmp(name, "print") == 0) {
        return TYPE_PRINT;
    } else if (strcmp(name, "halt") == 0) {
        return TYPE_HALT;
    }
    return TYPE_NONE;
}
Token generate_keyword(char *current, int *current_index, int line, int character) {
    char *keyword_name = malloc(10 * sizeof(char));
    int keyword_length = 0;
    while(isalpha(current[*current_index])) {
        keyword_name[keyword_length] = current[*current_index];
        *current_index += 1;
        keyword_length++;
    }
    keyword_name[keyword_length] = '\0';
    TokenType type = check_builtin_keywords(keyword_name);
    assert(type != TYPE_NONE && "Custom identifiers are not implemented yet\n");
    Token token = init_token(type, keyword_name, line, character);
    return token;
}
int lexer() {
    int length;
    char *current = open_file("test.rasm", &length);
    int current_index = 0;
    int line = 1;
    int character = 1;
    while(current_index <= length) {
        if (current[current_index] == '\0') {
            break;
        }
        if(current[current_index] == '\n') {
            current_index++;
            line++;
            character = 0;
        }
        if(isalpha(current[current_index])) {
            Token token = generate_keyword(current, &current_index, line, character);
            print_token(token);
        } else if(isdigit(current[current_index])) {
            current_index++;
        }
        character++;
    }
    return 0;
}