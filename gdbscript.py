#!/usr/bin/env python

gdb.execute("target remote :1234")
gdb.execute("set architecture aarch64")
img_base = int(gdb.selected_frame().read_register('x1'))
gdb.execute("add-symbol-file main.so 0x{addr:x}".format(addr=img_base))
gdb.execute("set $x0 = 1")