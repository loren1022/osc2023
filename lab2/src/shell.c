#include "shell.h"
#include "mini_uart.h"
#include "utils.h"
#include "mailbox.h"
#include "reboot.h"

#define MAX_BUFFER_SIZE 256u
static char buffer[MAX_BUFFER_SIZE];

/*
 * This is the same as `strcmp` in standard library
 */
int strcmp(const char *s1, const char *s2)
{
        while (*s1 == *s2++) {
                if (*s1++ == 0) return 0;
        }
        return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

void read_cmd(void)
{
        unsigned int idx = 0;
        char c = '\0';
        
        while (1) {
                c = uart_recv();
                if (c == '\r' || c == '\n') {
                        uart_send_string("\r\n");
                        
                        if (idx < MAX_BUFFER_SIZE) buffer[idx] = '\0';
                        else buffer[MAX_BUFFER_SIZE - 1] = '\0';
                        
                        break;
                } else {
                        uart_send(c);
                        buffer[idx++] = c;
                } 
        }
}

void send_help_message(void)
{
        uart_send_string("help:\t\tprint this help menu.\r\n");
        uart_send_string("hello:\t\tprint Hello World!\r\n");
        uart_send_string("reboot:\t\treboot device\r\n");
        uart_send_string("hw-info:\tprint hardware information\r\n");
}

void parse_cmd(void)
{
        if (!strcmp(buffer, "\0")) {
                uart_send_string("\r\n");
        } else if (!strcmp(buffer, "hello")) {
                uart_send_string("Hello World!\r\n");
        } else if (!strcmp(buffer, "help")) {
                send_help_message();
        } else if (!strcmp(buffer, "hw-info")) {
                get_board_revision();
                get_arm_memory();
        } else if (strcmp(buffer, "reboot") == 0) {
                uart_send_string("rebooting ...\r\n");
                reset(100);
        } else {
                uart_send_string("Command not found, ");
                uart_send_string("type 'help' for commands.\r\n");
        }
}

void shell_loop(void)
{
        while (1) {
                uart_send_string("# ");
                read_cmd();
                parse_cmd();
        }
}