#include "ap_interfaces.h"
#include <iostream>

using namespace std;

void my_adder(unsigned input1, unsigned input2, unsigned& output, unsigned char special = 0) {
AP_INTERFACE(input1,ap_none);
AP_INTERFACE(input2,ap_none);
AP_INTERFACE(output,ap_none);
AP_INTERFACE(special,ap_none);
AP_BUS_AXI4_LITE(input1, CONTROL_BUS);
AP_BUS_AXI4_LITE(input2, CONTROL_BUS);
AP_BUS_AXI4_LITE(output, CONTROL_BUS);
AP_BUS_AXI4_LITE(special, CONTROL_BUS);
AP_CONTROL_BUS_AXI(MY_ADDER_CONTROL); 

      unsigned tmp = input1 + input2;
      switch (special) {
      case 1:
            tmp *= tmp;
            break;
      case 2:
            tmp += 3;
            break;
      }
      output = tmp;
}

#ifndef __SYNTHESIS__
int main() {
      unsigned in1 = 43;
      unsigned in2 = 98767;
      unsigned out = 0;

      my_adder(in1, in2, out, 0);
      if (out != in1+in2) {
           cout << "Error: " << in1 << "+" << in2 << "=" << in1+in2 << " != " << out << endl;
            return -1;
      } else {
            cout << "Succes!" << endl;
            return 0;
      }
}
#endif
