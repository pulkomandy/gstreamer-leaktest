# GStreamer
GST_CXXFLAGS := $(shell pkg-config --cflags gstreamer-0.10 gstreamer-app-0.10)
GST_LDFLAGS := $(shell pkg-config --libs gstreamer-0.10 gstreamer-app-0.10)

CXX ?= g++
CXXFLAGS += -Wno-write-strings -g $(GST_CXXFLAGS)
LDFLAGS := $(GST_LDFLAGS) 

MAKEFLAGS := -j5
all : videotest

OBJS := main.o \
	video.o

videotest : $(OBJS)
	$(CXX) -o videotest  $(OBJS) $(LDFLAGS)  

$(OBJS) : %.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:	
	rm -f $(OBJS) videotest

