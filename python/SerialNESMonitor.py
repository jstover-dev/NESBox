#!/usr/bin/env python

from __future__ import print_function

import sys
import time

from signal import signal, SIGINT
from SerialMonitor import SerialMonitor


class SerialNESMonitor(SerialMonitor):
    def __init__(self, device, baudrate=9600):
        super(SerialNESMonitor, self).__init__(device=device, baudrate=baudrate)

    def run(self):
        while not self.cancelled:
            for c in self._serial.read():
                pass
                #print(format(ord(c), '016b'))


    def update_pads(self):
        self._serial.write(chr(0x81))
        self._serial.write(chr(0x82))


monitor = SerialNESMonitor('/dev/ttyAMA0', 9600)


def _SIGINT(s,f):
    monitor.stop()
    sys.exit(1)


if __name__ == '__main__':
    signal(SIGINT, _SIGINT)
    monitor.start()

    while True:
        time.sleep(0.01)
        monitor.update_pads()

        #time.sleep(2)

