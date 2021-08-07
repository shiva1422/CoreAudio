//
//  View.h
//  CoreAudio
//
//  Created by Shiva Shankar patel psy on 6/24/21.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface View : NSObject

@property int startX ,startY ,width,height;

-(void) setBoundsToStartX:(int) startX startY:(int)startY width:(int)width height:(int)height;

-(void) printBounds;

-(void) draw;

@end

@interface ImageView : View

@end
NS_ASSUME_NONNULL_END
