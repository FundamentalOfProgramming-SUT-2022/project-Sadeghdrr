#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>
#include <conio.h>
#include <dir.h>
#include <dirent.h>
#include <windows.h>
#include <winuser.h>
#include "rlutil.h"

#define MAX 1000
#define ULTRA_MAX 100000

char command[MAX];
char command_tajzie[30][MAX];
char clipboard[MAX];
char temp_clipboard[MAX];
char output[ULTRA_MAX];
int temp_adad;

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

bool check_existance()
{
    char direction[MAX];
    char temp[MAX];
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
                sprintf(temp, "The \"%s\" is a wrong address!", direction);
                strcat(output, temp);
                return false;
            }
        }
    }

    if (access(direction, F_OK) != 0)
    {
        sprintf(temp, "The \"%s\" file doesn't exists!", direction);
        strcat(output, temp);
        return false;
    }

    return true;
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

void swap(char *first, char *seccond)
{
    char temp;
    for (int i = 0; i < MAX; i++)
    {
        temp = *(first + i);
        *(first + i) = *(seccond + i);
        *(seccond + i) = temp;
    }
}

void undo_backup()
{
    char whole_text_backup[ULTRA_MAX];
    char backuped_file_direction[MAX];

    memset(whole_text_backup, 0, ULTRA_MAX);
    memset(backuped_file_direction, 0, MAX);

    strcat(backuped_file_direction, command_tajzie[2]);
    backuped_file_direction[3] = 'u';
    backuped_file_direction[4] = 'n';
    backuped_file_direction[5] = 'd';
    backuped_file_direction[6] = 'o';

    FILE *file_to_be_backuped = fopen(command_tajzie[2], "r");
    FILE *backuped_file = fopen(backuped_file_direction, "w");

    put_to_array(file_to_be_backuped, whole_text_backup);

    fputs(whole_text_backup, backuped_file);

    fclose(file_to_be_backuped);
    fclose(backuped_file);
}

bool brace_counter()
{
    int brace_difference = 0;
    char whole_text[ULTRA_MAX];

    memset(whole_text, 0, ULTRA_MAX);

    FILE *file_to_be_edited = fopen(command_tajzie[2], "r");
    put_to_array(file_to_be_edited, whole_text);
    fclose(file_to_be_edited);

    for (int i = 0; whole_text[i] != '\0'; i++)
    {
        if (whole_text[i] == '{')
            brace_difference++;
        else if (whole_text[i] == '}')
            brace_difference--;
    }

    if (brace_difference == 0)
    {
        return true;
    }
    else
    {
        strcat(output, "The braces in text aren't equal!");
        return false;
    }
}

void clean(char *whole_text)
{
    char cleaned_whole_text[ULTRA_MAX];
    memset(cleaned_whole_text, 0, ULTRA_MAX);

    for (int i = 0, j = 0; whole_text[i] != '\0'; i++, j++)
    {
        if (isspace(whole_text[i]))
        {
            int k = i;
            while (isspace(whole_text[k]))
            {
                k++;
            }

            if (whole_text[k] == '{')
            {
                k++;

                cleaned_whole_text[j] = ' ';
                cleaned_whole_text[++j] = '{';

                if (isspace(whole_text[k]))
                {
                    while (isspace(whole_text[k]))
                    {
                        k++;
                    }

                    cleaned_whole_text[++j] = whole_text[k];
                }

                else
                {
                    cleaned_whole_text[++j] = whole_text[k];
                }

                i = k;

                continue;
            }

            else
            {
                while (i < k)
                {
                    cleaned_whole_text[j++] = whole_text[i++];
                }
                cleaned_whole_text[j] = whole_text[i];

                continue;
            }
        }

        else if (whole_text[i] == '{' && whole_text[i - 1] != '{' && whole_text[i - 1] != '}')
        {
            cleaned_whole_text[j] = ' ';
            cleaned_whole_text[++j] = '{';

            continue;
        }

        cleaned_whole_text[j] = whole_text[i];
    }

    memset(whole_text, 0, ULTRA_MAX);
    strcat(whole_text, cleaned_whole_text);
}

int line_counter(char *direction)
{
    int lines = 1;

    FILE *file = fopen(direction, "r");

    char tmp = fgetc(file);

    while (tmp != EOF)
    {
        if (tmp == '\n')
            lines++;

        tmp = fgetc(file);
    }

    fclose(file);

    return lines;
}

void print_output()
{
    if (output[strlen(output) - 1] != '\n')
        output[strlen(output) - 1] = '\n';
    printf("%s", output);
}

bool dash_file()
{
    for (int i = 0; command_tajzie[i][0] != '\0'; i++)
        if (strcmp(command_tajzie[i], "--file") == 0)
            return true;

    return false;
}

char *name_of(char *direction)
{
    int len = strlen(direction);
    char *file_name = calloc(MAX, sizeof(char));

    int i = len;
    while (direction[i] != '/')
    {
        i--;
    }
    i++;

    for (int j = 0; direction[i + j] != '\0'; j++)
    {
        file_name[j] = direction[i + j];
    }

    return file_name;
}

bool saved_check(char *direction_in_root, char *direction)
{
    FILE *file = fopen(direction, "r");
    FILE *file_in_root = fopen(direction_in_root, "r");

    char temp[MAX];
    char temp2[MAX];

    while (fgets(temp, MAX, file) != NULL && fgets(temp2, MAX, file_in_root) != NULL)
    {
        if (strcmp(temp2, temp))
            return false;

        if (feof(file) && !feof(file_in_root))
            return false;
        else if (feof(file_in_root) && !feof(file))
            return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------------------------------------------

void createfile()
{
    char direction[MAX];
    char temp[MAX];
    memset(direction, 0, MAX);

    if (access(command_tajzie[2], F_OK) == 0)
    {
        sprintf(temp, "The file already exists and cannot be made again!\n");
        strcat(output, temp);
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

        FILE *file_to_be_made = fopen(direction, "a");
        fclose(file_to_be_made);
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
        strcat(output, print_str);
    }

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

void find(char search_string[MAX])
{
    char direction[MAX];
    char whole_text[ULTRA_MAX];
    int all_repeats[MAX];
    int all_repeats_byword[MAX];
    char temp[MAX];
    int repeat_counter = 0;
    int pos_by_char = 0;
    int pos_by_word = 1;
    int current_char_pos_in_text = 0;
    int current_char_pos_in_search = 0;
    int wild_card_pos;
    bool wildcard = false;
    bool begin_astrisk = false;
    bool simularity = true;

    memset(direction, 0, MAX);
    memset(whole_text, 0, ULTRA_MAX);
    memset(all_repeats, -1, MAX * sizeof(int));
    memset(all_repeats_byword, -1, MAX * sizeof(int));

    strcat(direction, command_tajzie[2]);

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
        remove_asterisk(search_string);
    }

    else
    {
        remove_backslash(search_string);
    }

    put_to_array(file_to_be_seeked, whole_text);

    fclose(file_to_be_seeked);

    if (wildcard)
    {
        sprintf(temp, "The wildcard isn't available yet!\n");
        strcat(output, temp);
        return;
    }

    else
    {
        while (whole_text[pos_by_char] != '\0')
        {
            if (whole_text[current_char_pos_in_text] == search_string[current_char_pos_in_search])
            {
                while (search_string[current_char_pos_in_search] != '\0')
                {
                    if (search_string[current_char_pos_in_search] != whole_text[current_char_pos_in_text])
                    {
                        simularity = false;
                        break;
                    }
                    current_char_pos_in_search++;
                    current_char_pos_in_text++;
                }

                if (simularity)
                {
                    all_repeats[repeat_counter] = pos_by_char;
                    all_repeats_byword[repeat_counter] = pos_by_word;
                    repeat_counter++;
                    for (int i = pos_by_char; i <= current_char_pos_in_text; i++)
                    {
                        if (whole_text[i] == ' ' || whole_text[i] == '\n')
                            pos_by_word++;
                    }
                    pos_by_char = current_char_pos_in_text;
                    current_char_pos_in_search = 0;
                }

                else
                {
                    simularity = true;
                    pos_by_char++;
                    if (whole_text[pos_by_char] == ' ' || whole_text[pos_by_char] == '\n')
                        pos_by_word++;
                    current_char_pos_in_text = pos_by_char;
                    current_char_pos_in_search = 0;
                }
            }

            else
            {
                pos_by_char++;
                if (whole_text[pos_by_char] == ' ' || whole_text[pos_by_char] == '\n')
                    pos_by_word++;
                current_char_pos_in_text = pos_by_char;
                current_char_pos_in_search = 0;
            }
        }
    }

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
            sprintf(temp, "The at_index value in invalid, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The \"%d\" index of repeatation byword is \"%d\".", at_index, all_repeats_byword[at_index - 1]);
            strcat(output, temp);
            return;
        }
    }

    else if ((strcmp(command_tajzie[6], "-all") == 0 && strcmp(command_tajzie[5], "-byword") == 0) || (strcmp(command_tajzie[6], "-byword") == 0 && strcmp(command_tajzie[5], "-all") == 0))
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            strcat(output, "All values of repeatation (byword index):\n");
            for (int i = 0; all_repeats_byword[i] != -1; i++)
            {
                sprintf(temp, "%d ", all_repeats_byword[i]);
                strcat(output, temp);
            }
        }
    }

    // simple
    else if (command_tajzie[5][0] == '\0' || command_tajzie[5][0] == '=')
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The first repeatation is in \"%d\" char.", all_repeats[0]);
            strcat(output, temp);
            return;
        }
    }

    else if (strcmp(command_tajzie[5], "-at") == 0 && (command_tajzie[7][0] == '\0' || command_tajzie[7][0] == '='))
    {
        long at_index;
        char *end_of_at_index;

        at_index = strtol(command_tajzie[6], &end_of_at_index, 10);

        if (all_repeats_byword[at_index - 1] == -1)
        {
            sprintf(temp, "The at_index value in invalid, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The \"%d\" index of repeatation is in \"%d\" char.", at_index, all_repeats[at_index - 1]);
            strcat(output, temp);
            return;
        }
    }

    else if (strcmp(command_tajzie[5], "-byword") == 0 && (command_tajzie[6][0] == '\0' || command_tajzie[6][0] == '='))
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\".");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The first repeat was in \"%d\"th word.", all_repeats_byword[0]);
            strcat(output, temp);
        }
    }

    else if (strcmp(command_tajzie[5], "-count") == 0 && (command_tajzie[6][0] == '\0' || command_tajzie[6][0] == '='))
    {
        int tedad_repeatation = 0;
        int i = 0;

        while (all_repeats[i] != -1)
        {
            tedad_repeatation++;
            i++;
        }

        sprintf(temp, "The count of repeatation is \"%d\".", tedad_repeatation);
        strcat(output, temp);
        return;
    }

    else if (strcmp(command_tajzie[5], "-all") == 0 && (command_tajzie[6][0] == '\0' || command_tajzie[6][0] == '='))
    {
        if (all_repeats[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            strcat(output, "All values of repeatation:\n");

            for (int i = 0; all_repeats[i] != -1; i++)
            {
                sprintf(temp, "%d ", all_repeats[i]);
                strcat(output, temp);
            }

            return;
        }
    }

    else
    {
        strcat(output, "Not a accessable combination!");
        return;
    }
}

