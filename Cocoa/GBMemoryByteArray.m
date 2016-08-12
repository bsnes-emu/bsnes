#import "GBMemoryByteArray.h"
#import "GBCompleteByteSlice.h"


@implementation GBMemoryByteArray
{
    Document *_document;
}

- (instancetype) initWithDocument:(Document *)document
{
    if ((self = [super init])) {
        _document = document;
    }
    return self;
}

- (unsigned long long)length
{
    return 0x10000;
}

- (void)copyBytes:(unsigned char *)dst range:(HFRange)range
{
    uint16_t addr = (uint16_t) range.location;
    unsigned long long length = range.length;
    while (length) {
        *(dst++) = [_document readMemory:addr++];
        length--;
    }
}

- (NSArray *)byteSlices
{
    return @[[[GBCompleteByteSlice alloc] initWithByteArray:self]];
}

- (HFByteArray *)subarrayWithRange:(HFRange)range
{
    unsigned char arr[range.length];
    [self copyBytes:arr range:range];
    HFByteArray *ret = [[HFBTreeByteArray alloc] init];
    HFFullMemoryByteSlice *slice = [[HFFullMemoryByteSlice alloc] initWithData:[NSData dataWithBytes:arr length:range.length]];
    [ret insertByteSlice:slice inRange:HFRangeMake(0, 0)];
    return ret;
}

- (void)insertByteSlice:(HFByteSlice *)slice inRange:(HFRange)lrange
{
    if (slice.length != lrange.length) return; /* Insertion is not allowed, only overwriting. */
    [_document performAtomicBlock:^{
        uint8_t values[lrange.length];
        [slice copyBytes:values range:HFRangeMake(0, lrange.length)];
        uint16_t addr = (uint16_t) lrange.location;
        uint8_t *src = values;
        unsigned long long length = lrange.length;
        while (length) {
            [_document writeMemory:addr++ value:*(src++)];
            length--;
        }
    }];
}

@end
