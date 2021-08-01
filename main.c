#include <stdio.h>
#include <stdlib.h>

void variable_checker(FILE*);
int compare_strings(char [], char []);
void which_keyword(FILE* fp);
int check_to(FILE* fp);
int check_from(FILE* fp);
void int_value(FILE* fp);
void skip_blanks(FILE* fp);
void int_procedure(FILE* fp);
void move_procedure(FILE* fp);
void add_procedure(FILE* fp);
void sub_procedure(FILE* fp);
void line_or_codeblock(FILE* fp);
void codeblock(FILE* fp);
void loop_procedure(FILE* fp);
void out_procedure(FILE* fp);
int list_element(FILE* fp);
void eol_checker(FILE* fp);
void skip_comments(FILE* fp);
void check_times(FILE* fp);
int int_const(FILE* fp);
void string_check(FILE* fp);
int string_length(char s[]);

FILE* flexeme;
int counter = 0, identifiers = 0, keywords = 0,total_paranthesis = 0, EOL_char = 0,count1 = 0,temp = 0, error_temp=0;

int main()
{
    int z;
    char name [20]= "";
    printf("Please enter your file name:\n");
    scanf("%s", name);
    z = string_length(name);
    name[z] = '.';
    name[z+1] = 'b';
    name[z+2] = 'a';
    FILE *fp;
    if ((fp = fopen(name,"r")) == NULL)
        printf("File not found!\n");
    else
    {
        printf("File found\n");
        flexeme = fopen("lexeme.txt", "w");
        while(!feof(fp))
        {
            which_keyword(fp);
        }
        printf("Number of Identifiers:%d\nNumber of Keywords:%d\nNumber of Paranthesis:%d\nNumber of EOL Character:%d\n",identifiers,keywords,total_paranthesis,EOL_char);
        fclose(flexeme);
        fclose(fp);
        if (error_temp == 1)
        {
            rename("lexeme.txt","error_lexeme.txt");
        }
    }
    return 0;
}

void variable_checker(FILE* fp)
{
    char a;
    int variable_long = 1;
    fprintf(flexeme,"Identifier (");
    identifiers++;
    while (!feof(fp))
    {
        a = fgetc(fp);
        if((a>=48 && a<=57) || (a>=65 && a<=90) || (a>=97 && a<=122) || a==95)
        {
            variable_long++;
            if (variable_long == 21)
            {
                printf("%d Error:Variable cannot be this long.\n",counter);
                error_temp = 1;
                return;
            }
            fprintf(flexeme,"%c",a);
        }
        else if (a==' ' || a=='.' || a=='\n')
        {
            if(a=='\n')
            {
                counter++;
            }
            fseek(fp, -sizeof(char), SEEK_CUR);
            fprintf(flexeme,")\n");
            return;
        }
        else if(a==EOF)
        {
            printf("Error:File ended during analyze.\n");
            error_temp = 1;
            return;
        }
        else if(a==',')
        {
            fseek(fp, -sizeof(char), SEEK_CUR);
            fprintf(flexeme,")\n");
            return;
        }
        else
        {
            printf("%d Error:Variable name is not accepted.\n",counter);
            error_temp = 1;
        }
    }
}

void which_keyword(FILE* fp)
{
    char str [5]="";
    skip_blanks(fp);
    fgets(str,5,fp);
    if (compare_strings(str,"int ") == 1)
    {
        fprintf(flexeme,"Keyword (int)\n");
        keywords++;
        int_procedure(fp);
        return ;
    }
    else if (compare_strings(str,"move"))
    {
        fprintf(flexeme,"Keyword (move)\n");
        keywords++;
        move_procedure(fp);
    }
    else if (compare_strings(str,"add "))
    {
        fprintf(flexeme,"Keyword (add)\n");
        keywords++;
        add_procedure(fp);
    }
    else if (compare_strings(str,"sub "))
    {
        fprintf(flexeme,"Keyword (sub)\n");
        keywords++;
        sub_procedure(fp);
    }
    else if (compare_strings(str,"loop"))
    {
        fprintf(flexeme,"Keyword (loop)\n");
        keywords++;
        loop_procedure(fp);
    }
    else if (compare_strings(str,"out "))
    {
        fprintf(flexeme,"Keyword (out)\n");
        keywords++;
        out_procedure(fp);
    }
    else if (compare_strings(str,"\n"))
    {
        counter++;
        fgetc(fp);
    }
    else if (compare_strings(str,"."));
    else if (compare_strings(str,"}"));
    else
    {
        printf("%d Expected Keyword!\n",counter);
        error_temp = 1;
    }
}

int compare_strings(char a[], char b[])
{
    int c = 0;

    while (a[c] == b[c])
    {
        if (a[c] == '\0' || b[c] == '\0')
            break;
        c++;
    }

    if (a[c] == '\0' && b[c] == '\0')
        return 1;
    else
        return 0;
}

int string_length(char s[])
{
    int c = 0;

    while (s[c] != '\0')
        c++;

    return c;
}

