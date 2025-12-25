if [[ ! $(id -u) -eq 0 ]]; then
	echo "This script must be run as root!"

	exit 1
fi

pid=$(pidof -s "tf_linux64")
libpath=$(realpath "supero.so")

if [[ -z "$pid" ]] || [[ "$pid" == "" ]]; then
	echo "ERROR: inject.sh: process not running."

	exit 1
fi

gdbbin="gdb"
${gdbbin} -n -q -batch\
	-ex "attach ${pid}"\
	-ex "set \$dlopen = (void* (*)(char *, int))dlopen"\
	-ex "set \$dlsym = (void* (*)(void*, char *))dlsym"\
	-ex "set \$dlerror = (char* (*)(void))dlerror"\
	-ex "echo Obtaining lib\n"\
	-ex "set \$superolib = \$dlopen(\"${libpath}\", 1)"\
	-ex "print \$superolib"\
	-ex "echo Obtaining main\n"\
	-ex "set \$mainfunc = (void (*)(void)) \$dlsym(\$superolib, \"SuperoMain\")"\
	-ex "print \$mainfunc"\
	-ex "echo Calling main\n"\
	-ex "call \$mainfunc()"\
	-ex "echo Calling dlerror\n"\
	-ex "call \$dlerror()"\
	-ex "detach"\
	-ex "quit"