void find_infile(char search_string[MAX])
{
    char direction[MAX];
    char whole_text[ULTRA_MAX];
    int all_repeats[MAX];
    int all_repeats_byword[MAX];
    char temp[MAX];
    int repeat_counter = 0;
    int pos_by_char = 0;
    int pos_by_word = 1;
    int current_char_pos_in_text = 0;
    int current_char_pos_in_search = 0;
    int wild_card_pos;
    bool wildcard = false;
    bool begin_astrisk = false;
    bool simularity = true;

    memset(direction, 0, MAX);
    memset(whole_text, 0, ULTRA_MAX);
    memset(all_repeats, -1, MAX * sizeof(int));
    memset(all_repeats_byword, -1, MAX * sizeof(int));

    strcat(direction, command_tajzie[2]);

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
        remove_asterisk(search_string);
    }

    else
    {
        remove_backslash(search_string);
    }

    put_to_array(file_to_be_seeked, whole_text);

    fclose(file_to_be_seeked);

    if (wildcard)
    {
        sprintf(temp, "The wildcard isn't available yet!\n");
        strcat(output, temp);
        return;
    }

    else
    {
        while (whole_text[pos_by_char] != '\0')
        {
            if (whole_text[current_char_pos_in_text] == search_string[current_char_pos_in_search])
            {
                while (search_string[current_char_pos_in_search] != '\0')
                {
                    if (search_string[current_char_pos_in_search] != whole_text[current_char_pos_in_text])
                    {
                        simularity = false;
                        break;
                    }
                    current_char_pos_in_search++;
                    current_char_pos_in_text++;
                }

                if (simularity)
                {
                    all_repeats[repeat_counter] = pos_by_char;
                    all_repeats_byword[repeat_counter] = pos_by_word;
                    repeat_counter++;
                    for (int i = pos_by_char; i <= current_char_pos_in_text; i++)
                    {
                        if (whole_text[i] == ' ' || whole_text[i] == '\n')
                            pos_by_word++;
                    }
                    pos_by_char = current_char_pos_in_text;
                    current_char_pos_in_search = 0;
                }

                else
                {
                    simularity = true;
                    pos_by_char++;
                    if (whole_text[pos_by_char] == ' ' || whole_text[pos_by_char] == '\n')
                        pos_by_word++;
                    current_char_pos_in_text = pos_by_char;
                    current_char_pos_in_search = 0;
                }
            }

            else
            {
                pos_by_char++;
                if (whole_text[pos_by_char] == ' ' || whole_text[pos_by_char] == '\n')
                    pos_by_word++;
                current_char_pos_in_text = pos_by_char;
                current_char_pos_in_search = 0;
            }
        }
    }

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
            sprintf(temp, "The at_index value in invalid, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The \"%d\" index of repeatation byword is \"%d\".", at_index, all_repeats_byword[at_index - 1]);
            strcat(output, temp);
            return;
        }
    }

    else if ((strcmp(command_tajzie[6], "-all") == 0 && strcmp(command_tajzie[5], "-byword") == 0) || (strcmp(command_tajzie[6], "-byword") == 0 && strcmp(command_tajzie[5], "-all") == 0))
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            strcat(output, "All values of repeatation (byword index):\n");
            for (int i = 0; all_repeats_byword[i] != -1; i++)
            {
                sprintf(temp, "%d ", all_repeats_byword[i]);
                strcat(output, temp);
            }
        }
    }

    // simple
    else if (command_tajzie[5][0] == '\0' || command_tajzie[5][0] == '=')
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The first repeatation is in \"%d\" char.", all_repeats[0]);
            strcat(output, temp);
            return;
        }
    }

    else if (strcmp(command_tajzie[5], "-at") == 0 && (command_tajzie[7][0] == '\0' || command_tajzie[7][0] == '='))
    {
        long at_index;
        char *end_of_at_index;

        at_index = strtol(command_tajzie[6], &end_of_at_index, 10);

        if (all_repeats_byword[at_index - 1] == -1)
        {
            sprintf(temp, "The at_index value in invalid, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The \"%d\" index of repeatation is in \"%d\" char.", at_index, all_repeats[at_index - 1]);
            strcat(output, temp);
            return;
        }
    }

    else if (strcmp(command_tajzie[5], "-byword") == 0 && (command_tajzie[6][0] == '\0' || command_tajzie[6][0] == '='))
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\".");
            strcat(output, temp);
            return;
        }

        else
        {
            sprintf(temp, "The first repeat was in \"%d\"th word.", all_repeats_byword[0]);
            strcat(output, temp);
        }
    }

    else if (strcmp(command_tajzie[5], "-count") == 0 && (command_tajzie[6][0] == '\0' || command_tajzie[6][0] == '='))
    {
        int tedad_repeatation = 0;
        int i = 0;

        while (all_repeats[i] != -1)
        {
            tedad_repeatation++;
            i++;
        }

        sprintf(temp, "The count of repeatation is \"%d\".", tedad_repeatation);
        strcat(output, temp);
        return;
    }

    else if (strcmp(command_tajzie[5], "-all") == 0 && (command_tajzie[6][0] == '\0' || command_tajzie[6][0] == '='))
    {
        if (all_repeats[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"find\" returned \"-1\"");
            strcat(output, temp);
            return;
        }

        else
        {
            strcat(output, "All values of repeatation:\n");

            for (int i = 0; all_repeats[i] != -1; i++)
            {
                sprintf(temp, "%d ", all_repeats[i]);
                strcat(output, temp);
            }

            return;
        }
    }

    else
    {
        strcat(output, "Not a accessable combination!");
        return;
    }
}

