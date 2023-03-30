#include "pch.h"
#include "../common/comm.h"
#include "./include/api.h"

#include "lock.h"
#include <iostream>
#include <linux/videodev2.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include "capture_op.h"
#include "gsml_capturer.h"
rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> OpenGSMLCapture(CaptureOp* op) 
{
	 auto  video_capture=std::make_unique<GSMLCapturer>(op);
	 video_capture->Start(); 
     rtc::scoped_refptr<GSMLTrackSource> video_source = GSMLTrackSource::Create(std::move(video_capture));
     return video_source;
}

GSMLCapturer::GSMLCapturer(CaptureOp* lhs):_op(lhs) 
{
	 
}
void GSMLCapturer::Start()
{

	_thread = std::thread(std::bind(&GSMLCapturer::Run, this));
}
bool GSMLCapturer::open_cam()
{

    // int32_t count=_op->GetType()==RenderPosition::FRONT_BACK?2:1;
     _op->_ctx0=(context_t *)malloc(sizeof(context_t));
    //for(int i=0;i<count;i++)
	{
        context_t * p=_op->_ctx0;
         
	    p->cam_fd=-1;
	 
         p->cam_pixfmt = V4L2_PIX_FMT_YUYV;
         p->cam_w = 1280;
         p->cam_h = 720;
    //  p->frame = 0;
         p->g_buff = NULL;
         p->capture_dmabuf = false;  // opencv display v4l2 can't be true
         p->fps = 30;

         p->enable_verbose = false;

        std::string devname="/dev/video" + std::to_string(_op->GetIndex());
       // std::cout<<"设备:"<<devname<<std::endl;
        //ctx.cam_devname=devname+std::to_string();
         p->cam_fd = open(devname.c_str(), O_RDWR);
        if( p->cam_fd==-1) 
        {
             ERROR_RETURN("Failed to open camera device %s: %s (%d)",
                devname.c_str(), strerror(errno), errno);
        }
        struct v4l2_format fmt;
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width =  p->cam_w;
        fmt.fmt.pix.height =  p->cam_h;
        fmt.fmt.pix.pixelformat =  p->cam_pixfmt;
        fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
        if (ioctl( p->cam_fd, VIDIOC_S_FMT, &fmt) < 0)
            ERROR_RETURN("Failed to set camera output format: %s (%d)",
                    strerror(errno), errno);
        /* Get the real format in case the desired is not supported */
        memset(&fmt, 0, sizeof fmt);
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl( p->cam_fd, VIDIOC_G_FMT, &fmt) < 0)
            ERROR_RETURN("Failed to get camera output format: %s (%d)",
                    strerror(errno), errno);
        if (fmt.fmt.pix.width !=  p->cam_w ||
                fmt.fmt.pix.height !=  p->cam_h ||
                fmt.fmt.pix.pixelformat !=  p->cam_pixfmt)
        {
            WARN("The desired format is not supported");
             p->cam_w = fmt.fmt.pix.width;
             p->cam_h = fmt.fmt.pix.height;
             p->cam_pixfmt =fmt.fmt.pix.pixelformat;
        }

        struct v4l2_streamparm streamparm;
        memset (&streamparm, 0x00, sizeof (struct v4l2_streamparm));
        streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ioctl ( p->cam_fd, VIDIOC_G_PARM, &streamparm);

        INFO("Camera ouput format: (%d x %d)  stride: %d, imagesize: %d, frate: %u / %u",
                fmt.fmt.pix.width,
                fmt.fmt.pix.height,
                fmt.fmt.pix.bytesperline,
                fmt.fmt.pix.sizeimage,
                streamparm.parm.capture.timeperframe.denominator,
                streamparm.parm.capture.timeperframe.numerator);
    }
        
    return true;
}
void GSMLCapturer::Run()
{
    if(!open_cam()) return;
	prepare_buffer();
	start_streams();
	_run=true;
	struct pollfd fds[1];
   
	struct v4l2_buffer v4l2_buf;
	while(_run)
	{	
		int cam_fd=-1;
        context_t * p=nullptr;
		if((_op->GetType()!=RenderPosition::FRONT&&_op->GetType()!=RenderPosition::BACK)||_op->IsForward())
		{
			cam_fd=_op->_ctx0->cam_fd;
			p=_op->_ctx0; 
		}
        else{
                cam_fd=_op->_ctx1->cam_fd;
		     	p=_op->_ctx1; 
            
        }
         
        assert(p!=nullptr);
		/*
        else
		{
            if(_op->IsForward())
			 {
                cam_fd=_ctx[0].cam_fd;
                p=&_ctx[0]; 
             }   
            else 
            {
                cam_fd=_ctx[1].cam_fd;
                p=&_ctx[1];
            }
		}
        */
		fds[0].fd = cam_fd;
    	fds[0].events = POLLIN;
		if(poll(fds, 1, 5000) > 0)
		{
		
			if (fds[0].revents & POLLIN)
            {
			

				/* Dequeue a camera buff */
				memset(&v4l2_buf, 0, sizeof(v4l2_buf));
              
				v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				if (p->capture_dmabuf)
					v4l2_buf.memory = V4L2_MEMORY_DMABUF;
				else
					v4l2_buf.memory = V4L2_MEMORY_MMAP;
				
				if (ioctl(cam_fd, VIDIOC_DQBUF, &v4l2_buf) < 0)
					printf("Failed to dequeue camera buff: %s (%d)",
							strerror(errno), errno);
				//_ctx.frame++;
				rtc::scoped_refptr<webrtc::I420Buffer> buffer=webrtc::I420Buffer::Create(p->cam_w,p->cam_h);
				const int conversionResult = libyuv::ConvertToI420((uint8_t*)p->g_buff[v4l2_buf.index].start, 0,
				buffer->MutableDataY(), buffer->StrideY(),
				buffer->MutableDataU(), buffer->StrideU(),
				buffer->MutableDataV(), buffer->StrideV(),
				0, 0,
				p->cam_w,p->cam_h,
				buffer->width(), buffer->height(),
				libyuv::kRotate0, libyuv::FOURCC_YUYV);
				int width = p->cam_w;
				int height = p->cam_h;
				if (conversionResult >= 0)
				{
					webrtc::VideoFrame videoFrame(buffer, webrtc::VideoRotation::kVideoRotation_0, rtc::TimeMicros());
					if ((p->cam_w == 0) && (p->cam_h == 0)) {
						_broadcaster.OnFrame(videoFrame);

					}
					else
					{

						if (height == 0) {
							height = (videoFrame.height() * width) / videoFrame.width();
						}
						else if (width == 0) {
							width = (videoFrame.width() * height) / videoFrame.height();
						}
						int stride_y = width;
						int stride_uv = (width + 1) / 2;
						rtc::scoped_refptr<webrtc::I420Buffer> scaled_buffer = webrtc::I420Buffer::Create(width, height, stride_y, stride_uv, stride_uv);
						scaled_buffer->ScaleFrom(*videoFrame.video_frame_buffer()->ToI420());
						webrtc::VideoFrame frame = webrtc::VideoFrame(scaled_buffer, webrtc::kVideoRotation_0, rtc::TimeMicros());

						_broadcaster.OnFrame(frame);
					}
				}
                 if (ioctl(p->cam_fd, VIDIOC_QBUF, &v4l2_buf))
                      printf("Failed to queue camera buffers: %s (%d)",
                        strerror(errno), errno);
			}
		}
	}
	stop_streams();
    close_cam();


}
void GSMLCapturer::Destroy() {
	Stop();
}

