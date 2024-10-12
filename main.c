#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"

#define IMAGE_PROCESSING_IMPLEMENTATION
#include "ImageProcessing.h"

#define UI_IMPLEMENTATION
#include "UI.h"

const char* TITLE = "PixelProc";
const int FACTOR = 80;
const int SCREEN_WIDTH = FACTOR*16;
const int SCREEN_HEIGHT= FACTOR*9;

int main() 
{

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);

  Image img = LoadImage("./images/image.png");
  Texture2D texture = LoadTextureFromImage(img);

  while (!WindowShouldClose()) {
    BeginDrawing();
      BeginGrid(1, 4, 200, 50, 20, 20, 10, 20);
      ClearBackground(BLACK);
    
      Vector2 pos = getNextGridPosition();
      if(DrawButton("To Gray Scale", pos.y)/*Pressed*/)
      {
        rgb2gray(&img);
        texture = LoadTextureFromImage(img);       
      }

      pos = getNextGridPosition();
      if(DrawButton("Quantise 8bits", pos.y)) 
      {
        quantise(&img, 8);
        texture = LoadTextureFromImage(img);
      }

      pos = getNextGridPosition();
      if(DrawButton("FlipH Image", pos.y))
      {
        flip_img(&img, true);
        texture = LoadTextureFromImage(img);
      }

      pos = getNextGridPosition();
      if(DrawButton("FlipV Image", pos.y))
      {
        flip_img(&img, false);
        texture = LoadTextureFromImage(img);
      }


      pos = getNextGridPosition();
      if (DrawButton("Export Image", pos.y)/*Pressed*/)
      {
        ExportImage(img, "image.png");
      }

      // Draw Image
      DrawTexture(texture, SCREEN_WIDTH/2 - texture.width/2, SCREEN_HEIGHT/2 - texture.height/2, WHITE);
      EndGrid();
    EndDrawing();
  }
}
