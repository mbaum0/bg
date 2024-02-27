/**
 * @file bmfont.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BMFont BMFont;
typedef struct BMFontHeader BMFontHeader;
typedef struct BMFontBlockInfo BMFontBlockInfo;
typedef struct BMFontInfoBlock BMFontInfoBlock;
typedef struct BMFontCommonBlock BMFontCommonBlock;
typedef struct BMFontChar BMFontChar;
typedef struct BMFontPagesBlock BMFontPagesBlock;
typedef struct BMFontKerningPair BMFontKerningPair;
typedef struct BMFontCharsBlock BMFontCharsBlock;

struct BMFontHeader {
    char type[3]; // Should be "BMF"
    uint8_t version;
};

struct BMFontBlockInfo {
    uint8_t blockType;
    uint32_t blockSize;
};

struct BMFontInfoBlock {
    int16_t fontSize;
    uint8_t bitField;
    uint8_t charSet;
    uint16_t stretchH;
    uint8_t aa;
    uint8_t paddingUp;
    uint8_t paddingRight;
    uint8_t paddingDown;
    uint8_t paddingLeft;
    uint8_t spacingHoriz;
    uint8_t spacingVert;
    uint8_t outline;
    char name[20];
};

struct BMFontCommonBlock {
    uint16_t lineHeight;
    uint16_t base;
    uint16_t scaleW;
    uint16_t scaleH;
    uint16_t pages;
    uint8_t bitField;
    uint8_t alphaChnl;
    uint8_t redChnl;
    uint8_t greenChnl;
    uint8_t blueChnl;
};

struct BMFontChar {
    uint32_t id;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int16_t xoffset;
    int16_t yoffset;
    int16_t xadvance;
    uint8_t page;
    uint8_t chnl;
};

struct BMFontCharsBlock {
    int charCount;
    BMFontChar* chars;
};

struct BMFontPagesBlock {
    uint32_t pageCount;
    char** pageNames;
};

struct BMFontKerningPair {
    uint32_t first;
    uint32_t second;
    int16_t amount;
};

struct BMFont {
    BMFontInfoBlock info;
    BMFontCommonBlock common;
    BMFontPagesBlock pages;
    BMFontCharsBlock chars;
};

/**
 * @brief Parses a bmfont file into a BMFont struct
 *
 * @param filename
 * @param dst
 * @return int 0 if successful, 1 otherwise
 */
int BMFont_OpenFont(const char* filename, BMFont* dst);

/**
 * @brief Frees up any memory allocated by a BMFont
 *
 * @param dst
 */
void BMFont_DestroyFont(BMFont* dst);
