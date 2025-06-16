#pragma once


void enter_user_mode();
void cmd_help(const char* input);
void cmd_clear(const char* input);
void cmd_echo(const char* input); 
void cmd_sysinfo(const char* input);
void cmd_nicinfo(const char* input);
void cmd_diskinfo(const char* input);
void cmd_diskwrite(const char* input);
void cmd_diskread(const char* input);