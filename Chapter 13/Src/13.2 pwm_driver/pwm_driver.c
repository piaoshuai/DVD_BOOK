#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>

#define DEVICE_NAME "PWM_MOUDLE"
#define PWM_MOUDLE_PHY_ADDR 0x6CA00000		//This Address is based XPS

MODULE_AUTHOR("Xilinx XUP");
MODULE_DESCRIPTION("PWM moudle dirver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");



static int pwm_driver_major;
static struct class* pwm_driver_class = NULL;
static struct device* pwm_driver_device = NULL;

unsigned long pwm_fre_addr = 0;		//pwm moulde's frequency visual address
unsigned long pwm_duty_addr = 0;	//pwm moulde's duty visual address
static long frequency=0;

static struct file_operations pwm_driver_fops = {
    .owner = THIS_MODULE,
};

static ssize_t sys_pwm_frequency_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long value = 0;
    int i;
    frequency=0;
    outl(value,  pwm_fre_addr); //close pwm moudle before we modfiy the frequency

    for (i = 0; i < count-1; i++){
        frequency  *= 10;
        frequency += buf[i] - '0';
    }
    if(value>100000000) value=100000000;
    value=100000000/frequency;  // 100Mhz/frequency 100Mhz is set by XPS
	
    outl(value,  pwm_fre_addr);
    return count;
} 


static ssize_t sys_pwm_duty_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count) //duty cycle 
{
    long value = 0;
    int i;
//
	outl(value,  pwm_duty_addr); //close pwm moudle before we modfiy the duty cycle

    for (i = 0; i < count-1; i++){
        value  *= 10;
        value += buf[i] - '0';
    }
	if (value>100) value=100;
	value=100000000/frequency*value/100;
   
    if (value!= 0)
        value = value | 0x80000000;
    	outl(value,  pwm_duty_addr);

    return count;
} 

static DEVICE_ATTR(pwm_frequency, S_IWUSR, NULL, sys_pwm_frequency_set);
static DEVICE_ATTR(pwm_duty, S_IWUSR, NULL, sys_pwm_duty_set);


static int __init pwm_driver_module_init(void)
{
    int ret;

    pwm_driver_major=register_chrdev(0, DEVICE_NAME, &pwm_driver_fops);
    if (pwm_driver_major < 0){
        printk("failed to register device.\n");
        return -1;
    }

    pwm_driver_class = class_create(THIS_MODULE, "pwm_driver");
    if (IS_ERR(pwm_driver_class)){
        printk("failed to create pwm moudle class.\n");
        unregister_chrdev(pwm_driver_major, DEVICE_NAME);
        return -1;
    }


    pwm_driver_device = device_create(pwm_driver_class, NULL, MKDEV(pwm_driver_major, 0), NULL, "pwm_device");
    if (IS_ERR(pwm_driver_device)){
        printk("failed to create device .\n");
        unregister_chrdev(pwm_driver_major, DEVICE_NAME);
        return -1;
    }
   
    ret = device_create_file(pwm_driver_device, &dev_attr_pwm_frequency);
    if (ret < 0)
        printk("failed to create pwm_frequency endpoint\n");
    
    ret = device_create_file(pwm_driver_device, &dev_attr_pwm_duty);
    if (ret < 0)
        printk("failed to create pwm_duty endpoint\n");
   
   	pwm_fre_addr = (unsigned long)ioremap(PWM_MOUDLE_PHY_ADDR, sizeof(u32));//To get Custom IP--PWM moudle's virtual address
	pwm_duty_addr = pwm_fre_addr+4;		
        			
    printk(" pwm driver initial successfully!\n");
    return 0;
}



static void __exit pwm_driver_module_exit(void)
{
    device_remove_file(pwm_driver_device, &dev_attr_pwm_frequency);
    device_remove_file(pwm_driver_device, &dev_attr_pwm_duty);
    device_destroy(pwm_driver_class, MKDEV(pwm_driver_major, 0));
    class_unregister(pwm_driver_class);
    class_destroy(pwm_driver_class);
    unregister_chrdev(pwm_driver_major, DEVICE_NAME);
    printk("pwm module exit.\n");
}

module_init(pwm_driver_module_init);
module_exit(pwm_driver_module_exit);


