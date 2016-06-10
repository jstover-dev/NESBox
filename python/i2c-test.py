#!/usr/bin/env python

import smbus
import time
bus = smbus.SMBus(1)
address = 0x42


CMD_PAD_1_STATE = 0x81 
CMD_PAD_2_STATE = 0x82
CMD_RESET_STATE = 0x83
CMD_POWER_STATE = 0x84
CMD_LED_ON      = 0x85
CMD_LED_OFF     = 0x86

OPCODES = [ 0, CMD_PAD_1_STATE, CMD_PAD_2_STATE, CMD_RESET_STATE, CMD_POWER_STATE, CMD_LED_ON, CMD_LED_OFF, 0 ]



#for op in OPCODES:
#    print "Writing", op, "..."
#    bus.write_byte(address, op)
#    print bus.read_byte(address)
#    time.sleep(1)



while True:
    try:
        bus.write_byte(address, CMD_PAD_1_STATE)
        p1 = bus.read_byte(address)
        bus.write_byte(address, CMD_PAD_2_STATE)
        p2 = bus.read_byte(address)
    except IOError,e:
        print e

    #time.sleep(0.015)
    print p1, p2
