RUN:

  valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=40 --log-file="val" --error-limit=no ./videotest file:///home/whatever/video.mp4 loopcount 

Increasing the loop count should not result in more leaked memory in the valgrind
report (check for the total number of bytes and blocks near the top of the file).

On my PC I get a difference of 32 to 36 bytes in 1 to 3 blocks per loop.
