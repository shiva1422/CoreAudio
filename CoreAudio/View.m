//
//  View.m
//  CoreAudio
//
//  Created by Shiva Shankar patel psy on 6/24/21.
//

#import "View.h"

@implementation View

@synthesize startX,startY,width,height;

-(void) setBoundsToStartX:(int)startX startY:(int)startY width:(int)width height:(int)height
{
    self.startX = startX;
    self.startY = startY;
    self.width = width;
    self.height = height;
}

-(void) printBounds
{
    NSLog(@"The Bounds are startx %i startY %i width %i height %i",startX,startY,width,height);
}
@end
