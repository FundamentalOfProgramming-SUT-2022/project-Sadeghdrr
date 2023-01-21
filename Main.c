#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <conio.h>
#include <dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>

#define MAX 500

char command[MAX];
char command_tajzie[20][MAX];
char clipboard[MAX];

void clear()
{
    memset(command, 0, MAX);
    for (int i = 0; i < 20; i++)
    {
        memset(command_tajzie[i], 0, MAX);
    }
}

void tajzieh()
{
    int i = 0, j = 0, k = 0;

    while (true)
    {
        if (command[i] == '\0')
            return;

        else if (command[i] == '\\')
        {
            if (command[i + 1] == 'n')
            {
                command_tajzie[k][j] = '\n';
                i += 2;
                j++;
                continue;
            }

            else if (command[i + 1] == 't')
            {
                command_tajzie[k][j] = '\t';
                i += 2;
                j++;
                continue;
            }

            else if (command[i + 1] == '"')
            {
                command_tajzie[k][j] = '"';
                i += 2;
                j++;
                continue;
            }

            else if (command[i + 1] == '\\')
            {
                command_tajzie[k][j] = '\\';
                i += 2;
                j++;
                continue;
            }
        }

        else if (command[i] == ' ')
        {
            if (command[i + 1] == '"')
            {
                i += 2;
                j = 0;
                k++;

                while (true)
                {
                    if (command[i] == '"' && command[i - 1] != '\\')
                        break;

                    else if (command[i] == '\\')
                    {
                        if (command[i + 1] == 'n')
                        {
                            command_tajzie[k][j] = '\n';
                            i += 2;
                            j++;
                            continue;
                        }

                        else if (command[i + 1] == 't')
                        {
                            command_tajzie[k][j] = '\t';
                            i += 2;
                            j++;
                            continue;
                        }

                        else if (command[i + 1] == '"')
                        {
                            command_tajzie[k][j] = '"';
                            i += 2;
                            j++;
                            continue;
                        }

                        else if (command[i + 1] == '\\')
                        {
                            command_tajzie[k][j] = '\\';
                            i += 2;
                            j++;
                            continue;
                        }
                    }

                    command_tajzie[k][j] = command[i];
                    i++;
                    j++;
                }

                i++;
            }

            i++;
            j = 0;
            k++;
            continue;
        }

        command_tajzie[k][j] = command[i];
        i++;
        j++;
    }
}

int check_existance()
{
    char direction[MAX];
    memset(direction, 0, MAX);

    strncpy(direction, command_tajzie[2], 3);

    int i = 3;
    while (command_tajzie[2][i] != '\0')
    {
        direction[i] = command_tajzie[2][i];
        i++;

        if (command_tajzie[2][i] == '/')
        {
            DIR *folder = opendir(direction);
            if (folder)
                closedir(folder);
            else if (ENOENT == errno)
            {
                printf("Wrong address!\n");
                return 0;
            }
        }
    }

    if (access(direction, F_OK) != 0)
    {
        printf("The file doesn't exists!\n");
        return 0;
    }

    return 1;
}

void createfile()
{
    char direction[MAX];
    memset(direction, 0, MAX);

    if (access(command_tajzie[2], F_OK) == 0)
    {
        printf("The file already exists and cannot be made again!\n");
        return;
    }

    else
    {
        strncpy(direction, command_tajzie[2], 3);

        int i = 3;
        while (command_tajzie[2][i] != '\0')
        {
            direction[i] = command_tajzie[2][i];
            i++;

            if (command_tajzie[2][i] == '/')
                mkdir(direction);
        }

        FILE *file_to_be_made = fopen(direction, "w");
        fclose(file_to_be_made);
        printf("File has successfully made!\n");
    }

    return;
}

void insertstr()
{
    char direction[MAX];
    char temp_copy_direction[MAX];
    char temp_arr[MAX];
    long line;
    long char_pos;
    long current_line = 1;
    long current_char_pos = 0;
    char *end_of_line_pos, *end_of_char_pos;

    memset(direction, 0, MAX);
    memset(temp_arr, 0, MAX);
    memset(temp_copy_direction, 0, MAX);

    strcat(direction, command_tajzie[2]);

    strcat(strncat(temp_copy_direction, direction, strlen(direction) - 4), "_temp.txt");

    FILE *temp_copy = fopen(temp_copy_direction, "w+");
    FILE *file_to_be_written = fopen(direction, "r");

    line = strtol(command_tajzie[6], &end_of_line_pos, 10);
    char_pos = strtol(end_of_line_pos + 1, &end_of_char_pos, 10);

    while (current_line < line)
    {
        fputs(fgets(temp_arr, MAX + 5, file_to_be_written), temp_copy);
        current_line++;
    }

    while (current_char_pos < char_pos)
    {
        fputc(fgetc(file_to_be_written), temp_copy);
        current_char_pos++;
    }

    for (int i = 0; i < strlen(command_tajzie[4]); i++)
    {
        fputc(command_tajzie[4][i], temp_copy);
        current_char_pos++;
        if (command_tajzie[4][i] == '\n')
        {
            current_line++;
            current_char_pos = 0;
        }
    }

    while (fgets(temp_arr, MAX + 5, file_to_be_written) != NULL)
    {
        fputs(temp_arr, temp_copy);
    }

    fclose(file_to_be_written);
    fclose(temp_copy);

    file_to_be_written = fopen(direction, "w");
    temp_copy = fopen(temp_copy_direction, "r");

    while (fgets(temp_arr, MAX + 5, temp_copy) != NULL)
    {
        fputs(temp_arr, file_to_be_written);
    }

    fclose(temp_copy);
    fclose(file_to_be_written);

    remove(temp_copy_direction);

    printf("Succesfully Insterted!\n");
}

