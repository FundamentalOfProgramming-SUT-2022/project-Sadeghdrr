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

#define MAX 1000
#define ULTRA_MAX 100000

char command[MAX];
char command_tajzie[20][MAX];
char clipboard[MAX];
char temp_clipboard[MAX];

// -------------------------------------------------------------------------------------------------------------------------------------

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
                command_tajzie[k][j++] = '\n';
                i += 2;
                continue;
            }

            else if (command[i + 1] == 't')
            {
                command_tajzie[k][j++] = '\t';
                i += 2;
                continue;
            }

            else if (command[i + 1] == '"')
            {
                command_tajzie[k][j++] = '"';
                i += 2;
                continue;
            }

            else if (command[i + 1] == '*')
            {
                command_tajzie[k][j++] = '\\';
                command_tajzie[k][j++] = '*';
                i += 2;
                continue;
            }

            else if (command[i + 1] == '\\')
            {
                command_tajzie[k][j++] = '\\';
                i += 2;
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
                            command_tajzie[k][j++] = '\n';
                            i += 2;
                            continue;
                        }

                        else if (command[i + 1] == 't')
                        {
                            command_tajzie[k][j++] = '\t';
                            i += 2;
                            continue;
                        }

                        else if (command[i + 1] == '"')
                        {
                            command_tajzie[k][j++] = '"';
                            i += 2;
                            continue;
                        }

                        else if (command[i + 1] == '*')
                        {
                            command_tajzie[k][j++] = '\\';
                            command_tajzie[k][j++] = '*';
                            i += 2;
                            continue;
                        }

                        else if (command[i + 1] == '\\')
                        {
                            command_tajzie[k][j++] = '\\';
                            i += 2;
                            continue;
                        }
                    }

                    command_tajzie[k][j++] = command[i++];
                }
                i++;
            }

            i++;
            j = 0;
            k++;
            continue;
        }

        command_tajzie[k][j++] = command[i++];
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

int say_wild_pos(const char *search_string)
{
    int ans = 1;

    for (int i = 0; i < strlen(search_string); i++)
    {
        if (search_string[i] == ' ')
            ans++;
        else if (search_string[i] == '*')
            return ans;
    }
}

bool say_begin_astrisk(const char *search_string)
{
    for (int i = 0; i < strlen(search_string); i++)
    {
        if (search_string[i] == '*')
        {
            if (i == 0)
                return true;
            else if (search_string[i - 1] == ' ' || search_string[i - 1] == '\n')
                return true;
        }
    }

    return false;
}

void remove_backslash(char *search_string)
{
    char temp[MAX];
    memset(temp, 0, MAX);

    strcat(temp, search_string);

    int i = 0, j = 0;
    for (; j < strlen(search_string); i++, j++)
    {
        if (temp[j] == '\\' && temp[j + 1] == '*')
        {
            j++;
        }

        *(search_string + i) = temp[j];
    }

    *(search_string + i) = '\0';
}

void remove_asterisk(char *search_string)
{
    char temp[MAX];
    memset(temp, 0, MAX);

    strcat(temp, search_string);

    int i = 0, j = 0;
    for (; j < strlen(search_string); i++, j++)
    {
        if (temp[j] == '*')
        {
            j++;
        }

        *(search_string + i) = temp[j];
    }

    *(search_string + i) = '\0';
}

void put_to_array(FILE *file, char *array)
{
    char temp = fgetc(file);
    for (int i = 0; temp != EOF; i++)
    {
        *(array + i) = temp;
        temp = fgetc(file);
    }
}

void tajzieh_string(const char string[], char save_string[][MAX])
{
    int i = 0, j = 0, k = 0;
    while (true)
    {
        if (string[i] == '\0')
            return;

        else if (string[i] == ' ' || string[i] == '\n')
        {
            save_string[k][j] = '\0';
            i++;
            j = 0;
            k++;
        }

        save_string[k][j++] = string[i++];
    }
}

