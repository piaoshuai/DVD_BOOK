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

#include <stdio.h>
#include "xil_io.h"
#include "platform.h"
#include "xparameters.h"

#define MY_COUNTER_CONTROL_BASEADDR XPAR_MY_ADDER_TOP_0_S_AXI_MY_ADDER_CONTROL_BASEADDR
#define MY_COUNTER_START 0x0000
#define MY_COUNTER_DONE 0x0004
#define MY_COUNTER_IDLE 0x0008

#define MY_COUNTER_IO_BASEADDR XPAR_MY_ADDER_TOP_0_S_AXI_CONTROL_BUS_BASEADDR
#define MY_COUNTER_INPUT1 0x0000
#define MY_COUNTER_INPUT2 0x0004
#define MY_COUNTER_OUTPUT 0x0008
#define MY_COUNTER_SPECIAL 0x000c

#define MY_WriteReg(BaseAddress, RegOffset, RegisterValue) \
	Xil_Out32((BaseAddress) + (RegOffset), (RegisterValue))
#define MY_ReadReg(BaseAddress, RegOffset) \
	Xil_In32((BaseAddress) + (RegOffset))

void print(char *str);

int main()
{
	print("*** AutoESL pcore test ***\r\n");
	unsigned in1 = 77;
	unsigned in2 = 8976987;
	unsigned special = 1; // square the sum...
	unsigned out_sw = in1+in2;
	out_sw *= out_sw;
	unsigned out_hw;

	// write inputs
	MY_WriteReg(MY_COUNTER_IO_BASEADDR, MY_COUNTER_INPUT1, in1);
	MY_WriteReg(MY_COUNTER_IO_BASEADDR, MY_COUNTER_INPUT2, in2);
	MY_WriteReg(MY_COUNTER_IO_BASEADDR, MY_COUNTER_SPECIAL, special);

	// now toggle start
	MY_WriteReg(MY_COUNTER_CONTROL_BASEADDR, MY_COUNTER_START, 0);
	MY_WriteReg(MY_COUNTER_CONTROL_BASEADDR, MY_COUNTER_START, 0xFFFFFFFF);
	MY_WriteReg(MY_COUNTER_CONTROL_BASEADDR, MY_COUNTER_START, 0);

	// now wait for idle
	while (!MY_ReadReg(MY_COUNTER_CONTROL_BASEADDR, MY_COUNTER_IDLE)) {
	}

	// and read the result
	out_hw = MY_ReadReg(MY_COUNTER_IO_BASEADDR, MY_COUNTER_OUTPUT);

	int retval;
	if (out_hw != out_sw) {
		printf("Error: %u + %u = %u != %u\r\n", in1, in2, out_sw, out_hw);
		retval = -1;
	} else {
		print("HW adder ok!\r\n");
		retval = 0;
	}

	return retval;
}
