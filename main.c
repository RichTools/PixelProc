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


  
  SetTargetFPS(60);

  bool dragging = false;
  Vector2 offset = { 0, 0 };

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
      Vector2 mousePos = GetMousePosition();

      const int EDGE_SIZE = 5;
      Rectangle BTMRIGHT = {SCREEN_WIDTH/2 + texture.width/2, 
                           SCREEN_HEIGHT/2 + texture.height/2,
                           EDGE_SIZE, EDGE_SIZE};
      Rectangle TOPRIGHT = {SCREEN_WIDTH/2 + texture.width/2,
                            SCREEN_HEIGHT/2 - texture.height/2 - EDGE_SIZE,
                            EDGE_SIZE, EDGE_SIZE};
      Rectangle TOPLEFT = {SCREEN_WIDTH/2 - texture.width/2 - EDGE_SIZE, 
                           SCREEN_HEIGHT/2 - texture.height/2 - EDGE_SIZE, 
                           EDGE_SIZE, EDGE_SIZE};
      Rectangle BTMLEFT = {SCREEN_WIDTH/2 - texture.width/2 - EDGE_SIZE,
                           SCREEN_HEIGHT/2 + texture.height/2, 
                           EDGE_SIZE, EDGE_SIZE};

      Rectangle RIGHTMID = {SCREEN_WIDTH/2 + texture.width/2, SCREEN_HEIGHT/2, EDGE_SIZE, EDGE_SIZE};
      Rectangle TOPMID = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + texture.height/2, EDGE_SIZE, EDGE_SIZE};
      Rectangle BTMMID = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - texture.height/2 - EDGE_SIZE, EDGE_SIZE, EDGE_SIZE};
      Rectangle LEFTMID = {SCREEN_WIDTH/2 - texture.width/2 - EDGE_SIZE, SCREEN_HEIGHT/2, EDGE_SIZE, EDGE_SIZE};
                  
      // Draw Corners
      DrawRectangleRec(BTMRIGHT, WHITE);
      DrawRectangleRec(RIGHTMID, WHITE);
      DrawRectangleRec(LEFTMID, WHITE);
      DrawRectangleRec(TOPMID, WHITE);
      DrawRectangleRec(BTMMID, WHITE);
      DrawRectangleRec(TOPRIGHT, WHITE);
      DrawRectangleRec(TOPLEFT, WHITE);
      DrawRectangleRec(BTMLEFT, WHITE);

      Rectangle* edgeRect;

      // Start dragging if the mouse button is pressed and is over the rectangle
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          if (CheckCollisionPointRec(mousePos, RIGHTMID)) {
                dragging = true;
                edgeRect = &RIGHTMID;
                offset.x = mousePos.x - RIGHTMID.x; // Set offset from top-left of the rectangle
          }
          else if (CheckCollisionPointRec(mousePos, LEFTMID)) {
                dragging = true;
                edgeRect = &LEFTMID;
                offset.x = mousePos.x - LEFTMID.x;
          }
      }

        // Stop dragging if the mouse button is released
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            dragging = false;
        }

        // Update rectangle position if dragging
        if (dragging) {
            edgeRect->x = mousePos.x;
            nn_interpolate(&img, texture.width + offset.x, texture.height);
            texture = LoadTextureFromImage(img);
        }

      EndGrid();
    EndDrawing();
  }
}
