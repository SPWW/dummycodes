//
//  converter.h
//  
//
//  Created by wangshupeng on 8/1/2016.
//
//

#ifndef _converter_h
#define _converter_h
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif
#define RNDTO2(X) ( ( (X) & 0xFFFFFFFE ))
#define RNDTO32(X) ( ( (X) % 32 ) ? ( ( (X) + 32 ) & 0xFFFFFFE0 ) : (X) )

class converter{

private:
    //class gloable
    int errorStatus = 0;
    char errorLog[128] = { 0 };
    
    
    //decode
    bool tag_inited_decode = false;
    std::string input_file_name;
    AVFormatContext   *pFormatCtx = nullptr;
    AVCodecContext    *pCodecCtxOrig = NULL;
    AVCodecContext    *pCodecCtx = NULL;
    AVCodec           *pCodec = NULL;
    int               videoStream;
   
    //encode
    bool tag_inited_encode = false;
    AVCodec* en_codec;
    AVCodecContext* nv_codec_context;
    
    //udp output
    bool tag_inited_udp = false;
    string m_output;
    AVFormatContext* m_formatContext;
    AVStream* m_stream;
    
    //file output
    bool tag_inited_file = false;
    FILE *fp_yuv;
    string output_file_name;
    
    
    
    
    
    //private funcs
    
    void init_output_file(){
        tag_inited_file = true;
        fp_yuv=fopen(output_file_name.c_str(),"wb+");
    }
    
    void clean_output_file(){
        if(tag_inited_file == true)
            fclose(fp_yuv);
    }
    
    
    
    
    
    void init_nvenc_encoder(){
        tag_inited_encode = true;
        //find nvenc by name
        en_codec = avcodec_find_encoder_by_name("nvenc");
        
        //alloc context
        nv_codec_context = avcodec_alloc_context3(en_codec);
        
        
        //config encoder
        nv_codec_context->bit_rate = 1244179;
        nv_codec_context->width = 1920;                                        // resolution must be a multiple of two (1280x720),(1900x1080),(720x480)
        nv_codec_context->height = 1080;
        nv_codec_context->time_base.num = 1;                                   // framerate numerator
        nv_codec_context->time_base.den = 25;                                  // framerate denominator
        nv_codec_context->gop_size = 10;                                       // emit one intra frame every ten frames
        nv_codec_context->max_b_frames = 1;                                    // maximum number of b-frames between non b-frames
        nv_codec_context->keyint_min = 1;                                      // minimum GOP size
        nv_codec_context->i_quant_factor = (float)0.71;                        // qscale factor between P and I frames
        nv_codec_context->b_frame_strategy = 20;                               ///// find out exactly what this does
        nv_codec_context->qcompress = (float)0.6;                              ///// find out exactly what this does
        nv_codec_context->qmin = 20;                                           // minimum quantizer
        nv_codec_context->qmax = 51;                                           // maximum quantizer
        nv_codec_context->max_qdiff = 4;                                       // maximum quantizer difference between frames
        nv_codec_context->refs = 4;                                            // number of reference frames
        nv_codec_context->trellis = 1;                                         // trellis RD Quantization
        nv_codec_context->pix_fmt = AV_PIX_FMT_YUV420P;                           // universal pixel format for video encoding
        nv_codec_context->codec_id = AV_CODEC_ID_H264;
        //nv_codec_context->codec_type = AVMEDIA_TYPE_VIDEO;
        
        
        //open encoder
        if (avcodec_open2(nv_codec_context, en_codec,NULL) < 0) {
            fprintf(stderr, "Could not open codec\n");
            exit(1);
        }else printf("\nH264 codec opened\n");
        
        
        return;
    }
    
    int init_udp_output(){
        tag_inited_udp = true;
        //alloc out put context
        if (avformat_alloc_output_context2(&m_formatContext, NULL, "h264", m_output.c_str()) < 0) {
            cerr << "Cannot allocate output context: "
            << av_make_error_string(errorLog, 128, errorStatus) << endl;
            return -1;
        }
        
        // get output format
        AVOutputFormat *m_outputFormat = m_formatContext->oformat;
        
        //create a new stream
        m_stream = avformat_new_stream(m_formatContext, en_codec);
        if (!m_stream) {
            cerr << "Cannot create a new stream: "
            << av_make_error_string(errorLog, 128, errorStatus) << endl;
            return -1;
        }
        //setup stream info
        m_stream->codec->pix_fmt = AV_PIX_FMT_YUV420P;
        m_stream->codec->width = 1920;
        m_stream->codec->height = 1080;
        
        //dump output format
        av_dump_format(m_formatContext, 0, m_output.c_str(), 1);
        
        //open audio output context
        if ((errorStatus = avio_open(&m_formatContext->pb, m_output.c_str(), AVIO_FLAG_WRITE)) < 0) {
            cerr << "Cannot open output: "
            << av_make_error_string(errorLog, 128, errorStatus) << endl;
            return -1;
        }
        
        //writer video header
        if (avformat_write_header(m_formatContext, NULL) < 0) {
            cerr << "Cannot write header to stream: "
            << av_make_error_string(errorLog, 128, errorStatus) << endl;
            return -1;
        }
        
        return 0;
    }
    
public:
    converter():m_output("udp://147.8.179.120:6666"),output_file_name("out_put_file.yuv"){
        // Register all formats and codecs
        av_register_all();
        
        // init network function
        avformat_network_init();
        
        // set error levle
        av_log_set_level(AV_LOG_TRACE);
        
        init_output_file();
    }
    
