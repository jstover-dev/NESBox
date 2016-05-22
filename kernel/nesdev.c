#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>

//#include <linux/slab.h>     /* kmalloc() */
//#include <linux/proc_fs.h>
//#include <linux/fcntl.h>    /* O_ACCMODE */


#include "nesdev.h"


static ssize_t nesdev_read(struct file *, char *, size_t, loff_t *);
static ssize_t nesdev_write(struct file *, const char *, size_t, loff_t *);
static int nesdev_open(struct inode *, struct file *);
static int nesdev_release(struct inode *, struct file *);

static int __init nesdev_init(void);
static void __exit nesdev_exit(void);

struct file_operations nes_fops = {
  read:    nesdev_read,
  write:   nesdev_write,
  open:    nesdev_open,
  release: nesdev_release
};

module_init(nesdev_init);
module_exit(nesdev_exit);



static unsigned int device_major = 0;

static struct class *device_class = NULL;

static struct nesdev *nes_device = NULL;


static int
nesdev_create(struct nesdev *dev, int minor, struct class *class) {

  int err = 0;
  dev_t devno = MKDEV(device_major, minor);
  struct device *device = NULL;

  BUG_ON( dev==NULL || class==NULL );

  dev->pad_data = 0;
  dev->panel_data = 0;
  mutex_init(&dev->mutex);
  cdev_init(&dev->cdev, &nes_fops);

  err = cdev_add(&dev->cdev, devno, 1);
  if (err) {
    printk(KERN_WARNING "[target] Error %d while trying to add %s%d\n", err, DEVICE_NAME, minor);
    return err;
  }

  device = device_create(class, NULL, devno, NULL, DEVICE_NAME "%d", minor);
  if (IS_ERR(device)) {
    printk(KERN_WARNING "[target] Error %d while trying to create %s%d", err, DEVICE_NAME, minor);
    cdev_del(&dev->cdev);
    return err;
  }

  return 0;

}



static int __init
nesdev_init(void) {

  int err = 0;
  dev_t dev = 0;

  // create device node
  err = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
  if (err < 0) {
    printk(KERN_WARNING "[target] alloc_chrdev_region() failed\n");
    return err;
  }
  device_major = MAJOR(dev);


  // create device class
  device_class = class_create(THIS_MODULE, DEVICE_NAME);
  if (IS_ERR(device_class)) {
    err = PTR_ERR(device_class);
    nesdev_cleanup();
    return err;
  }


  // construct device
  err = nesdev_create(nes_device, 0, device_class);
  if (err) {
    nesdev_cleanup();
    return err;
  }

  // Device created, loading finished
  printk(KERN_INFO "[target] loaded.\n");
  return 0;
}


static void __exit
nesdev_exit(void) {
  nesdev_cleanup();
  printk(KERN_INFO "[target] unloaded.\n");
}


static int
nesdev_open(struct inode *inode, struct file *filp) {
  return 0;
}


static int
nesdev_release(struct inode *inode, struct file *filp) {
  return 0;
}


static ssize_t
nesdev_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
  ssize_t retVal = 0;

  if (mutex_lock_killable(&nes_device->mutex)) {
    return -EINTR;
  }

  // nes_device critical read section


  mutex_unlock(&nes_device->mutex);
  return retVal;
}


static ssize_t
nesdev_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {

  ssize_t retVal = 0;

  if (mutex_lock_killable(&nes_device->mutex)) {
    return -EINTR;
  }

  // nes_device critical write section


  mutex_unlock(&nes_device->mutex);
  return 1;
}


static void
nesdev_cleanup(void) {
  device_destroy(device_class, MKDEV(device_major, 0));
  cdev_del(&nes_device->cdev);
  mutex_destroy(&nes_device->mutex);

  if (device_class) {
    class_destroy(device_class);
  }
  unregister_chrdev_region(MKDEV(device_major, 0), 1);
}


MODULE_AUTHOR("Joshua Stover <joshua.stover2@griffithuni.edu.au>");
MODULE_DESCRIPTION("Nintendo Entertainment System hardware interface module");
MODULE_LICENSE("GPL");
