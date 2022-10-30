#import "GBJoyConManager.h"
#import "GBTintedImageCell.h"
#import <objc/runtime.h>

@implementation GBJoyConManager
{
    GBTintedImageCell *_tintedImageCell;
    NSImageCell *_imageCell;
    NSMutableDictionary<NSString *, NSString *> *_pairings;
    NSButton *_autoPairCheckbox;
    bool _unpairing;
}

+ (instancetype)sharedInstance
{
    static GBJoyConManager *manager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [[self alloc] _init];
    });
    return manager;
}

- (NSArray <JOYController *> *)joycons
{
    NSMutableArray *ret = [[JOYController allControllers] mutableCopy];
    for (JOYController *controller in [JOYController allControllers]) {
        if (controller.joyconType == JOYJoyConTypeNone) {
            [ret removeObject:controller];
        }
    }
    return ret;
}

- (instancetype)init
{
    return [self.class sharedInstance];
}

- (instancetype) _init
{
    self = [super init];
    _imageCell = [[NSImageCell alloc] init];
    _tintedImageCell = [[GBTintedImageCell alloc] init];
    if (@available(macOS 10.14, *)) {
        _tintedImageCell.tint = [NSColor controlAccentColor];
    }
    else {
        _tintedImageCell.tint = [NSColor selectedMenuItemColor];
    }
    _pairings = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBJoyConPairings"] ?: @{} mutableCopy];
    
    // Sanity check the pairings
    for (NSString *key in _pairings) {
        if (![_pairings[_pairings[key]] isEqualToString:key]) {
            [_pairings removeAllObjects];
            break;
        }
    }
    
    [JOYController registerListener:self];
    for (JOYController *controller in [JOYController allControllers]) {
        [self controllerConnected:controller];
    }
    
    return self;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    return self.joycons.count;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    if (row >= [self numberOfRowsInTableView:tableView]) return nil;
    
    unsigned columnIndex = [[tableView tableColumns] indexOfObject:tableColumn];
    switch (columnIndex) {
        case 0: {
            JOYController *controller = self.joycons[row];
            switch (controller.joyconType) {
                case JOYJoyConTypeNone:
                    return nil;
                case JOYJoyConTypeLeft:
                    return [NSImage imageNamed:@"JoyConLeftTemplate"];
                case JOYJoyConTypeRight:
                    return [NSImage imageNamed:@"JoyConRightTemplate"];
                case JOYJoyConTypeCombined:
                    return [NSImage imageNamed:@"JoyConCombinedTemplate"];
            }
        }
        case 1: {
            JOYController *controller = self.joycons[row];
            NSMutableAttributedString *ret = [[NSMutableAttributedString alloc] initWithString:controller.deviceName
                                                                                    attributes:@{NSFontAttributeName:
                                                                                                     [NSFont systemFontOfSize:[NSFont systemFontSize]]}];

            [ret appendAttributedString:[[NSAttributedString alloc] initWithString:[@"\n" stringByAppendingString:controller.uniqueID]
                                                                        attributes:@{NSFontAttributeName:
                                                                                         [NSFont systemFontOfSize:[NSFont smallSystemFontSize]],
                                                                                     NSForegroundColorAttributeName:[NSColor disabledControlTextColor]}]];
            return ret;
        }
        case 2:
            return @(rand() % 3);
    }
    return nil;
}

-(NSCell *)tableView:(NSTableView *)tableView dataCellForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    if (row >= [self numberOfRowsInTableView:tableView]) return [[NSCell alloc] init];

    unsigned columnIndex = [[tableView tableColumns] indexOfObject:tableColumn];
    if (columnIndex == 2) {
        JOYCombinedController *controller = (JOYCombinedController *)self.joycons[row];
        if (controller.joyconType == JOYJoyConTypeCombined) {
            NSButtonCell *cell = [[NSButtonCell alloc] initTextCell:@"Separate Joy-Cons"];
            cell.bezelStyle = NSBezelStyleRounded;
            cell.action = @selector(invoke);
            id block = ^(void) {
                for (JOYController *child in controller.children) {
                    [_pairings removeObjectForKey:child.uniqueID];
                }
                [[NSUserDefaults standardUserDefaults] setObject:_pairings forKey:@"GBJoyConPairings"];
                _unpairing = true;
                [controller breakApart];
                _unpairing = false;
            };
            // To retain the block
            objc_setAssociatedObject(cell, @selector(breakApart), block, OBJC_ASSOCIATION_RETAIN);
            cell.target = block;
            return cell;
        }
    }
    if (columnIndex == 0) {
        JOYController *controller = self.joycons[row];
        for (JOYButton *button in controller.buttons) {
            if (button.isPressed) {
                return _tintedImageCell;
            }
        }
        return _imageCell;
    }
    return nil;
}

