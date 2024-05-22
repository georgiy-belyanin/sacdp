AES-like character device
=========================

A simple Linux 6.0+ kernel module representing character device, simulating one of the AES stages

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
```bash
sudo dd if=source if=/dev/aes0 bs=16 # encoding, first 256 bytes are key, later - are values to encode
sudo dd if=/dev/aes0 of=dest bs=16 #reading encoded string
sudo dd if=dest if=/dev/aes1 bs=16 #decoding
sudo dd if=/dev/aes1 of=dest bs=16 #reading decoding results
```

