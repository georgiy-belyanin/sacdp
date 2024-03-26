AES-like character device
=========================

A simple Linux kernel module representing character device, simulating one of the AES stages

Building:
--------
```bash
make
```

Testing:
--------
```bash
sudo insmod aes.ko
```

Example usage:
--------------
```c
sudo dd if=source if=/dev/aes-0 bs=16 # writing, first 256 bytes are key, later - are values to encode
sudo dd if=/dev/aes-0 of=dest bs=16 count=5 #reading
```

Known problems:
---------------
On some systems `class_create` is defined before 
