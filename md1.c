

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>
int  x,y;
bool  ax=0,ay=0,ac=0;
char  c;

static ssize_t solve_read(struct file * file, char * buf,size_t count, loff_t *ppos)
{	int d;
	char answer[50];
	if (*ppos != 0)
                return 0;

	if  ((ax==0)||(ay==0)||(ac==0))
	{strcpy(answer,"параметры не введеныЭ \n");
	strcpy(buf,answer);
	*ppos = strlen(answer);
	return strlen(answer);}
	switch (c)
	{
	case '+':
	d=x+y;
	break;
	case '-':
	d=x-y;
	break;
	case '*':
	d=x*y;
	break;
	case '/':
	d=x/y;
	break;
	default:
	strcpy(answer," неверная операция \n");
	strcpy(buf,answer);
        *ppos = strlen(answer);
	return strlen(answer);
	}
	sprintf(answer,"%d\n",d);

	strcpy(buf,answer);
	*ppos = strlen(answer);

	return strlen(answer);
}

static int par1_write(struct file *file, const char *buf, unsigned long count, void *data)
{
	sscanf(buf,"%d",&x);
	ax=1;
	printk(KERN_INFO "x = %d\n", x);
	return count;
}
static int par2_write(struct file *file, const char *buf, unsigned long count, void *data)
{
        sscanf(buf,"%d",&y);
	ay=1;
	printk(KERN_INFO "y = %d\n", y);
        return count;
}
static int solve_write(struct file *file, const char *buf, unsigned long count, void *data)
{
        sscanf(buf,"%c",&c);
	ac=1;
	printk(KERN_INFO "c = %c\n", c);
        return count;
}

static const struct file_operations solve_fops = {
	.owner	= THIS_MODULE,
	.read	= solve_read,
	.write	= solve_write,
};
static const struct file_operations par1_fops = {
	.owner = THIS_MODULE,
	.write = par1_write,
};
static const struct file_operations par2_fops = {
        .owner = THIS_MODULE,
        .write = par2_write,
};

static struct miscdevice solve_dev = {MISC_DYNAMIC_MINOR,"kalk_result",&solve_fops};
static struct miscdevice par1_dev = {MISC_DYNAMIC_MINOR,"kalk_par1",&par1_fops};
static struct miscdevice par2_dev = {MISC_DYNAMIC_MINOR,"kalk_par2",&par2_fops};

static int __init
hello_init(void)
{
	printk(KERN_INFO"Module loaded\n");
	misc_register(&solve_dev);
	misc_register(&par2_dev);
	misc_register(&par1_dev);
	return 0;
}

module_init(hello_init);

static void __exit
hello_exit(void)
{
	printk(KERN_INFO"Module unloaded\n");
	misc_deregister(&solve_dev);
	misc_deregister(&par2_dev);
	misc_deregister(&par1_dev);
}

module_exit(hello_exit);

MODULE_LICENSE("fucking license");
MODULE_AUTHOR("alexey ");
MODULE_DESCRIPTION("choto delaet");
MODULE_VERSION("dev");
