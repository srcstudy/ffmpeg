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

//#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "urldecode.h"

//static AVFormatContext *fmt_ctx = NULL;



#ifdef __cplusplus
}
#endif

int main(){ 	
	/*  if (avformat_open_input(&fmt_ctx, "/root/temp2/test-ffmpeg/1/index.ts", NULL, NULL) < 0) {
        printf("Could not open source file\n");
        //exit(1);
    } */
	
	//const char *url = "http://www.elesos.com/test 123.mp4";  //OK
	//const char *url = "http%3a%2f%2fwww.elesos.com%2ftest+123.mp4";  //OK
	const char *url = "http%3A%2F%2Fwww.elesos.com%2Ftest%20123.mp4";  //OK
	
	char *decoded_url = ff_urldecode(url);
	if(!decoded_url){
		printf("failed\n");
		av_freep(&decoded_url);
		return 0;
	}
	printf("decoded_url=%s\n", decoded_url);
	av_freep(&decoded_url);
	
	return 0; 
} 