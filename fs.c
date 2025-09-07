/* fs.c: Animates Figlet-generated ASCII art across the terminal until 'q' is pressed. */
/* Inspired by sl by Toyoda Masashi[](https://github.com/mtoyoda/sl) */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fs.h"

int INFINITE = 0; // Set to 1 via -i flag for continuous scroll
int SPEED = 40000; // Default usleep microseconds; adjustable via -s

int my_mvaddstr(int y, int x, char *str) {
    for (; x < 0; ++x, ++str)
        if (*str == '\0') return ERR;
    for (; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR) return ERR;
    return OK;
}

void option(char *str) {
    while (*str != '\0') {
        switch (*str++) {
            case 'i': INFINITE = 1; break;
            default: break;
        }
    }
}

void handle_sigint(int sig) {
    endwin();
    exit(0);
}

int main(int argc, char *argv[]) {
    int x, i;
    char *text = NULL;
    char *font = "standard";
    int width = 132;

    // Parse Figlet-specific options
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            text = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            font = argv[++i];
        } else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            width = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            SPEED = atoi(argv[++i]);
        } else if (*argv[i] == '-') {
            option(argv[i] + 1);
        }
    }

    if (text == NULL) {
        fprintf(stderr, "Usage: %s -t \"text\" [-f font] [-w width] [-i] [-s speed]\n", argv[0]);
        return 1;
    }

    // Generate Figlet art to temp file
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "figlet -w %d -f %s \"%s\" > /tmp/fs_art.txt", width, font, text);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error running figlet. Is it installed?\n");
        return 1;
    }

    // Read temp file and compute dimensions
    FILE *file = fopen("/tmp/fs_art.txt", "r");
    if (!file) {
        fprintf(stderr, "Error opening temp file.\n");
        return 1;
    }

    char **art_lines = NULL;
    int art_height = 0;
    int art_length = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Strip newline
        line[strcspn(line, "\n")] = '\0';
        int len = strlen(line);
        if (len > art_length) art_length = len;

        // Dynamic array for lines
        art_lines = realloc(art_lines, (art_height + 1) * sizeof(char *));
        art_lines[art_height] = strdup(line);
        art_height++;
    }
    fclose(file);
    remove("/tmp/fs_art.txt"); // Clean up

    if (art_height == 0) {
        fprintf(stderr, "No art generated.\n");
        return 1;
    }

    // Ncurses setup
    initscr();
    signal(SIGINT, handle_sigint);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);

    x = COLS - 1;  // Start at right edge
    while (1) {
        clear();  // Clear screen each frame
        if (INFINITE) {
            // Draw multiple instances
            int offset = x;
            while (offset < COLS) {
                add_art(offset, art_lines, art_height, art_length);
                offset += art_length;
            }
            if (x <= -art_length) {
                x += art_length; // Shift back for continuous flow
            }
        } else {
            // Single instance, reset when off-screen
            if (add_art(x, art_lines, art_height, art_length) == ERR) {
                x = COLS - 1;
            }
        }
        int ch = getch();
        if (ch == 'q') break;  // Exit on 'q'
        refresh();
        usleep(SPEED);
        x--;  // Move left
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();

    // Free memory
    for (i = 0; i < art_height; i++) {
        free(art_lines[i]);
    }
    free(art_lines);

    return 0;
}

int add_art(int x, char **art_lines, int art_height, int art_length) {
    if (x < -art_length) return ERR;
    int y = LINES / 2 - art_height / 2;
    for (int i = 0; i < art_height; i++) {
        my_mvaddstr(y + i, x, art_lines[i]);
    }
    return OK;
}
