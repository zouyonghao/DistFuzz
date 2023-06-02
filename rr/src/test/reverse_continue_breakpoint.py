from util import *

send_gdb('break breakpoint')
expect_gdb('Breakpoint 1')
send_gdb('c')
expect_gdb('Breakpoint 1')

send_gdb('break breakpoint2')
expect_gdb('Breakpoint 2')
send_gdb('reverse-cont')
expect_gdb('Breakpoint 2')

ok()