void cat()
{
    char print_str[MAX];
    char direction[MAX];
    memset(direction, 0, MAX);
    memset(print_str, 0, MAX);

    strcat(direction, command_tajzie[2]);

    FILE *file_to_be_read = fopen(direction, "r");

    while (fgets(print_str, MAX + 5, file_to_be_read) != NULL)
    {
        printf("%s", print_str);
    }

    putchar('\n');

    fclose(file_to_be_read);
}

void removestr()
{
    char direction[MAX];
    char temp_copy_direction[MAX];
    char temp_arr[MAX];
    long line;
    long char_pos;
    long size;
    long current_line = 1;
    long current_char_pos = 0;
    char *end_of_line_pos, *end_of_char_pos, *end_of_size;

    memset(direction, 0, MAX);
    memset(temp_copy_direction, 0, MAX);

    strcat(direction, command_tajzie[2]);
    strcat(strncat(temp_copy_direction, direction, strlen(direction) - 4), "_temp.txt");

    FILE *file_to_be_edited = fopen(direction, "r");
    FILE *temp_copy = fopen(temp_copy_direction, "w");

    line = strtol(command_tajzie[4], &end_of_line_pos, 10);
    char_pos = strtol(end_of_line_pos + 1, &end_of_char_pos, 10);
    size = strtol(command_tajzie[6], &end_of_size, 10);

    while (current_line < line)
    {
        fputs(fgets(temp_arr, MAX + 5, file_to_be_edited), temp_copy);
        current_line++;
    }

    while (current_char_pos < char_pos)
    {
        fputc(fgetc(file_to_be_edited), temp_copy);
        current_char_pos++;
    }

    if (strcmp(command_tajzie[7], "-f") == 0)
        fseek(file_to_be_edited, size, SEEK_CUR);
    else
    {
        fseek(temp_copy, -1 * size, SEEK_CUR);
        for (int i = 0; i < size; i++)
        {
            fputc(0, temp_copy);
        }
        fseek(temp_copy, -1 * size, SEEK_CUR);
    }

    while (fgets(temp_arr, MAX + 5, file_to_be_edited) != NULL)
    {
        fputs(temp_arr, temp_copy);
    }

    fclose(file_to_be_edited);
    fclose(temp_copy);

    file_to_be_edited = fopen(direction, "w");
    temp_copy = fopen(temp_copy_direction, "r");

    while (fgets(temp_arr, MAX + 5, temp_copy) != NULL)
    {
        fputs(temp_arr, file_to_be_edited);
    }

    fclose(file_to_be_edited);
    fclose(temp_copy);

    remove(temp_copy_direction);

    printf("Succesfully removed!\n");
}

void copystr()
{
    char direction[MAX];
    char temp_arr[MAX];
    long line;
    long char_pos;
    long size;
    long current_line = 1;
    long current_char_pos = 0;
    char *end_of_line_pos, *end_of_char_pos, *end_of_size;

    memset(direction, 0, MAX);
    memset(clipboard, 0, MAX);
    memset(temp_arr, 0, MAX);

    strcat(direction, command_tajzie[2]);

    FILE *file_to_be_copied = fopen(direction, "r");

    line = strtol(command_tajzie[4], &end_of_line_pos, 10);
    char_pos = strtol(end_of_line_pos + 1, &end_of_char_pos, 10);
    size = strtol(command_tajzie[6], &end_of_size, 10);

    while (current_line < line)
    {
        fgets(temp_arr, MAX + 5, file_to_be_copied);
        current_line++;
    }

    while (current_char_pos < char_pos)
    {
        fgetc(file_to_be_copied);
        current_char_pos++;
    }

    if (strcmp(command_tajzie[7], "-b") == 0)
        fseek(file_to_be_copied, -1*size, SEEK_CUR);
    
    for(int i=0; i<size; i++)
    {
        clipboard[i] = fgetc(file_to_be_copied);
    }

    fclose(file_to_be_copied);

    printf("Text \"%s\" has been succesfully copied!\n", clipboard);
}

void barrasi()
{
    if (strcmp(command_tajzie[0], "createfile") == 0)
        createfile();

    else if (strcmp(command_tajzie[0], "insertstr") == 0)
    {
        if (check_existance())
            insertstr();
    }

    else if (strcmp(command_tajzie[0], "cat") == 0)
    {
        if (check_existance())
            cat();
    }

    else if (strcmp(command_tajzie[0], "removestr") == 0)
    {
        if (check_existance())
            removestr();
    }

    else if (strcmp(command_tajzie[0], "copystr") == 0)
    {
        if (check_existance())
            copystr();
    }

    else if (strcmp(command_tajzie[0], "exit") == 0)
        return;

    else
        printf("Wrong command!\n");
}

int main()
{
    printf("Hi, Enter your command.\nAnd for ending the procces, enter \"exit\".\n");

    while (strcmp(command, "exit"))
    {
        clear();
        gets(command);
        tajzieh();
        barrasi();
    }

    printf("Have a good day!\n");

    return 0;
}