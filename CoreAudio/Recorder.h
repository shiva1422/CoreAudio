//
//  Recorder.h
//  CoreAudio
//
//  Created by Shiva Shankar patel psy on 8/5/21.
//

#ifndef Recorder_h
#define Recorder_h

#include <stdio.h>
#import<AudioToolbox/AudioToolbox.h>
//User Info Struct for Recording Audio Queue Callbacks
typedef struct Recorder
{
    AudioFileID recordFile;//file to write to
    SInt64 recordPacket;//indexx of packet writing to the file.
    Boolean running;
}Recorder;
#endif /* Recorder_h */

OSStatus getDefaultInputDeviceSampleRate(Float64 *outSampleRate);
void checkError(OSStatus status,const char *operation);
int computeOptimalBufferSize(AudioStreamBasicDescription *format,AudioQueueRef queue,float bufferDuration);
void startRecorder();

//retrieves the magic cookies if any from AudioQ and copies to the audioFile.//needed for encoded formats like AAC.
void copyEncoderCookieToFile(AudioQueueRef queue,AudioFileID file);
