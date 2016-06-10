/*
 * Copyright (c) 2016 Joshua Stover
 *
 * NESBox controller driver for Raspberry Pi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/i2c.h>
//#include <linux/i2c-dev.h>

#define MODULE_NAME "nesbox"
#define MODULE_DESC "NESBox i2c controller driver for Raspberry Pi"

MODULE_AUTHOR("Joshua Stover <jmstover6@gmail.com>");
MODULE_DESCRIPTION(MODULE_DESC);
MODULE_LICENSE("GPL");


#define NB_PAD_COUNT      2
#define NB_BUTTON_COUNT   4

#define NB_I2C_ADDRESS    0x42
#define NB_I2C_NAME 	  "nesbox"

#define NB_CMD_PAD1_STATE 0x81
#define NB_CMD_PAD2_STATE 0x82


//static int nb_buttons[] = { BTN_A, BTN_B, BTN_SELECT, BTN_START };

#define NB_BIT_A      0x0
#define NB_BIT_B      0x1
#define NB_BIT_START  0x2
#define NB_BIT_SELECT 0x3
#define NB_BIT_UP     0x4
#define NB_BIT_DOWN   0x5
#define NB_BIT_LEFT   0x6
#define NB_BIT_RIGHT  0x7

#define NB_BUTTON(a,b) ((a >> b) & 0x1)

#define NB_ABS_X(a) (NB_BUTTON(a,NB_BIT_LEFT) - NB_BUTTON(a,NB_BIT_RIGHT))
#define NB_ABS_Y(a) (NB_BUTTON(a,NB_BIT_UP) - NB_BUTTON(a,NB_BIT_DOWN))

struct nb_features {
	uint16_t fw_version;
};

struct nb_pad {
	struct input_dev *dev;
    uint8_t data[3];
};

struct nb_nesbox {
    struct nb_pad pads[NB_PAD_COUNT];
	struct i2c_client *client;
};


// query the device for and populate nb_status
static int nb_query_features(struct i2c_client *client, struct nb_features *features){
	features->fw_version = 1;
	//i2c_smbus_write_byte(client, 0x80);
	return 0;
}


/* input device */
/*************************************/

static int nb_open(struct input_dev *dev) { return 0; }
static void nb_close(struct input_dev *dev){ }

static int __init nb_setup_pads(struct nb_nesbox *nb, struct nb_features *features) {

	struct nb_pad *pad;
	struct input_dev *dev;
	int i;
	int error;

	// Register the controller pads as input devices
	for(i=0; i<NB_PAD_COUNT; i++){

 		pad = &nb->pads[i];

		pad->dev = dev = input_allocate_device();
		if (!dev) {
			pr_err("Failed to allocate input device memory (input_allocate_device returned %d)\n", error);
			return -ENOMEM;
		}

		dev->name = "NESBox Pad";
		dev->id.bustype = BUS_I2C;
		dev->id.vendor = 0x0001;
		dev->id.version = features.fw_version;

		input_set_drvdata(dev, nb);
		
		dev->open = nb_open;
		dev->close = nb_close;

		// enable KEY and ABS input types
		dev->evbit[0] |= BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);

		// dpad axes range from -1 to +1
		input_set_abs_params(dev, ABS_X, -1, 1, 0, 0);
		input_set_abs_params(dev, ABS_Y, -1, 1, 0, 0);
		
		// buttons
		__set_bit(BTN_A, dev->keybit);
		__set_bit(BTN_B, dev->keybit);
		__set_bit(BTN_SELECT, dev->keybit);
		__set_bit(BTN_START, dev->keybit);

		// register the pad
		error = input_register_device(pad->dev);
		if (error){
		    dev_err(&nb->client->dev, "Failed to register input device (input_register_device returned %d)\n", error);	
			input_free_device(pad->dev);
			pad->dev = NULL;
			return error;
		}

		i2c_set_clientdata(nb->client, nb);

	}

	return 0;
}


/* i2c driver */
/*************************************/
static int nb_probe(struct i2c_client *client, const struct i2c_device_id *idp){

	struct nb_nesbox *nb;
	struct nb_features features = {0};
	int error;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)){
		dev_err(&client->dev, "i2c_check_functionality error.\n");
		return -EIO;
	}

	error = nb_query_features(client, &features);
	if (error){
		return error;
	}

	nb = kzalloc(sizeof(*nb), GFP_KERNEL);
	if (!nb){
		error = -ENOMEM;
		goto err_free_mem;
	}	
		
	error = nb_setup_pads(nb, &features);
	if (error){
		goto err_free_mem;
	}
		
	return 0;

err_free_mem:
	kfree(nb);
	return error;
}

static int nb_remove(struct i2c_client *client){ return 0; }


static struct i2c_device_id nb_id[] = {
	{ "nesbox", 0 },
	{ }
};

static struct i2c_driver nb_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "nesbox",
	},
	.probe = nb_probe,
	.remove = nb_remove,
	.id_table = nb_id,
};

static struct i2c_board_info nb_devices[] = {
{
	I2C_BOARD_INFO(NB_I2C_NAME, NB_I2C_ADDRESS),
	.irq = 79, /* RPi i2c IRQ? */
},
};


/* init / exit functions */
/*************************************/

static int __init nb_init(void){

	int res;

	// register i2c driver
	res = i2c_add_driver(&nb_driver);
	if (res){
		return res;
	}

	// register `nesbox` board
	i2c_register_board_info(0, nb_devices, ARRAY_SIZE(nb_devices));


	printk(KERN_INFO "%s: Loaded", MODULE_NAME);
	return 0;
}

static void __exit nb_exit(void){
	printk(KERN_INFO "%s: Unloaded", MODULE_NAME);
}

module_init(nb_init);
module_exit(nb_exit);

