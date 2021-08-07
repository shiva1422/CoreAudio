//
//  Recorder.c
//  CoreAudio
//
//  Created by Shiva Shankar patel psy on 8/5/21.
//

#include "Recorder.h"
#define NUMBUFFERS 3    //one for filling,draining and othere as spare
void checkError(OSStatus status,const char *operation)
{
    if(status == noErr)
        return;
    else
        printf("Check Error Error %s",operation);
}
void startRecorder()
{
    //setup AudioFormat;
    Recorder recorder = {0};
    AudioStreamBasicDescription recordingFormat;
    memset(&recordingFormat , 0 ,sizeof(recordingFormat));
    recordingFormat.mFormatID = kAudioFormatMPEG4AAC;
    recordingFormat.mChannelsPerFrame = 2;
    
    //to avoid extrawork of resampling use devices default samplingRate;
    recordingFormat.mSampleRate = 44100;
    
    UInt32 propSize = sizeof(recordingFormat);
    checkError(AudioFormatGetProperty(kAudioFormatProperty_FormatInfo, 0, NULL, &propSize, &recordingFormat), "AudioFormatGetPropertyFailed");
    
    //create AudioQueue;
    
    AudioQueueRef  queue ={0};
    
    checkError(AudioQueueNewInput(&recordingFormat, AQInputCallback, &recorder, NULL, NULL, 0, &queue), "AudioQueueNewInputFailed");
    //some formats use magic cookies that are created aby Audio Queue these also should be written to output file
    
    
    //newInput func above also fills all the ASBD that are not know earlier.after which use kAudioConverterCurrentOutputStreamDescription to get properties from ASBD
    //retrieve filled out ASBD from AudioQueue.
    checkError(AudioQueueGetProperty(queue,kAudioConverterCurrentOutputStreamDescription, &recordingFormat, &propSize), "couldnt get queues format");
    //with this more detaild ASBD we can now create audio file to write into
    
    CFURLRef fileRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR("recording.caf"), kCFURLPOSIXPathStyle, false);
    
    checkError(AudioFileCreateWithURL(fileRef, kAudioFileCAFType, &recordingFormat, kAudioFileFlags_EraseFile, &recorder.recordFile), "AudioFileCreateWithURL failed");
    CFRelease(fileRef);
    
    //copy magic cookies to the file from the queue as some formats need them.some cutom function MyCopyEncoderCookieToFile(queue, recorder.recordFile);
    
    
    //queue fills the buffers with audio and sends them to the callback function,but we need to create buffers before the start.
    
    int bufferSize = computeOptimalBufferSize(&recordingFormat, queue, 0.5);
    
    //create bufs and enque to the audioq.
    int bufferIndex;
    for(bufferIndex = 0;bufferIndex < NUMBUFFERS ; ++bufferIndex)
    {
        AudioQueueBufferRef buffer;
        checkError(AudioQueueAllocateBuffer(queue, bufferSize, &buffer), "AudioQueueAllocBufferFailed");
        checkError(AudioQueueEnqueueBuffer(queue, buffer, 0, NULL), "AudioQueueEnqueBuffer failed");
        
    }
    //buffer enqueued now start the buffer;
    recorder.running = true;
    checkError(AudioQueueStart(queue, NULL), "AudioQueueStart failed");//time instead of null
    while(getchar() != 32 + '\0')
    {
        //recording until space
    }
    printf("\n recording done");
    recorder.running = false;
    //stop the queue to finish its work after recording
    
    checkError(AudioQueueStop(queue, TRUE), "AudioQueueStop failed");
    //in some cases magic cookies can be  updated during recording process reset the cookie on the output file before closing it using the same function used to write the cookie initially
   
    //dispose all resources allocated by the audioq and close the file
    AudioQueueDispose(queue, TRUE);
    AudioFileClose(recorder.recordFile);
    
    return ;
    
    
    
    
}
OSStatus getDefaultInputDeviceSampleRate(Float64 *outSampleRate)
{
    //on IOS use Audio Sessions Services instead of audio hardware services.
    OSStatus error;
    AudioDeviceID deviceId = 0;
    AudioObjectPropertyAddress propAddress;
    UInt32 propSize;
    propAddress.mSelector = kAudioHardwarePropertyDefaultInputDevice;
    propAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propAddress.mElement = 0;
    propSize = sizeof(deviceId);
    error = AudioHardwareServiceGetPropertyData(kAudioObjectSystemObject, &propAddress, 0, NULL,&propSize,&deviceId);
   
    //if no error
    propAddress.mSelector = kAudioDevicePropertyNominalSampleRate;
    propAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propAddress.mElement = 0;
    propSize = sizeof(Float64);
    error = AudioHardwareServiceGetPropertyData(deviceId, &propAddress, 0, NULL, &propSize, outSampleRate);
    
    return error;
}

int computeOptimalBufferSize(AudioStreamBasicDescription *format,AudioQueueRef queue,float bufferDuration)
{
    return 512;
}

void copyEncoderCookieToFile(AudioQueueRef queue,AudioFileID file)
{
    OSStatus error;
    UInt32 propertySize;
    error = AudioQueueGetPropertySize(queue, kAudioConverterCompressionMagicCookie, &propertySize);
    if(error == noErr && propertySize > 0)
    {
        Byte *magicCookie = (Byte *)malloc(propertySize);
        checkError(AudioQueueGetProperty(queue, kAudioQueueProperty_MagicCookie, magicCookie, &propertySize), "could not get audio q magic cookies ");
        checkError(AudioFileSetProperty(file, kAudioFilePropertyMagicCookieData, propertySize, magicCookie), "couldnt set audio files magic Cookie ");
        free(magicCookie);
    }
}

