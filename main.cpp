#include <stdio.h>

#include "video.h"

int main(int argc, char* argv[])
{
	if (argc < 3) 
	{
	 printf("missing filename\n");
	 exit(0);	
	}
	gst_init (NULL,NULL);

	int k;
	sscanf(argv[2], "%d", &k);
	for (int q = 0; q < k; q++) {
	video testvideo(argv[1]);

	while (testvideo.getvisible())
	{
		testvideo.update();
	}
	}

	gst_deinit();
}

