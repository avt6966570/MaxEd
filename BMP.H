/* SPECIFIES THE TYPE OF COMPRESSION FOR A COMPRESSED BITMAP */

/* Specifies that the bitmap is not compressed. */
#define BMP_NOT_COMPRESSED       0L

/*
* COMPRESSION_RLE8 -- Specifies a run-length encoded format for bitmaps with 8 bits per pixel. 
* The compression format is a two-byte format consisting of a count byte followed by a byte 
* containing a color index.
*/
#define BMP_COMPRESSION_RLE8     1L 

/*
* COMPRESSION_RLE4 -- Specifies a run-length encoded format for bitmaps with 4 bits per pixel. 
* The compression format is a two-byte format consisting of a count byte followed by two word-length color 
* indexes.
*/
#define BMP_COMPRESSION_RLE4     2L



/* Spec structures for BMP decoding */

#pragma pack(push)
#pragma pack(2)
typedef struct __BITMAPFILEHEADER {
	unsigned short	bfType;				// Specifies the type of file. It must be BM.		
	unsigned long   bfSize;				// Specifies the size in DWORDs of the file. 
	unsigned short	bfReserved1;		// Is reserved and must be set to zero.					
	unsigned short	bfReserved2;		// Is reserved and must be set to zero. 		
	unsigned long   bfOffBits;			// Specifies in bytes the offset from the BITMAPFILEHEADER of the actual bitmap in the file.		
} BITMAPFILEHEADER_t;				
#pragma pack(pop)


typedef struct __BITMAPINFOHEADER {
	unsigned long	biSize;				// Specifies the number of bytes required by the BITMAPINFOHEADER structure. 
	long	biWidth;					// Specifies the width of the bitmap in pixels. 
	long	biHeight;					// Specifies the height of the bitmap in pixels. 
	unsigned short	biPlanes;			// Specifies the number of planes for the target device andmust be set to 1. 
	unsigned short	biBitCount;			// Specifies the number of bits per pixel. This value must be 1, 4, 8, or 24.
	
	unsigned long	biCompression;		/*
										Specifies the type of compression for a compressed bitmap. 
										It can be one of the following values:.
										Value		Meaning
										BI_RGB		Specifies that the bitmap is not compressed.
										BI_RLE8		Specifies a run-length encoded format for bitmaps with 8 bits per pixel. The 
													compression format is a two-byte format consisting of a count byte 
													followed by a byte containing a color index. See the following 'Comments' 
													section for more information.
										BI_RLE4		Specifies a run-length encoded format for bitmaps with 4 bits per pixel. The 
													compression format is a two-byte format consisting of a count byte 
													followed by two word-length color indexes. See the following 'Comments' 
													section for more information.
										*/

	unsigned long  biSizeImage;			// Specifies the size in bytes of the image.
	
	long   biXPelsPerMeter;				// Specifies the horizontal resolution in pixels per meter of the target 
										// device for the bitmap. An application can use this value to select a 
										// bitmap from a resource group that best matches the characteristics of 
										// the current device.

	long   biYPelsPerMeter;				// Specifies the vertical resolution in pixels per meter of the target device for the bitmap.
										
	unsigned long  biClrUsed;			/*
										Specifies the number of color indexes in the color table 
										actually used by the bitmap. If this value is 0, the 	
										bitmap uses the maximum number of colors corresponding 	
										to the value of the biBitCount field. See the 		
										description of the BITMAPINFO data structure earlier in 	
										this chapter for more information on the maximum sizes 
										of the color table. If biClrUsed is nonzero, then the 
										biClrUsed field specifies the actual number of colors 
										which the graphics engine or device driver will access 
										if the biBitCount field is less than 24. If the 	
										biBitCount field is set to 24, the biClrUsed field 	
										specifies the size of the reference color table used to 
										optimize performance of Windows color palettes.
										If the bitmap is a 'packed' bitmap (that is, a bitmap in 
										which the bitmap array immediately follows the 	
										BITMAPINFO header and which is referenced by a single 	
										pointer), the biClrUsed field must be set to 0 or to the 
										actual size of the color table.
										*/
										
	unsigned long  biClrImportant;		/*
										Specifies the number of color indexes that are considered 
										important for displaying the bitmap. If this value is 0, 	
										then all colors are important. 
										*/

} BITMAPINFOHEADER_t;




