#define	baudrate	115200

void init_comms(void);
void putch(unsigned char);
unsigned char getch(void);
unsigned char getche(void);
void txstring(const char * s, unsigned char fl);
void test_oerr(void);


