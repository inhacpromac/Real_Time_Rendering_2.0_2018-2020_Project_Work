/*
 * Ref - Game Graphics Programming - Allen Sherrod
*/

#ifndef _DDS_LOADER_H_
#define _DDS_LOADER_H_

enum DDS_COMPRESSION { DDS_NULL = 0, DDS_DXT1, DDS_DXT3, DDS_DXT5 };

struct ImageInfoDDS
{
   ImageInfoDDS() : m_width(0), m_height(0), m_type(DDS_NULL),
                    m_components(0), m_numMipMaps(0)
   {

   }

   int m_width;
   int m_height;
   DDS_COMPRESSION m_type;
   int m_components;
   int m_numMipMaps;
};

unsigned char *LoadDDS(char *file, ImageInfoDDS &info);

#endif
