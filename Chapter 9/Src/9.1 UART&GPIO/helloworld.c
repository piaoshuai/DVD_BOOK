/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include "xparameters.h"
#include <stdio.h>
#include "platform.h"
#include "xuartps.h"
#include "xgpio.h"
#include "xgpiops.h"

XUartPs Uart_Ps;		/* The instance of the UART Driver */

int main()
{
	XUartPs_Config *Config;
	int Status;
	int SentCount = 0, RecvCount = 0;
	u8 HelloZynq[] = "Hello Zynq\n";
    u8 RecvBuf[3] = "";

    XGpio Gpio;
    XGpioPs Gpiops;
    XGpioPs_Config *ConfigPtr;
    u8 axi_Gpio_Data, ps_Gpio_Data;

	/*
	 * Look up the configuration in the config table and then initialize it.
     */
	Config = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Send "Hello Zynq"
	 */
	while (SentCount < (sizeof(HelloZynq) - 1)) {
		SentCount += XUartPs_Send(&Uart_Ps, &HelloZynq[SentCount], 1);
	}

	/*
	 * Initialize the GPIO driver
	 */
	Status = XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the direction for all signals to be output
	 */
	XGpio_SetDataDirection(&Gpio, 1, 0x0);

	/*
	 * Initialize the PS GPIO driver.
	 */
	ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpiops, ConfigPtr, ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the direction for all signals to be
	 * outputs and Enable the Output enable for the LED Pins.
	 */
	XGpioPs_SetDirection(&Gpiops, 2, 0xF);
	XGpioPs_SetOutputEnable(&Gpiops, 2, 0xF);

	while (1){
		/*
		 * Recieve Data and Display data.
		 */
		RecvCount = 0;
		while (RecvCount < 3) {
			while (!XUartPs_IsReceiveData(Config->BaseAddress));
			XUartPs_Recv(&Uart_Ps, &RecvBuf[RecvCount], 1);
			if (RecvBuf[RecvCount] == '\r') {
				RecvBuf[RecvCount] = '\n';
				XUartPs_Send(&Uart_Ps, &RecvBuf[RecvCount], 1);
				break;
			}
			XUartPs_Send(&Uart_Ps, &RecvBuf[RecvCount++], 1);
		}

		/*
		 * Change ASCII char to number
		 */
		if (RecvBuf[0] >= '0' && RecvBuf[0] <= '9')
			axi_Gpio_Data = RecvBuf[0] - '0';
		else if (RecvBuf[0] >= 'a' && RecvBuf[0] <= 'z')
			axi_Gpio_Data = RecvBuf[0] - 'a' + 10;
		else if (RecvBuf[0] >= 'A' && RecvBuf[0] <= 'Z')
			axi_Gpio_Data = RecvBuf[0] - 'A' + 10;
		XGpio_DiscreteWrite(&Gpio, 1, axi_Gpio_Data);

		if (RecvBuf[1] >= '0' && RecvBuf[1] <= '9')
			ps_Gpio_Data = RecvBuf[1] - '0';
		else if (RecvBuf[1] >= 'a' && RecvBuf[1] <= 'z')
			ps_Gpio_Data = RecvBuf[1] - 'a' + 10;
		else if (RecvBuf[1] >= 'A' && RecvBuf[1] <= 'Z')
			ps_Gpio_Data = RecvBuf[1] - 'A' + 10;
		XGpioPs_Write(&Gpiops, 2, ps_Gpio_Data);
	}
    return 0;
}
