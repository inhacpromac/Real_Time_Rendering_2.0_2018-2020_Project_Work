/*
 * Ref - Game Graphics Programming - Allen Sherrod
*/

#include<assert.h>
#include "dds.h"


#ifndef MAKE4CC
	#define MAKE4CC(ch0, ch1, ch2, ch3) \
                ((unsigned long)(unsigned char)(ch0) | \
                ((unsigned long)(unsigned char)(ch1) << 8) | \
                ((unsigned long)(unsigned char)(ch2) << 16) | \
                ((unsigned long)(unsigned char)(ch3) << 24 ))
#endif

#define DS_FOURCC_DXT1  (MAKE4CC('D','X','T','1'))
#define DS_FOURCC_DXT2  (MAKE4CC('D','X','T','2'))
#define DS_FOURCC_DXT3  (MAKE4CC('D','X','T','3'))
#define DS_FOURCC_DXT4  (MAKE4CC('D','X','T','4'))
#define DS_FOURCC_DXT5  (MAKE4CC('D','X','T','5'))


enum ENDIAN { ENDIAN_UNKNOWN = 0, ENDIAN_LITTLE, ENDIAN_BIG };


ENDIAN GetEndian()
{
   unsigned long data = 0x12345678;
   unsigned char *ptr = (unsigned char*)&data;

   if(*ptr == 0x12 && *(ptr+1) == 0x34 && *(ptr+2) == 0x56 && *(ptr+3) == 0x78)
   {
      return ENDIAN_BIG;
   }
   else if(*ptr == 0x78 && *(ptr + 1) == 0x56 && *(ptr + 2) == 0x34 && *(ptr + 3) == 0x12)
   {
      return ENDIAN_LITTLE;
   }

   return ENDIAN_UNKNOWN;
}


void SwapBytes(char *data, int size)
{
	assert((size & 1) == 0);

	char *ptr = data;
	char temp = 0;

	for(int i = 0, j = size - 1; i < size / 2; i++, j--)
	{
		temp = ptr[i];
		ptr[i] = ptr[j];
	   ptr[j] = temp;
	}
}


unsigned char *LoadDDS(const char *file, ImageInfoDDS &info)
{
   fprintf(gpFile, "\t\t\t\tEentering - *LoadDDS(const char *file, ImageInfoDDS &info)\n");
   
   const int ddsHeightOffset = 12;
   const int ddsWidthOffset = 16;
   const int ddsLinearSizeOffset = 20;
   const int ddsMipMapNumOffset = 28;
   const int ddsFourCCOffset = 84;
   const int ddsImageDataOffset = 128;


   ENDIAN e = GetEndian();
   bool byteSwap = false;

   if(e == ENDIAN_BIG)
      byteSwap = true;

   FILE *fp = fopen(file, "rb");

   if(fp == NULL)
      return NULL;


   char imageID[4];
   fread(imageID, 1, 4, fp);

   if(strncmp(imageID, "DDS ", 4) != 0)
   {
      fclose(fp);
      return false;
   }


   unsigned int dwHeight = 0, dwWidth = 0,
                dwLinearSize, dwMipMaps = 0,
                dwFourCC = 0;

   fseek(fp, ddsHeightOffset, SEEK_SET);
   fread(&dwHeight, sizeof(unsigned int), 1, fp);

   if(byteSwap == true)
   SwapBytes((char*)&dwHeight, sizeof(unsigned int));
   
   fseek(fp, ddsWidthOffset, SEEK_SET);
   fread(&dwWidth, sizeof(unsigned int), 1, fp);

   if(byteSwap == true)
   SwapBytes((char*)&dwWidth, sizeof(unsigned int));

   fseek(fp, ddsLinearSizeOffset, SEEK_SET);
   fread(&dwLinearSize, sizeof(unsigned int), 1, fp);
   
   if(byteSwap == true)
   SwapBytes((char*)&dwLinearSize, sizeof(unsigned int));
   
   fseek(fp, ddsMipMapNumOffset, SEEK_SET);
   fread(&dwMipMaps, sizeof(unsigned int), 1, fp);

   if(byteSwap == true)
      SwapBytes((char*)&dwMipMaps, sizeof(unsigned int));

   fseek(fp, ddsFourCCOffset, SEEK_SET);
   fread(&dwFourCC, sizeof(unsigned int), 1, fp);

   if(byteSwap == true)
   SwapBytes((char*)&dwFourCC, sizeof(unsigned int));

   if(dwLinearSize == 0)
   dwLinearSize = dwHeight * dwWidth;

   if(dwLinearSize <= 0)
   {
      fclose(fp);
      return NULL;
   }

   info.m_numMipMaps = dwMipMaps;
   info.m_width = dwWidth;
   info.m_height = dwHeight;


   int mipFactor = 0;

   switch(dwFourCC)
   {
      case DS_FOURCC_DXT1:
            mipFactor = 2;
            info.m_components = 3;
            info.m_type = DDS_DXT1;
            break;

      case DS_FOURCC_DXT3:
            mipFactor = 4;
            info.m_components = 4;
            info.m_type = DDS_DXT3;
            break;

      case DS_FOURCC_DXT5:
            mipFactor = 4;
            info.m_components = 4;
            info.m_type = DDS_DXT5;
            break;

      default:
            fclose(fp);
            return NULL;
            break;
   }


   int totalSize = 0;

   // Take into account multiple mip maps.
   if(dwMipMaps > 1)
      totalSize = dwLinearSize * mipFactor;
   else
      totalSize = dwLinearSize;

   unsigned char *image = NULL;

   image = new unsigned char[totalSize * sizeof(unsigned char)];
   
	if(image != NULL)
	{
	   fseek(fp, ddsImageDataOffset, SEEK_SET);
	   fread(image, 1, totalSize, fp);
	}

   fclose(fp);

   fprintf(gpFile, "\t\t\t\tExitting - *LoadDDS(const char *file, ImageInfoDDS &info)\n");
   
   return image;
}

GLuint LoadTexture(const char *szFilename)
{
   fprintf(gpFile, "\t\t\tEentering - LoadTexture(const char *szFilename)\n");

	GLuint texture = 0;
	
	ImageInfoDDS info;
	unsigned char *image = LoadDDS(szFilename, info);

	int w = info.m_width;
   int h = info.m_height;
	int mipFactor = 0;

   if(info.m_type == DDS_DXT1)
   mipFactor = 8;
   else
   mipFactor = 16;

   int mipSize;
   int mipOffset = 0;
   int type = 0;

	
	glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
	
   switch(info.m_type)
   {
      case DDS_DXT1:
        	type = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
         break;

      case DDS_DXT3:
         type = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
         break;

      case DDS_DXT5:
         type = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
         break;
    }

   for(int i = 0; i < info.m_numMipMaps; i++)
   {
      mipSize = ((w + 3) / 4) * ((h + 3) / 4) * mipFactor;

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glCompressedTexImage2DARB(GL_TEXTURE_2D, i, type, w, h, 0, mipSize, image + mipOffset);

      // Half the image size for the next mip-map level...
      w >>= 1;
      h >>= 1;

      // Move the offset to the next mip map.
      mipOffset += mipSize;
   }

	glBindTexture(GL_TEXTURE_2D, 0);

   delete[] image;
	
   fprintf(gpFile, "\t\t\tExitting - LoadTexture(const char *szFilename)\n");

	return texture;
}
