#include "7ZItf.h"
#include "Precomp.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "CpuArch.h"

#include "7z.h"
#include "7zAlloc.h"
#include "7zBuf.h"
#include "7zCrc.h"
#include "7zFile.h"
#include "7zVersion.h"
#ifndef USE_WINDOWS_FILE
/* for mkdir */
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <errno.h>
#endif
#endif
extern "C"
{
#define kInputBufSize ((size_t)1 << 18)

	static const ISzAlloc g_Alloc = { SzAlloc, SzFree };


	static void Print(const char *s)
	{
		fputs(s, stdout);
	}
	static int Buf_EnsureSize(CBuf *dest, size_t size)
	{
		if (dest->size >= size)
			return 1;
		Buf_Free(dest, &g_Alloc);
		return Buf_Create(dest, size, &g_Alloc);
	}
	#ifndef _WIN32

static uint8_t kUtf8Limits[5] = { 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

static bool Utf16_To_Utf8(uint8_t *dest, size_t *destLen, const UInt16 *src, size_t srcLen)
{
  size_t destPos = 0, srcPos = 0;
  for (;;)
  {
    unsigned numAdds;
    UInt32 value;
    if (srcPos == srcLen)
    {
      *destLen = destPos;
      return True;
    }
    value = src[srcPos++];
    if (value < 0x80)
    {
      if (dest)
        dest[destPos] = (char)value;
      destPos++;
      continue;
    }
    if (value >= 0xD800 && value < 0xE000)
    {
      UInt32 c2;
      if (value >= 0xDC00 || srcPos == srcLen)
        break;
      c2 = src[srcPos++];
      if (c2 < 0xDC00 || c2 >= 0xE000)
        break;
      value = (((value - 0xD800) << 10) | (c2 - 0xDC00)) + 0x10000;
    }
    for (numAdds = 1; numAdds < 5; numAdds++)
      if (value < (((UInt32)1) << (numAdds * 5 + 6)))
        break;
    if (dest)
      dest[destPos] = (char)(kUtf8Limits[numAdds - 1] + (value >> (6 * numAdds)));
    destPos++;
    do
    {
      numAdds--;
      if (dest)
        dest[destPos] = (char)(0x80 + ((value >> (6 * numAdds)) & 0x3F));
      destPos++;
    }
    while (numAdds != 0);
  }
  *destLen = destPos;
  return false;
}

static SRes Utf16_To_Utf8Buf(CBuf *dest, const UInt16 *src, size_t srcLen)
{
  size_t destLen = 0;
  SRes res;
  Utf16_To_Utf8(NULL, &destLen, src, srcLen);
  destLen += 1;
  if (!Buf_EnsureSize(dest, destLen))
    return SZ_ERROR_MEM;
  res = Utf16_To_Utf8(dest->data, &destLen, src, srcLen);
  dest->data[destLen] = 0;
  return res ? SZ_OK : SZ_ERROR_FAIL;
}
#endif
	static SRes Utf16_To_Char(CBuf *buf, const UInt16 *s, int fileMode)
{
  int len = 0;
  for (len = 0; s[len] != '\0'; len++);

  #ifdef _WIN32
  {
    int size = len * 3 + 100;
    if (!Buf_EnsureSize(buf, size))
      return SZ_ERROR_MEM;
    {
      char defaultChar = '_';
      BOOL defUsed;
      int numChars = WideCharToMultiByte(fileMode ?
          (
          #ifdef UNDER_CE
          CP_ACP
          #else
          AreFileApisANSI() ? CP_ACP : CP_OEMCP
          #endif
          ) : CP_OEMCP,
          0, s, len, (char *)buf->data, size, &defaultChar, &defUsed);
      if (numChars == 0 || numChars >= size)
        return SZ_ERROR_FAIL;
      buf->data[numChars] = 0;
      return SZ_OK;
    }
  }
  #else
  fileMode = fileMode;
  return Utf16_To_Utf8Buf(buf, s, len);
  #endif
}
static WRes MyCreateDir(const UInt16 *name)
{
  #ifdef USE_WINDOWS_FILE
  
  return CreateDirectoryW(name, NULL) ? 0 : GetLastError();
  
  #else

  CBuf buf;
  WRes res;
  Buf_Init(&buf);
  RINOK(Utf16_To_Char(&buf, name, 1));

  res =
  #ifdef _WIN32
  _mkdir((const char *)buf.data)
  #else
  mkdir((const char *)buf.data, 0777)
  #endif
  == 0 ? 0 : errno;
  Buf_Free(&buf, &g_Alloc);
  return res;
  
  #endif
}
static WRes OutFile_OpenUtf16(CSzFile *p, const UInt16 *name)
{
  #ifdef USE_WINDOWS_FILE
  return OutFile_OpenW(p, name);
  #else
  CBuf buf;
  WRes res;
  Buf_Init(&buf);
  RINOK(Utf16_To_Char(&buf, name, 1));
  res = OutFile_Open(p, (const char *)buf.data);
  Buf_Free(&buf, &g_Alloc);
  return res;
  #endif
}


static SRes PrintString(const UInt16 *s)
{
  CBuf buf;
  SRes res;
  Buf_Init(&buf);
  res = Utf16_To_Char(&buf, s, 0);
  if (res == SZ_OK)
    fputs((const char *)buf.data, stdout);
  Buf_Free(&buf, &g_Alloc);
  return res;
}

	static void UInt64ToStr(UInt64 value, char *s, int numDigits)
	{
		char temp[32];
		int pos = 0;
		do
		{
			temp[pos++] = (char)('0' + (unsigned)(value % 10));
			value /= 10;
		} while (value != 0);

		for (numDigits -= pos; numDigits > 0; numDigits--)
			*s++ = ' ';

		do
			*s++ = temp[--pos];
		while (pos);
		*s = '\0';
	}
#define PERIOD_4 (4 * 365 + 1)
#define PERIOD_100 (PERIOD_4 * 25 - 1)
#define PERIOD_400 (PERIOD_100 * 4 + 1)
	static void PrintLF()
	{
		Print("\n");
	}

	static void PrintError(const char *s)
	{
		Print("\nERROR: ");
		Print(s);
		PrintLF();
	}
	bool Decompress(const char * file)
	{
		ISzAlloc allocImp;
		ISzAlloc allocTempImp;

		CFileInStream archiveStream;
		CLookToRead2 lookStream;
		CSzArEx db;
		SRes res;
		UInt16 *temp = NULL;
		size_t tempSize = 0;
		allocImp = g_Alloc;
		allocTempImp = g_Alloc;
		if (InFile_Open(&archiveStream.file, file))

		{
			PrintError("can not open input file");
			return 1;
		}
		FileInStream_CreateVTable(&archiveStream);
		LookToRead2_CreateVTable(&lookStream, False);
		lookStream.buf = NULL;

		res = SZ_OK;
		{
			lookStream.buf = (Byte *)ISzAlloc_Alloc(&allocImp, kInputBufSize);
			if (!lookStream.buf)
				res = SZ_ERROR_MEM;
			else
			{
				lookStream.bufSize = kInputBufSize;
				lookStream.realStream = &archiveStream.vt;
				LookToRead2_Init(&lookStream);
			}
		}

		CrcGenerateTable();

		SzArEx_Init(&db);

		if (res == SZ_OK)
		{
			res = SzArEx_Open(&db, &lookStream.vt, &allocImp, &allocTempImp);
		}

		if (res == SZ_OK)
		{
			 
			int  fullPaths = 1;

		 

			if (res == SZ_OK)
			{
				UInt32 i;

				/*
				if you need cache, use these 3 variables.
				if you use external function, you can make these variable as static.
				*/
				UInt32 blockIndex = 0xFFFFFFFF; /* it can have any value before first call (if outBuffer = 0) */
				Byte *outBuffer = 0; /* it must be 0 before first call for each new archive. */
				size_t outBufferSize = 0;  /* it can have any value before first call (if outBuffer = 0) */

				for (i = 0; i < db.NumFiles; i++)
				{
					size_t offset = 0;
					size_t outSizeProcessed = 0;
					// const CSzFileItem *f = db.Files + i;
					size_t len;
					unsigned isDir = SzArEx_IsDir(&db, i);
					if (isDir && !fullPaths)
						continue;
					len = SzArEx_GetFileNameUtf16(&db, i, NULL);
					// len = SzArEx_GetFullNameLen(&db, i);

					if (len > tempSize)
					{
						SzFree(NULL, temp);
						tempSize = len;
						temp = (UInt16 *)SzAlloc(NULL, tempSize * sizeof(temp[0]));
						if (!temp)
						{
							res = SZ_ERROR_MEM;
							break;
						}
					}

					SzArEx_GetFileNameUtf16(&db, i, temp);
					res = PrintString(temp);
					if (res != SZ_OK)
						break;

					if (isDir)
						Print("/");
					else
					{
						res = SzArEx_Extract(&db, &lookStream.vt, i,
							&blockIndex, &outBuffer, &outBufferSize,
							&offset, &outSizeProcessed,
							&allocImp, &allocTempImp);
						if (res != SZ_OK)
							break;
					}


					{
						CSzFile outFile;
						size_t processedSize;
						size_t j;
						UInt16 *name = (UInt16 *)temp;
						const UInt16 *destPath = (const UInt16 *)name;

						for (j = 0; name[j] != 0; j++)
							if (name[j] == '/')
							{
								if (fullPaths)
								{
									name[j] = 0;
									MyCreateDir(name);
									name[j] = CHAR_PATH_SEPARATOR;
								}
								else
									destPath = name + j + 1;
							}

						if (isDir)
						{
							MyCreateDir(destPath);
							PrintLF();
							continue;
						}
						else if (OutFile_OpenUtf16(&outFile, destPath))
						{
							PrintError("can not open output file");
							res = SZ_ERROR_FAIL;
							break;
						}

						processedSize = outSizeProcessed;

						if (File_Write(&outFile, outBuffer + offset, &processedSize) != 0 || processedSize != outSizeProcessed)
						{
							PrintError("can not write output file");
							res = SZ_ERROR_FAIL;
							break;
						}

#ifdef USE_WINDOWS_FILE
						{
							FILETIME mtime, ctime;
							FILETIME *mtimePtr = NULL;
							FILETIME *ctimePtr = NULL;

							if (SzBitWithVals_Check(&db.MTime, i))
							{
								const CNtfsFileTime *t = &db.MTime.Vals[i];
								mtime.dwLowDateTime = (DWORD)(t->Low);
								mtime.dwHighDateTime = (DWORD)(t->High);
								mtimePtr = &mtime;
							}
							if (SzBitWithVals_Check(&db.CTime, i))
							{
								const CNtfsFileTime *t = &db.CTime.Vals[i];
								ctime.dwLowDateTime = (DWORD)(t->Low);
								ctime.dwHighDateTime = (DWORD)(t->High);
								ctimePtr = &ctime;
							}
							if (mtimePtr || ctimePtr)
								SetFileTime(outFile.handle, ctimePtr, NULL, mtimePtr);
						}
#endif
						if (File_Close(&outFile))
						{
							PrintError("can not close output file");
							res = SZ_ERROR_FAIL;
							break;
						}
#ifdef USE_WINDOWS_FILE
						if (SzBitWithVals_Check(&db.Attribs, i))
						{
							UInt32 attrib = db.Attribs.Vals[i];
							/* p7zip stores posix attributes in high 16 bits and adds 0x8000 as marker.
							   We remove posix bits, if we detect posix mode field */
							if ((attrib & 0xF0000000) != 0)
								attrib &= 0x7FFF;
							SetFileAttributesW((LPCWSTR)destPath, attrib);
						}
#endif
					}
					PrintLF();
				}
				ISzAlloc_Free(&allocImp, outBuffer);
			}
		}

		SzFree(NULL, temp);
		SzArEx_Free(&db, &allocImp);
		ISzAlloc_Free(&allocImp, lookStream.buf);
		File_Close(&archiveStream.file);
		return res == SZ_OK;
	}
}
