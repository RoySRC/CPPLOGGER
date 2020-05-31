import os
import stat
import subprocess

executable = stat.S_IEXEC | stat.S_IXGRP | stat.S_IXOTH
executables = []
for filename in os.listdir('.'):
    if os.path.isfile(filename):
        st = os.stat(filename)
        mode = st.st_mode
        if mode & executable:
            executables += [filename]

for exec in executables:
	print("".join(["="]*97))
	print("Running", exec)
	print("".join(["="]*97))
	arch = subprocess.check_output("./"+exec, shell=True);
	for i in arch.decode().split("\n"):
		print(" ", i)
	print()