#import <Core/gb.h>
#import <HexFiend/HFRepresenter.h>


@interface GBHexStatusBarRepresenter : HFRepresenter
@property GB_gameboy_t *gb;
@property (nonatomic) bool useDecimalLength;
@property (nonatomic) uint16_t bankForDescription;
@property (nonatomic) uint16_t baseAddress;
@end
