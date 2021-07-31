#import <Cocoa/Cocoa.h>
#import "GBGLShader.h"

@interface GBOpenGLView : NSOpenGLView
@property (nonatomic) GBGLShader *shader;
@end
