#include <linux/init.h> 
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include "field.h"

#define AES_KEY_SIZE 256
#define AES_BLOCK_SIZE 16

#define min_num(x, y) ((x) < (y) ? (x) : (y))

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Georgiy Belyanin");
MODULE_DESCRIPTION("AES-like character dev");
MODULE_VERSION("0.1");

struct cdev_data {
    struct cdev cdev;
};
struct aes_block {
    unsigned char values[AES_BLOCK_SIZE];
    struct aes_block *next;
};

static int cdev_major = 0;
static struct class *aesdev_class = NULL;
static struct cdev_data aesdev_data;
static unsigned char key[AES_KEY_SIZE];
static unsigned int key_index;
static struct aes_block *head;
static struct aes_block *tail;
static unsigned char shuffle[AES_BLOCK_SIZE] = { 5, 7, 4, 6, 13, 15, 12, 14, 1, 3, 0, 2, 9, 11, 8, 10 };

static struct aes_block *push_block(void) {
    struct aes_block *aes_block = kmalloc(sizeof(*aes_block), GFP_KERNEL);

    if (aes_block == NULL) return NULL;
    aes_block->next = NULL;

    if (tail == NULL)
        head = aes_block;
    else
        tail->next = aes_block;
    tail = aes_block;

    return aes_block;
}
static struct aes_block *pop_block(void) {
    struct aes_block *aes_block = head;

    if (head != NULL)
        head = head->next;
    if (head == NULL)
        tail = NULL;

    return aes_block;
}

static int transform_block(struct aes_block *aes_block) {
    size_t i, j;
    unsigned char h;
    unsigned char result[AES_BLOCK_SIZE];

    for (j = 0; j < AES_KEY_SIZE / AES_BLOCK_SIZE; j++) {
        for (i = 0; i < AES_BLOCK_SIZE; i++) {
            f_t f = f_from_uint8(aes_block->values[i]);
            if (f == NULL) return -ENOMEM;

            f_t g = f_inv(f);
            destroy_f(f);
            if (g == NULL) return -ENOMEM;
            h = f_to_uint8(g);
            destroy_f(g);

            aes_block->values[i] = h ^ key[i + j * AES_BLOCK_SIZE];
        }

        for (i = 0; i < AES_BLOCK_SIZE; i++)
            result[i] = aes_block->values[shuffle[i]];
        for (i = 0; i < AES_BLOCK_SIZE; i++)
            aes_block->values[i] = result[i];
    }
    return 0;
}

static void block_cleanup(void) {
    struct aes_block *aes_block = head;
    while (aes_block != NULL) {
        aes_block = head->next;
        kfree(head);
    }
}

static ssize_t cdev_read(struct file *file, char __user *buf, size_t count, loff_t *offset) {
    size_t block_count = count / AES_BLOCK_SIZE;
    struct aes_block *aes_block;
    size_t i;
    size_t err;

    if (count % AES_BLOCK_SIZE != 0) {
        printk(KERN_ERR "Expected read size as a multiple of AES_BLOCK_SIZE: %lu\n", count);
        return -EFAULT;
    }

    for (i = 0; i < block_count && (aes_block = pop_block()) != NULL; i++) {
        err = copy_to_user(buf, aes_block->values, AES_BLOCK_SIZE);
        if (err != 0) {
            printk(KERN_ERR "Copy data to user failed\n");
            return -EFAULT;
        }
        kfree(aes_block);
    }

    return count;
}

static ssize_t cdev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset) {
    size_t i;
    size_t n;
    size_t key_read = 0;
    struct aes_block *aes_block;
    int err;

    printk(KERN_DEBUG "Entering: %s\n", __func__);

    if (count % AES_BLOCK_SIZE != 0) {
        printk(KERN_ERR "Expected write size as a multiple of AES_BLOCK_SIZE: %lu\n", count);
        return -EFAULT;
    }

    if (key_index < AES_KEY_SIZE) {
        key_read = min_num(count, AES_KEY_SIZE - key_index);
        n = copy_from_user(key + key_index, buf, key_read);
        if (n != 0) {
            printk(KERN_ERR "Copy data from user failed\n");
            return -EFAULT;
        }

        key_index += key_read;
        if (key_read == count)
            return count;
    }


    for (i = 0; i < (count - key_read) / AES_BLOCK_SIZE; i++) {
        aes_block = push_block();
        if (aes_block == NULL) {
            printk(KERN_ERR "No memoery left\n");
            return -EFAULT;
        }
        n = copy_from_user(aes_block->values, buf + key_read + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        if (n != 0) {
            printk(KERN_ERR "Copy data from user failed\n");
            return -EFAULT;
        }
        err = transform_block(aes_block);
        if (n < 0) {
            printk(KERN_ERR "Error while transforming aes block, probably no memory left\n");
            return -EFAULT;
        }
    }

    return count;
}

static const struct file_operations cdev_fops = {
    .owner    = THIS_MODULE,
    .read    = cdev_read,
    .write   = cdev_write
};

int init_module(void) {
    int err;
    int result = 0;
    struct device *dev_ret;
    dev_t dev;

    printk(KERN_DEBUG "Entering: %s\n", __func__);

    err = alloc_chrdev_region(&dev, 0, 1, "aes");
    if (err < 0) {
        printk(KERN_ERR "Allocate a range of char device numbers failed.\n");
        result = err;
        goto cleanup_0;
    }

    cdev_major = MAJOR(dev);
    printk(KERN_DEBUG "AES device major number is: %d\n", cdev_major);
    if (IS_ERR(aesdev_class = class_create("aes"))) {
        result = PTR_ERR(aesdev_class);
        goto cleanup_1;
    }

    if (IS_ERR(dev_ret = device_create(aesdev_class, NULL, MKDEV(cdev_major, 0), NULL, "aes-0"))) {
        result = PTR_ERR(dev_ret);
        goto cleanup_3;
    }

    cdev_init(&aesdev_data.cdev, &cdev_fops);
    aesdev_data.cdev.owner = THIS_MODULE;

    err = cdev_add(&aesdev_data.cdev, MKDEV(cdev_major, 0), 1);
    if (err < 0) {
        printk (KERN_ERR "Unable to add AES char device\n");
        result = err;
        goto cleanup_4;
    }

    key_index = 0;
    head = NULL;
    tail = NULL;

    goto cleanup_0;

cleanup_4:
    device_destroy(aesdev_class, MKDEV(cdev_major, 0));
cleanup_3:
    class_unregister(aesdev_class);
    class_destroy(aesdev_class);
cleanup_1:
    unregister_chrdev_region(MKDEV(cdev_major, 0), 1);
cleanup_0:
    return result;
}

void cleanup_module(void) {
    printk(KERN_DEBUG "Entering: %s\n", __func__);
    block_cleanup();
    device_destroy(aesdev_class, MKDEV(cdev_major, 0));
    class_unregister(aesdev_class);
    class_destroy(aesdev_class);
    unregister_chrdev_region(MKDEV(cdev_major, 0), 1);
}

