#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>


void GPIO_INIT()
{

        SYSCTL_RCGCGPIO_R |= 0x01;  //enable clock to portA

       //GPIO_PORTA_LOCK_R = 0X4C4F434B;
       //GPIO_PORTA_CR_R = 0X1F;

       GPIO_PORTA_DIR_R = 0x0E;    //set correct directions

       GPIO_PORTA_DEN_R = 0x1F;    //enable digital operation at pins
       GPIO_PORTA_PUR_R = 0x11;    //enable pullups for switches

}


int UART_TxRx(int data)
{

    int ret;    // return variable
    /*
     *
     *  clock Regs..............
     *  UARTCC
     *  UARTCTL....set to zero then set to 1 after configuration
     */

        SYSCTL_RCGCUART_R = 0x01;   // choose  UART0
        UART0_CC_R = 0x0;           // system clock source // check for delayed enable....?
        UART0_CTL_R = 0x00;      // UARTEN is disabled..

     /*
     *  control Regs...................
     *  UARTRSR/ECR
        UARTFR
        UARTLCRH
        UARTCTL
        UARTILPR
        UART9BITADDR
        UART9BITAMASK
        UARTPP

        data Regs....................
        UARTDR

     *
     *
     *
     *
     * */

        /* 9600 = f/16 * divisor ----------- 9600*16/f ===  104.1667
         *
         *
                baudRate Regs..............
                UARTIBRD ===== 104
                UARTFBRD ===== 0.1667 x 64 + 0.5 ==== 11
         *
         *
         */
        UART0_IBRD_R = 104;
        UART0_FBRD_R = 11;
        UART0_LCRH_R = 0xE2;    // odd parity-one stop bit-fifo dis-8bit len-checkparity as 1


        UART0_CTL_R = 0x311;      // UARTEN is enabled, EOT is set ... sys_clk/16, TxRx enabled  //


    /*  Using UART0 for Transmitting operation
     *
     * PA0----->Tx
     * PA1----->Rx
     *
     *
     * */

    GPIO_PORTA_AFSEL_R = 0x03;  // UART is enabled on PA0 and PA1
    GPIO_PORTA_PCTL_R  = 0x03;  // encoding is 1 for UART0, PMC.... setting 1



    if((UART0_FR_R & (1<<5)) == 1)
        UART0_DR_R = data;



    if((UART0_FR_R & !(1<<4)) == 0)
        ret = UART0_DR_R;



    return ret;


}




void main()
{
    /* 0x0F ---> sw1
     *
     * 0xAA ---->sw2
     * */

    int data = 0xA6;
    int des;

    GPIO_INIT();

    des = UART_TxRx(data);



    while(1);

}