void GSMLCapturer::AddOrUpdateSink(
	rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
	const rtc::VideoSinkWants& wants) {
	_broadcaster.AddOrUpdateSink(sink, wants);
}
 void GSMLCapturer::RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink)
{
	_broadcaster.RemoveSink(sink);
}
bool GSMLCapturer::prepare_buffer()
{
   //int32_t count=_op->GetType()==RenderPosition::FRONT_BACK?2:1;
   //for(int32_t i=0;i<count;i++)
   { 
       context_t * p=_op->_ctx0;
      p->g_buff = (nv_buffer *)malloc(V4L2_BUFFERS_NUM * sizeof(nv_buffer));
    if ( p->g_buff == NULL)
        ERROR_RETURN("Failed to allocate global buffer context");

    if ( p->capture_dmabuf) {
        if (!request_camera_buff(p))
            ERROR_RETURN("Failed to set up camera buff");
    } else {
        if (!request_camera_buff_mmap(p))
            ERROR_RETURN("Failed to set up camera buff");
    }
     INFO("Succeed in preparing stream buffers");
   }
   
    return true;
}
bool GSMLCapturer::request_camera_buff( context_t * p)
{
    
  //  for(int32_t i=0;i<count;i++)
    { 
    //    context_t * p=&_ctx[i];
        struct v4l2_requestbuffers rb;
        memset(&rb, 0, sizeof(rb));
        rb.count = V4L2_BUFFERS_NUM;
        rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        rb.memory = V4L2_MEMORY_DMABUF;
        if (ioctl( p->cam_fd, VIDIOC_REQBUFS, &rb) < 0)
            ERROR_RETURN("Failed to request v4l2 buffers: %s (%d)",
                    strerror(errno), errno);
        if (rb.count != V4L2_BUFFERS_NUM)
            ERROR_RETURN("V4l2 buffer number is not as desired");

        for (unsigned int index = 0; index < V4L2_BUFFERS_NUM; index++)
        {
            struct v4l2_buffer buf;

            /* Query camera v4l2 buf length */
            memset(&buf, 0, sizeof buf);
            buf.index = index;
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_DMABUF;

            if (ioctl( p->cam_fd, VIDIOC_QUERYBUF, &buf) < 0)
                ERROR_RETURN("Failed to query buff: %s (%d)",
                        strerror(errno), errno);

            /* TODO: add support for multi-planer
            Enqueue empty v4l2 buff into camera capture plane */
            buf.m.fd = (unsigned long) p->g_buff[index].dmabuff_fd;
            if (buf.length !=  p->g_buff[index].size)
            {
                WARN("Camera v4l2 buf length is not expected");
                p->g_buff[index].size = buf.length;
            }

            if (ioctl( p->cam_fd, VIDIOC_QBUF, &buf) < 0)
                ERROR_RETURN("Failed to enqueue buffers: %s (%d)",
                        strerror(errno), errno);
        } 
    }   
    return true;
}

