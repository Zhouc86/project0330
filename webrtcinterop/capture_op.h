#pragma once
#include "../include/api.h"
typedef struct
{
    /* User accessible pointer */
    unsigned char * start;
    /* Buffer length */
    unsigned int size;
    /* File descriptor of NvBuffer */
    int dmabuff_fd;
} nv_buffer;
typedef struct
{
    /* Camera v4l2 context */
   // char cam_dev[16];
    int cam_fd;
   // int cam_fd1;
   // int fd_r;
    unsigned int cam_pixfmt;
    unsigned int cam_w;
    unsigned int cam_h;
   // unsigned int frame;
     

    /* Global buffer ptr */
    nv_buffer * g_buff;
    bool capture_dmabuf;

    /* EGL renderer
    NvEglRenderer *renderer; */
   // int render_dmabuf_fd;
    int fps;

    /* CUDA processing 
    bool enable_cuda;
    EGLDisplay egl_display;
    EGLImageKHR egl_image;*/

    /* MJPEG decoding 
    NvJPEGDecoder *jpegdec;*/

    /* Verbose option */
    bool enable_verbose;

} context_t;
class CaptureOp
{
public:
  
    CaptureOp(RenderPosition type,int32_t index);
    RenderPosition GetType();
    int32_t GetIndex();
    
    bool IsForward();
    void SetForward(bool b);
    context_t* _ctx0=nullptr;
    context_t* _ctx1=nullptr;
private:
    RenderPosition _type;
    bool _forward;
    int32_t _index;

   



};