// kernel/main.c
void kernel_main() {
    const char* msg = "Cloud OS kernel loaded!";
    char* video = (char*) 0xb8000;
    for (int i = 0; msg[i] != '\0'; i++) {
        video[i * 2] = msg[i];
        video[i * 2 + 1] = 0x07;  // White on black
    }
}
