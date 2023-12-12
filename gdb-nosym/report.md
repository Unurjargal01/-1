start
disassemble f
break *0x0000555555400665
run
jump *0x0000555555400665
continue 10000
print $rax
quit