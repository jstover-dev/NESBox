/*
  nesdev.h

  Module for interacting with a custom arduino-based interface to the
  Nintendo Entertainment System hardware.

  based on the example module provided at
  https://github.com/euspectre/kedr/tree/master/sources/examples/sample_target
*/
#ifndef NESDEVKM_H
#define NESDEVKM_H


#define MODULE_NAME "nesdev"

#define DEVICE_NAME "nes"

struct nesdev {
    uint16_t pad_data;
    uint8_t panel_data;
    struct mutex mutex;
    struct cdev cdev;
};

static int nesdev_create(struct nes_dev *, int, struct class *);

static void nesdev_cleanup(void);


#endif
