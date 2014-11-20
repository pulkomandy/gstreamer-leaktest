#ifndef VIDEO_H
#define VIDEO_H
#include <string>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappbuffer.h>

using namespace std;

class video 
{
public:
    video(string filename);
    ~video();
    void update();
    void playonce();
    void clean();
    int getvisible();
private:
    bool playonceb;
    bool inited;
    int visible;
    GstElement* pipeline;
    GstAppSink* app_sink;
};
#endif
