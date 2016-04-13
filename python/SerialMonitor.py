#!/usr/bin/env python

from __future__ import print_function

import threading

from abc import ABCMeta, abstractmethod
from serial import Serial, SerialException


class SerialMonitorException(Exception):
    pass


class SerialMonitor(object):

    __metaclass__ = ABCMeta

    #def __init__(self, device, baudrate):
    def __init__(self, *args, **kwargs):
        self._serial = Serial()
        self._serial.port = kwargs.get('device')
        self._serial.baudrate = kwargs.get('baudrate')
        #self._serial.port = device
        #self._serial.baudrate = baudrate
        self._serial.timeout = 0.5

        self.running = False
        self.cancelled = False


    def _msg_welcome_(self):
        print("--- {0} on {1} [{2}] [{3}{4}{5}]".format(
            self.__class__.__name__,
            self._serial.portstr,
            self._serial.baudrate,
            self._serial.bytesize,
            self._serial.parity,
            self._serial.stopbits
        ))

    def _msg_open_success_(self):
        print("--- Ctrl-C / BREAK to quit")


    def _run(self):
        self.running = True
        self.run()
        self.running = False
        print("\n--- Stopped ---")

    @abstractmethod
    def run(self):
        pass

    def start(self):
        if self.running:
            raise SerialMonitorException("Monitor thread already started.")

        try:
            self._msg_welcome_()
            self._serial.open()
            self._msg_open_success_()
            self._thread = threading.Thread(target = self._run)
            self._thread.daemon = True
            self._thread.start()
        except SerialException, e:
            print("[{0}] {1}".format(self._serial.getPort(), e))


    def stop(self):
        if self.running:
            self.cancelled = True
            self._thread.join()

    def cancel(self):
        if self.running:
            self.cancelled = True
