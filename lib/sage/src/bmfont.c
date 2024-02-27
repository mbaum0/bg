/**
 * @file bmfont.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "bmfont.h"

void parseBlockInfo(FILE* f, BMFontBlockInfo* block) {
    uint8_t data[5];

    fread(data, 5, 1, f);
    block->blockType = data[0];
    block->blockSize = data[1] | (data[2] << 8);
}

void parseInfoBlock(FILE* f, BMFontInfoBlock* block, size_t size) {
    uint8_t data[14];
    fread(data, 14, 1, f);
    block->fontSize = data[0] | (data[1] << 8);
    block->bitField = data[2];
    block->charSet = data[3];
    block->stretchH = data[4] | (data[5] << 8);
    block->aa = data[6];
    block->paddingUp = data[7];
    block->paddingRight = data[8];
    block->paddingDown = data[9];
    block->paddingLeft = data[10];
    block->spacingHoriz = data[11];
    block->spacingVert = data[12];
    block->outline = data[13];
    uint32_t nameSize = size - 14;
    uint32_t nameSizeDiff = 0;
    if (nameSize > 20) {
        nameSizeDiff = nameSize - 20;
        nameSize = 20;
    }
    fread(block->name, nameSize, 1, f);
    fseek(f, nameSizeDiff, SEEK_CUR);
}

void parseCommonBlock(FILE* f, BMFontCommonBlock* block, size_t size) {
    (void)size;
    uint8_t data[15];
    fread(data, 15, 1, f);
    block->lineHeight = data[0] | (data[1] << 8);
    block->base = data[2] | (data[3] << 8);
    block->scaleW = data[4] | (data[5] << 8);
    block->scaleH = data[6] | (data[7] << 8);
    block->pages = data[8] | (data[9] << 8);
    block->bitField = data[10];
    block->alphaChnl = data[11];
    block->redChnl = data[12];
    block->greenChnl = data[13];
    block->blueChnl = data[14];
}

void parsePagesBlock(FILE* f, BMFontPagesBlock* block, size_t size) {
    char* data = malloc(size);
    fread(data, size, 1, f);
    uint32_t wordSize = 0;
    char a = data[wordSize];
    while (a != '\0') {
        wordSize++;
        a = data[wordSize];
    }
    wordSize++; // add one more for null-termination

    uint32_t numPages = size / wordSize;
    block->pageNames = malloc(numPages * wordSize);

    for (uint32_t i = 0; i < numPages; i++) {
        char* word = malloc(wordSize);
        strncpy(word, data, wordSize);
        block->pageNames[i] = word;
    }
    block->pageCount = numPages;
    free(data);
}

void freePagesBlock(BMFontPagesBlock* block) {
    for (uint32_t i = 0; i < block->pageCount; i++) {
        free(block->pageNames[i]);
    }
    free(block->pageNames);
}

void parseCharsBlock(FILE* f, BMFontCharsBlock* block, size_t size) {
    uint32_t numChars = size / 20;
    block->chars = malloc(20 * numChars);
    block->charCount = numChars;
    fread(block->chars, 20, numChars, f);
}

void freeCharsBlock(BMFontCharsBlock* block) {
    free(block->chars);
}

int BMFont_OpenFont(const char* filename, BMFont* dst) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0L, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    // Read header
    BMFontHeader header;
    fread(&header, 4, 1, file);

    // Check if the file is a valid BMFont binary file
    if (strncmp(header.type, "BMF", 3) != 0) {
        fprintf(stderr, "Invalid BMFont file\n");
        fclose(file);
        return 1;
    }

    BMFontBlockInfo blockInfo;
    long cursor = ftell(file);

    while (cursor < fsize) {
        cursor = ftell(file);
        parseBlockInfo(file, &blockInfo);
        switch (blockInfo.blockType) {
        case 1: // info block
            parseInfoBlock(file, &dst->info, blockInfo.blockSize);
            break;
        case 2: // common block
            parseCommonBlock(file, &dst->common, blockInfo.blockSize);
            break;
        case 3: // pages block
            parsePagesBlock(file, &dst->pages, blockInfo.blockSize);
            break;
        case 4: // chars block
            parseCharsBlock(file, &dst->chars, blockInfo.blockSize);
        }
    }

    fclose(file);

    return 0;
}

void BMFont_DestroyFont(BMFont* dst) {
    freePagesBlock(&dst->pages);
    freeCharsBlock(&dst->chars);
}
