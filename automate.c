#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int state = 0, start = 0 ;
char c;
char lexeme[10];
FILE* sourceFile;
FILE* outputFile;


typedef enum TOKEN
{
    SI,
    SINON,
    ALORS,
    OPREL,
    ID,
    NB,
    ERROR
} TOKEN;


void flushBuffer()
{
    for(int i=0; i<10; lexeme[i++] = 0);
}


int nextchar()
{
    int ch = fgetc(sourceFile);
    if(ch != EOF)
        return ch;
    else
    {
        fclose(sourceFile);
        fclose(outputFile);
        exit(0);
    }

}


void accept(int tokenType)
{
    state = 0;
    switch(tokenType)
    {
    case 2:
        fprintf(outputFile," LT");
        break;
    case 3:
        fprintf(outputFile," LE");
        break;
    case 4:
        fprintf(outputFile," NE");
        break;
    case 6:
        fprintf(outputFile," GT");
        break;
    case 7:
        fprintf(outputFile," GE");
        break;
    case 9:
        fprintf(outputFile," AFFECT");
        break;
    case 10:
        fprintf(outputFile," EQ");
        break;
    case 13:
        fprintf(outputFile," ID");
        break;
    case 19:
        fprintf(outputFile," EXPONENTFLOAT");
        break;
    case 24:
        fprintf(outputFile," FLOAT");
        break;
    default:
        fprintf(outputFile," NOT FOUND");
        break;
    }
}


int installId(char* lexeme)
{
    char* reservedKeywords = {"SI",
                              "SINON",
                              "ALORS",
                              "OPREL",
                              "ID",
                              "NB",
                              "ERROR"
                             };

    for(int i=0; i<7; i++)
    {
        if(strcmp(lexeme, reservedKeywords[i]) == 0)
        {
            fprintf(outputFile," %s","expression");
            return 0;
        }
    }

    accept(13);
}


int fail()
{
    switch(start)
    {
    //  OPREL || Affectation
    case 0 :
        start = 11 ;
        break ;
    //  Identificateurs
    case 11 :
        start = 14 ;
        break ;
    //  Nombres
    case 14 :
        start = 22 ;
        break ;
    case 22 :
        if(feof(sourceFile))
        {
            fclose(outputFile);
            fclose(sourceFile);
            exit(0);
        }
        else
        {
            printf("Error at reading : %c", c);
        }
        break ;
    }
    return start;
}

void startParsing()
{
    int count;

    while (1)
    {

        switch(state)
        {
        case 0 :
            c = nextchar();
            if (c==' '||c=='\t'||c=='\n')
                state = 0 ;
            else if (c == '<')
                state = 1 ;
            else if (c == '>')
                state = 5 ;
            else if (c == '=')
                state = 8 ;
            else
                state = fail() ;
            break;
        case 1:
            c = nextchar();
            if(c == '=')
                accept(3);
            else if(c == '>')
                accept(4);
            else
            {
                accept(2);
                ungetc(c,sourceFile);
            }

            break;
        case 5:
            c = nextchar();
            if(c == '=')
                accept(7);
            else
                accept(6);
            break;
        case 8:
            c = nextchar();
            if(c == '=')
                accept(10);
            else
                accept(9);
            break;

        case 11 :
            flushBuffer();
            lexeme[0]=c;
            if (isalpha(c)||c=='_')
                state = 12 ;
            else
                state = fail() ;
            break ;

        case 12 :
            while(c = nextchar())
            {
                if (isalnum(c)||c=='_')
                {
                    state = 12 ;
                    lexeme[count++] = c;
                }
                else
                {
                    state = 13 ;
                    break;
                }
            }
        case 13 :
            installId(lexeme);
        case 14:
            if(isdigit(c))
            {
                state = 15;
            }
            else if(c == '.')
                state = 20;
            else
                state = fail();
        case 15:
            while(c = nextchar())
            {
                if(isdigit(c) == 0)
                    break;
            }
            printf("After\n");
            if(c == 'e' || c == 'E')
                state = 16;
            else if (c == '.')
                state = 21;
            else
                state = fail();
            break;
        case 16:
            c = nextchar();
            if(c =='+' || c=='-')
                state = 17;
            else if(isdigit(c))
                state = 18;
            else
                state = fail();
            break;
        case 17:
            c = nextchar();
            if(isdigit(c))
                state = 18;
            else
                state = fail();
            break;
        case 18:
            while(c = nextchar())
                if(!isdigit(c))
                    break;
            state = 19;
        case 19:
            accept(19);
            break;
        case 22:
            c = nextchar();

            if(isdigit(c))
                state = 23;
            else if(c =='.')
                state = 25;
            else
                state = fail();
        case 23:
            while(c = nextchar())
                if(!isdigit(c))
                    break;
            if(c == '.')
                state = 26;
            else
                state = 24;
        case 24:
            accept(24);
            break;
        case 25:
            if(isdigit(c))
                state = 26;
            else
                state = fail();
        case 26:
            while(c = nextchar())
                if(isdigit(c) == 0)
                    break;
            state = 24;
        }
    }
}


int main()
{
    sourceFile = fopen("sourceCode","r");
    outputFile = fopen("outputFile","w");


    startParsing();
    //accept(6);
    //fclose(sourceFile);
    //fclose(outputFile);
}
