/* (very) simple example for "XMC 2 go" kit, using XMC lib in DAVE v4 */

/* Author: WoS, 7/2015 - reworked from a xmc lib example with help from jferreira and chismo (Infineon xmc forum) */

/* In Dave, use "new project" -> "DAVE project" -> "Simple main project" and replace the
   inital content of main.c with this code. Requires terminal program (115200/8/n/1) to run.*/

/* Made for illustration only. Use at your own risk. No guarantees whatsoever. */

#include <xmc_gpio.h>
#include <xmc_uart.h>
#include "mcurses.h"
#include "demo.h"

void putChar(uint8_t msg)
{
	XMC_UART_CH_Transmit(XMC_UART0_CH0, msg);
}

char getChar(void)
{
	//is there some data?
	if (!XMC_USIC_CH_GetReceiveBufferStatus(XMC_UART0_CH0))
	  return 0;

	return XMC_UART_CH_GetReceivedData(XMC_UART0_CH0);
}

int main(void)  {
	/* UART configuration */
	const XMC_UART_CH_CONFIG_t uart_config = {
	  .baudrate  = 115200,
	  .data_bits = 8U,
	  .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE,
	  .stop_bits = 1U
	};

	/* Configure UART channel, to do this you need to fix the ifdef at line 161 in xmc_uart.h of the XMC peripheral library */
	XMC_UART_CH_Init(XMC_UART0_CH0, &uart_config);
	XMC_UART_CH_SetInputSource(XMC_UART0_CH0, XMC_UART_CH_INPUT_RXD, USIC0_C0_DX0_DX3INS);
	XMC_UART_CH_SetInputSource(XMC_UART0_CH0, XMC_UART_CH_INPUT_RXD1, USIC0_C0_DX3_P2_2);

	/* Start UART channel */
	XMC_UART_CH_Start(XMC_UART0_CH0);

	/* GPIO configuration */
	const XMC_GPIO_CONFIG_t p2_2_conf = {
	  .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
	  .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};
	const XMC_GPIO_CONFIG_t p2_1_conf = {
	  .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6,
	  .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};
	const XMC_GPIO_CONFIG_t p1_0_conf = {
	  .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
	  .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW
	};

	/* Configure GPIO, P2 also needs to be set to digital - mode set is not enough */
	XMC_GPIO_Init(P2_1,&p2_1_conf);					/* UART TX */
	XMC_GPIO_Init(P2_2,&p2_2_conf);					/* UART RX */
	XMC_GPIO_Init(P1_0,&p1_0_conf);					/* LED */

	/* Send some text */
 	const uint8_t message[] = "Hello world!!\n";
 	uint32_t index;
	for (index = 0; index < sizeof(message) - 1; index++) {
		XMC_UART_CH_Transmit(XMC_UART0_CH0, message[index]);
	}

	/* Toggle LED (was off after init, now it is on) */
	XMC_GPIO_ToggleOutput(P1_0);

	setFunction_putchar(putChar);
	setFunction_getchar(getChar);

	/* Wait until receiving key 'A', then toggle LED */
	while(1) {
		tempDemo();
		//keyDemo();
	}

	/* Never get here... */
	return 0;
}