void swap(char* first, char *seccond)
{
    char temp;
    for(int i=0; i<MAX; i++)
    {
        temp = *(first + i);
        *(first + i) = *(seccond + i);
        *(seccond + i) = temp;
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------

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
    memset(temp_clipboard, 0, MAX);
    memset(temp_copy_direction, 0, MAX);

    strcat(direction, command_tajzie[2]);
    strcat(temp_clipboard, command_tajzie[4]);

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
    memset(temp_clipboard, 0, MAX);
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
    {
        for (int i = 0; i < size; i++)
        {
            temp_clipboard[i] = fgetc(file_to_be_edited);
        }
    }
    else
    {
        fseek(temp_copy, -1 * size, SEEK_CUR);
        fseek(file_to_be_edited, -1 * size, SEEK_CUR);
        for (int i = 0; i < size; i++)
        {
            temp_clipboard[i] = fgetc(file_to_be_edited);
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
        fseek(file_to_be_copied, -1 * size, SEEK_CUR);

    for (int i = 0; i < size; i++)
    {
        clipboard[i] = fgetc(file_to_be_copied);
    }

    fclose(file_to_be_copied);
}

void cutstr()
{
    memset(clipboard, 0, MAX);
    copystr();
    removestr();
}

void pastestr()
{
    strcat(command_tajzie[6], command_tajzie[4]);
    memset(command_tajzie[4], 0, MAX);
    strcat(command_tajzie[4], clipboard);
    insertstr();
}

void find()
{
    char direction[MAX];
    char search_string[MAX];
    char whole_text[ULTRA_MAX];
    char search_string_tajzie[10][MAX];
    char whole_text_tajzie[100][MAX];
    int all_repeats[MAX];
    int all_repeats_byword[MAX];
    int repeat_counter = 0;
    int pos_by_char = 0;
    int pos_by_word = 1;
    int current_word_pos_in_text = 1;
    int current_word_pos_in_search = 1;
    int wild_card_pos;
    bool wildcard = false;
    bool begin_astrisk = false;
    bool simularity = true;

    memset(direction, 0, MAX);
    memset(search_string, 0, MAX);
    memset(whole_text, 0, ULTRA_MAX);
    memset(all_repeats, -1, MAX * sizeof(int));
    memset(all_repeats_byword, -1, MAX * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        memset(search_string_tajzie[i], 0, MAX);
    }
    for (int i = 0; i < 50; i++)
    {
        memset(whole_text_tajzie[i], 0, MAX);
    }

    strcat(direction, command_tajzie[2]);
    strcat(search_string, command_tajzie[4]);

    FILE *file_to_be_seeked = fopen(direction, "r");

    for (int i = 0; i < strlen(search_string); i++)
    {
        if ((search_string[i] == '*' && i == 0) || (search_string[i] == '*' && search_string[i - 1] != '\\'))
        {
            wildcard = true;
            break;
        }
    }

    if (wildcard)
    {
        wild_card_pos = say_wild_pos(search_string);
        begin_astrisk = say_begin_astrisk(search_string);
    }

    else
    {
        remove_backslash(search_string);
    }

    remove_asterisk(search_string);
    put_to_array(file_to_be_seeked, whole_text);
    tajzieh_string(whole_text, whole_text_tajzie);
    tajzieh_string(search_string, search_string_tajzie);

    if (wildcard)
    {
        while (whole_text_tajzie[pos_by_word - 1][0] != '\0')
        {
            while (search_string_tajzie[current_word_pos_in_search - 1][0] != '\0')
            {
                if (current_word_pos_in_search == wild_card_pos)
                {
                    if (strlen(whole_text_tajzie[current_word_pos_in_text - 1]) < strlen(search_string_tajzie[current_word_pos_in_search - 1]))
                    {
                        simularity = false;
                        break;
                    }

                    if (begin_astrisk)
                    {
                        for (int i = 1; i <= strlen(search_string_tajzie[current_word_pos_in_search - 1]); i++)
                        {
                            if (whole_text_tajzie[current_word_pos_in_text - 1][strlen(whole_text_tajzie[current_word_pos_in_text - 1]) - i] !=
                                search_string_tajzie[current_word_pos_in_search - 1][strlen(search_string_tajzie[current_word_pos_in_search - 1]) - i])
                            {
                                simularity = false;
                                break;
                            }
                        }
                    }

                    else
                    {
                        for (int i = 0; i < strlen(search_string_tajzie[current_word_pos_in_search - 1]); i++)
                        {
                            if (whole_text_tajzie[current_word_pos_in_text - 1][i] != search_string_tajzie[current_word_pos_in_search - 1][i])
                            {
                                simularity = false;
                                break;
                            }
                        }
                    }
                }

                else
                {
                    if (strcmp(whole_text_tajzie[current_word_pos_in_text - 1], search_string_tajzie[current_word_pos_in_search - 1]) != 0)
                    {
                        simularity = false;
                        break;
                    }
                }

                current_word_pos_in_search++;
                current_word_pos_in_text++;
            }

            if (simularity)
            {
                all_repeats[repeat_counter] = pos_by_char;
                all_repeats_byword[repeat_counter] = pos_by_word;
                repeat_counter++;
            }

            simularity = true;
            pos_by_char += strlen(whole_text_tajzie[pos_by_word - 1]) + 1;
            pos_by_word++;
            current_word_pos_in_search = 1;
            current_word_pos_in_text = pos_by_word;
        }
    }

    else
    {
        while (whole_text_tajzie[pos_by_word - 1][0] != '\0')
        {
            while (search_string_tajzie[current_word_pos_in_search - 1][0] != '\0')
            {
                if (strcmp(whole_text_tajzie[current_word_pos_in_text - 1], search_string_tajzie[current_word_pos_in_search - 1]) != 0)
                {
                    simularity = false;
                    break;
                }

                current_word_pos_in_search++;
                current_word_pos_in_text++;
            }

            if (simularity)
            {
                all_repeats[repeat_counter] = pos_by_char;
                all_repeats_byword[repeat_counter] = pos_by_word;
                repeat_counter++;
            }

            simularity = true;
            pos_by_char += strlen(whole_text_tajzie[pos_by_word - 1]) + 1;
            pos_by_word++;
            current_word_pos_in_search = 1;
            current_word_pos_in_text = pos_by_word;
        }
    }

    fclose(file_to_be_seeked);

    // tarkibi
    // mojaz:   at-byword   byword-all
    if ((strcmp(command_tajzie[6], "-at") == 0 && strcmp(command_tajzie[5], "-byword") == 0) || (strcmp(command_tajzie[7], "-byword") == 0 && strcmp(command_tajzie[5], "-at") == 0))
    {
        long at_index;
        char *end_of_at_index;

        if (strcmp(command_tajzie[6], "-at") == 0)
        {
            at_index = strtol(command_tajzie[7], &end_of_at_index, 10);
        }

        else
        {
            at_index = strtol(command_tajzie[6], &end_of_at_index, 10);
        }

        if (all_repeats_byword[at_index - 1] == -1)
        {
            printf("The at_index value in invalid, and the \"find\" returned \"-1\"\n");
            return;
        }

        else
        {
            printf("The \"%d\" index of repeatation byword is \"%d\".\n", at_index, all_repeats_byword[at_index - 1]);
            return;
        }
    }

    else if ((strcmp(command_tajzie[6], "-all") == 0 && strcmp(command_tajzie[5], "-byword") == 0) || (strcmp(command_tajzie[6], "-byword") == 0 && strcmp(command_tajzie[5], "-all") == 0))
    {
        if (all_repeats_byword[0] == -1)
        {
            printf("The value hasn't been repeated at all, and the \"find\" returned \"-1\"\n");
            return;
        }

        else
        {
            printf("All values of repeatation (byword index):\n");
            for (int i = 0; all_repeats_byword[i] != -1; i++)
            {
                printf("%d ", all_repeats_byword[i]);
            }
            printf("\n");
        }
    }

    // simple
    else if (command_tajzie[5][0] == '\0')
    {
        if (all_repeats_byword[0] == -1)
        {
            printf("The value hasn't been repeated at all, and the \"find\" returned \"-1\"\n");
            return;
        }

        else
        {
            printf("The first repeatation is in \"%d\" char.\n", all_repeats[0]);
            return;
        }
    }

    else if (strcmp(command_tajzie[5], "-at") == 0 && command_tajzie[7][0] == '\0')
    {
        long at_index;
        char *end_of_at_index;

        at_index = strtol(command_tajzie[6], &end_of_at_index, 10);

        if (all_repeats_byword[at_index - 1] == -1)
        {
            printf("The at_index value in invalid, and the \"find\" returned \"-1\"\n");
            return;
        }

        else
        {
            printf("The \"%d\" index of repeatation byword is \"%d\".\n", at_index, all_repeats_byword[at_index - 1]);
            return;
        }
    }

    else if (strcmp(command_tajzie[5], "-byword") == 0 && command_tajzie[6][0] == '\0')
    {
        if (all_repeats_byword[0] == -1)
        {
            printf("The value hasn't been repeated at all, and the \"find\" returned \"-1\".\n");
            return;
        }

        else
        {
            printf("The first repeat was in \"%d\"th word.\n", all_repeats_byword[0]);
        }
    }

    else if (strcmp(command_tajzie[5], "-count") == 0 && command_tajzie[6][0] == '\0')
    {
        int tedad_repeatation = 0;
        int i = 0;

        while (all_repeats[i] != -1)
        {
            tedad_repeatation++;
            i++;
        }

        printf("The count of repeatation is \"%d\".\n", tedad_repeatation);
        return;
    }

    else if (strcmp(command_tajzie[5], "-all") == 0 && command_tajzie[6][0] == '\0')
    {
        if (all_repeats[0] == -1)
        {
            printf("The value hasn't been repeated at all, and the \"find\" returned \"-1\"\n");
            return;
        }

        else
        {
            printf("All values of repeatation:\n");

            for (int i = 0; all_repeats[i] != -1; i++)
            {
                printf("%d ", all_repeats[i]);
            }

            printf("\n");

            return;
        }
    }

    else
    {
        printf("Not a accessable combination!\n");
        return;
    }
}

void barrasi()
{
    if (strcmp(command_tajzie[0], "createfile") == 0)
    {
        createfile();
    }

    else if (strcmp(command_tajzie[0], "insertstr") == 0)
    {
        if (check_existance())
        {
            insertstr();
            printf("Succesfully \"%s\" Insterted!\n", temp_clipboard);
        }
    }

    else if (strcmp(command_tajzie[0], "cat") == 0)
    {
        if (check_existance())
            cat();
    }

    else if (strcmp(command_tajzie[0], "removestr") == 0)
    {
        if (check_existance())
        {
            removestr();
            printf("Succesfully \"%s\" removed!\n", temp_clipboard);
        }
    }

    else if (strcmp(command_tajzie[0], "copystr") == 0)
    {
        if (check_existance())
        {
            copystr();
            printf("Text \"%s\" has been succesfully copied!\n", clipboard);
        }
    }

    else if (strcmp(command_tajzie[0], "cutstr") == 0)
    {
        if (check_existance())
        {
            cutstr();
            printf("Text \"%s\" has been succesfully cut!\n", clipboard);
        }
    }

    else if (strcmp(command_tajzie[0], "pastestr") == 0)
    {
        if (check_existance())
        {
            pastestr();
            printf("Text \"%s\" has been succesfully pasted!\n", clipboard);
        }
    }

    else if (strcmp(command_tajzie[0], "find") == 0)
    {
        swap(command_tajzie[1], command_tajzie[3]);
        swap(command_tajzie[2], command_tajzie[4]);
        if (check_existance())
            find();
    }

    else if (strcmp(command_tajzie[0], "exit") == 0)
    {
        printf("Have a good day!\n");
        return;
    }

    else
        printf("Wrong command!\n");
}

// ------------------------------------------------------------------------------------------------------------------------------------

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

    return 0;
}