#ifndef _TAGS_H
#define _TAGS_H 1

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


typedef struct tag_name {
  uint16_t tag;
  char * name;
} tag_name_t;

#define TAG_NAME_ENTRY(name) { name, #name }

static tag_name_t tag_names[] = {
	/* generated from tiff_tags enum */
  TAG_NAME_ENTRY( NewSubfileType ),
  TAG_NAME_ENTRY( SubfileType ),
  TAG_NAME_ENTRY( ImageWidth ),
  TAG_NAME_ENTRY( ImageLength ),
  TAG_NAME_ENTRY( BitsPerSample ),
  TAG_NAME_ENTRY( Compression ),
  TAG_NAME_ENTRY( PhotometricInterpretation ),
  TAG_NAME_ENTRY( Threshholding ),
  TAG_NAME_ENTRY( CellWidth ),
  TAG_NAME_ENTRY( CellLength ),
  TAG_NAME_ENTRY( FillOrder ),
  TAG_NAME_ENTRY( DocumentName ),
  TAG_NAME_ENTRY( ImageDescription ),
  TAG_NAME_ENTRY( Make ),
  TAG_NAME_ENTRY( Model ),
  TAG_NAME_ENTRY( StripOffsets ),
  TAG_NAME_ENTRY( Orientation ),
  TAG_NAME_ENTRY( SamplesPerPixel ),
  TAG_NAME_ENTRY( RowsPerStrip ),
  TAG_NAME_ENTRY( StripByteCounts ),
  TAG_NAME_ENTRY( MinSampleValue ),
  TAG_NAME_ENTRY( MaxSampleValue ),
  TAG_NAME_ENTRY( XResolution ),
  TAG_NAME_ENTRY( YResolution ),
  TAG_NAME_ENTRY( PlanarConfiguration ),
  TAG_NAME_ENTRY( PageName ),
  TAG_NAME_ENTRY( XPosition ),
  TAG_NAME_ENTRY( YPosition ),
  TAG_NAME_ENTRY( FreeOffsets ),
  TAG_NAME_ENTRY( FreeByteCounts ),
  TAG_NAME_ENTRY( GrayResponseUnit ),
  TAG_NAME_ENTRY( GrayResponseCurve ),
  TAG_NAME_ENTRY( T4Options ),
  TAG_NAME_ENTRY( T6Options ),
  TAG_NAME_ENTRY( ResolutionUnit ),
  TAG_NAME_ENTRY( PageNumber ),
  TAG_NAME_ENTRY( Software ),
  TAG_NAME_ENTRY( DateTime ),
  TAG_NAME_ENTRY( Artist ),
  TAG_NAME_ENTRY( HostComputer ),
  TAG_NAME_ENTRY( Predictor ),
  TAG_NAME_ENTRY( WhitePoint ),
  TAG_NAME_ENTRY( PrimaryChromaticities ),
  TAG_NAME_ENTRY( ColorMap ),
  TAG_NAME_ENTRY( HalftoneHints ),
  TAG_NAME_ENTRY( TileWidth ),
  TAG_NAME_ENTRY( TileLength ),
  TAG_NAME_ENTRY( TileOffsets ),
  TAG_NAME_ENTRY( TileByteCounts ),
  TAG_NAME_ENTRY( InkSet ),
  TAG_NAME_ENTRY( InkNames ),
  TAG_NAME_ENTRY( NumberOfInks ),
  TAG_NAME_ENTRY( DotRange ),
  TAG_NAME_ENTRY( TargetPrinter ),
  TAG_NAME_ENTRY( ExtraSamples ),
  TAG_NAME_ENTRY( SampleFormat ),
  TAG_NAME_ENTRY( SMinSampleValue ),
  TAG_NAME_ENTRY( SMaxSampleValue ),
  TAG_NAME_ENTRY( TransferRange ),
  TAG_NAME_ENTRY( JPEGProc ),
  TAG_NAME_ENTRY( JPEGInterchangeFormat ),
  TAG_NAME_ENTRY( JPEGInterchangeFormatLngth ),
  TAG_NAME_ENTRY( JPEGRestartInterval ),
  TAG_NAME_ENTRY( JPEGLosslessPredictors ),
  TAG_NAME_ENTRY( JPEGPointTransforms ),
  TAG_NAME_ENTRY( JPEGQTables ),
  TAG_NAME_ENTRY( JPEGDCTables ),
  TAG_NAME_ENTRY( JPEGACTables ),
  TAG_NAME_ENTRY( YCbCrCoefficients ),
  TAG_NAME_ENTRY( YCbCrSubSampling ),
  TAG_NAME_ENTRY( YCbCrPositioning ),
  TAG_NAME_ENTRY( ReferenceBlackWhite ),
  TAG_NAME_ENTRY( Copyright )
};
#endif
