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

char command[500];
char command_tajzie[20][500];

void clear()
{
    memset(command, 0, 500);

    for (int i = 0; i < 20; i++)
    {
        memset(command_tajzie[i], 0, 500);
    }
}

void tajzieh()
{
    int i = 0, j = 0, k = 0;

    while (true)
    {
        command_tajzie[k][j] = command[i];
        i++;
        j++;

        if (command[i] == '\0')
            return;

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
        }
    }
}

void createfile()
{
    if (access(command_tajzie[2], F_OK) == 0)
    {
        printf("The file already exists and cannot be made again!");
        return;
    }

    else
    {
        char direction[500];
        memset(direction, 0, 500);
        strncpy(direction, command_tajzie[2], 3);

        int i = 3;
        while (command_tajzie[2][i] != '\0')
        {
            while (command_tajzie[2][i] != '/')
            {
                direction[i] = command_tajzie[2][i];
                i++;
            }
            mkdir(direction);
        }

        FILE *file_to_be_made = fopen(direction, "w");
        fclose(file_to_be_made);
    }

    return;
}

void barrasi()
{
    if (strcmp(command_tajzie[0], "creatfile") == 0)
    {
        createfile();
    }
    // else if ()
    // {
    // }
}

int main()
{
    while (strcmp(command, "exit"))
    {
        clear();
        gets(command);
        tajzieh();
        barrasi();
    }

    printf("Have a good day!");

    return 0;
}