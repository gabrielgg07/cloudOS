
#include "../include/terminal.h"
#include "../include/keyboard.h"
#include "../include/lib/string.h"
#include "../include/shell.h"
#include "commands.h"


#define MAX_INPUT 256

void run_shell() {
    char input[MAX_INPUT];
    int index = 0; 
    terminal_print("DiamondOS Shell\n");
    terminal_print("> ");
    char c;
    while (1) {
        while ((c = keyboard_buffer_dequeue()) == 0);  // Blocking read

        if (c == '\n') {
            
            terminal_put_char('\n');
            input[index] = '\0';
            if (index > 0) {
                process_command(input);
            }
            index = 0;
            terminal_print("> ");
        } else if (c == '\b') {
            if (index > 0) {
                index--;
                terminal_backspace();  // Optional helper to move cursor & erase
            }
        } else if (index < MAX_INPUT - 1) {
            input[index] = c;
            index++;
            terminal_put_char(c);  // Echo to screen
        }
    }
}


typedef void (*command_func_t)(const char*);

typedef struct {
    const char* name;
    command_func_t func;
} command_entry_t;

command_entry_t command_table[] = {
    {"help", cmd_help},
    {"clear", cmd_clear},
    {"echo", cmd_echo},
    {"user", enter_user_mode},
    {NULL, NULL}
};

void process_command(const char* input) {
    for (int i = 0; command_table[i].name != NULL; i++) {
        if (strncmp(input, command_table[i].name, strlen(command_table[i].name)) == 0) {
            command_table[i].func(input);
            return;
        }
    }
    terminal_print("Unknown command\n");
}