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
      
      float gamma_lighten = 1.0f;
      pos = getNextGridPosition();
      if (DrawButton("Lighten", pos.y))
      {
        gamma_lighten -= 0.1;
        gamma_transform(&img, gamma_lighten);
        texture = LoadTextureFromImage(img);
      }
      
      float gamma_darken = 1.0f;
      pos = getNextGridPosition();
      if (DrawButton("Darken", pos.y))
      {
        gamma_darken += 0.1f;
        gamma_transform(&img, gamma_darken);
        texture = LoadTextureFromImage(img);
      }


      pos = getNextGridPosition();
      if (DrawButton("Export Image", pos.y)/*Pressed*/)
      {
        ExportImage(img, "image.png");
      }

      // Draw Image
      Rectangle bounds = (Rectangle){
          SCREEN_WIDTH - (SCREEN_WIDTH - 500),  // x position
          10, // y position
          SCREEN_WIDTH - 500,                   // width of box
          SCREEN_HEIGHT - 200                    // height of box
      };

      // Draw the texture maintaining aspect ratio
      DrawTextureInBox(texture, bounds, WHITE);

      // draw the bounds for debugging
      //DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, RED);
      

      Vector2 mousePos = GetMousePosition();

      const int EDGE_SIZE = 5;

      Vector2 dim = GetScaledDimensions(texture, bounds);
      float x_texture = bounds.x + (bounds.width - dim.x) * 0.5f;
      float y_texture = bounds.y + (bounds.height - dim.y) * 0.5f;
    

      Rectangle BTMRIGHT = {x_texture + dim.x - EDGE_SIZE, y_texture + dim.y - EDGE_SIZE, EDGE_SIZE, EDGE_SIZE};
      Rectangle TOPRIGHT = {x_texture + dim.x - EDGE_SIZE, y_texture, EDGE_SIZE, EDGE_SIZE};
      Rectangle TOPLEFT = {x_texture, y_texture, EDGE_SIZE, EDGE_SIZE};
      Rectangle BTMLEFT = {x_texture, y_texture + dim.y - EDGE_SIZE, EDGE_SIZE, EDGE_SIZE};
      Rectangle RIGHTMID = {x_texture + dim.x - EDGE_SIZE, y_texture + dim.y/2 , EDGE_SIZE, EDGE_SIZE};
      Rectangle TOPMID = {x_texture + dim.x / 2,  y_texture, EDGE_SIZE, EDGE_SIZE};
      Rectangle BTMMID = {x_texture + dim.x / 2, y_texture + dim.y - EDGE_SIZE, EDGE_SIZE, EDGE_SIZE};
      Rectangle LEFTMID = {x_texture, y_texture + dim.y / 2, EDGE_SIZE, EDGE_SIZE};
                  
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
          else if (CheckCollisionPointRec(mousePos, TOPMID)) {
                dragging = true;
                edgeRect = &TOPMID;
                offset.y = mousePos.y - TOPMID.y;
          }
          else if (CheckCollisionPointRec(mousePos, BTMMID)) {
                dragging = true;
                edgeRect = &BTMMID;
                offset.y = mousePos.y - BTMMID.y;
          }
          else if (CheckCollisionPointRec(mousePos, TOPRIGHT)) {
                dragging = true;
                edgeRect = &TOPRIGHT;
                offset.x = mousePos.x - TOPRIGHT.x;
                offset.y = mousePos.y - TOPRIGHT.y;
          }
          else if (CheckCollisionPointRec(mousePos, TOPLEFT)) {
                dragging = true;
                edgeRect = &TOPLEFT;
                offset.x = mousePos.x - TOPLEFT.x;
                offset.y = mousePos.y - TOPLEFT.y;
          }
          else if (CheckCollisionPointRec(mousePos, BTMLEFT)) {
                dragging = true;
                edgeRect = &BTMLEFT;
                offset.x = mousePos.x - BTMLEFT.x;
                offset.y = mousePos.y - BTMLEFT.y;
          }
          else if (CheckCollisionPointRec(mousePos, BTMRIGHT)) {
                dragging = true;
                edgeRect = &BTMRIGHT;
                offset.x = mousePos.x - BTMRIGHT.x;
                offset.y = mousePos.y - BTMRIGHT.y;
          }
       }

      // Stop dragging if the mouse button is released
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
          dragging = false;
      }

      // Update rectangle position if dragging
      if (dragging) {
          edgeRect->x = mousePos.x;
          edgeRect->y = mousePos.y;
          nn_interpolate(&img, texture.width + offset.x, texture.height + offset.y);
          texture = LoadTextureFromImage(img);
        }

      EndGrid();
    EndDrawing();
  }
}
