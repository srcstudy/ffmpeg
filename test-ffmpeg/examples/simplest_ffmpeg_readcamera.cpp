/**


 * ��򵥵Ļ���FFmpeg��AVDevice���ӣ���ȡ����ͷ��
 * Simplest FFmpeg Device (Read Camera)
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ������ʵ���˱�������ͷ���ݵĻ�ȡ�������ʾ���ǻ���FFmpeg
 * ��libavdevice�����򵥵����ӡ�ͨ�������ӣ�����ѧϰFFmpeg��
 * libavdevice����ʹ�÷�����
 * ��������Windows�¿���ʹ��2�ַ�ʽ��ȡ����ͷ���ݣ�
 *  1.VFW: Video for Windows ��Ļ��׽�豸��ע������URL���豸����ţ�
 *          ��0��9��
 *  2.dshow: ʹ��Directshow��ע�����߻����ϵ�����ͷ�豸������
 *         ��Integrated Camera����ʹ�õ�ʱ����Ҫ�ĳ��Լ�����������ͷ��
 *          �������ơ�
 * ��Linux�¿���ʹ��video4linux2��ȡ����ͷ�豸��
 * ��MacOS�¿���ʹ��avfoundation��ȡ����ͷ�豸��
 *
 * This software read data from Computer's Camera and play it.
 * It's the simplest example about usage of FFmpeg's libavdevice Library. 
 * It's suiltable for the beginner of FFmpeg.
 * This software support 2 methods to read camera in Microsoft Windows:
 *  1.gdigrab: VfW (Video for Windows) capture input device.
 *             The filename passed as input is the capture driver number,
 *             ranging from 0 to 9.
 *  2.dshow: Use Directshow. Camera's name in author's computer is 
 *             "Integrated Camera".
 * It use video4linux2 to read Camera in Linux.
 * It use avfoundation to read Camera in MacOS.
 * 
 */


#include <stdio.h>

#define __STDC_CONSTANT_MACROS


#ifdef __cplusplus
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>


#ifdef __cplusplus
};
#endif



#define OUTPUT_YUV420P 1






int main(int argc, char* argv[]){

	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	

	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	
	//Open File
	//char filepath[]="src01_480x272_22.h265";
	//avformat_open_input(&pFormatCtx,filepath,NULL,NULL)

	avdevice_register_all();



	AVInputFormat *ifmt=av_find_input_format("video4linux2");
	if(avformat_open_input(&pFormatCtx,"/dev/video0",ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}


	if(avformat_find_stream_info(pFormatCtx,NULL)<0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
			break;
		}
	if(videoindex==-1)
	{
		printf("Couldn't find a video stream.\n");
		return -1;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}
	AVFrame	*pFrame,*pFrameYUV;
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	//unsigned char *out_buffer=(unsigned char *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	//avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	//SDL----------------------------
	



	int ret, got_picture;

	AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));

#if OUTPUT_YUV420P 
    FILE *fp_yuv=fopen("output.yuv","wb+");  
#endif  

	struct SwsContext *img_convert_ctx;
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
								     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	

	for (;;) {
		if(av_read_frame(pFormatCtx, packet)>=0){
				printf("av_read_frame.\n");
				if(packet->stream_index==videoindex){
					
					ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
					if(ret < 0){
						printf("Decode Error.\n");
						return -1;
					}
					if(got_picture){					
			
						sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
								              pFrameYUV->data, pFrameYUV->linesize);

#if OUTPUT_YUV420P  
						int y_size=pCodecCtx->width*pCodecCtx->height;    
						fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y   
						fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U  
						fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V  
#endif  

									

					}
				}
				av_free_packet(packet);
			}else{
				//Exit 
				printf("Exit...\n");
				break;
			}
		

	}
	

	sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

	

	//av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

