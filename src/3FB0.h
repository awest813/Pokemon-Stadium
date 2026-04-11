#ifndef _3FB0_H_
#define _3FB0_H_

#include "global.h"
#include "5580.h"
#include "jpegutils.h"
#include "CE80.h"
#include "src/memmap.h"
#include "src/memory.h"

typedef struct ret_func_8000484C {
	/* 0x00 */ char unk00[0x90];
	/* 0x90 */ s32 unk_90;
} ret_func_8000484C; // size >= 0x94

typedef struct PERSSZP {
    /* 0x00 */ char magic[0x8]; // PERS-SZP
    /* 0x08 */ u32 header_size;
    /* 0x0C */ u32 decompressed_size1; // How do these differ?
    /* 0x10 */ u32 decompressed_size2;
    /* 0x14 */ u32 unk_14;
} PERSSZP; // sze = 0x18

typedef struct PRESJPEG {
    /* 0x00 */ char magic[0x8]; // PRESJPEG
    /* 0x08 */ u32 unk_08; // header size? always 0x10
    /* 0x0C */ u32 unk_0C; 
} PRESJPEG; // size = 0x10

typedef struct Yay0 {
    /* 0x00 */ char magic[0x4]; // Yay0
    /* 0x04 */ u32 decompressed_size;
    /* 0x08 */ u32 link_table_offset;
    /* 0x0C */ u32 byte_chunk_offset;
} Yay0; // size = 0x10

typedef struct BinArchive {
    union {
        struct {
            /* 0x00 */ u16 unk_00;
            /* 0x02 */ u16 unk_02;
        };
        s32 raw;
    };
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 total_size;
    /* 0x0C */ u32 num_files;
} BinArchive; // size = 0x10

typedef struct BinArchiveFile {
    /* 0x00 */ u32 offset;
    /* 0x04 */ u32 size;
    /* 0x08 */ void* unk_08;
    /* 0x0C */ char unk0C[0x4];
} BinArchiveFile; // size = 0x10

typedef struct unk_func_800041C0 {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} unk_func_800041C0; // size >= 0x8

typedef struct unk_func_80003680_sp300 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ char unk02[0x2];
    /* 0x04 */ s16 width;
    /* 0x06 */ s16 height;
    /* 0x08 */ u8* quantization_table;
    /* 0x0C */ u8* huffman_table;
    /* 0x10 */ u8* start_of_frame;
    /* 0x14 */ u8* start_of_scan;
} unk_func_80003680_sp300; // size = 0x18

typedef struct unk_func_80003680_sp90 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
} unk_func_80003680_sp90; // size >= 0x18

typedef void (*ret_func_80004454)(void);

s32 JpegStream_ReadU16(u8* arg0);
void JpegStream_ParseMarkers(unk_func_80003680_sp300* arg0, u8* arg1);
void RSPTask_InitJpeg(RSPTask* arg0, unk_func_80003680_sp90* arg1);
void Jpeg_SetHuffmanData(unk_func_80003680_sp27* arg0, JpegHuffmanTable* arg1, unk_func_80003680_sp300* arg2);
void Jpeg_SetQuantizationData(unk_func_80003680_sp90* arg0, u8(arg1)[2][0x80], unk_func_80003680_sp300* arg2);
s32 JPEG_Decompress(u32 addr, s32 arg1, u8* arg2);
void func_80003860(void);
void SZP_Decompress(u8* in_header, u8* memory);
void func_80003964(u8* arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_80003A14(u8* arg0, s16 arg1, s16 arg2, s32 arg3);
s32 func_80003B04(s16 arg0);
void ROM_LoadDirect(u32 arg0, u32 arg1, u32 arg2, s32 arg3);
void func_80003BE0(u8* arg0, u8* arg1, u8* arg2);
void* Jpeg_AllocAndDecompress(u8* addr, PRESJPEG* arg1, s32 side);
s32 SZP_AllocAndDecompress(s32 arg0, PERSSZP* arg1, s32 side);
u32* ROM_AllocAndCopy(u8* arg0, s32 arg1, u32 arg2, s32 arg3);
void* ROM_LoadAndDecompress(u8* romStart, u8* romEnd, s32 arg2, s32 arg3);
void* LBA_LoadAndDecompress(unk_func_800041C0* arg0, s32 arg1);
unk_func_800041C0* LBALoader_Init(unk_func_800041C0* arg0, s32 arg1, s32 arg2);
unk_func_800041C0* LBALoader_InitFromOffset(unk_func_800041C0* arg0, u32 arg1, u32 offset, u32 size);
void func_800041C0(s32 arg0, s32 arg1, s32 arg2);
void func_80004200(u32 block_addr, u32 addr);
u8* Memmap_LoadAndMapSegment(s32 id, u8* rom_start, u8* rom_end, s32 arg3);
MainPoolBlock* func_800042E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80004364(u32 base_addr, u32 addr);
void Fragment_RelocateAndInit(s32 arg0, Fragment* addr);
ret_func_80004454 Memmap_LoadAndInitFragment(s32 arg0, u8* romStart, u8* romEnd);
void* func_800044A8(s32 arg0, s32 arg1, s32 arg2);
BinArchive* Archive_ROM_Load(u8* romStart, u8* romEnd, s32 arg2, s32 arg3);
void* func_80004660(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800047A4(void* arg0);
Fragment* Archive_ROM_LoadFile(BinArchive* arg0, BinArchiveFile* arg1);
void* Archive_LBA_LoadFile(BinArchive* arg0, BinArchiveFile* arg1);
void* Archive_GetFile(BinArchive* archive, s32 file_number);
s32 func_8000495C(BinArchive* archive, s32 file_number);

#endif // _3FB0_H_
