//
//  L6740_SPI.h
//  
//
//  Created by Travis Priest on 2/14/18.
//

#ifndef PI_SPI_h
#define PI_SPI_h


#endif /* L6740_SPI_h */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;

#define BCM2708_PERI_BASE        0x3F000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */



// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

#define foreach(item, array) \
int keep;\
int size;\
int count;\
for(keep = 1, \
count = 0,\
size = sizeof (array) / sizeof *(array); \
keep && count != size; \
keep = !keep, count++) \
for(item = (array) + count; keep; keep = !keep)



char* L6740_spi_port;
int fd;

//setup for gpio for CS lines
void setup_CS(int *);


//Program functions for spi interface
void pi_spi_write(int , char[] );
char* pi_spi_read(int);
int pi_set_spi_port(char * );
int pi_spi_open(void);
uint32_t pi_spi_get_mode(void);
uint8_t pi_spi_get_bits(void);
uint32_t pi_spi_get_speed(void);
uint32_t pi_spi_set_mode(uint32_t);
uint8_t pi_spi_set_bits(uint8_t);
uint32_t pi_spi_set_speed(uint32_t);
uint8_t* transfer(int, uint8_t * , size_t);
