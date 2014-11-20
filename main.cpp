/* Copyright 2014, Adrien Destugues <pulkomandy@pulkomandy.tk>
 * Distributed under the terms of the MIT license
 */
#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>

#include "video.h"

int main(int argc, char* argv[])
{
	if (argc < 3) 
	{
		printf("missing filename\n");
		exit(EXIT_FAILURE);
	}

	gst_init (NULL,NULL);

	struct rusage usage;

	int k;
	sscanf(argv[2], "%d", &k);

	for (int q = 0; q < k; q++) {
		getrusage(RUSAGE_CHILDREN, &usage);
		printf("Loop %d: maxrss=%d\n", q, usage.ru_maxrss);

		video testvideo(argv[1]);

		while (testvideo.getvisible())
		{
			testvideo.update();
		}
	}

	gst_deinit();
	exit(EXIT_SUCCESS);
}

