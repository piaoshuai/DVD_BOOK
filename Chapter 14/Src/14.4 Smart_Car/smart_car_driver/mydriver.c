#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>

#define DEVICE_NAME "device"
#define MYPWM_PHY_ADDR 0x6CA00000
#define MYGPIO_PHY_ADDR 0x41200000

MODULE_AUTHOR("Min Ma");
MODULE_DESCRIPTION("Smartcar dirver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");

static int mydriver_major;
static struct class* mydriver_class = NULL;
static struct device* mydriver_device = NULL;

unsigned long mypwm_addr = 0;
unsigned long mygpio_addr = 0;

static struct file_operations mydriver_fops = {
    .owner = THIS_MODULE,
};

static ssize_t sys_direction_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    printk("gpio must be output\n");
    return count;
}


static ssize_t sys_value_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    unsigned char num = 0;
    if (buf[2] >= '0' && buf[2] <= '9')
        num = ((buf[2] - '0') << 4);
    else if (buf[2] >= 'a' && buf[2] <= 'f')
        num = ((buf[2] - 'a' + 10) << 4);
    else if (buf[2] >= 'A' && buf[2] <= 'F')
        num = ((buf[2] - 'A' + 10) << 4);


    if (buf[3] >= '0' && buf[3] <= '9')
        num += buf[3] - '0';
    else if (buf[3] >= 'a' && buf[3] <= 'f')
        num += buf[3] - 'a' + 10;
    else if (buf[3] >= 'A' && buf[3] <= 'F')
        num += buf[3] - 'A' + 10;
    outb(num, mygpio_addr);
    return count;
}

static ssize_t sys_pwm1_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    outl(num, mypwm_addr);
    return count;
}

static ssize_t sys_pwm2_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    outl(num, mypwm_addr+4);
    return count;
}

static ssize_t sys_pwm3_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    outl(num, mypwm_addr+8);
    return count;
}

static ssize_t sys_pwm4_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    outl(num, mypwm_addr+12);
    return count;
}

static DEVICE_ATTR(direction, S_IWUSR, NULL, sys_direction_set);             //GPIO input or output, we set output as default
static DEVICE_ATTR(value, S_IWUSR, NULL, sys_value_set);             //GPIO output
static DEVICE_ATTR(pwm1_speed, S_IWUSR, NULL, sys_pwm1_speed_set);
static DEVICE_ATTR(pwm2_speed, S_IWUSR, NULL, sys_pwm2_speed_set);
static DEVICE_ATTR(pwm3_speed, S_IWUSR, NULL, sys_pwm3_speed_set);
static DEVICE_ATTR(pwm4_speed, S_IWUSR, NULL, sys_pwm4_speed_set);

static int __init mydriver_module_init(void)
{
    int ret;

    mydriver_major=register_chrdev(0, DEVICE_NAME, &mydriver_fops);
    if (mydriver_major < 0){
        printk("failed to register device.\n");
        return -1;
    }

    mydriver_class = class_create(THIS_MODULE, "mydriver");
    if (IS_ERR(mydriver_class)){
        printk("failed to create device class.\n");
        unregister_chrdev(mydriver_major, DEVICE_NAME);
        return -1;
    }


    mydriver_device = device_create(mydriver_class, NULL, MKDEV(mydriver_major, 0), NULL, "my_device");
    if (IS_ERR(mydriver_device)){
        printk("failed to create device .\n");
        unregister_chrdev(mydriver_major, DEVICE_NAME);
        return -1;
    }

    ret = device_create_file(mydriver_device, &dev_attr_direction);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
    ret = device_create_file(mydriver_device, &dev_attr_value);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
    ret = device_create_file(mydriver_device, &dev_attr_pwm1_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");

    ret = device_create_file(mydriver_device, &dev_attr_pwm2_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
    ret = device_create_file(mydriver_device, &dev_attr_pwm3_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
    ret = device_create_file(mydriver_device, &dev_attr_pwm4_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");

    mypwm_addr = (unsigned long)ioremap(MYPWM_PHY_ADDR, sizeof(u32));
    mygpio_addr = (unsigned long)ioremap(MYGPIO_PHY_ADDR, sizeof(u32));
    outb(0x00, mygpio_addr+4);
    outb(0x55, mygpio_addr);

    printk("my pwm driver initial successfully!\n");
    return 0;
}

static void __exit mydriver_module_exit(void)
{
    device_remove_file(mydriver_device, &dev_attr_direction);
    device_remove_file(mydriver_device, &dev_attr_value);
    device_remove_file(mydriver_device, &dev_attr_pwm1_speed);
    device_remove_file(mydriver_device, &dev_attr_pwm2_speed);
    device_remove_file(mydriver_device, &dev_attr_pwm3_speed);
    device_remove_file(mydriver_device, &dev_attr_pwm4_speed);
    device_destroy(mydriver_class, MKDEV(mydriver_major, 0));
    class_unregister(mydriver_class);
    class_destroy(mydriver_class);
    unregister_chrdev(mydriver_major, DEVICE_NAME);
    printk("pwm module exit.\n");
}

module_init(mydriver_module_init);
module_exit(mydriver_module_exit);