void replace()
{
    // find
    char direction[MAX];
    char search_string[MAX];
    char whole_text[ULTRA_MAX];
    int all_repeats[MAX];
    int all_repeats_byword[MAX];
    char temp[MAX];
    int repeat_counter = 0;
    int pos_by_char = 0;
    int pos_by_word = 1;
    int current_char_pos_in_text = 0;
    int current_char_pos_in_search = 0;
    int wild_card_pos;
    bool wildcard = false;
    bool begin_astrisk = false;
    bool simularity = true;

    memset(direction, 0, MAX);
    memset(search_string, 0, MAX);
    memset(whole_text, 0, ULTRA_MAX);
    memset(all_repeats, -1, MAX * sizeof(int));
    memset(all_repeats_byword, -1, MAX * sizeof(int));

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
        remove_asterisk(search_string);
    }

    else
    {
        remove_backslash(search_string);
    }

    put_to_array(file_to_be_seeked, whole_text);

    fclose(file_to_be_seeked);

    if (wildcard)
    {
        sprintf(temp, "The wildcard isn't available yet!\n");
        strcat(output, temp);
        return;
    }

    else
    {
        while (whole_text[pos_by_char] != '\0')
        {
            if (whole_text[current_char_pos_in_text] == search_string[current_char_pos_in_search])
            {
                while (search_string[current_char_pos_in_search] != '\0')
                {
                    if (search_string[current_char_pos_in_search] != whole_text[current_char_pos_in_text])
                    {
                        simularity = false;
                        break;
                    }
                    current_char_pos_in_search++;
                    current_char_pos_in_text++;
                }

                if (simularity)
                {
                    all_repeats[repeat_counter] = pos_by_char;
                    all_repeats_byword[repeat_counter] = pos_by_word;
                    repeat_counter++;
                    for (int i = pos_by_char; i <= current_char_pos_in_text; i++)
                    {
                        if (whole_text[i] == ' ' || whole_text[i] == '\n')
                            pos_by_word++;
                    }
                    pos_by_char = current_char_pos_in_text;
                    current_char_pos_in_search = 0;
                }

                else
                {
                    simularity = true;
                    pos_by_char++;
                    if (whole_text[pos_by_char] == ' ' || whole_text[pos_by_char] == '\n')
                        pos_by_word++;
                    current_char_pos_in_text = pos_by_char;
                    current_char_pos_in_search = 0;
                }
            }

            else
            {
                pos_by_char++;
                if (whole_text[pos_by_char] == ' ' || whole_text[pos_by_char] == '\n')
                    pos_by_word++;
                current_char_pos_in_text = pos_by_char;
                current_char_pos_in_search = 0;
            }
        }
    }

    // replacing
    if (command_tajzie[5][0] == '\0')
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"replace\" returned \"-1\"\n");
            strcat(output, temp);
            return;
        }

        else
        {
            temp_adad = all_repeats[0];

            // removestr
            int line_pos = 1;
            int char_pos = 0;
            char buffer[MAX];
            int size_for_removing;

            memset(buffer, 0, MAX);

            char_pos = all_repeats[0];
            size_for_removing = strlen(search_string);

            strcat(command_tajzie[9], "--pos");
            sprintf(buffer, "%d:%d", line_pos, char_pos);
            strcat(command_tajzie[10], buffer);

            strcat(command_tajzie[11], "-size");
            memset(buffer, 0, MAX);
            sprintf(buffer, "%d", size_for_removing);
            strcat(command_tajzie[12], buffer);

            strcat(command_tajzie[13], "-f");

            swap(command_tajzie[3], command_tajzie[9]);
            swap(command_tajzie[4], command_tajzie[10]);
            swap(command_tajzie[5], command_tajzie[11]);
            swap(command_tajzie[6], command_tajzie[12]);
            swap(command_tajzie[7], command_tajzie[13]);
            swap(command_tajzie[8], command_tajzie[14]);

            removestr();

            // insertstr
            swap(command_tajzie[3], command_tajzie[5]);
            swap(command_tajzie[4], command_tajzie[6]);
            swap(command_tajzie[13], command_tajzie[3]);
            swap(command_tajzie[14], command_tajzie[4]);

            insertstr();
        }
    }

    else if (strcmp(command_tajzie[5], "-at") == 0 && strcmp(command_tajzie[9], "-all") != 0)
    {
        long at_index;
        char *end_of_at_index;

        at_index = strtol(command_tajzie[6], &end_of_at_index, 10);

        if (all_repeats_byword[at_index - 1] == -1)
        {
            sprintf(temp, "The at_index value in invalid, and the \"replace\" returned \"-1\"\n");
            strcat(output, temp);
            return;
        }

        else
        {
            temp_adad = all_repeats[0];

            // removestr
            int line_pos = 1;
            int char_pos = 0;
            char temp[MAX];
            int size_for_removing;

            memset(temp, 0, MAX);

            char_pos = all_repeats[at_index - 1];
            size_for_removing = strlen(search_string);

            strcat(command_tajzie[9], "--pos");
            sprintf(temp, "%d:%d", line_pos, char_pos);
            strcat(command_tajzie[10], temp);

            strcat(command_tajzie[11], "-size");
            memset(temp, 0, MAX);
            sprintf(temp, "%d", size_for_removing);
            strcat(command_tajzie[12], temp);

            strcat(command_tajzie[13], "-f");

            swap(command_tajzie[3], command_tajzie[9]);
            swap(command_tajzie[4], command_tajzie[10]);
            swap(command_tajzie[5], command_tajzie[11]);
            swap(command_tajzie[6], command_tajzie[12]);
            swap(command_tajzie[7], command_tajzie[13]);
            swap(command_tajzie[8], command_tajzie[14]);

            removestr();

            // insertstr
            swap(command_tajzie[3], command_tajzie[5]);
            swap(command_tajzie[4], command_tajzie[6]);
            swap(command_tajzie[13], command_tajzie[3]);
            swap(command_tajzie[14], command_tajzie[4]);

            insertstr();
        }
    }

    else if (strcmp(command_tajzie[5], "-all") == 0 && strcmp(command_tajzie[6], "-at") != 0)
    {
        if (all_repeats_byword[0] == -1)
        {
            sprintf(temp, "The value hasn't been repeated at all, and the \"replace\" returned \"-1\"\n");
            strcat(output, temp);
            return;
        }

        else
        {
            temp_adad = all_repeats[0];

            // removestr
            int line_pos = 1;
            int char_pos = 0;
            int deleted_counter = 0;
            char temp[MAX];
            int size_for_removing;

            for (int x = 0; all_repeats_byword[x] != -1; x++)
            {
                line_pos = 1;
                char_pos = all_repeats[x] - deleted_counter;

                memset(whole_text, 0, ULTRA_MAX);
                memset(temp, 0, MAX);

                fseek(file_to_be_seeked, 0, SEEK_SET);
                put_to_array(file_to_be_seeked, whole_text);

                size_for_removing = strlen(search_string);

                memset(command_tajzie[9], 0, MAX);
                memset(command_tajzie[10], 0, MAX);
                strcat(command_tajzie[9], "--pos");
                sprintf(temp, "%d:%d", line_pos, char_pos);
                strcat(command_tajzie[10], temp);

                memset(command_tajzie[11], 0, MAX);
                memset(command_tajzie[12], 0, MAX);
                memset(temp, 0, MAX);
                strcat(command_tajzie[11], "-size");
                sprintf(temp, "%d", size_for_removing);
                strcat(command_tajzie[12], temp);

                memset(command_tajzie[13], 0, MAX);
                strcat(command_tajzie[13], "-f");

                swap(command_tajzie[3], command_tajzie[9]);
                swap(command_tajzie[4], command_tajzie[10]);
                swap(command_tajzie[5], command_tajzie[11]);
                swap(command_tajzie[6], command_tajzie[12]);
                swap(command_tajzie[7], command_tajzie[13]);
                swap(command_tajzie[8], command_tajzie[14]);

                removestr();

                // insertstr
                swap(command_tajzie[3], command_tajzie[5]);
                swap(command_tajzie[4], command_tajzie[6]);
                swap(command_tajzie[13], command_tajzie[3]);
                swap(command_tajzie[14], command_tajzie[4]);

                insertstr();

                deleted_counter += size_for_removing - strlen(command_tajzie[4]);

                swap(command_tajzie[9], command_tajzie[3]);
                swap(command_tajzie[10], command_tajzie[4]);
                swap(command_tajzie[11], command_tajzie[5]);
                swap(command_tajzie[12], command_tajzie[6]);
                swap(command_tajzie[9], command_tajzie[7]);
                swap(command_tajzie[10], command_tajzie[8]);
            }

            fclose(file_to_be_seeked);
        }
    }

    else
    {
        sprintf(temp, "Not a accessable combination!\n");
        strcat(output, temp);
        return;
    }
}

