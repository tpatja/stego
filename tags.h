#ifndef _TAGS_H
#define _TAGS_H 1

typedef struct {
  uint8_t type;
  char* name;
} field_type_name_t;

typedef struct tag_name {
  uint16_t tag;
  char * name;
} tag_name_t;


#define NAME_ENTRY( name ) { name, #name } 

enum IFD_field_type {
  BYTE = 1,  /* 8-bit uint32_t */
  ASCII = 2, /* 8-bit byte containing 7-bit ASCII code. last byte must be NUL */
  SHORT = 3, /* 2-byte uint32_t */
  LONG = 4,  /* 4-byte uint32_t */
  RATIONAL = 5, /* 64 bits. two LONGs */
};

enum tiff_tags {
	NewSubfileType = 254, /* FE LONG 1 */
	SubfileType = 255, /* FF SHORT 1*/
	ImageWidth = 256, /* 100 SHORT or LONG 1 */
	ImageLength = 257, /* 101 SHORT or LONG 1*/
  BitsPerSample = 258, /* 102 SHORT SamplesPerPixel */
  Compression = 259, /* 103 SHORT 1 */
  PhotometricInterpretation = 262, /* 106 SHORT 1 */
  Threshholding =  263, /* 107 SHORT 1 */
  CellWidth =  264, /* 108 SHORT 1 */
  CellLength =  265, /* 109 SHORT 1 */
  FillOrder =  266, /* 10A SHORT 1 */
  DocumentName =  269, /* , 10D ASCII */
  ImageDescription =  270,/* , 10E ASCII */
  Make =  271 ,/* , 10F ASCII */
  Model =  272,/* , 110 ASCII */
  StripOffsets =  273, /* 111 SHORT  or LONG StripsPerImage */
  Orientation =  274, /* 112 SHORT 1 */
  SamplesPerPixel =  277, /* 115 SHORT 1 */
  RowsPerStrip =  278, /* 116  SHORT  or LONG 1 */
  StripByteCounts =  279, /*117 LONG  or SHORT StripsPerImage */
  MinSampleValue =  280, /* 118 SHORT SamplesPerPixel */
  MaxSampleValue =  281, /* 119 SHORT SamplesPerPixel */
  XResolution =  282, /* 11A RATIONAL 1 */
  YResolution =  283, /* 11B RATIONAL 1 */
  PlanarConfiguration =  284, /* 11C SHORT 1 */
  PageName =  285,/* , 11D ASCII */
  XPosition =  286,/* , 11E RATIONAL */
  YPosition =  287,/* , 11F RATIONAL */
  FreeOffsets =  288,/* , 120 LONG */
  FreeByteCounts =  289,/* , 121 LONG */
  GrayResponseUnit  =  290, /* 122 SHORT */
  GrayResponseCurve = 291, /* 123 SHORT 2**BitsPerSample */
  T4Options = 292, /* 124 LONG 1*/
  T6Options = 293, /* 125 LONG 1*/
  ResolutionUnit = 296, /* 128 SHORT 1*/
  PageNumber = 297, /* 129 SHORT 2*/
  Software = 305, /* 131 ASCII */
  DateTime = 306, /* 132 ASCII 20 */
  Artist = 315, /* 13B ASCII */
  HostComputer = 316, /* 13C ASCII */
  Predictor = 317, /* 13D SHORT 1 */
  WhitePoint = 318, /* 13E RATIONAL 2 */
  PrimaryChromaticities = 319, /* 13F RATIONAL 6 */
  ColorMap = 320, /* 140 SHORT 3 * (2**BitsPerSample) */
  HalftoneHints = 321, /* 141 SHORT 2 */
  TileWidth = 322, /* 142 SHORT or LONG 1 */
  TileLength = 323, /* 143 SHORT or LONG 1 */
  TileOffsets = 324, /* 144 LONG TilesPerImage */
  TileByteCounts = 325, /* 145 SHORT or LONG TilesPerImage */
  InkSet = 332, /* 14C SHORT 1 */
  InkNames = 333, /* 14D ASCII total number of characters in all ink name strings, including zeros */
  NumberOfInks = 334, /* 14E SHORT 1 */
  DotRange = 336, /* 150 BYTE or SHORT 2, or 2*NumberOfInks */
  TargetPrinter = 337, /* 151 ASCII any */
  ExtraSamples = 338, /* 152 BYTE number of extra components per pixel */
  SampleFormat = 339, /* 153 SHORT SamplesPerPixel */
  SMinSampleValue = 340, /* 154 Any SamplesPerPixel */
  SMaxSampleValue = 341, /* 155 Any SamplesPerPixel */
  TransferRange = 342, /* 156 SHORT 6 */
  JPEGProc = 512, /* 200 SHORT 1*/
  JPEGInterchangeFormat = 513, /* 201 LONG 1 */
  JPEGInterchangeFormatLngth = 514, /* 202 LONG 1 */
  JPEGRestartInterval = 515, /* 203 SHORT 1 */
  JPEGLosslessPredictors = 517, /* 205 SHORT SamplesPerPixel */
  JPEGPointTransforms = 518, /* 206 SHORT SamplesPerPixel */
  JPEGQTables = 519, /* 207 LONG SamplesPerPixel */
  JPEGDCTables = 520, /* 208 LONG SamplesPerPixel */
  JPEGACTables = 521, /* 209 LONG SamplesPerPixel */
  YCbCrCoefficients = 529, /* 211 RATIONAL 3 */
  YCbCrSubSampling = 530, /* 212 SHORT 2 */
  YCbCrPositioning = 531, /* 213 SHORT 1 */
  ReferenceBlackWhite = 532, /* 214 LONG 2*SamplesPerPixel */
  Copyright = 33432 /* 8298 ASCII Any */

};

