start
disassemble f
break *0x0000555555400665
run
continue 9999
print $rax
quit