#pragma once

#include "Renderer.h"

#define SCREEN_WIDTH Renderer::Instance()->getWidth()
#define SCREEN_HEIGHT Renderer::Instance()->getHeight()
#define SCALE_FACTOR SCREEN_WIDTH / 320.0f

#define BLACK_SPEED SCREEN_WIDTH * 0.15 //15%� SCREEN_WIDTH/s 
#define BLUE_SPEED SCREEN_WIDTH * 0.26 //26% SCREEN_WIDTH/s
#define RED_SPEED SCREEN_WIDTH * 0.37 //37% SCREEN_WIDTH/s
#define FALL_SPEED SCREEN_WIDTH * 0.3
#define BIRD_SCREEN_DURATON 6
#define BIRD_BOUNCE_ANGLE 60

#define BIRD_WIDTH_PIXELS 32
#define BIRD_HEIGHT_PIXELS 32
#define BIRD_WIDTH_2_PIXELS 40
#define SCREEN_WIDTH_PERCENTAGE 10

#define BIRD_WIDTH_SCREEN  SCREEN_WIDTH / SCREEN_WIDTH_PERCENTAGE
#define BIRD_HEIGHT_SCREEN (BIRD_WIDTH_SCREEN * BIRD_HEIGHT_PIXELS) / BIRD_WIDTH_PIXELS
#define BIRD_WIDTH_2_SCREEN SCREEN_WIDTH / (SCREEN_WIDTH_PERCENTAGE * BIRD_WIDTH_2_PIXELS / BIRD_WIDTH_PIXELS)

#define TREE_PATH "assets\\arbol.rgba"
#define BUSH_PATH  "assets\\arbusto.rgba"
#define GROUND_PATH "assets\\suelo.rgba"

#define CURSOR_X SCREEN_WIDTH / 2
#define CURSOR_Y SCREEN_HEIGHT / 2

#define HIT_DURATION 1

const float FRAME_DURATION = 1.0f / 3.0f; // const so it's not calculated each time it's used