typedef struct __RGBQUAD {
	unsigned char    rgbBlue;
	unsigned char    rgbGreen;
	unsigned char    rgbRed;
	unsigned char    rgbReserved;
} RGBQUAD_t;


typedef struct __RGBTRIPLE {
    unsigned char    rgbtBlue;
    unsigned char    rgbtGreen;
    unsigned char    rgbtRed;
} RGBTRIPLE_t;


typedef struct __BITMAPINFO { 
	BITMAPINFOHEADER_t    bmiHeader;
	RGBQUAD_t             bmiColors[1];
} BITMAPINFO_t;


/*
BITMAPCOREHEADER  [3.0] --

Device-Independent  Bitmap  Format  Information  The  BITMAPCOREHEADER
structure  contains  information about the dimensions and color format
of  a device-independent bitmap that is compatible with Microsoft OS/2
Presentation   Manager   versions   1.1   and  1.2  bitmaps.  

An OS/2 Presentation Manager device-independent bitmap consists of two
distinct  parts:  a  BITMAPCOREINFO  data structure that describes the
dimensions  and  colors  of  the  bitmap,  and an array of bytes which
define  the  pixels  of  the  bitmap. The bits in the array are packed
together,  but  each  scan  line  must be zero-padded to end on a long
boundary.  Segment  boundaries  can  appear  anywhere  in  the bitmap,
however. The origin of the bitmap is the lower-left corner.
*/
typedef struct __BITMAPCOREHEADER {
	unsigned long   bcSize;
	unsigned short  bcWidth;
	unsigned short  bcHeight;
	unsigned short  bcPlanes;
	unsigned short  bcBitCount;
} BITMAPCOREHEADER_t;


/*
BITMAPCOREINFO  [3.0] --

Device-Indpendent  Bitmap  Information  The  _BITMAPCOREINFO structure
fully  defines  the  dimensions  and  color  information  for        a
device-independent  bitmap  that  is  compatible  with  Microsoft OS/2
Presentation Manager versions 1.1 and 1.2 bitmaps.

The   BITMAPCOREINFO  data  structure  combines  the  BITMAPCOREHEADER
structure  and  a  color table to provide a complete definition of the
dimensions   and  colors  of  a  device-independent  bitmap.  See  the
description  of the BITMAPCOREINFO data structure for more information
about specifying a device-independent bitmap.
*/

typedef struct __BITMAPCOREINFO {
	BITMAPCOREHEADER_t  bmciHeader;
	RGBTRIPLE_t			bmciColors[1];
} BITMAPCOREINFO_t;


//=============================================================================
//									LOAD BMP
//=============================================================================
#include <stdio.h>	// io file: open, close...
#include <string.h>	// memcpy

// I dont want to include windows.h	;)
#ifndef LOBYTE
#define LOBYTE(w)  ((unsigned char)(w))
#endif
#ifndef HIBYTE
#define HIBYTE(w)  ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))
#endif

enum BMPLOADERROR {
	BMPER_OK,
	BMPER_FILE_NOT_FOUND,
	BMPER_NO_MEMORY,
	BMPER_UNABLE_READ_PALLETTE,
	BMPER_UNABLE_READ_BITMAPHEADER,		// fail read BMP file header
	BMPER_UNABLE_PARSE_BITMAPHEADER,	// check BITMAPINFOHEADER_t size
	BMPER_INVALID_BM_SIGNATURE,			// check BM signature fail (bfhdr.bfType != 0x4d42)
	BMPER_NOT_SUPPORTED_BMP_TYPE,		// check BITMAPINFOHEADER_t size fail
	BMPER_RLE_COMPRESSION_NOT_SUPPORTED,
	BMPER_INVALID_BPP					// only 1,4,8,24 bit image load supported yet
};


// Palette setup...
bool _setupPal24(FILE* imgfile, RGBTRIPLE_t* pPal24, int bits)
{
	RGBQUAD_t rgbQuad; 

	for(int i = 0; i < (1 << bits); i++) {
	    if(fread(&rgbQuad, sizeof(RGBQUAD_t), 1, imgfile) != 1) {
			::printf("BMP: unable to read palette \\%s\\", imgfile->_tmpfname);
			return false;
		}

		// fill color array
	    pPal24[i].rgbtRed   = rgbQuad.rgbRed;
	    pPal24[i].rgbtGreen = rgbQuad.rgbGreen;
	    pPal24[i].rgbtBlue  = rgbQuad.rgbBlue;
	}
	return true;
}

