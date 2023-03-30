 
 
#include <cstdint>
#include <memory>
#include <iostream>
#include "../common/comm.h"
#include "api.h"
#include "VideoRenderer.h"
#include "message_queue.h"
 
VideoRenderer::VideoRenderer() 
{
  start_=false;
}
VideoRenderer::~VideoRenderer()
{

}
/*
void VideoRenderer::SetRenderSize(int32_t width,int32_t height)
{
  
   SetSize(width,height);

}
void VideoRenderer::SetSize(int32_t width, int32_t height)
{
    
     if (width_ == width && height_ == height) {
    return;
  }

  width_ = width;
  height_ = height;
  image_.reset(new uint8_t[width * height * 4]);
  
}
*/
void VideoRenderer::FrameCallback(void* user_data,void* data,const int32_t stride,const int32_t frame_width,const int32_t frame_height)
{
    std::cout<<"FrameCallback"<<std::endl;
  //  auto ptr = static_cast<VideoRenderer *>(user_data);
 // 	ptr->OnArgb32FrameReady(data, stride, frame_width, frame_height);
   
}
void VideoRenderer::OnArgb32FrameReady(const void* data, const int stride, const int frame_width, const int frame_height)
{
  /*
  if(start_==false) return;
  std::cout<<frame_width<<","<<frame_height<<std::endl;
 
  */
}
void VideoRenderer::StartRender(bool bStart)
{
  start_=bStart;
}