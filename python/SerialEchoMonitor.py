#!/usr/bin/env python

from __future__ import print_function

import sys
import time

from signal import signal, SIGINT
from SerialMonitor import SerialMonitor


class SerialEchoMonitor(SerialMonitor):
    def __init__(self, device, baudrate=9600):
        super(SerialEchoMonitor, self).__init__(device=device, baudrate=baudrate)

    def run(self):
        while not self.cancelled:
            for c in self._serial.read():
                print(c, end='')


monitor = SerialEchoMonitor('/dev/ttyAMA0', 9600)


def _SIGINT(s,f):
    monitor.stop()
    sys.exit(1)

if __name__ == '__main__':
    signal(SIGINT, _SIGINT)
    monitor.start()

    while True:
        time.sleep(2)

