#pragma once

#define		CUBE_MAP_RIGHT		                101	
#define		CUBE_MAP_LEFT		                102
#define		CUBE_MAP_BOTTOM		                103
#define		CUBE_MAP_TOP		                104
#define		CUBE_MAP_FRONT		                105
#define		CUBE_MAP_BACK		                106

#define     BUFFER_SIZE_INCREMENT               1048576

#define     WATER_MAP_RESOLUTION			    256 //WMR
#define     WATER_HEIGTH_MAP_RESOLUTION		    256	//WHMR
#define     WATER_NORMAL_MAP_RESOLUTION		    512 //WNMR

//These Constants will be their in the Program
// ODWNMR : One Divided by WNMR
// ODWHMR : One Divided by WHMR
// WHMID  : Water Height Map Index
// TDWNMR : Two Divided by WNMR

#define 	NUMBER_OF_PARTICLES					50
#define 	NUMBER_OF_SPHERE					1
#define		NUMBER_OF_STEPS						25000

#define		NUMBER_OF_SIMULATION_STEPS			10000

float transX[NUMBER_OF_PARTICLES + NUMBER_OF_SPHERE][NUMBER_OF_STEPS];
float transY[NUMBER_OF_PARTICLES + NUMBER_OF_SPHERE][NUMBER_OF_STEPS];
float transZ[NUMBER_OF_PARTICLES + NUMBER_OF_SPHERE][NUMBER_OF_STEPS];

class CBuffer
{
private:
	BYTE *Buffer;
	int BufferSize, Position;

public:
	CBuffer();
	~CBuffer();

	void AddData(void *Data, int DataSize);
	void Empty();
	void *GetData();
	int GetDataSize();

private:
	void SetDefaults();
};

CBuffer::CBuffer()
{
	SetDefaults();
}

CBuffer::~CBuffer()
{
	Empty();
}

void CBuffer::AddData(void *Data, int DataSize)
{
	int Remaining = BufferSize - Position;

	if (DataSize > Remaining)
	{
		BYTE *OldBuffer = Buffer;
		int OldBufferSize = BufferSize;

		int Needed = DataSize - Remaining;

		BufferSize += Needed > BUFFER_SIZE_INCREMENT ? Needed : BUFFER_SIZE_INCREMENT;

		Buffer = new BYTE[BufferSize];

		memcpy(Buffer, OldBuffer, OldBufferSize);

		delete[] OldBuffer;
	}

	memcpy(Buffer + Position, Data, DataSize);

	Position += DataSize;
}

void CBuffer::Empty()
{
	delete[] Buffer;

	SetDefaults();
}

void *CBuffer::GetData()
{
	return Buffer;
}

int CBuffer::GetDataSize()
{
	return Position;
}

void CBuffer::SetDefaults()
{
	Buffer = NULL;
	BufferSize = 0;
	Position = 0;
}