- (void)controllerConnected:(JOYController *)controller
{
    for (JOYController *partner in [JOYController allControllers]) {
        if ([partner.uniqueID isEqualToString:_pairings[controller.uniqueID]]) {
            [self pairJoyCon:controller withJoyCon:partner];
            break;
        }
    }
    if (controller.joyconType == JOYJoyConTypeLeft || controller.joyconType == JOYJoyConTypeRight) {
        [self autopair];
    }
    [self.tableView reloadData];
}

- (void)autopair
{
    if (_unpairing) return;
    if (![[NSUserDefaults standardUserDefaults] boolForKey:@"GBJoyConAutoPair"]) return;
    NSArray<JOYController *> *controllers = [[JOYController allControllers] copy];
    for (JOYController *first in controllers) {
        if (_pairings[first.uniqueID]) continue; // Has an established partner
        if (first.joyconType != JOYJoyConTypeLeft) continue;
        for (JOYController *second in controllers) {
            if (_pairings[second.uniqueID]) continue; // Has an established partner
            if (second.joyconType != JOYJoyConTypeRight) continue;
            [self pairJoyCon:first withJoyCon:second];
            break;
        }
    }
    [self.tableView reloadData];
}

- (void)controllerDisconnected:(JOYController *)controller
{
    [self.tableView reloadData];
}

- (BOOL)tableView:(NSTableView *)tableView shouldEditTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    return false;
}

- (JOYCombinedController *)pairJoyCon:(JOYController *)first withJoyCon:(JOYController *)second
{
    if (first.joyconType != JOYJoyConTypeLeft && first.joyconType != JOYJoyConTypeRight) return nil; // Not a Joy-Con
    if (second.joyconType != JOYJoyConTypeLeft && second.joyconType != JOYJoyConTypeRight) return nil; // Not a Joy-Con
    if (first.joyconType == second.joyconType) return nil; // Not a sensible pair
    
    _pairings[first.uniqueID] = second.uniqueID;
    _pairings[second.uniqueID] = first.uniqueID;
    [[NSUserDefaults standardUserDefaults] setObject:_pairings forKey:@"GBJoyConPairings"];
    return [[JOYCombinedController alloc] initWithChildren:@[first, second]];
}

- (void)controller:(JOYController *)controller buttonChangedState:(JOYButton *)button
{
    if (!_arrangementMode) return;
    if (controller.joyconType == JOYJoyConTypeNone) return;
    [self.tableView setNeedsDisplay:true];
    if (controller.joyconType != JOYJoyConTypeLeft && controller.joyconType != JOYJoyConTypeRight) return;
    if (button.usage != JOYButtonUsageL1 && button.usage != JOYButtonUsageR1) return;
    
    
    // L or R were pressed on a single Joy-Con, try and pair available Joy-Cons
    JOYController *left = nil;
    JOYController *right = nil;
    for (JOYController *controller in [JOYController allControllers]) {
        if (!left && controller.joyconType == JOYJoyConTypeLeft) {
            for (JOYButton *button in controller.buttons) {
                if (button.usage == JOYButtonUsageL1 && button.isPressed) {
                    left = controller;
                    break;
                }
            }
        }
        if (!right && controller.joyconType == JOYJoyConTypeRight) {
            for (JOYButton *button in controller.buttons) {
                if (button.usage == JOYButtonUsageR1 && button.isPressed) {
                    right = controller;
                    break;
                }
            }
        }
        if (left && right) {
            [self pairJoyCon:left withJoyCon:right];
            return;
        }
    }
}

- (void)setAutoPairCheckbox:(NSButton *)autoPairCheckbox
{
    _autoPairCheckbox = autoPairCheckbox;
    [_autoPairCheckbox setState:[[NSUserDefaults standardUserDefaults] boolForKey:@"GBJoyConAutoPair"]];
}

- (IBAction)toggleAutoPair:(NSButton *)sender
{
    [[NSUserDefaults standardUserDefaults] setBool:sender.state forKey:@"GBJoyConAutoPair"];
    [self autopair];
}

@end