int grep(int condition)
{
    char direction[MAX];
    char temp_arr[MAX];
    char search_string[MAX];
    int c_option_counter = 0;

    memset(direction, 0, MAX);
    memset(temp_arr, 0, MAX);
    memset(search_string, 0, MAX);

    char temp[MAX];

    strcat(direction, command_tajzie[2]);
    strcat(search_string, command_tajzie[4]);

    FILE *file_to_seeked = fopen(direction, "r");

    // no_option
    if (condition == 0)
    {
        while (fgets(temp_arr, MAX + 5, file_to_seeked) != NULL)
        {
            if (strstr(temp_arr, search_string) != NULL)
            {
                sprintf(temp, "%s: %s", direction, temp_arr);
                strcat(output, temp);

                if (temp_arr[strlen(temp_arr) - 1] != '\n')
                    strcat(output, "\n");
            }
        }
    }

    // c_option
    else if (condition == 1)
    {
        while (fgets(temp_arr, MAX + 5, file_to_seeked) != NULL)
        {
            if (strstr(temp_arr, search_string) != NULL)
                c_option_counter++;
        }
    }

    // l_option
    else if (condition == 2)
    {
        while (fgets(temp_arr, MAX + 5, file_to_seeked) != NULL)
        {
            if (strstr(temp_arr, search_string) != NULL)
            {
                sprintf(temp, "%s\n", direction);
                strcat(output, temp);
                return c_option_counter;
            }
        }
    }

    fclose(file_to_seeked);
    return c_option_counter;
}

void undo()
{
    char whole_text_of_backuped_file[ULTRA_MAX];
    char backuped_file_direction[MAX];

    memset(whole_text_of_backuped_file, 0, ULTRA_MAX);
    memset(backuped_file_direction, 0, MAX);

    strcat(backuped_file_direction, command_tajzie[2]);
    backuped_file_direction[3] = 'u';
    backuped_file_direction[4] = 'n';
    backuped_file_direction[5] = 'd';
    backuped_file_direction[6] = 'o';

    FILE *file_to_be_undone = fopen(command_tajzie[2], "w");
    FILE *backuped_file = fopen(backuped_file_direction, "r");

    put_to_array(backuped_file, whole_text_of_backuped_file);

    fputs(whole_text_of_backuped_file, file_to_be_undone);

    fclose(file_to_be_undone);
    fclose(backuped_file);
}

void auto_indent()
{
    char whole_text[ULTRA_MAX];
    char whole_text_indented[ULTRA_MAX];
    int tab_counter = 0;

    memset(whole_text, 0, ULTRA_MAX);
    memset(whole_text_indented, 0, ULTRA_MAX);

    FILE *file_to_be_edited = fopen(command_tajzie[2], "r");
    put_to_array(file_to_be_edited, whole_text);
    fclose(file_to_be_edited);

    clean(whole_text);

    for (int i = 0, j = 0; whole_text[i] != '\0'; i++, j++)
    {
        if (whole_text[i] == '{')
        {
            whole_text_indented[j] = '{';
            whole_text_indented[++j] = '\n';

            tab_counter++;

            for (int k = 0; k < tab_counter; k++)
            {
                whole_text_indented[++j] = '\t';
            }

            continue;
        }

        else if (whole_text[i] == '}')
        {
            tab_counter--;

            int k = i - 1;
            while (isspace(whole_text[k]))
            {
                k--;
            }

            if (whole_text[k] != '}')
            {
                whole_text_indented[j] = '\n';
                for (int k = 0; k < tab_counter; k++)
                {
                    whole_text_indented[++j] = '\t';
                }
                whole_text_indented[++j] = '}';
                whole_text_indented[++j] = '\n';
                for (int k = 0; k < tab_counter; k++)
                {
                    whole_text_indented[++j] = '\t';
                }
            }

            else
            {
                whole_text_indented[--j] = '}';
                whole_text_indented[++j] = '\n';

                for (int k = 0; k < tab_counter; k++)
                {
                    whole_text_indented[++j] = '\t';
                }
            }

            continue;
        }

        whole_text_indented[j] = whole_text[i];
    }

    file_to_be_edited = fopen(command_tajzie[2], "w");
    fputs(whole_text_indented, file_to_be_edited);
    fclose(file_to_be_edited);

    printf("File has been succesfully edited!\n");
}

void compare(int file1_lines_num, int file2_lines_num)
{
    char file1_line[MAX];
    char file2_line[MAX];
    char temp[MAX];
    int current_line = 1;

    memset(file1_line, 0, MAX);
    memset(file2_line, 0, MAX);

    FILE *file1 = fopen(command_tajzie[2], "r");
    FILE *file2 = fopen(command_tajzie[3], "r");

    while ((fgets(file1_line, MAX + 5, file1) != NULL) && (fgets(file2_line, MAX + 5, file2) != NULL))
    {
        if (strcmp(file1_line, file2_line))
        {
            sprintf(temp, "=============== #%d ===============\n", current_line);
            strcat(output, temp);

            if (current_line == file1_lines_num)
                sprintf(temp, "%s\n%s\n", file1_line, file2_line);
            else if (current_line == file2_lines_num)
                sprintf(temp, "%s%s\n\n", file1_line, file2_line);
            else
                sprintf(temp, "%s%s\n", file1_line, file2_line);

            strcat(output, temp);
        }

        current_line++;
    }

    if ((file1_lines_num - file2_lines_num) > 0)
    {
        sprintf(temp, "<<<<<<<<<<<<<<< #%d - #%d >>>>>>>>>>>>>>>\n", current_line, file1_lines_num);
        strcat(output, temp);

        do
        {
            sprintf(temp, "%s", file1_line);
            strcat(output, temp);
        } while (fgets(file1_line, MAX + 5, file1) != NULL);

        sprintf(temp, "\n\n");
        strcat(output, temp);
    }

    else if ((file1_lines_num - file2_lines_num) < 0)
    {
        sprintf(temp, ">>>>>>>>>>>>>>> #%d - #%d <<<<<<<<<<<<<<<\n", current_line, file2_lines_num);
        strcat(output, temp);

        while (fgets(file2_line, MAX + 5, file2) != NULL)
        {
            sprintf(temp, "%s", file2_line);
            strcat(output, temp);
        }

        sprintf(temp, "\n\n");
        strcat(output, temp);
    }

    fclose(file1);
    fclose(file2);
}

void full_tree(char *current_path, int current_depth)
{
    char next_path[MAX];
    char temp[MAX];
    memset(next_path, 0, MAX);

    struct dirent *current_object;
    DIR *folder = opendir(current_path);

    if (!folder)
        return;

    while ((current_object = readdir(folder)) != NULL)
    {
        if (strcmp(current_object->d_name, ".") && strcmp(current_object->d_name, ".."))
        {
            for (int i = 0; i < 2 * current_depth; i++)
            {
                if (i % 2 == 0)
                {
                    sprintf(temp, "|");
                    strcat(output, temp);
                }
                else
                {
                    sprintf(temp, " ");
                    strcat(output, temp);
                }
            }

            sprintf(temp, "|__%s\n", current_object->d_name);
            strcat(output, temp);

            strcpy(next_path, current_path);
            strcat(next_path, "/");
            strcat(next_path, current_object->d_name);

            full_tree(next_path, current_depth + 1);
        }
    }

    closedir(folder);
}

void tree(char *current_path, int current_depth, int final_depth)
{
    char next_path[MAX];
    char temp[MAX];
    memset(next_path, 0, MAX);

    struct dirent *current_object;
    DIR *folder = opendir(current_path);

    if (!folder || current_depth == final_depth)
        return;

    while ((current_object = readdir(folder)) != NULL)
    {
        if (strcmp(current_object->d_name, ".") && strcmp(current_object->d_name, ".."))
        {
            for (int i = 0; i < 2 * current_depth; i++)
            {
                if (i % 2 == 0)
                {
                    sprintf(temp, "|");
                    strcat(output, temp);
                }
                else
                {
                    sprintf(temp, " ");
                    strcat(output, temp);
                }
            }

            sprintf(temp, "|__%s\n", current_object->d_name);
            strcat(output, temp);

            strcpy(next_path, current_path);
            strcat(next_path, "/");
            strcat(next_path, current_object->d_name);

            tree(next_path, current_depth + 1, final_depth);
        }
    }

    closedir(folder);
}

int check_arman()
{
    for (int i = 0; command_tajzie[i][0] != '\0'; i++)
    {
        if (strcmp(command_tajzie[i], "=D") == 0)
            return 1;
    }

    return 0;
}