bool GSMLCapturer::stop_streams()
{
   enum v4l2_buf_type type;
    //int32_t count=_op->GetType()==RenderPosition::FRONT_BACK?2:1;
    //for(int32_t i=0;i<count;i++)
    { 
        context_t * p=_op->_ctx0;
          for (unsigned int index = 0; index < V4L2_BUFFERS_NUM; index++)
           {
               
             if(munmap(p->g_buff[index].start,p->g_buff[index].size)==-1)
             {
                ERROR_RETURN("munmap failed: %s (%d)", strerror(errno), errno);
             }
           }
    /* Stop v4l2 streaming */
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl( p->cam_fd, VIDIOC_STREAMOFF, &type))
            ERROR_RETURN("Failed to stop streaming: %s (%d)",
                    strerror(errno), errno);

        INFO("Camera video streaming off ...");
     
      
    }
    return true;
}

bool GSMLCapturer::start_streams()
{
	  enum v4l2_buf_type type;
 //   int32_t count=_op->GetType()==RenderPosition::FRONT_BACK?2:1;
   // for(int32_t i=0;i<count;i++)
    { 
        context_t * p=_op->_ctx0;
       
    /* Start v4l2 streaming */
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl( p->cam_fd, VIDIOC_STREAMON, &type) < 0)
            ERROR_RETURN("Failed to start streaming: %s (%d)",
                    strerror(errno), errno);
       

      
    
	 usleep(200);
     INFO("Camera video streaming on ...");
    }
  
    return true;
} 

bool GSMLCapturer::request_camera_buff_mmap(context_t * p)
{
    struct v4l2_requestbuffers rb;
 //   int32_t count=_op->GetType()==RenderPosition::FRONT_BACK?2:1;
 //   for(int32_t i=0;i<count;i++)
    { 
      
        memset(&rb, 0, sizeof(rb));
        rb.count = V4L2_BUFFERS_NUM;
        rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        rb.memory = V4L2_MEMORY_MMAP;
        if (ioctl( p->cam_fd, VIDIOC_REQBUFS, &rb) < 0)
            ERROR_RETURN("Failed to request v4l2 buffers: %s (%d)",
                    strerror(errno), errno);
        if (rb.count != V4L2_BUFFERS_NUM)
            ERROR_RETURN("V4l2 buffer number is not as desired");

        for (unsigned int index = 0; index < V4L2_BUFFERS_NUM; index++)
        {
            struct v4l2_buffer buf;

            /* Query camera v4l2 buf length */
            memset(&buf, 0, sizeof buf);
            buf.index = index;
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            buf.memory = V4L2_MEMORY_MMAP;
            if (ioctl(p->cam_fd, VIDIOC_QUERYBUF, &buf) < 0)
                ERROR_RETURN("Failed to query buff: %s (%d)",
                        strerror(errno), errno);

            p->g_buff[index].size = buf.length;
            p->g_buff[index].start = (unsigned char *)
                mmap (NULL /* start anywhere */,
                        buf.length,
                        PROT_READ | PROT_WRITE /* required */,
                        MAP_SHARED /* recommended */,
                        p->cam_fd, buf.m.offset);
            if (MAP_FAILED ==  p->g_buff[index].start)
                ERROR_RETURN("Failed to map buffers");

            if (ioctl( p->cam_fd, VIDIOC_QBUF, &buf) < 0)
                ERROR_RETURN("Failed to enqueue buffers: %s (%d)",
                        strerror(errno), errno);
        }
    }    
    return true;
}
void GSMLCapturer::close_cam()
{
   //   int32_t count=_op->GetType()==RenderPosition::FRONT_BACK?2:1;
    //for(int32_t i=0;i<count;i++)
    { 
        context_t * p=_op->_ctx0;
         
        if(p->g_buff!=nullptr)
          {
           
              free(p->g_buff);
          } 
        if(p->cam_fd>0)
           close(p->cam_fd);
        
    }
    free(_op->_ctx0);
}
void GSMLCapturer::Stop()
{
    _run=false;
    _thread.join();
}