static tag_name_t tag_names[] = {
	/* generated from tiff_tags enum */
  NAME_ENTRY( NewSubfileType ),
  NAME_ENTRY( SubfileType ),
  NAME_ENTRY( ImageWidth ),
  NAME_ENTRY( ImageLength ),
  NAME_ENTRY( BitsPerSample ),
  NAME_ENTRY( Compression ),
  NAME_ENTRY( PhotometricInterpretation ),
  NAME_ENTRY( Threshholding ),
  NAME_ENTRY( CellWidth ),
  NAME_ENTRY( CellLength ),
  NAME_ENTRY( FillOrder ),
  NAME_ENTRY( DocumentName ),
  NAME_ENTRY( ImageDescription ),
  NAME_ENTRY( Make ),
  NAME_ENTRY( Model ),
  NAME_ENTRY( StripOffsets ),
  NAME_ENTRY( Orientation ),
  NAME_ENTRY( SamplesPerPixel ),
  NAME_ENTRY( RowsPerStrip ),
  NAME_ENTRY( StripByteCounts ),
  NAME_ENTRY( MinSampleValue ),
  NAME_ENTRY( MaxSampleValue ),
  NAME_ENTRY( XResolution ),
  NAME_ENTRY( YResolution ),
  NAME_ENTRY( PlanarConfiguration ),
  NAME_ENTRY( PageName ),
  NAME_ENTRY( XPosition ),
  NAME_ENTRY( YPosition ),
  NAME_ENTRY( FreeOffsets ),
  NAME_ENTRY( FreeByteCounts ),
  NAME_ENTRY( GrayResponseUnit ),
  NAME_ENTRY( GrayResponseCurve ),
  NAME_ENTRY( T4Options ),
  NAME_ENTRY( T6Options ),
  NAME_ENTRY( ResolutionUnit ),
  NAME_ENTRY( PageNumber ),
  NAME_ENTRY( Software ),
  NAME_ENTRY( DateTime ),
  NAME_ENTRY( Artist ),
  NAME_ENTRY( HostComputer ),
  NAME_ENTRY( Predictor ),
  NAME_ENTRY( WhitePoint ),
  NAME_ENTRY( PrimaryChromaticities ),
  NAME_ENTRY( ColorMap ),
  NAME_ENTRY( HalftoneHints ),
  NAME_ENTRY( TileWidth ),
  NAME_ENTRY( TileLength ),
  NAME_ENTRY( TileOffsets ),
  NAME_ENTRY( TileByteCounts ),
  NAME_ENTRY( InkSet ),
  NAME_ENTRY( InkNames ),
  NAME_ENTRY( NumberOfInks ),
  NAME_ENTRY( DotRange ),
  NAME_ENTRY( TargetPrinter ),
  NAME_ENTRY( ExtraSamples ),
  NAME_ENTRY( SampleFormat ),
  NAME_ENTRY( SMinSampleValue ),
  NAME_ENTRY( SMaxSampleValue ),
  NAME_ENTRY( TransferRange ),
  NAME_ENTRY( JPEGProc ),
  NAME_ENTRY( JPEGInterchangeFormat ),
  NAME_ENTRY( JPEGInterchangeFormatLngth ),
  NAME_ENTRY( JPEGRestartInterval ),
  NAME_ENTRY( JPEGLosslessPredictors ),
  NAME_ENTRY( JPEGPointTransforms ),
  NAME_ENTRY( JPEGQTables ),
  NAME_ENTRY( JPEGDCTables ),
  NAME_ENTRY( JPEGACTables ),
  NAME_ENTRY( YCbCrCoefficients ),
  NAME_ENTRY( YCbCrSubSampling ),
  NAME_ENTRY( YCbCrPositioning ),
  NAME_ENTRY( ReferenceBlackWhite ),
  NAME_ENTRY( Copyright )
};

static field_type_name_t field_type_names[] = {
  NAME_ENTRY( BYTE ),
  NAME_ENTRY( ASCII ),
  NAME_ENTRY( SHORT ),
  NAME_ENTRY( LONG ),
  NAME_ENTRY( RATIONAL ),
};

static uint8_t n_tag_names = sizeof(tag_names) / sizeof(tag_name_t);
static uint8_t n_field_names = 
  sizeof(field_type_names) / sizeof(field_type_name_t);


#endif
