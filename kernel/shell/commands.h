#pragma once
#include "../include/lib/string.h"


void enter_user_mode();
void cmd_help(const char* input);
void cmd_clear(const char* input);
void cmd_echo(const char* input); 
void cmd_sysinfo(const char* input);
void cmd_nicinfo(const char* input);
void cmd_diskinfo(const char* input);
void cmd_diskwrite(const char* input);
void cmd_diskread(const char* input);
const char* get_args(const char* input);

void cmd_ls(const char* input);
void cmd_cat(const char* input);
void cmd_touch(const char* input);
void cmd_mkdir(const char* input);
void cmd_cd(const char* input);
void cmd_echoDirect(const char* input);