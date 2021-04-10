//
//  GBVisualizerView.h
//  SameBoySDL
//
//  Created by Lior Halphon on 7/4/21.
//  Copyright © 2021 Lior Halphon. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <Core/gb.h>

@interface GBVisualizerView : NSView
- (void)addSample:(GB_sample_t *)sample;
@end
