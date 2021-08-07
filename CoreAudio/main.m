//
//  main.m
//  CoreAudio
//
//  Created by Shiva Shankar patel psy on 6/16/21.
//

// id can point to any object its type is not know at compile type but checked during runtime .can t use dot operator with id

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "View.h"
int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // insert code here...
        NSLog(@"Hello, World!");
        
        View *myView =[[View alloc] init];
        [myView setBoundsToStartX:100 startY:100 width:150 height:200];
        [myView printBounds];
    }
    return 0;
}
