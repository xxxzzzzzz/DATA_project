#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h> //file_operations这个结构体的头文件
#include<linux/cdev.h>//使用cdev_init cdev_add包含该头文件
#include<linux/device.h> //在/dev下创建设备节点函数device_create需要这个头文件
#include<asm/uaccess.h>//copy_from_user,copy_to_user要包含该头文件
static int test_open(struct inode *inode, struct file *filp)
{

//	printk("enter open!!\n");
	return 0;
}

static int test_open2(struct inode *inode, struct file *filp)
{

//	printk("enter open!!\n");
	return 0;
}

static int test_release(struct inode *inode, struct file *filp)
{
	//printk("enter release!!\n");
	return 0;
}

static int test_release2(struct inode *inode, struct file *filp)
{
	//printk("enter release!!\n");
	return 0;
}
static int data = 500;//拷贝int数据
static ssize_t test_read(struct file *file, char __user *buf,size_t len, loff_t *ppos)
{
	int ret = copy_to_user(buf,&data,sizeof(&data));
	printk("enter read!!\n");
	if(ret)//如果ret等于0表示copy_to_user成功，如果大于0表示有多少没有copy成功
	{
		printk("transfer failed num = %d\n",ret);
		return -EINVAL;
	}
	return 0;
}

static int kdata; //这里定义kdata是接受data拷贝过来的int数据
ssize_t test_write(struct file *file, const char __user *data,size_t len, loff_t *ppos)
{
	printk("enter write!!\n");
	int ret = copy_from_user(&kdata,data,len);//copy_from_user返回值，如果全部数据传输成功返回0，
						   //如果传输了一半断了，返回剩下没有传输的数据大小 
	if(ret)//如果没有传输完，ret会得到一个大于0的数
	{
		printk("transfer failed num = %d\n",ret);
		return -EINVAL;
	}
	printk("%d\n",kdata);
	return 0;
}

static const struct file_operations test_fops =
{
	.owner = THIS_MODULE, //这个必须加
	.open = test_open,   
	.release = test_release,
	.read = test_read, 
	.write = test_write,
};

static const struct file_operations test_fops2 =
{
	.owner = THIS_MODULE, //这个必须加
	.open = test_open2,   
	.release = test_release2,
};

static int major;//用来获取自动创建的主设备号的变量
dev_t dev_id;//自动创建的主次设备号放在这里面 
static struct cdev ptestchar_cdev;//为第1个驱动test_fops创建cdev来关联
static struct cdev ptestchar_cdev2;//为第2个驱动test_fops2创建cdev来关联
static struct class *test_class; 

static int __init chrdev_init(void) 
{ 					
	printk("init module_driver\n");


	int ret = alloc_chrdev_region(&dev_id, 0, 2,"test_foops");
	if(ret<0)
	{

		printk("MKDEV register failed\n");
		return -EINVAL;
	}
		printk("MKDEV register success\n");
		printk("get major = %d minor = %d\n",MAJOR(dev_id),MINOR(dev_id));
	major = MAJOR(dev_id);//把自动创建好的注次设备号的主设备获取出来，到时候给创建设备节点的函数class_device_create用	

	cdev_init(&ptestchar_cdev,&test_fops);
	ret = cdev_add(&ptestchar_cdev,dev_id,2);

	if(ret)
	{
		printk("cdev_add register failed\n");
		return -EINVAL;
	}
		printk("cdev_add register success\n");

	dev_id = MKDEV(major,2);//因为我们前面已经分配了主设备号，次设备号0，1了，我们第二个驱动就要从次设备号2开始向后分配 
	ret = register_chrdev_region(dev_id, 1, "test_fops2");
	if(ret<0)
		printk("test_fops2 register chrdev failed\n");

	cdev_init(&ptestchar_cdev2,&test_fops2);//按照上面的老方法把cdev和fops关联起来
	ret = cdev_add(&ptestchar_cdev2,dev_id,1);//因为现在dev_id是从次设备号2开始计算的，所以cdev被挂在次设备号2上，fops也间接被挂在次设备号2上

	test_class = class_create(THIS_MODULE, "xzz_class"); 
	if (IS_ERR(test_class))
	{
		printk("class_create failed\n");
		return -1;
	}
	device_create(test_class, NULL, MKDEV(major,0), NULL,"xzz_dev_class0");//xzz_dev_class2指向的是test_fops
	device_create(test_class, NULL, MKDEV(major,1), NULL,"xzz_dev_class1");//xzz_dev_class2指向的是test_fops
	device_create(test_class, NULL, MKDEV(major,2), NULL,"xzz_dev_class2");//xzz_dev_class2指向的是test_fops2
	return 0;
}

static void __exit chrdev_exit(void) 
{
	/*1.注销class和主次设备号关系*/
	//device_destroy(test_class,dev_id);//不能直接用dev_id变量来注销设备，因为我建立了多个次设备,dev_id默认是主设备号，次设备号0
	device_destroy(test_class,MKDEV(major,0));
	device_destroy(test_class,MKDEV(major,1));
	device_destroy(test_class,MKDEV(major,2));
	class_destroy(test_class);
	/*2.注销字符设备驱动*/
	cdev_del(&ptestchar_cdev);
	cdev_del(&ptestchar_cdev2);
	/*3.注销我们申请的主次设备号*/
	unregister_chrdev_region(MKDEV(major,0),1);
	unregister_chrdev_region(MKDEV(major,1),1);
	unregister_chrdev_region(MKDEV(major,2),1);

	printk("exit module_driver");
}

module_init(chrdev_init);
module_exit(chrdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");
MODULE_DESCRIPTION("module_test"); 
