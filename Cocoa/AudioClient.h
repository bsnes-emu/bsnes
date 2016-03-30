#import <Foundation/Foundation.h>

@interface AudioClient : NSObject
@property (strong) void (^renderBlock)(UInt32 sampleRate, UInt32 nFrames, SInt16 *buffer);
@property (readonly) UInt32 rate;
@property (readonly, getter=isPlaying) bool playing;
-(void) start;
-(void) stop;
-(id) initWithRendererBlock:(void (^)(UInt32 sampleRate, UInt32 nFrames, SInt16 *buffer)) block
              andSampleRate:(UInt32) rate;
@end
