
unsigned int counter = 0;

int main(void) {
    unsigned int *p_uint;
    
    p_uint = &counter;
    while (*p_uint < 21) {
        ++(*p_uint);
    }
    
    p_uint = (unsigned int *)0x20000002U;
    *p_uint = 0xDEADBEEF;
    
    return 0;
}
