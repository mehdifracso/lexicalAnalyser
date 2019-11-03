#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int state = 0, start = 0 ;
int lexical_value ;


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

void retract(int tokenType)
{
    //  Writes the token in the output file
    //  +
    //  Resets the state to 0
}
void installId(char* lexeme)
{
    char* reservedKeywords = ["SI",
                              "SINON",
                              "ALORS",
                              "OPREL",
                              "ID",
                              "NB",
                              "ERROR"];

    for(int i=0; i<7; i++)
    {
        if(strcmp(lexeme, reservedKeywords[i]) == 0)
            return 0;
    }

    return 1;
}

void getToken(char* lexeme)
{
    char* reservedKeywords = ["SI",
                              "SINON",
                              "ALORS",
                              "OPREL",
                              "ID",
                              "NB",
                              "ERROR"];

    for(int i=0; i<7; i++)
    {
        if(strcmp(lexeme, reservedKeywords[i]) == 0)
            return TOKEN[i]; /// CAN WE DO THIS ??
    }

    return ID;
}


int fail()
{
    forward = token_debut ;
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
        recover() ;
        break ;
    default : /* erreur */
    }
    return start ;
}

TOKEN nexttoken()
{
    while (1)
    {
        switch(state)
        {
        case 0 :
            c = nextchar();
            if (c==' '||c=='\t'||c=='\n')
            {
                state = 0 ;
                //debut ++ ;
            }
            else if (c == '<')
                state = 1 ;
            else if (c == '>')
                state = 5 ;
            else if (c == '=')
                state = 8 ;
            else
                state = fail() ;
            break ;
        case 1:
            c = nextchar();
            if(c == '=')
                retract(3);
            else if(c == '>')
                retract(4);
            else
                retract(2);
            break;
        case 5:
            c = nextchar();
            if(c == '=')
                retract(7);
            else
                retract(6);
            break;

        case 8:
            c = nextchar();
            if(c == '=')
                retract(10);
            else
                retract(9);
            break;

        case 11 :
            c = nextchar() ;
            if (isalpha(c)||c=='_')
                state = 12 ;
            else
                state = fail() ;
            break ;

        case 12 :
            c = nextchar() ;
            if (isalnum(c)||c=='_')
                state = 12 ;
            else
                state = 13 ;
            break ;
        case 13 :
            retract(13) ;
            installId() ;
            return(gettoken()) ;    /// Check this out
        case 14:
            c = nextchar();
            if(isdigit(c))
                state = 15;
            else if(c == '.')
                state = 20;
            else
                fail();
            break;
        case 15:
            while(c = nextchar())
            {
                if(!isdigit(c)) break;
            }
            if(c == 'e' || c == 'E')
                state = 16;
            else if (c == '.')
                state = 21;
            else
                fail();
            break;
        case 16:
            c = nextchar();
            if(c =='+' || c=='-')
                state = 17;
            else if(isdigit(c))
                state = 18;
            else
                fail();
            break;
        case 17:
            c = nextchar();
            if(isdigit(c))
                state = 18;
            else
                fail();
            break;
        case 18;
            while(c = nextchar()) if(!isdigit()) break;
            state = 19;
        case 19:
            retract(19);
            break;
        case 22:
            c = nextchar();

            if(isdigit(c))
                state = 23;
            else if(c =='.')
                state = 25;
            else
                fail();
            break;
        case 23:
            while(c = nextchar()) if(!isdigit()) break;
            if(c == '.')
                state = 26;
            else
                state = 24;
        case 24:
            retract(24);
            break;
        case 25:
            if(isdigit(c))
                state = 26;
            else
                fail();
            break;
        case 26:
            while(c = nextchar()) if(!isdigit()) break;
            state = 24;
            break;
        }
    }
}
