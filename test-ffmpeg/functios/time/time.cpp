// g++ *.cpp
//g++ urldecode.cpp -I/root/ffmpeg_build/include/
/*g++ urldecode.cpp -I/root/ffmpeg_build/include/ -L/root/ffmpeg_build/lib -lavformat -lavcodec \
 -lavutil -lswscale -lm -lz -pthread -lva-drm -lva -lvdpau -lX11 -lm -lva -lswresample


*/
#include<bits/stdc++.h> 
using namespace std; 

#ifdef __cplusplus
extern "C" {
#endif


#include "libavutil/avutil.h"
#include "libavutil/time.h"






#ifdef __cplusplus
}
#endif

/*
ts=1588834911718974
ts=1588834912719060
ts=1000086


ts=1588834914453201
ts=1588834915453284
ts=1000083
*/

int main(){ 	
	
	
	int64_t start = av_gettime();	
	printf("ts=%"PRId64"\n", start);
	av_usleep(1000000); //1 second
	int64_t end = av_gettime();
	printf("ts=%"PRId64"\n", end);
	
	int64_t duration = end - start;
	printf("ts=%"PRId64"\n", duration);
	
	return 0; 
} 


