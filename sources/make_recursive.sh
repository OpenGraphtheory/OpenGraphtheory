 
function do_make
{
	cd "$1"

	# call make
	if [ -e makefile ] || [ -e Makefile ]; then
		echo -n "   $2..."
		make > /dev/null 2>&1 && echo -e " \t[  OK  ]" || echo -e " \t[FAILED]"
	fi

	# recurse into subdirectories
	for d in *; do
		if [ -d "$d" ]; then
			do_make "$d" "$2/$d"
		fi
	done

	# and return
	cd ..
}


# go to directory where -->this<-- script is located
script_dir="`cd "$(dirname "$0")";pwd`"
cd "$script_dir"


# should compilation of all codes be forced?
if [ "$1" == "force" ]; then
	# EVERY PROGRAM depends on simple_xml.cpp. Therefore, it is sufficient (for
	# now) to touch this file and compile it first (that's why do_make is called
	# with parameter opengt first)
	touch "opengt/simple_xml.cpp"
fi


# start compiling
do_make "opengt" "opengt"
do_make "programs" "programs"