int int_const(FILE* fp)
{

    fprintf(flexeme,"Integer (");
    int cdigit=0;
    char a;
    a=fgetc(fp);
    if(a=='-')fprintf(flexeme,"%c",a);
    else if(a>=48 && a<=57)
    {
        fprintf(flexeme,"%c",a);
        cdigit++;
    }
    else
    {
        printf("%d Error:Unacceptable integer value.\n",counter);
        return -2;
    }
    while(!feof(fp))
    {
        a = fgetc(fp);
        if(a>=48 && a<=57)
        {
            fprintf(flexeme,"%c",a);
            cdigit++;
        }
        else if(a==' ')
        {
            fprintf(flexeme,")\n");
            return 0;
        }
        else
        {
            printf("%d Error:Unacceptable integer value.\n",counter);
            error_temp = 1;
            return -2;
        }
        if(cdigit==101)
        {
            printf("%d Error:Integer value can't be longer than 100.\n",counter);
            error_temp = 1;
            return -1;
        }
    }
    return -2;

}

int check_to(FILE* fp)
{
    char s;
    s = fgetc(fp);
    if(s=='t')
    {
        s = fgetc(fp);
        if(s=='o')
        {
            fprintf(flexeme,"Keyword (to)\n");
            keywords++;
            return 0;
        }
    }
    else
    {
        printf("%d Error:Expected keyword 'to'.\n", counter);
        error_temp = 1;
    }

    return -1;
}

