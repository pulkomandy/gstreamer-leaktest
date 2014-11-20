#include "video.h"
#include <string.h>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/gstcaps.h>

using namespace std;

//static GstAppSink* app_sink;



/* Callback: The appsink has received a buffer */
static GstFlowReturn new_buffer (GstAppSink *app_sink) 
{

    // Get the frame from video stream
    GstBuffer* sample = gst_app_sink_pull_buffer(app_sink);
    if(!sample) 
    {
        // Finished playing.
        return GST_FLOW_ERROR;
    }

	// TODO is it safe to release the buffer, before the texture is drawn?
	// May be a problem with Vivante direct-mapping.
    gst_buffer_unref(sample);
    return GST_FLOW_OK;
}
static void pad_added_handler (GstElement *src, GstPad *new_pad,  GstAppSink *app_sink ) 
{
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;
    GstPad *sink_pad_video;
    g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

    new_pad_caps = gst_pad_get_caps(new_pad);
    new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
    new_pad_type = gst_structure_get_name (new_pad_struct);
    if (g_str_has_prefix (new_pad_type, "video/x-raw"))
    {
        sink_pad_video = gst_element_get_static_pad ((GstElement*)app_sink, "sink");
        ret = gst_pad_link (new_pad, sink_pad_video);
        if (GST_PAD_LINK_FAILED (ret)) 
        {
                g_print (" Type is '%s' but link failed.\n", new_pad_type);
        }
        else
        {
                g_print (" Link succeeded (type '%s').\n", new_pad_type);
        }
       gst_object_unref (sink_pad_video);
    } 
    gst_caps_unref (new_pad_caps);
}

video::video(string filen)
{
	playonceb = true;
	visible = 1;

    app_sink = (GstAppSink *)gst_element_factory_make ("appsink", "app_sink");

    GstElement* source;
    source = gst_element_factory_make ("uridecodebin", "source");
        
    g_object_set(source, "uri", filen.c_str(), NULL);
    g_signal_connect (source, "pad-added", G_CALLBACK (pad_added_handler), app_sink);

    pipeline = gst_pipeline_new("test-pipeline");
    if (!gst_bin_add(GST_BIN(pipeline), source)) 
    {
        g_print("Unable to add source element to the pipeline.\n");
        gst_object_unref (pipeline);
        gst_object_unref(source);
        gst_object_unref(app_sink);
        return;
    }
    if (!gst_bin_add(GST_BIN(pipeline), (GstElement *)app_sink)) 
    {
        g_print("Unable to add sink element to the pipeline.\n");
        gst_object_unref (pipeline);
        gst_object_unref(app_sink);
        return;
    }
    if (gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) 
    {
        g_print ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return;
    }
    inited = true;
}
     
video::~video()
{
    printf("videoclass destruct\n");
    if (inited)
    {
        clean();
    }
}

void video::update()
{
	if(visible && inited && playonceb)
	{
		if (new_buffer(app_sink) != GST_FLOW_OK)
		{
			playonceb = false; clean();
		}
	}
}



void video::clean()
{       
	gst_element_set_state(pipeline,GST_STATE_NULL);
	gst_object_unref(GST_OBJECT (pipeline));
		// Freeing the pipeline will also free everything inside it (source, sink)
    inited = false;
	visible = 0;
}

int video::getvisible()
{
    return visible;
}
