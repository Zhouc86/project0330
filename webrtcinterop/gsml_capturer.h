#pragma once
#include <pc/video_track_source.h>
#include <thread>
#include <string>
#include "media/base/video_broadcaster.h"
#define ERROR_RETURN(fmt, ...) \
    do { \
        printf("ERROR: %s(): (line:%d) " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        return false; \
    } while(0)
#define INFO(fmt, ...) \
    if (p->enable_verbose) \
        printf("INFO: %s(): (line:%d) " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);

#define WARN(fmt, ...) \
        printf("WARN: %s(): (line:%d) " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);

#define V4L2_BUFFERS_NUM    4
class CaptureOp;
 
rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> OpenGSMLCapture(CaptureOp* op) ;

 
class GSMLCapturer : public rtc::VideoSourceInterface<webrtc::VideoFrame>
{
public:
	GSMLCapturer(CaptureOp* lhs);
	virtual ~GSMLCapturer()=default;
	void Start();
	void Run();
	void Stop();


	void AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
		const rtc::VideoSinkWants& wants) override;
	void RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) override;
	 
private:
   bool open_cam();
   void close_cam();
   bool prepare_buffer();
   bool start_streams();
   bool stop_streams();
   bool request_camera_buff(context_t * p);
   bool request_camera_buff_mmap(context_t * p);

private:
	void Destroy();
private:
	rtc::VideoBroadcaster   _broadcaster;
	std::thread _thread;
	bool _run;
	CaptureOp* _op;


 
	
};
class GSMLTrackSource : public webrtc::VideoTrackSource {
public:
	static rtc::scoped_refptr<GSMLTrackSource> Create(std::unique_ptr<GSMLCapturer> capturer) {
		return new rtc::RefCountedObject<GSMLTrackSource>(std::move(capturer));
	}

protected:
	explicit GSMLTrackSource(std::unique_ptr<GSMLCapturer> capturer)
		: VideoTrackSource(/*remote=*/false), capturer_(std::move(capturer)) {}
    ~GSMLTrackSource()
    {
       capturer_->Stop();
    }
private:
	rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override {
		return capturer_.get();
	}
	std::unique_ptr<GSMLCapturer> capturer_;
};