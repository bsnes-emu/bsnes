#import <Cocoa/Cocoa.h>

@protocol GBImageViewDelegate;

@interface GBImageViewGridConfiguration : NSObject
@property (nonatomic, strong) NSColor *color;
@property (nonatomic) NSUInteger size;
- (instancetype) initWithColor: (NSColor *) color size: (NSUInteger) size;
@end

@interface GBImageView : NSImageView
@property (nonatomic, strong) NSArray *horizontalGrids;
@property (nonatomic, strong) NSArray *verticalGrids;
@property (nonatomic) bool displayScrollRect;
@property NSRect scrollRect;
@property (nonatomic, weak) IBOutlet id<GBImageViewDelegate> delegate;
@end

@protocol GBImageViewDelegate <NSObject>
@optional
- (void) mouseDidLeaveImageView: (GBImageView *)view;
- (void) imageView: (GBImageView *)view mouseMovedToX:(NSUInteger) x Y:(NSUInteger) y;
@end
