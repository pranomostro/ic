#!/usr/bin/awk -f

BEGIN {
	ret=1
	if(ARGC!=2)
	{
		print("ic REGEX") > "/dev/stderr";
		exit(255);
	}
	pat=ARGV[1]
	ARGV[1]=""
}

$0~pat { ret=0; exit }
END { exit(ret) }