bool _setupPal32(FILE *imgfile, RGBQUAD_t* pPal32, int bits)
{
	for(int i = 0; i < (1 << bits); i++) {
	    if(fread(pPal32, sizeof(RGBQUAD_t), 1, imgfile) != 1) {
			::printf("BMP: unable to read palette \\%s\\", imgfile->_tmpfname);
			return false;
		}
	}
	return true;
}

int LoadBMP(const char* filename, bool PalDecode, int* width, int* height,
			 int* bpp, unsigned char** data)
// load bmp always like as RGB 8-8-8
{
	BITMAPFILEHEADER_t bfhdr;
	BITMAPINFOHEADER_t bmihdr;
	RGBTRIPLE_t* palette;
	FILE* file;
	long i, j, pos;
	unsigned char tmp, *pdata;

	// try to open image file
	if(!(file = fopen(filename, "rb")))
		return BMPER_FILE_NOT_FOUND;

	// try read BMP file header
	if (fread(&bfhdr, sizeof(BITMAPFILEHEADER_t), 1, file) != 1)
		return fclose(file), BMPER_UNABLE_READ_BITMAPHEADER;

	// check BM signature
	if (bfhdr.bfType != 0x4d42)
		return fclose(file), BMPER_INVALID_BM_SIGNATURE;
	
	// try read BMP info header
	if (fread(&bmihdr, sizeof(BITMAPINFOHEADER_t), 1, file) != 1)
		return fclose(file), BMPER_UNABLE_READ_BITMAPHEADER;

	// BMP: check BITMAPINFOHEADER_t size
	if (bmihdr.biSize != sizeof(BITMAPINFOHEADER_t))
		return fclose(file), BMPER_UNABLE_PARSE_BITMAPHEADER;

	// BMP: check compressin...
	if (bmihdr.biCompression != BMP_NOT_COMPRESSED)
		return fclose(file), BMPER_RLE_COMPRESSION_NOT_SUPPORTED;


	switch(bmihdr.biBitCount)
	{
	case 1:
		// setup file stream cursor on initial bute-array (prepare color-byte-array read)
		fseek(file, bfhdr.bfOffBits, SEEK_SET);

		if(PalDecode)
		{
			// alloc memory for new byte-array
			pdata = new unsigned char[bmihdr.biWidth * bmihdr.biHeight * 3];
			if(!pdata)
				return BMPER_NO_MEMORY;

			// palette decode:
			for(i = 0; i < bmihdr.biHeight*bmihdr.biWidth; )
			{
				tmp = fgetc(file);
				for(int j = 7; j >= 0; j--, i+=3)
					pdata[i] = pdata[i+1] = pdata[i+2] =
					((( tmp & ((0x02 << j)-0x01)) >> j) == 0 ) ? 0:255;
			}

			bmihdr.biBitCount = 24;
		}
		else
		{
			// alloc memory for new byte-array
			pdata = new unsigned char[bmihdr.biWidth * bmihdr.biHeight * 1];
			if(!pdata)
				return BMPER_NO_MEMORY;

			// without palette decode:
			for(i = 0; i < bmihdr.biHeight*bmihdr.biWidth; )
			{
				tmp = fgetc(file);
				for(int j = 7; j >= 0; j--, i++)
					pdata[i] = ((( tmp & ((0x02 << j)-0x01)) >> j) == 0 ) ? 0:255;
			}
		}

		break;

	case 4:
		if(PalDecode)
		{
			palette = new RGBTRIPLE_t[16];

			// write palette
			if(!_setupPal24(file, palette, bmihdr.biBitCount))
				return false;

			// setup file stream cursor on initial byte-array (prepare color-byte-array read)
			fseek(file, bfhdr.bfOffBits, SEEK_SET);

			// alloc memory for new byte-array
			pdata = new unsigned char[bmihdr.biWidth * bmihdr.biHeight * 3];
			if(!pdata)
				return BMPER_NO_MEMORY;

			// palette decode:
			for(pos = 0; pos < bmihdr.biWidth * bmihdr.biHeight * 3; )
			{
				tmp = fgetc(file);
				memcpy(&pdata[0]+pos, &palette[(int)HIBYTE(tmp)],
					sizeof(RGBTRIPLE_t)); pos += sizeof(RGBTRIPLE_t);
				memcpy(&pdata[0]+pos, &palette[(int)LOBYTE(tmp)],
					sizeof(RGBTRIPLE_t)); pos += sizeof(RGBTRIPLE_t);
			}

			delete [] palette;

			bmihdr.biBitCount = 24; // !!!
		}
		else
		{
			// setup file stream cursor on initial byte-array (prepare color-byte-array read)
			fseek(file, bfhdr.bfOffBits, SEEK_SET);

			// alloc memory for new byte-array
			pdata = new unsigned char[bmihdr.biWidth * bmihdr.biHeight * 1];
			if(!pdata)
				return BMPER_NO_MEMORY;

			// without palette decode:
			for(i = 0; i < bmihdr.biHeight*bmihdr.biWidth; )
			{
				tmp = fgetc(file);
				pdata[i++] = (unsigned char)HIBYTE(tmp);
				pdata[i++] = (unsigned char)LOBYTE(tmp);
			}
		}
		break; //case 4

	case 8:
		/*if(PalDecode)
		{*/
			palette = new RGBTRIPLE_t[256];

			// setup palette
			if(!_setupPal24(file, palette, bmihdr.biBitCount))
				return false;

			// setup file stream cursor on initial bute-array (prepare color-byte-array read)
			fseek(file, bfhdr.bfOffBits, SEEK_SET);

			// alloc memory for new byte-array
			pdata = new unsigned char[bmihdr.biWidth * bmihdr.biHeight * 3];
			if(!pdata)
				return BMPER_NO_MEMORY;

			// palette decode
			for(pos = 0; pos < bmihdr.biWidth * bmihdr.biHeight * 3; pos += sizeof(RGBTRIPLE_t))
				memcpy(&pdata[0]+pos, &palette[fgetc(file)], sizeof(RGBTRIPLE_t));
			
			delete [] palette;
			
			bmihdr.biBitCount = 24; // !!!
		/*}
		else
		{
			// setup file stream cursor on initial bute-array (prepare color-byte-array read)
			fseek(file, bfhdr.bfOffBits, SEEK_SET);

			// alloc memory for new byte-array
			tex.Data.resize(bmihdr.biWidth * bmihdr.biHeight * 1);

			// without palette decode:
			for(i = 0, pos = 0; i < bmihdr.biHeight; i++, pos += bmihdr.biWidth)
				fread(&pdata[0]+pos, bmihdr.biWidth, 1, file);
		}*/

		break; //case 8

	case 24:
		// setup file stream cursor on initial bute-array (prepare color-byte-array read)
		fseek(file, bfhdr.bfOffBits, SEEK_SET);

		// alloc memory for new byte-array
		pdata = new unsigned char[bmihdr.biWidth * bmihdr.biHeight * 3];
		if(!pdata)
			return BMPER_NO_MEMORY;

		// read byte array color
		for(i = 0, pos = 0; i < bmihdr.biHeight; i++, pos += bmihdr.biWidth*3)
			fread(&pdata[0] + pos, bmihdr.biWidth, 3, file);

		// reorder B-G-R to R-G-B
		for(i = 0; i < bmihdr.biHeight; i++)
		{
			pos = 3*bmihdr.biWidth*i;
			for(j = 0; j < bmihdr.biWidth; j++, pos += 3)
			{
				tmp = pdata[pos];
				pdata[pos] = pdata[pos + 2];
				pdata[pos + 2] = tmp;
			}
		}

		break; //case 24

	default:
		fclose(file);
	    return BMPER_INVALID_BPP;
	}

	fclose(file);
	*width  = (unsigned)bmihdr.biWidth; 
	*height = (unsigned)bmihdr.biHeight;
	*bpp    = bmihdr.biBitCount;
	*data	= pdata;
	return BMPER_OK;
}

//#include <windows.h>
// I dont want to include windows.h	;)
#undef LOBYTE
#undef HIBYTE