int where_arman()
{
    for (int i = 0; command_tajzie[i][0] != '\0'; i++)
    {
        if (strcmp(command_tajzie[i], "=D") == 0)
            return i;
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------

void barrasi(char direction[])
{
    memset(output, 0, ULTRA_MAX);

    if (strcmp(command_tajzie[0], "createfile") == 0)
    {
        createfile();

        command_tajzie[2][3] = 'u';
        command_tajzie[2][4] = 'n';
        command_tajzie[2][5] = 'd';
        command_tajzie[2][6] = 'o';

        createfile();
    }

    else if (check_arman())
    {
        // input
        if (strcmp(command_tajzie[0], "cat") == 0)
        {
            if (check_existance())
            {
                undo_backup();
                cat();
            }
        }

        else if (strcmp(command_tajzie[0], "find") == 0)
        {
            swap(command_tajzie[1], command_tajzie[3]);
            swap(command_tajzie[2], command_tajzie[4]);
            if (check_existance())
            {
                undo_backup();
                find(command_tajzie[4]);
            }
        }

        else if (strcmp(command_tajzie[0], "grep") == 0)
        {
            int i;
            int temp_bin;
            int sum_for_c_option = 0;
            bool no_option = false;
            bool c_option = false;
            bool l_option = false;

            if (strcmp(command_tajzie[1], "--str") == 0)
            {
                i = 4;
                no_option = true;
                swap(command_tajzie[1], command_tajzie[3]);
            }

            else if (strcmp(command_tajzie[1], "-c") == 0)
            {
                i = 5;
                c_option = true;
                memset(command_tajzie[1], 0, MAX);
                swap(command_tajzie[4], command_tajzie[1]);
                swap(command_tajzie[3], command_tajzie[4]);
                swap(command_tajzie[2], command_tajzie[3]);
            }

            else
            {
                i = 5;
                l_option = true;
                memset(command_tajzie[1], 0, MAX);
                swap(command_tajzie[4], command_tajzie[1]);
                swap(command_tajzie[3], command_tajzie[4]);
                swap(command_tajzie[2], command_tajzie[3]);
            }

            while (command_tajzie[i][0] != 0 && command_tajzie[i][0] != '=')
            {
                swap(command_tajzie[2], command_tajzie[i]);

                if (check_existance())
                {
                    undo_backup();

                    if (no_option)
                    {
                        temp_bin = grep(0);
                    }

                    else if (c_option)
                    {
                        sum_for_c_option += grep(1);
                    }

                    else if (l_option)
                    {
                        temp_bin = grep(2);
                    }
                }

                i++;
            }

            if (c_option)
            {
                char temp[MAX];
                sprintf(temp, "There is \"%d\" lines that have \"%s\".", sum_for_c_option, command_tajzie[4]);
                strcat(output, temp);
            }
        }

        else if (strcmp(command_tajzie[0], "tree") == 0)
        {
            long depth;
            char *end_of_depth;
            depth = strtol(command_tajzie[1], &end_of_depth, 10);

            if (depth < -1)
            {
                strcat(output, "Invalid depth!\n");
            }
            else if (depth == 0)
            {
                strcat(output, "\n");
            }
            else if (depth == -1)
            {
                full_tree("../root", 0);
            }
            else
            {
                tree("../root", 0, depth);
            }
        }

        // output
        int arman_index = where_arman();

        int i;
        for (i = 0; command_tajzie[i + arman_index + 1][0] != '\0'; i++)
        {
            swap(command_tajzie[i], command_tajzie[i + arman_index + 1]);
        }

        if (strcmp(command_tajzie[0], "insertstr") == 0)
        {
            memset(command_tajzie[5], 0, MAX);
            memset(command_tajzie[6], 0, MAX);

            strcat(command_tajzie[5], "--str");
            strncat(command_tajzie[6], output, MAX);

            swap(command_tajzie[5], command_tajzie[3]);
            swap(command_tajzie[6], command_tajzie[4]);

            if (check_existance())
            {
                memset(output, 0, MAX);

                undo_backup();
                insertstr();
                printf("Succesfully Insterted!\n");
            }
            else
                print_output();
        }

        else if (strcmp(command_tajzie[0], "find") == 0)
        {
            memset(command_tajzie[6], 0, MAX);
            memset(command_tajzie[7], 0, MAX);
            memset(command_tajzie[8], 0, MAX);

            strcat(command_tajzie[6], "--str");
            strncat(command_tajzie[7], output, MAX);

            swap(command_tajzie[6], command_tajzie[3]);
            swap(command_tajzie[7], command_tajzie[4]);
            swap(command_tajzie[8], command_tajzie[5]);

            swap(command_tajzie[6], command_tajzie[5]);
            swap(command_tajzie[7], command_tajzie[6]);
            swap(command_tajzie[8], command_tajzie[7]);

            if (check_existance())
            {
                memset(output, 0, MAX);

                undo_backup();
                find(command_tajzie[4]);
            }
            print_output();
        }

        else if (strcmp(command_tajzie[0], "grep") == 0)
        {
            int i;
            int temp_bin;
            int sum_for_c_option = 0;
            bool no_option = false;
            bool c_option = false;
            bool l_option = false;

            if (strcmp(command_tajzie[1], "--file") == 0)
            {
                i = 4;
                no_option = true;

                int j;
                for (j = 2; command_tajzie[2 + j][0] != '\0'; j += 2)
                {
                    swap(command_tajzie[2], command_tajzie[2 + j]);
                    swap(command_tajzie[3], command_tajzie[3 + j]);
                }
                swap(command_tajzie[2], command_tajzie[2 + j]);
                swap(command_tajzie[3], command_tajzie[3 + j]);

                if (command_tajzie[3 + j][0] == '\0')
                {
                    memset(command_tajzie[j + 2 - arman_index], 0, MAX);
                }
                else
                {
                    memset(command_tajzie[j + 3 - arman_index], 0, MAX);
                }
                strcat(command_tajzie[3], "--str");
                strncat(command_tajzie[2], output, MAX);
            }

            else if (strcmp(command_tajzie[1], "-c") == 0)
            {
                i = 5;
                c_option = true;

                memset(command_tajzie[1], 0, MAX);
                swap(command_tajzie[2], command_tajzie[1]);
                int j;
                for (j = 2; command_tajzie[3 + j][0] != '\0'; j += 2)
                {
                    swap(command_tajzie[3], command_tajzie[3 + j]);
                    swap(command_tajzie[4], command_tajzie[4 + j]);
                }
                swap(command_tajzie[3], command_tajzie[3 + j]);
                swap(command_tajzie[4], command_tajzie[4 + j]);

                if (command_tajzie[4 + j][0] == '\0')
                {
                    memset(command_tajzie[j + 3 - arman_index], 0, MAX);
                }
                else
                {
                    memset(command_tajzie[j + 4 - arman_index], 0, MAX);
                }

                strcat(command_tajzie[3], "--str");
                strncat(command_tajzie[4], output, MAX);
            }

            else
            {
                i = 5;
                l_option = true;

                memset(command_tajzie[1], 0, MAX);
                swap(command_tajzie[2], command_tajzie[1]);
                int j;
                for (j = 2; command_tajzie[3 + j][0] != '\0'; j += 2)
                {
                    swap(command_tajzie[3], command_tajzie[3 + j]);
                    swap(command_tajzie[4], command_tajzie[4 + j]);
                }
                swap(command_tajzie[3], command_tajzie[3 + j]);
                swap(command_tajzie[4], command_tajzie[4 + j]);

                if (command_tajzie[4 + j][0] == '\0')
                {
                    memset(command_tajzie[j + 3 - arman_index], 0, MAX);
                }
                else
                {
                    memset(command_tajzie[j + 4 - arman_index], 0, MAX);
                }
                memset(command_tajzie[j + 4 - arman_index], 0, MAX);

                strcat(command_tajzie[3], "--str");
                strncat(command_tajzie[4], output, MAX);
            }

            memset(output, 0, MAX);

            while (command_tajzie[i][0] != 0)
            {
                swap(command_tajzie[2], command_tajzie[i]);

                if (check_existance())
                {
                    undo_backup();

                    if (no_option)
                    {
                        temp_bin = grep(0);
                    }

                    else if (c_option)
                    {
                        sum_for_c_option += grep(1);
                    }

                    else if (l_option)
                    {
                        temp_bin = grep(2);
                    }
                }

                i++;
            }

            if (c_option)
            {
                char temp[MAX];
                sprintf(temp, "There is \"%d\" lines that have \"%s\".", sum_for_c_option, command_tajzie[4]);
                strcat(output, temp);
            }

            print_output();
        }
    }

    else if (strcmp(command_tajzie[0], "insertstr") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[5], "--file");
            strcat(command_tajzie[6], direction);
            swap(command_tajzie[5], command_tajzie[1]);
            swap(command_tajzie[6], command_tajzie[2]);
            swap(command_tajzie[5], command_tajzie[3]);
            swap(command_tajzie[6], command_tajzie[4]);
        }

        if (check_existance())
        {
            undo_backup();
            insertstr();
        }
    }

    else if (strcmp(command_tajzie[0], "cat") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[1], "--file");
            strcat(command_tajzie[2], direction);
        }

        if (check_existance())
        {
            undo_backup();
            cat();
        }
    }

    else if (strcmp(command_tajzie[0], "removestr") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[7], "--file");
            strcat(command_tajzie[8], direction);
            swap(command_tajzie[7], command_tajzie[1]);
            swap(command_tajzie[8], command_tajzie[2]);
            swap(command_tajzie[7], command_tajzie[3]);
            swap(command_tajzie[8], command_tajzie[4]);
            swap(command_tajzie[7], command_tajzie[5]);
            swap(command_tajzie[8], command_tajzie[6]);
        }

        if (check_existance())
        {
            undo_backup();
            removestr();
        }
    }

    else if (strcmp(command_tajzie[0], "copystr") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[7], "--file");
            strcat(command_tajzie[8], direction);
            swap(command_tajzie[7], command_tajzie[1]);
            swap(command_tajzie[8], command_tajzie[2]);
            swap(command_tajzie[7], command_tajzie[3]);
            swap(command_tajzie[8], command_tajzie[4]);
            swap(command_tajzie[7], command_tajzie[5]);
            swap(command_tajzie[8], command_tajzie[6]);
        }

        if (check_existance())
        {
            undo_backup();
            copystr();
        }
    }

    else if (strcmp(command_tajzie[0], "cutstr") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[7], "--file");
            strcat(command_tajzie[8], direction);
            swap(command_tajzie[7], command_tajzie[1]);
            swap(command_tajzie[8], command_tajzie[2]);
            swap(command_tajzie[7], command_tajzie[3]);
            swap(command_tajzie[8], command_tajzie[4]);
            swap(command_tajzie[7], command_tajzie[5]);
            swap(command_tajzie[8], command_tajzie[6]);
        }

        if (check_existance())
        {
            undo_backup();
            cutstr();
        }
    }

    else if (strcmp(command_tajzie[0], "pastestr") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[3], "--file");
            strcat(command_tajzie[4], direction);
            swap(command_tajzie[3], command_tajzie[1]);
            swap(command_tajzie[4], command_tajzie[2]);
        }

        if (check_existance())
        {
            undo_backup();
            pastestr();
        }
    }

    else if (strcmp(command_tajzie[0], "find") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[7], "--file");
            strcat(command_tajzie[8], direction);
            swap(command_tajzie[7], command_tajzie[1]);
            swap(command_tajzie[8], command_tajzie[2]);
            swap(command_tajzie[7], command_tajzie[3]);
            swap(command_tajzie[8], command_tajzie[4]);
            swap(command_tajzie[7], command_tajzie[5]);
            swap(command_tajzie[8], command_tajzie[6]);
        }

        swap(command_tajzie[1], command_tajzie[3]);
        swap(command_tajzie[2], command_tajzie[4]);

        if (check_existance())
        {
            undo_backup();
            find(command_tajzie[4]);
        }
    }

    else if (strcmp(command_tajzie[0], "replace") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[7], "--file");
            strcat(command_tajzie[8], direction);
            swap(command_tajzie[7], command_tajzie[1]);
            swap(command_tajzie[8], command_tajzie[2]);
            swap(command_tajzie[7], command_tajzie[3]);
            swap(command_tajzie[8], command_tajzie[4]);
        }

        else
        {
            swap(command_tajzie[1], command_tajzie[5]);
            swap(command_tajzie[2], command_tajzie[6]);
            swap(command_tajzie[3], command_tajzie[5]);
            swap(command_tajzie[4], command_tajzie[6]);
            swap(command_tajzie[5], command_tajzie[7]);
            swap(command_tajzie[6], command_tajzie[8]);
        }

        if (check_existance())
        {
            undo_backup();
            replace();
        }
    }

    else if (strcmp(command_tajzie[0], "grep") == 0)
    {
        int i;
        int temp_bin;
        int sum_for_c_option = 0;
        bool no_option = false;
        bool c_option = false;
        bool l_option = false;

        if (strcmp(command_tajzie[1], "--str") == 0)
        {
            i = 4;
            no_option = true;
            swap(command_tajzie[1], command_tajzie[3]);
        }

        else if (strcmp(command_tajzie[1], "-c") == 0)
        {
            i = 5;
            c_option = true;
            memset(command_tajzie[1], 0, MAX);
            swap(command_tajzie[4], command_tajzie[1]);
            swap(command_tajzie[3], command_tajzie[4]);
            swap(command_tajzie[2], command_tajzie[3]);
        }

        else
        {
            i = 5;
            l_option = true;
            memset(command_tajzie[1], 0, MAX);
            swap(command_tajzie[4], command_tajzie[1]);
            swap(command_tajzie[3], command_tajzie[4]);
            swap(command_tajzie[2], command_tajzie[3]);
        }

        while (command_tajzie[i][0] != 0)
        {
            swap(command_tajzie[2], command_tajzie[i]);

            if (check_existance())
            {
                undo_backup();

                if (no_option)
                {
                    temp_bin = grep(0);
                }

                else if (c_option)
                {
                    sum_for_c_option += grep(1);
                }

                else if (l_option)
                {
                    temp_bin = grep(2);
                }
            }

            i++;
        }

        if (c_option)
        {
            char temp[MAX];
            sprintf(temp, "There is \"%d\" lines that have \"%s\".", sum_for_c_option, command_tajzie[4]);
            strcat(output, temp);
        }
    }

    else if (strcmp(command_tajzie[0], "undo") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[1], "--file");
            strcat(command_tajzie[2], direction);
        }

        if (check_existance())
            undo();
    }

    else if (strcmp(command_tajzie[0], "auto-indent") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[1], "--file");
            strcat(command_tajzie[2], direction);
        }

        if (check_existance() && brace_counter())
        {
            undo_backup();
            auto_indent();
        }
    }

    else if (strcmp(command_tajzie[0], "compare") == 0)
    {
        if (!dash_file())
        {
            strcat(command_tajzie[3], "--file");
            strcat(command_tajzie[4], direction);
            swap(command_tajzie[3], command_tajzie[1]);
            swap(command_tajzie[4], command_tajzie[2]);
        }

        if (check_existance())
        {
            undo_backup();
            swap(command_tajzie[2], command_tajzie[3]);

            if (check_existance())
            {
                undo_backup();
                swap(command_tajzie[3], command_tajzie[2]);

                compare(line_counter(command_tajzie[2]), line_counter(command_tajzie[3]));
            }
        }
    }

    else if (strcmp(command_tajzie[0], "tree") == 0)
    {
        long depth;
        char *end_of_depth;
        depth = strtol(command_tajzie[1], &end_of_depth, 10);

        if (depth < -1)
        {
            strcat(output, "Invalid depth!\n");
        }
        else if (depth == 0)
        {
            strcat(output, "\n");
        }
        else if (depth == -1)
        {
            full_tree("../root", 0);
        }
        else
        {
            tree("../root", 0, depth);
        }
    }

    else if (strcmp(command_tajzie[0], "exit") == 0)
    {
        return;
    }

    else
    {
        char temp[MAX];
        sprintf(temp, "Wrong command!");
        strcat(output, temp);
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------

void vim_window()
{

    char file_name[MAX];
    char direction[MAX];
    char direction_in_root[MAX];
    char temp[MAX];
    char input_char;
    int line_sizes[MAX];
    bool saved = false;
    bool normal_mode = true;
    bool visual_mode = false;
    bool insert_mode = false;

    memset(file_name, 0, MAX);
    memset(direction, 0, MAX);
    memset(direction_in_root, 0, MAX);
    memset(temp, 0, MAX);
    memset(line_sizes, 0, MAX * sizeof(int));

    strcat(file_name, "Untitled.txt");
    strcat(direction, "../unsaved/Untitled.txt");

    char window[MAX][MAX];
    for (int i = 0; i < MAX; i++)
        memset(window[i], 0, MAX);
    int first_line = 1, last_line = 1, current_line = 1, max_lines = 40;
    int cursor_x = 0, cursor_y = 1;
    int first_pos_in_visual;
    int last_pos_in_visual;

    FILE *file = fopen(direction, "w");
    fclose(file);
    file = fopen(direction, "r");

    while (strcmp(command, "exit") != 0)
    {
        system("cls");
        memset(line_sizes, 0, MAX * sizeof(int));

        if (saved_check(direction_in_root, direction))
            saved = true;
        else
            saved = false;

        fseek(file, 0, SEEK_SET);
        current_line = 1;

        while (fgets(temp, MAX, file) != NULL && (current_line - first_line) < max_lines)
        {
            line_sizes[current_line - 1] = strlen(temp);

            if (current_line >= first_line)
            {
                printf("%3d  ", current_line);

                if (current_line - first_line == cursor_y - 1)
                {
                    if (visual_mode)
                    {
                        int i;
                        for (i = 0; temp[i] != '\0'; i++)
                        {
                            if (i == cursor_x && temp[i + 1] == '\0')
                            {
                                setBackgroundColor(LIGHTMAGENTA);
                                putchar(' ');
                                if (temp[i] == '\n')
                                    putchar(temp[i]);
                                setBackgroundColor(BLACK);
                            }

                            else if (i == cursor_x)
                            {
                                setBackgroundColor(LIGHTMAGENTA);
                                putchar(temp[i]);
                                setBackgroundColor(BLACK);
                            }

                            else if (first_pos_in_visual > cursor_x && i >= last_pos_in_visual && i < first_pos_in_visual)
                            {
                                setBackgroundColor(YELLOW);
                                putchar(temp[i]);
                                setBackgroundColor(BLACK);
                            }

                            else if (first_pos_in_visual < cursor_x && i <= last_pos_in_visual && i >= first_pos_in_visual)
                            {
                                setBackgroundColor(YELLOW);
                                putchar(temp[i]);
                                setBackgroundColor(BLACK);
                            }

                            else
                            {
                                putchar(temp[i]);
                            }
                        }
                    }

                    else
                    {
                        int i;
                        for (i = 0; temp[i] != '\0'; i++)
                        {
                            if (i == cursor_x && temp[i + 1] == '\0')
                            {
                                setBackgroundColor(LIGHTMAGENTA);
                                putchar(' ');
                                if (temp[i] == '\n')
                                    putchar(temp[i]);
                                setBackgroundColor(BLACK);
                            }

                            else if (i == cursor_x)
                            {
                                setBackgroundColor(LIGHTMAGENTA);
                                putchar(temp[i]);
                                setBackgroundColor(BLACK);
                            }

                            else
                            {
                                putchar(temp[i]);
                            }
                        }
                    }
                }

                else
                {
                    printf("%s", temp);
                }
            }

            current_line++;
        }

        last_line = current_line - 1;

        if ((current_line - first_line) < max_lines)
        {
            setColor(BLUE);
            putchar('\n');
            while ((current_line - first_line) < max_lines)
            {
                printf("~\n");
                current_line++;
            }
            setColor(WHITE);
        }

        putchar('\n');

        if (normal_mode)
        {
            setColor(BLACK);
            if (saved)
            {
                setBackgroundColor(LIGHTCYAN);
                printf(" NORMAL   ");
                setBackgroundColor(GREY);
                printf("  %s      ", file_name);
                setBackgroundColor(BLACK);
            }
            else
            {
                setBackgroundColor(LIGHTCYAN);
                printf(" NORMAL   ");
                setBackgroundColor(GREY);
                printf("  %s   +  ", file_name);
                setBackgroundColor(BLACK);
            }
            setColor(WHITE);

            setBackgroundColor(DARKGREY);
            for (int i = 0; i < 163; i++)
                putchar(' ');
            putchar('\n');
            setBackgroundColor(BLACK);

            input_char = getch();
            switch (input_char)
            {
            case ':':
                clear();

                putchar(':');

                int i = 0;
                while ((input_char = getchar()) != '\n')
                {
                    command[i] = input_char;
                    i++;
                }
                command[i] = '\0';

                tajzieh();

                // saving
                if (strcmp(command_tajzie[0], "save") == 0)
                {
                    if (strcmp(file_name, "Untitled.txt") == 0)
                    {
                        system("cls");
                        system("color CE");
                        gotoxy(100, 25);
                        printf("The file hasn't saved before!");
                        gotoxy(100, 26);
                        printf("Use \"saveas\" command first!");
                        gotoxy(90, 27);
                        printf("Or if you don't want to save the file, press \"f\".");
                        input_char = getch();
                        if (input_char == 'f')
                        {
                            fclose(file);
                            file = fopen(direction, "w");
                            fputc(' ', file);
                            fclose(file);
                            fopen(direction, "r");
                            fseek(file, 0, SEEK_SET);
                        }
                        system("color 07");

                        first_line = 1;
                        last_line = 1;
                        cursor_x = 0;
                        cursor_y = 1;
                        continue;
                    }

                    else
                    {
                        saved = true;
                        FILE *file_in_root = fopen(direction_in_root, "w");
                        fseek(file, 0, SEEK_SET);

                        while (fgets(temp, MAX, file) != NULL)
                        {
                            fputs(temp, file_in_root);
                        }

                        fclose(file_in_root);

                        memset(command_tajzie[2], 0, MAX);
                        strcat(command_tajzie[2], direction_in_root);

                        undo_backup();

                        system("cls");
                        system("color 25");
                        gotoxy(100, 25);
                        printf("Saved!");
                        getch();
                        system("color 07");
                        continue;
                    }
                }

                else if (strcmp(command_tajzie[0], "saveas") == 0)
                {
                    memset(direction_in_root, 0, MAX);
                    memset(file_name, 0, MAX);

                    strcat(direction_in_root, command_tajzie[1]);
                    strcat(file_name, name_of(direction_in_root));

                    clear();

                    strcat(command_tajzie[0], "createfile");
                    strcat(command_tajzie[1], "--file");
                    strcat(command_tajzie[2], direction_in_root);

                    createfile();

                    command_tajzie[2][3] = 'u';
                    command_tajzie[2][4] = 'n';
                    command_tajzie[2][5] = 'd';
                    command_tajzie[2][6] = 'o';

                    createfile();

                    saved = true;
                    FILE *file_in_root = fopen(direction_in_root, "w");
                    fseek(file, 0, SEEK_SET);

                    while (fgets(temp, MAX, file) != NULL)
                    {
                        fputs(temp, file_in_root);
                    }

                    fclose(file_in_root);

                    memset(command_tajzie[2], 0, MAX);
                    strcat(command_tajzie[2], direction_in_root);

                    undo_backup();

                    system("cls");
                    system("color 25");
                    gotoxy(100, 25);
                    printf("Saved!");
                    getch();
                    system("color 07");
                    continue;
                }

                else if (strcmp(command_tajzie[0], "open") == 0)
                {
                    swap(command_tajzie[1], command_tajzie[2]);
                    memset(output, 0, ULTRA_MAX);
                    if (!check_existance())
                    {
                        system("cls");
                        system("color CE");
                        gotoxy(100, 25);
                        print_output();
                        getch();
                        system("color 07");
                        continue;
                    }

                    swap(command_tajzie[1], command_tajzie[2]);

                    FILE *file_in_root;

                    if (strcmp(file_name, "Untitled.txt"))
                    {
                        file_in_root = fopen(direction_in_root, "w");

                        fseek(file, 0, SEEK_SET);

                        while (fgets(temp, MAX, file) != NULL)
                        {
                            fputs(temp, file_in_root);
                        }

                        fclose(file_in_root);

                        memset(command_tajzie[2], 0, MAX);
                        strcat(command_tajzie[2], direction_in_root);

                        undo_backup();
                    }

                    memset(direction_in_root, 0, MAX);
                    strcat(direction_in_root, command_tajzie[1]);

                    memset(file_name, 0, MAX);
                    strcat(file_name, name_of(direction_in_root));

                    fclose(file);
                    file = fopen(direction, "w");
                    file_in_root = fopen(direction_in_root, "r");

                    while (fgets(temp, MAX, file_in_root) != NULL)
                    {
                        fputs(temp, file);
                    }

                    fclose(file_in_root);
                    fclose(file);
                    file = fopen(direction, "r");

                    first_line = 1;
                    last_line = 1;
                    cursor_x = 0;
                    cursor_y = 1;
                    continue;
                }

                // phase1
                else
                {
                    bool replace_in_file = false;
                    if (strcmp(command_tajzie[1], "--file"))
                        replace_in_file = true;

                    barrasi(direction);

                    if (strcmp(command_tajzie[0], "replace") == 0 && replace_in_file)
                        cursor_x = temp_adad;

                    if (output[0] != '\0')
                    {
                        if (strcmp(output, "Wrong command!") == 0)
                        {
                            system("cls");
                            system("color CE");
                            gotoxy(100, 25);
                            printf("Wrong command!");
                            getch();
                            system("color 07");
                            continue;
                        }

                        fclose(file);
                        memset(direction, 0, MAX);
                        strcat(direction, "../unsaved/Untitled.txt");

                        file = fopen(direction, "w");
                        fputs(output, file);
                        fclose(file);

                        file = fopen(direction, "r");
                        memset(file_name, 0, MAX);
                        strcat(file_name, name_of(direction));

                        first_line = 1;
                        last_line = 40;
                    }
                }

                break;

            // undo
            case 'u':
                clear();
                strcat(command_tajzie[0], "undo");
                strcat(command_tajzie[1], "--file");
                strcat(command_tajzie[0], direction_in_root);

                undo();

                saved = true;
                FILE *file_in_root = fopen(direction_in_root, "r");
                fclose(file);
                file = fopen(direction, "w");

                while (fgets(temp, MAX, file_in_root) != NULL)
                {
                    fputs(temp, file);
                }

                fclose(file_in_root);
                fclose(file);
                file = fopen(direction, "r");

                first_line = 1;
                last_line = 1;
                cursor_x = 0;
                cursor_y = 1;
                fseek(file, 0, SEEK_SET);

                break;

            // auto indent
            case '=':
                clear();
                memset(output, 0, MAX);
                strcat(command_tajzie[0], "auto-indent");
                strcat(command_tajzie[1], "--file");
                strcat(command_tajzie[2], direction);

                if (brace_counter())
                {
                    undo_backup();
                    auto_indent();
                }

                else
                {
                    system("cls");
                    system("color CE");
                    gotoxy(100, 25);
                    print_output();
                    getch();
                    system("color 07");
                    continue;
                }

                break;

            // find in file
            // case '/':
            //     char search_string[MAX];
            //     memset(search_string, 0, MAX);

            //     putchar('/');

            //     i = 0;
            //     while ((input_char = getchar()) != '\n')
            //     {
            //         search_string[i] = input_char;
            //         i++;
            //     }
            //     search_string[i] = '\0';

            //     find_infile(search_string);
            //     break;

            // Navigation
            case 'w': // up
                if (cursor_y == 5 && first_line > 1)
                    first_line--;
                else if (cursor_y > 1)
                    cursor_y--;

                while (cursor_x > line_sizes[(cursor_y - 1) + (first_line - 1)] - 1)
                {
                    cursor_x--;
                }

                break;

            case 's': // down
                if (cursor_y == 36 && last_line - first_line > 36)
                    first_line++;
                else if (cursor_y <= last_line - first_line)
                    cursor_y++;

                while (cursor_x > line_sizes[(cursor_y - 1) + (first_line - 1)] - 1)
                {
                    cursor_x--;
                }

                break;

            case 'a': // left
                if (cursor_x > 0)
                    cursor_x--;
                break;

            case 'd': // right
                if (cursor_x < line_sizes[(cursor_y - 1) + (first_line - 1)] - 1)
                    cursor_x++;
                break;

            // pasting
            case 'p':
                clear();
                strcat(command_tajzie[0], "pastestr");

                strcat(command_tajzie[1], "--file");
                strcat(command_tajzie[2], direction);

                strcat(command_tajzie[3], "--pos");
                sprintf(temp, "%d", cursor_y);
                strcat(command_tajzie[4], temp);
                sprintf(temp, ":%d", cursor_x);
                strcat(command_tajzie[4], temp);

                pastestr();

                break;

            // Change mode
            case 'i':
                insert_mode = true;
                normal_mode = false;
                visual_mode = false;
                break;

            case 'v':
                visual_mode = true;
                insert_mode = false;
                normal_mode = false;
                first_pos_in_visual = cursor_x;
                break;

            default:
                break;
            }
        }

        else if (insert_mode)
        {
            setColor(BLACK);
            if (saved)
            {
                setBackgroundColor(LIGHTCYAN);
                printf(" INSERT   ");
                setBackgroundColor(GREY);
                printf("  %s      ", file_name);
                setBackgroundColor(BLACK);
            }
            else
            {
                setBackgroundColor(LIGHTCYAN);
                printf(" INSERT   ");
                setBackgroundColor(GREY);
                printf("  %s   +  ", file_name);
                setBackgroundColor(BLACK);
            }
            setColor(WHITE);

            setBackgroundColor(DARKGREY);
            for (int i = 0; i < 163; i++)
                putchar(' ');
            putchar('\n');
            setBackgroundColor(BLACK);

            input_char = getch();
            switch (input_char)
            {

            // Change mode
            case 27:
                normal_mode = true;
                visual_mode = false;
                insert_mode = false;
                break;

            case 8:
                clear();
                strcat(command_tajzie[0], "removestr");

                strcat(command_tajzie[1], "--file");
                strcat(command_tajzie[2], direction);

                strcat(command_tajzie[3], "--pos");
                sprintf(temp, "%d", cursor_y);
                strcat(command_tajzie[4], temp);
                sprintf(temp, ":%d", cursor_x);
                strcat(command_tajzie[4], temp);

                strcat(command_tajzie[5], "-size");
                sprintf(command_tajzie[6], "%d", 1);

                strcat(command_tajzie[7], "-b");

                removestr();

                if (temp_clipboard[0] == '\n')
                {
                    if (cursor_y == 5 && first_line > 1)
                        first_line--;
                    else if (cursor_y > 1)
                        cursor_y--;

                    cursor_x = line_sizes[(cursor_y - 1) + (first_line - 1)] - 1;
                }

                else
                    cursor_x--;

                break;

            // writing
            default:
                clear();
                strcat(command_tajzie[0], "insertstr");

                strcat(command_tajzie[1], "--file");
                strcat(command_tajzie[2], direction);

                strcat(command_tajzie[3], "--str");
                if (input_char == '\r')
                    command_tajzie[4][0] = '\n';
                else
                    command_tajzie[4][0] = input_char;

                strcat(command_tajzie[5], "--pos");
                sprintf(temp, "%d", cursor_y);
                strcat(command_tajzie[6], temp);
                sprintf(temp, ":%d", cursor_x);
                strcat(command_tajzie[6], temp);

                insertstr();

                if (input_char == '\r')
                {
                    if (cursor_y == 36 && last_line - first_line > 36)
                        first_line++;
                    else
                        cursor_y++;

                    cursor_x = 0;
                }

                else
                    cursor_x++;

                break;
            }
        }

        else if (visual_mode)
        {
            setColor(BLACK);
            if (saved)
            {
                setBackgroundColor(LIGHTCYAN);
                printf(" VISUAL   ");
                setBackgroundColor(GREY);
                printf("  %s      ", file_name);
                setBackgroundColor(BLACK);
            }
            else
            {
                setBackgroundColor(LIGHTCYAN);
                printf(" VISUAL   ");
                setBackgroundColor(GREY);
                printf("  %s   +  ", file_name);
                setBackgroundColor(BLACK);
            }
            setColor(WHITE);

            setBackgroundColor(DARKGREY);
            for (int i = 0; i < 163; i++)
                putchar(' ');
            putchar('\n');
            setBackgroundColor(BLACK);

            input_char = getch();
            switch (input_char)
            {
            // Navigation
            case 'w': // up
                if (cursor_y == 5 && first_line > 1)
                    first_line--;
                else if (cursor_y > 1)
                    cursor_y--;

                while (cursor_x > line_sizes[(cursor_y - 1) + (first_line - 1)] - 1)
                {
                    cursor_x--;
                }

                break;

            case 's': // down
                if (cursor_y == 36 && last_line - first_line > 36)
                    first_line++;
                else if (cursor_y <= last_line - first_line)
                    cursor_y++;

                while (cursor_x > line_sizes[(cursor_y - 1) + (first_line - 1)] - 1)
                {
                    cursor_x--;
                }

                break;

            case 'a': // left
                if (cursor_x > 0)
                    cursor_x--;
                last_pos_in_visual = cursor_x;
                break;

            case 'd': // right
                if (cursor_x < line_sizes[(cursor_y - 1) + (first_line - 1)] - 1)
                    cursor_x++;
                last_pos_in_visual = cursor_x;
                break;

            // selecting
            case 'c': // copystr
                clear();
                strcat(command_tajzie[0], "copystr");

                strcat(command_tajzie[1], "--file");
                strcat(command_tajzie[2], direction);

                strcat(command_tajzie[3], "--pos");
                sprintf(temp, "%d", cursor_y);
                strcat(command_tajzie[4], temp);
                sprintf(temp, ":%d", first_pos_in_visual);
                strcat(command_tajzie[4], temp);

                strcat(command_tajzie[5], "-size");
                sprintf(command_tajzie[6], "%d", abs(first_pos_in_visual - last_pos_in_visual));

                if (last_pos_in_visual < first_pos_in_visual)
                    strcat(command_tajzie[7], "-b");
                else if (first_pos_in_visual < last_pos_in_visual)
                    strcat(command_tajzie[7], "-f");

                copystr();

                normal_mode = true;
                visual_mode = false;
                insert_mode = false;
                break;

            case 'x':
                clear();
                strcat(command_tajzie[0], "cutstr");

                strcat(command_tajzie[1], "--file");
                strcat(command_tajzie[2], direction);

                strcat(command_tajzie[3], "--pos");
                sprintf(temp, "%d", cursor_y);
                strcat(command_tajzie[4], temp);
                sprintf(temp, ":%d", first_pos_in_visual);
                strcat(command_tajzie[4], temp);

                strcat(command_tajzie[5], "-size");
                sprintf(command_tajzie[6], "%d", abs(first_pos_in_visual - last_pos_in_visual));

                if (last_pos_in_visual < first_pos_in_visual)
                    strcat(command_tajzie[7], "-b");
                else if (first_pos_in_visual < last_pos_in_visual)
                    strcat(command_tajzie[7], "-f");

                cutstr();

                normal_mode = true;
                visual_mode = false;
                insert_mode = false;
                break;

            // Change mode
            case 27:
                normal_mode = true;
                visual_mode = false;
                insert_mode = false;

            default:
                break;
            }
        }
    }

    system("cls");
    system("color F0");
    gotoxy(100, 25);
    printf("Have a good day!");
    getch();

    fclose(file);
}

// -------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    vim_window();

    return 0;
}