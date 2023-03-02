#include "mbox/mbox.h"
#include "utils.h"

volatile unsigned int mbox_buf[16]__attribute__((aligned(16)));

int mbox_call_func(unsigned char prop_tag) {
    unsigned int addr = (((unsigned long)&mbox_buf) & (~(0xF))) | prop_tag;
    while(1) {
        if((read_reg_32(MAIL_STATUS) & MAIL_FULL) == 0) {
            break;
        }
    }
    write_reg_32(MAIL_WRITE, addr);

    //fill mbox request
    while(1) {
        while(1) {
            if((read_reg_32(MAIL_STATUS) & MAIL_EMPTY) == 0) {
                break;
            } 
        }
        if(addr == read_reg_32(MAIL_READ)) {
            return mbox_buf[1] == MAIL_RESPONSE;
        }
    }
}
