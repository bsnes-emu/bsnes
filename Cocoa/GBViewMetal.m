#import "GBViewMetal.h"

#define WIDTH 160
#define HEIGHT 144
#define PITCH (160 * 4)

static const MTLRegion region = {
    {0, 0, 0},         // MTLOrigin
    {WIDTH, HEIGHT, 1} // MTLSize
};

static const vector_float2 rect[] =
{
    {-1, -1},
    { 1, -1},
    {-1,  1},
    { 1,  1},
};

@implementation GBViewMetal
{
    id<MTLDevice> device;
    id<MTLTexture> texture, previous_texture;
    id<MTLBuffer> vertices;
    id<MTLRenderPipelineState> pipeline_state;
    id<MTLCommandQueue> command_queue;
}

- (void)createInternalView
{
    MTKView *view = [[MTKView alloc] initWithFrame:self.frame device:(device = MTLCreateSystemDefaultDevice())];
    view.delegate = self;
    self.internalView = view;
    
    MTLTextureDescriptor *texture_descriptor = [[MTLTextureDescriptor alloc] init];
    
    texture_descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    
    texture_descriptor.width = WIDTH;
    texture_descriptor.height = HEIGHT;
    
    texture = [device newTextureWithDescriptor:texture_descriptor];
    previous_texture = [device newTextureWithDescriptor:texture_descriptor];
    
    vertices = [device newBufferWithBytes:rect
                                   length:sizeof(rect)
                                  options:MTLResourceStorageModeShared];

    [self loadShader];
}

- (void) loadShader
{
    NSError *error = nil;
    NSString *shader_source = [NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"MasterShader"
                                                                                                 ofType:@"metal"
                                                                                            inDirectory:@"Shaders"]
                                                        encoding:NSUTF8StringEncoding
                                                           error:nil];
    id<MTLLibrary> library = [device newLibraryWithSource:shader_source
                                                   options:nil
                                                     error:&error];
    if (error) {
        NSLog(@"Error: %@", error);
    }
    
    id<MTLFunction> vertex_function = [library newFunctionWithName:@"vertex_shader"];
    id<MTLFunction> fragment_function = [library newFunctionWithName:@"fragment_shader"];
    
    // Set up a descriptor for creating a pipeline state object
    MTLRenderPipelineDescriptor *pipeline_state_descriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipeline_state_descriptor.vertexFunction = vertex_function;
    pipeline_state_descriptor.fragmentFunction = fragment_function;
    pipeline_state_descriptor.colorAttachments[0].pixelFormat = ((MTKView *)self.internalView).colorPixelFormat;
    
    error = nil;
    pipeline_state = [device newRenderPipelineStateWithDescriptor:pipeline_state_descriptor
                                                             error:&error];
    if (error)  {
        NSLog(@"Failed to created pipeline state, error %@", error);
    }
    
    command_queue = [device newCommandQueue];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    [texture replaceRegion:region
               mipmapLevel:0
                 withBytes:[self currentBuffer]
               bytesPerRow:PITCH];
    
    MTLRenderPassDescriptor *render_pass_descriptor = view.currentRenderPassDescriptor;
    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];

    if(render_pass_descriptor != nil)
    {
        id<MTLRenderCommandEncoder> render_encoder =
            [command_buffer renderCommandEncoderWithDescriptor:render_pass_descriptor];
        
        [render_encoder setViewport:(MTLViewport){0.0, 0.0,
            view.bounds.size.width * view.window.backingScaleFactor,
            view.bounds.size.height * view.window.backingScaleFactor,
            -1.0, 1.0}];
        
        [render_encoder setRenderPipelineState:pipeline_state];
        
        [render_encoder setVertexBuffer:vertices
                                 offset:0
                                atIndex:0];
        
        [render_encoder setFragmentTexture:texture
                                  atIndex:0];
        
        [render_encoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                          vertexStart:0
                          vertexCount:4];
        
        [render_encoder endEncoding];
        
        [command_buffer presentDrawable:view.currentDrawable];
    }
    
    
    [command_buffer commit];
}
@end
