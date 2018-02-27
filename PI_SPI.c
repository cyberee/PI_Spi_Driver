//
//  L6740_SPI.c
//  
//
//  Created by Travis Priest on 2/14/18.
//

//#include <stdio.h>

#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/mman.h>

#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "PI_SPI.h"

void pi_spi_write (int CS, char data[])
{
    //write data out the spi bus using CS as pin
    printf("Writing data in C!!!\n\r");
    
}

char * pi_spi_read (int CS)
{
    //read data from spi bus using CS as pin
    return "data read from spi\n\r";
}

//Set the L6740_spi_port Name
int pi_set_spi_port(char * port)
{
    printf("port set to1: %s\n\r", port);
    L6740_spi_port=port;
    //strcpy(L6740_spi_port,port);
    printf("port set to: %s\n\r", L6740_spi_port);
    return 1;
}
 
//Open the port set by L6740_spi_port
int pi_spi_open()
{
    printf("in driver!%s!!\n\r",L6740_spi_port);
    
    fd = open(L6740_spi_port, O_RDWR);
    printf("in driver!%s!%i!\n\r",L6740_spi_port,fd);
    return fd;
}

uint32_t pi_spi_get_mode()
{
    int ret;
    uint32_t mode;
    ret = ioctl(fd, SPI_IOC_RD_MODE,&mode);
    if (ret > 0)
    {
        return -1;
    }
    return mode;
}

uint8_t pi_spi_get_bits()
{
    int ret;
    uint8_t bits;
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD,&bits);
    if (ret > 0)
    {
        return -1;
    }
    return bits;
}

uint32_t pi_spi_get_speed()
{
    int ret;
    uint32_t speed;
    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ,&speed);
    if (ret > 0)
    {
        return -1;
    }
    return speed;
}

uint32_t pi_spi_set_mode(uint32_t mode)
{
    int ret;
    ret = ioctl(fd, SPI_IOC_WR_MODE,&mode);
    if (ret > 0)
    {
        return -1;
    }
    return mode;
}

uint8_t pi_spi_set_bits(uint8_t bits)
{
    int ret;
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD,&bits);
    if (ret > 0)
    {
        return -1;
    }
    return bits;
}

uint32_t pi_spi_set_speed(uint32_t speed)
{
    int ret;
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,&speed);
    if (ret > 0)
    {
        return -1;
    }
    return speed;
}


uint8_t * transfer(int cs, uint8_t * tx, size_t len)
{
    
    int ret;
    uint8_t * rx;
    uint32_t speed;
    uint8_t bits;
    speed = pi_spi_get_speed();
    bits = pi_spi_get_bits();
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = len,
        .delay_usecs = 0,
        .speed_hz = speed,
        .bits_per_word = bits,
    };
    
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        rx[0] = -1;
        return rx;
    }
    return (rx);
}

void setup_CS(int *CS)
{
    //
    // Set up a memory regions to access GPIO
    //
    int c;
    /* open /dev/mem */
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("can't open /dev/mem \n");
        exit(-1);
    }
    
    /* mmap GPIO */
    gpio_map = mmap(
                    NULL,             //Any adddress in our space will do
                    BLOCK_SIZE,       //Map length
                    PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
                    MAP_SHARED,       //Shared with other processes
                    mem_fd,           //File to map
                    GPIO_BASE         //Offset to GPIO peripheral
                    );
    
    close(mem_fd); //No need to keep mem_fd open after mmap
    
    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int)gpio_map);//errno also set!
        exit(-1);
    }
    
    // Always use volatile pointer!
    gpio = (volatile unsigned *)gpio_map;
    
    //Configure the pins to be CS lines
    foreach(c,CS)
    {
        INP_GPIO(c); // must use INP_GPIO before we can use OUT_GPIO
        OUT_GPIO(c);
    }
    
    
}
