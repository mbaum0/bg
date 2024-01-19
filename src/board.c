/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Gameboard and trim objects
 */
#include "board.h"

// void createPips(Sage* sage, SDL_Rect board){
//     SDL_Texture* pip = Sage_loadTexture(sage, "assets/pips_sm.png"); 
//     for (int32_t i = 0; i < 6; i++){
//         SDL_Rect pipSrc = {PIP_SRC_W*i, 0, PIP_SRC_W, PIP_SRC_H};
//         int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
//         int32_t pipHeight = pipWidth * PIP_RATIO;
//         int32_t pipX = board.x + board.w * PIP_BOARD_X_OFFSET_PCT + (i * pipWidth);
//         int32_t pipY = board.y;

//         SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
//         Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 3, false);
//         Sage_registerSprite(sage, pipSprite);
//     }
//     for (int32_t i = 0; i < 6; i++){
//         SDL_Rect pipSrc = {PIP_SRC_W*(i+6), 0, PIP_SRC_W, PIP_SRC_H};
//         int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
//         int32_t pipHeight = pipWidth * PIP_RATIO;
//         int32_t pipX = (board.w + board.x) - (board.w * PIP_BOARD_X_OFFSET_PCT) + (i * pipWidth) - (pipWidth * 6);
//         int32_t pipY = board.y;

//         SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
//         Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 3, false);
//         Sage_registerSprite(sage, pipSprite);
//     }
//     for (int32_t i = 0; i < 6; i++){
//         SDL_Rect pipSrc = {PIP_SRC_W*i, PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
//         int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
//         int32_t pipHeight = pipWidth * PIP_RATIO;
//         int32_t pipX = board.x + board.w * PIP_BOARD_X_OFFSET_PCT + (i * pipWidth);
//         int32_t pipY = board.y + board.h - pipHeight;

//         SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
//         Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 3, false);
//         Sage_registerSprite(sage, pipSprite);
//     }

//     for (int32_t i = 0; i < 6; i++){
//         SDL_Rect pipSrc = {PIP_SRC_W*(i+6), PIP_SRC_H, PIP_SRC_W, PIP_SRC_H};
//         int32_t pipWidth = board.w * PIP_BOARD_WIDTH_PCT;
//         int32_t pipHeight = pipWidth * PIP_RATIO;
//         int32_t pipX = (board.w + board.x) - (board.w * PIP_BOARD_X_OFFSET_PCT) + (i * pipWidth) - (pipWidth * 6);
//         int32_t pipY = board.y + board.h - pipHeight;

//         SDL_Rect pipDst = {pipX, pipY, pipWidth, pipHeight};
//         Sprite* pipSprite = Sprite_createEx(pip, pipSrc, pipDst, 2, false);
//         Sage_registerSprite(sage, pipSprite);
//     }
// }

// void createTrim(Sage* sage){
//     SDL_Texture* trim = Sage_loadTexture(sage, "assets/trim.png");
//     float trimWidth = 1.0;
//     float trimHeight = 1.0;
//     float trimX = board.x - (board.w * TRIM_BOARD_X_OFFSET_PCT);
//     float trimY = board.y - (board.h * TRIM_BOARD_Y_OFFSET_PCT);
//     SDL_Rect trimSrc = {0, 0, TRIM_SRC_W, TRIM_SRC_H};
//     SDL_Rect trimDst = {trimX, trimY, trimWidth, trimHeight};
//     Sprite* trimSprite = Sprite_createEx(trim, trimSrc, trimDst, 3, false);
//     Sage_registerSprite(sage, trimSprite);
// }

void Board_create(Sage* sage){
    SDL_Texture* background = Sage_loadTexture(sage, "assets/background.png");
    SDL_Rect bckSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H};
    SDL_FRect bckDst = {0, 0, 1, 1};
    Sprite* back = Sprite_createEx(background, bckSrc, bckDst, 0, false);
    Sage_registerSprite(sage, back);

    // SDL_Texture* boardTexture = Sage_loadTexture(sage, "assets/board.png");
    // SDL_Rect boardSrc = {0, 0, BOARD_SRC_W, BOARD_SRC_H}; 
    // int32_t boardWidth = gameWidth * BOARD_W_PERCENT;
    // int32_t boardHeight = gameHeight * BOARD_H_PERCENT;
    // if (boardWidth / boardHeight > MAX_ASPECT_RATIO){
    //     boardWidth = boardHeight * MAX_ASPECT_RATIO;
    // }
    // if (boardWidth / boardHeight < MIN_ASPECT_RATIO){
    //     boardHeight = boardWidth / MIN_ASPECT_RATIO;
    // }
    // int32_t boardX = (gameWidth - boardWidth) / 2;
    // int32_t boardY = (gameHeight - boardHeight) / 2;
    // board.h = boardHeight;
    // board.w = boardWidth;
    // board.x = boardX;
    // board.y = boardY;
    // board.pipW = boardWidth * PIP_BOARD_WIDTH_PCT;
    // board.pipH = board.pipW * PIP_RATIO;

    // SDL_Rect boardDst = {boardX, boardY, boardWidth, boardHeight};
    // Sprite* boardSprite = Sprite_createEx(boardTexture, boardSrc, boardDst, 1, false);
    // Sage_registerSprite(sage, boardSprite);

    //createPips(sage, boardDst);
    //createTrim(sage);
}