    int load_file(std::string file_name){
        tag_inited_decode = true;
        // Open video file
        if(avformat_open_input(&pFormatCtx, file_name.c_str(), NULL, NULL)!=0)
            return -1; // Couldn't open file
        
        // Retrieve stream information
        if(avformat_find_stream_info(pFormatCtx, NULL)<0)
            return -1; // Couldn't find stream information
    
    
        // Dump information about file onto standard error
            av_dump_format(pFormatCtx, 0, file_name.c_str(), 0);
        
        
        // Find the first video stream
        videoStream=-1;
        for(int i=0; i<pFormatCtx->nb_streams; ++i)
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
                videoStream=i;
                break;
            }
        if(videoStream==-1)
            return -1; // Didn't find a video stream
        
        // Get a pointer to the codec context for the video stream
        pCodecCtxOrig=pFormatCtx->streams[videoStream]->codec;
        // Find the decoder for the video stream
        pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
        if(pCodec==NULL) {
            fprintf(stderr, "Unsupported codec!\n");
            return -1; // Codec not found
        }
        
        
        // Copy context
        pCodecCtx = avcodec_alloc_context3(pCodec);
        if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
            fprintf(stderr, "Couldn't copy codec context");
            return -1; // Error copying codec context
        }
        
        // Open codec
        if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
            return -1; // Could not open codec
        
        
        return 0;
    }
    
    
    AVFrame* scale_pfmt2yuv(AVFrame* pFrame){
        
        static SwsContext *swsCtx = NULL;
        int width	 =	RNDTO2(pCodecCtx->width);
        int height	 =	RNDTO2(pCodecCtx->height);
        int ystride  =	RNDTO32(width);
        int uvstride =	RNDTO32(width / 2);
        int ysize	 = ystride * height;
        int vusize	 = uvstride * ( height / 2 );
        int size	 = ysize + ( 2 * vusize );
        
        uint8_t * pFrameYUV = (uint8_t*)malloc( size );
        uint8_t *plane[] = { pFrameYUV, pFrameYUV + ysize, pFrameYUV + ysize + vusize, 0 };
        int stride[] = { ystride, uvstride, uvstride, 0 };
        
        
        
        AVFrame* pictYUV = av_frame_alloc();
        pictYUV->data[0] =pFrameYUV;
        pictYUV->data[1] =pFrameYUV + ysize;
        pictYUV->data[2] =pFrameYUV + ysize + vusize;
        pictYUV->data[3] = 0;
        
        pictYUV->linesize[0] = ystride;
        pictYUV->linesize[1] = uvstride;
        pictYUV->linesize[2] = uvstride;
        pictYUV->linesize[3] = 0;
        
        pictYUV->format = AV_PIX_FMT_YUV420P;
        
        pictYUV->width = pCodecCtx->width;
        pictYUV->height = pCodecCtx->height;
        
        
        
        // initialize SWS context for software scaling
        
        swsCtx = sws_getCachedContext ( swsCtx, pCodecCtx->width, pCodecCtx->height,
                                       pCodecCtx->pix_fmt, width, height, AV_PIX_FMT_YUV420P,
                                       SWS_BILINEAR , NULL, NULL, NULL );
        
        
        
        // Convert the image from its native format to RGB
        sws_scale(swsCtx, (uint8_t const * const *)pFrame->data,
                  pFrame->linesize, 0, pCodecCtx->height,
                  plane, stride );
        
        return pictYUV;
    
    }
    
    
    AVPacket encode_frame(AVFrame* pictYUV){
        
        AVPacket Outpacket;
        int got_packet_ptr;
        av_init_packet(&Outpacket);
        Outpacket.data = NULL;
        Outpacket.size = 0;
        int out_size = avcodec_encode_video2(nv_codec_context, &Outpacket, pictYUV,&got_packet_ptr);
        return Outpacket;
    }
    
    
    void process_file(){
        init_nvenc_encoder();
        AVPacket  packet;
        // Allocate video frame
        AVFrame*    pFrame =av_frame_alloc();
        int               frameFinished;
        int               numBytes;
        
        while(av_read_frame(pFormatCtx, &packet)>=0) {
            // Is this a packet from the video stream?
            if(packet.stream_index==videoStream) {
                // Decode video frame
                avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
                
                // Did we get a video frame?
                if(frameFinished) {
                    
                    //convert rgb image to yuv image
                    AVFrame* pictYUV = scale_pfmt2yuv(pFrame);
                    
                    //encode with nvenc
                    AVPacket&& Outpacket = encode_frame(pictYUV);
                    
                    if (tag_inited_file==true) {
                    //output to file
                    fwrite(Outpacket.data,1,Outpacket.size,fp_yuv);
                    }
                    
                    if (tag_inited_udp==true){
                    //output to udp
                    av_write_frame(m_formatContext, &Outpacket);
                    }
                    
                    
                    //Free the packet that was allocated by encoder
                    av_packet_unref(&Outpacket);
                }
            }
            
            // Free the packet that was allocated by av_read_frame
            av_packet_unref(&packet);
        }
    
        // Free the YUV frame
        av_frame_free(&pFrame);
        
    }
    
    
    ~converter(){
        if (tag_inited_file==true) {
            clean_output_file();
        }
        // Close the codecs
        if (tag_inited_decode==true) {
            avcodec_close(pCodecCtx);
            avcodec_close(pCodecCtxOrig);
            // Close the video file
            avformat_close_input(&pFormatCtx);
        }

        
        
        if (tag_inited_udp==true) {
            avcodec_close(m_stream->codec);
            avformat_free_context(m_formatContext);

        }


    }
    
};


#endif
