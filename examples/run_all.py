import os
import stat
import subprocess

DIRECTORY = './bin/'

executable = stat.S_IEXEC | stat.S_IXGRP | stat.S_IXOTH
executables = []
for filename in os.listdir(DIRECTORY):
    filename = DIRECTORY+filename
    if os.path.isfile(filename):
        st = os.stat(filename)
        mode = st.st_mode
        if mode & executable:
            executables += [filename]

for executable in executables:
	print("".join(["="]*97))
	print("Running", executable)
	print("".join(["="]*97))
	arch = subprocess.check_output(executable)
	for i in arch.decode().split("\n"):
		print(" ", i)
	print()
