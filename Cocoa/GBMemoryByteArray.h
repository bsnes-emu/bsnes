#import "Document.h"
#import "HexFiend/HexFiend.h"
#import "HexFiend/HFByteArray.h"

@interface GBMemoryByteArray : HFByteArray
- (instancetype) initWithDocument:(Document *)document;
@end
