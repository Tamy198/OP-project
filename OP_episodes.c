/**  A program to monitor how many episodes of one piece you are watching 
 * Written and implemented by Thomas Blanch
 * Usage: 
 * Compile with gcc -Wall -g -o OP OP_episodes.c
 * Execute with ./OP OP_diary.txt
 * // ** [changes] ** == changes to implement
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 18 episodes with 3 digits
#define MAXLINE 80

#define UNINITIALISED -1
// *** create a way to determine the length of episodes rather than assuming ***
#define EPISODE_NUMBER_LEN 3
// the number of digits in intmax
#define TOT_INT_DIGITS 10

#define ASCII_NEWLINE 10
#define ASCII_0 48

int readline(FILE* fp, char* line, int* len);
char* int_to_string(int x, char* number);

int
main (int argc, char** argv) {
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("NULL file");
        exit(EXIT_FAILURE);
    }

    // ** ask the user if they are yet to create a file **
    // ** have a non-one piece mode and just any tv show? **

    // Determine how many days with no episodes
    // ** rework to work for days rather than # skipped **
    int skipped;
    printf("Enter how many days without episodes: ");
    scanf("%d", &skipped);
    if (skipped < 0) {
        printf("Must not be negative\n");
        exit(EXIT_FAILURE);
    }

    // Determine final episode they completed today
    int episode;
    printf("Enter the final episode you COMPLETED today: ");
    scanf("%d", &episode);
    // ** code a function to use the current date to determine the maximum 
    // episode number at this time based on weekly releases **
    if (episode <= 0) {
        printf("Must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    // Read file until it's final line and store relevant info from that line
    char line[MAXLINE + 1];
    int line_len = 0; 
    while (!readline(fp, line, &line_len)) {
        continue;
    }

    // Add the info from today to the file
    fclose(fp);
    FILE* fp2 = fopen(argv[1], "a");

    // Work out the last episode on last watching day
    char prev_day_str[EPISODE_NUMBER_LEN + 1];
    for (int i = 0; i < EPISODE_NUMBER_LEN; i++) {
        prev_day_str[i] = line[line_len - EPISODE_NUMBER_LEN - 1 + i];
    }
    prev_day_str[EPISODE_NUMBER_LEN] = '\0';
    int prev_episode = atoi(prev_day_str);
    int prev_day = (int) line[0] - ASCII_0;

    // Add only the day number for the skipped days
    fprintf(fp2, "\n");
    char number[TOT_INT_DIGITS];
    for (int day = prev_day + 1; day <= prev_day + skipped; day++) {
        fprintf(fp2, "%s", int_to_string(day, number));
        fprintf(fp2, ". \n");
    }

    // Increment and add the episodes watched on the given day
    fprintf(fp2, "%s", int_to_string(prev_day + skipped + 1, number));
    fprintf(fp2, ". ");
    for (int ep = prev_episode + 1; ep <= episode; ep++) {
        fprintf(fp2, "%s", int_to_string(ep, number));
        fprintf(fp2, " ");
    }

    fclose(fp);
    return 0;
}

// Reads a line of a text file and returns if it's the last line
int
readline(FILE* fp, char* line, int* len) {
    int ch;
    *len = 0;

    while ((ch = getc(fp)) != ASCII_NEWLINE && ch != EOF && *len < MAXLINE) {
        line[(*len)++] = ch;
    }
    line[(*len)++] = '\0';

    return ch == EOF;
}

// Convert an integer to a string
char*
int_to_string(int x, char* number) {
    // Convert each demical place to a char and add it to the string 
    for (int i = TOT_INT_DIGITS - 1; i >= 0; i--) {
        number[TOT_INT_DIGITS - 1 - i] = 
            (char) (x / ((int)pow(10, i)) + ASCII_0);
        x %= (int) pow(10, i);
    }
    number[TOT_INT_DIGITS] = '\0';

    // Remove redundant chars
    int redundants = 0;
    while (number[redundants] == ASCII_0) {
        redundants++;
    }
    for (int i = redundants; i < TOT_INT_DIGITS; i++) {
        number[i - redundants] = number[i];
    }
    number[TOT_INT_DIGITS - redundants] = '\0'; 

    return number;
}
