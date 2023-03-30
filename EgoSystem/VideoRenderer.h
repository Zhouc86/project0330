#pragma once
 
class VideoRenderer
{
public:
     VideoRenderer();
   //  void SetRenderWindow(int32_t width,int32_t height);
     void OnArgb32FrameReady(const void* data, const int stride, const int frame_width, const int frame_height);
     void StartRender(bool bStart);
     static void FrameCallback(void* user_data,void* data,const int32_t stride,const int32_t frame_width,const int32_t frame_height);

     ~VideoRenderer();
     //  const uint8_t* image() const { return image_.get(); }

  //  int width() const { return width_; }

  //  int height() const { return height_; }
private:
	//void SetSize(int32_t width, int32_t height);
   // std::unique_ptr<uint8_t[]> image_;
   // int width_;
   // int height_;
   // CMessageQueue* main_wnd_;
    bool start_;
   

};