int check_from(FILE* fp)
{
    char s;
    s = fgetc(fp);
    if(s=='f')
    {
        s = fgetc(fp);
        if(s=='r')
        {
            s = fgetc(fp);
            if(s=='o')
            {
                s = fgetc(fp);
                if(s=='m')
                {
                    s = fgetc(fp);
                    if(s == ' ')
                    {
                        fprintf(flexeme,"Keyword (from)\n");
                        keywords++;
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        printf("%d Error:Expected keyword 'from'\n",counter);
        error_temp = 1;
    }
    return -1;
}

void skip_blanks(FILE* fp)  //safe
{
    char c;
    while(!feof(fp))
    {
        c = fgetc(fp);
        if(c!=' ' && c!='\t')
        {
            if(c == '{')
            {
                total_paranthesis++;
                skip_comments(fp);
                skip_blanks(fp);
                break;
            }
            if(c == '\n')
            {
                counter++;
            }
            else
            {
                fseek(fp, -sizeof(char), SEEK_CUR);
                return;
            }
        }

    }
    return;
}


void int_procedure(FILE* fp)
{
    skip_blanks(fp);
    variable_checker(fp);
    skip_blanks(fp);
    eol_checker(fp);
}

void move_procedure(FILE* fp)
{
    skip_blanks(fp);
    int_value(fp);
    skip_blanks(fp);
    check_to(fp);
    skip_blanks(fp);
    variable_checker(fp);
    skip_blanks(fp);
    eol_checker(fp);
}


void add_procedure(FILE* fp)
{
    skip_blanks(fp);
    int_value(fp);
    skip_blanks(fp);
    check_to(fp);
    skip_blanks(fp);
    variable_checker(fp);
    skip_blanks(fp);
    eol_checker(fp);
}


void sub_procedure(FILE* fp)
{
    skip_blanks(fp);
    int_value(fp);
    skip_blanks(fp);
    check_from(fp);
    skip_blanks(fp);
    variable_checker(fp);
    skip_blanks(fp);
    eol_checker(fp);
}

void line_or_codeblock(FILE* fp)
{
    char a;
    a = fgetc(fp);
    if(a == '\n')
    {
        counter++;
        a=fgetc(fp);
    }
    if (a == '[')
    {
        total_paranthesis++;
        fprintf(flexeme,"StartofCodeBlock ([)\n");
        skip_blanks(fp);
        codeblock(fp);
    }
    else
    {
        fseek(fp, -sizeof(char), SEEK_CUR);
        which_keyword(fp);
    }
}

void codeblock(FILE* fp)
{
    char c ;
    c=fgetc(fp);
    while(!feof(fp))
    {
        if(c != ']')
        {
            if(c == '.')
            {
                skip_blanks(fp);
            }
            else
            {
                fseek(fp, -sizeof(char), SEEK_CUR);
                which_keyword(fp);
                skip_blanks(fp);
            }
            c=fgetc(fp);

        }
        else
        {
            fprintf(flexeme,"EndofCodeBlock (])\n");
            total_paranthesis++;
            return;
        }
    }
    printf("Error:Expected ']' before EOF.\n");
    error_temp = 1;
}

void loop_procedure(FILE* fp)
{
    skip_blanks(fp);
    int_value(fp);
    skip_blanks(fp);
    check_times(fp);
    skip_blanks(fp);
    line_or_codeblock(fp);
    skip_blanks(fp);
}

void int_value(FILE* fp)
{
    char c;
    c=fgetc(fp);
    fseek(fp, -sizeof(char), SEEK_CUR);
    if(c=='-' || (c>=48 && c<=57))
    {
        int_const(fp);
    }
    else variable_checker(fp);
}

void check_times(FILE* fp)
{
    char s;
    s = fgetc(fp);
    if(s=='t')
    {
        s = fgetc(fp);
        if(s=='i')
        {
            s = fgetc(fp);
            if(s=='m')
            {
                s = fgetc(fp);
                if(s=='e')
                {
                    s = fgetc(fp);
                    if(s == 's')
                    {
                        s = fgetc(fp);
                        if(s == ' ' || s=='\n')
                        {
                            if(s == '\n')
                            {
                                counter++;
                            }
                            fprintf(flexeme,"Keyword (times)\n");
                            keywords++;
                            return;
                        }
                    }
                }
            }
        }
    }
    printf("%d Error:Expected keyword 'times'.\n", counter);
    error_temp = 1;
    return;
}

void out_procedure(FILE* fp)
{
    count1=0;
    temp = 0;
    list_element(fp);
}

int list_element(FILE* fp)
{
    char c;
    int re = 0, newline_temp = 0;
    while(!feof(fp))
    {
        c = fgetc(fp);
        if(c=='-' || (c>=48 && c<=57))
        {
            fseek(fp, -sizeof(char), SEEK_CUR);
            int_const(fp);
            count1++;
        }
        else if((c>=48 && c<=57) || (c>=65 && c<=90) || (c>=97 && c<=122) || c==95)
        {
            newline_temp = 0;
            if(c=='n')
            {
                newline_temp++;
                c=fgetc(fp);
                if(c=='e')
                {
                    newline_temp++;
                    c=fgetc(fp);
                    if(c=='w')
                    {
                        newline_temp++;
                        c=fgetc(fp);
                        if(c=='l')
                        {
                            newline_temp++;
                            c=fgetc(fp);
                            if(c=='i')
                            {
                                newline_temp++;
                                c=fgetc(fp);
                                if(c=='n')
                                {
                                    newline_temp++;
                                    c=fgetc(fp);
                                    if(c=='e')
                                    {
                                        newline_temp++;
                                        c=fgetc(fp);
                                        if(((c>=48 && c<=57) || (c>=65 && c<=90) || (c>=97 && c<=122) || c==95))
                                        {
                                            fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                                            newline_temp = 0;
                                        }
                                        else
                                        {
                                            fprintf(flexeme,"StringEOL\n");
                                            fseek(fp, -sizeof(char), SEEK_CUR);
                                            count1++;
                                        }
                                    }
                                    else
                                    {
                                        fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                                        newline_temp = 0;
                                    }
                                }
                                else
                                {
                                    fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                                    newline_temp = 0;
                                }
                            }
                            else
                            {
                                fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                                newline_temp = 0;
                            }
                        }
                        else
                        {
                            fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                            newline_temp = 0;
                        }
                    }
                    else
                    {
                        fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                        newline_temp = 0;
                    }
                }
                else
                {
                    fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                    newline_temp = 0;
                }
            }
            else
            {
                fseek(fp, -((newline_temp)*(sizeof(char))), SEEK_CUR);
                newline_temp = 0;
            }
            if(newline_temp == 0)
            {
                fseek(fp, -sizeof(char), SEEK_CUR);
                variable_checker(fp);
                count1++;
            }
        }

        else if(c=='\n')
        {
            count1++;
        }
        else if(c==',')
        {
            fprintf(flexeme,"Comma (,)\n");
            temp++;
            if(count1!=temp)
            {
                printf("%d Error:Expected a statement before comma(,) output format.\n",counter);
                error_temp = 1;
                return 1;
            }
            re = list_element(fp);
            if(re==1)
            {
                return 1;
            }
        }
        else if(c=='"')
        {
            fprintf(flexeme,"String (\"");
            string_check(fp);
            count1++;
        }
        else if(c=='.')
        {
            fprintf(flexeme,"EndOfLine\n");
            EOL_char++;
            return 1;
        }
        else if(c==' ')
        {
            skip_blanks(fp);
        }
        else
        {
            printf("%d Error:Expected output format.\n",counter);
            return 1;
        }
    }
}

void string_check(FILE* fp)
{
    char c;
    c = fgetc(fp);
    while(!feof(fp))
    {
        if(c!='"')
        {
            fprintf(flexeme,"%c",c);
        }
        else
        {
            fprintf(flexeme,"\")\n");
            return;
        }
        c = fgetc(fp);
    }
    printf("%d Error:Expected ending side of \" before EOF.\n", counter);
    error_temp = 1;
}


void eol_checker(FILE* fp)
{
    char c;
    c = fgetc(fp);
    if(c=='.')
    {
        fprintf(flexeme,"EndOfLine\n");
        EOL_char++;
        return;
    }
    else
    {
        printf("%d Error:Expected EOL.\n",counter);
        error_temp = 1;
    }
}

void skip_comments(FILE* fp)
{
    char c ;
    while(!feof(fp))
    {
        c = fgetc(fp);
        if(c == '}')
        {
            total_paranthesis++;
            return;
        }
    }
    printf("%d Error:Expected '}' before EOL.\n", counter);
    error_temp = 1;
}
