#ifndef _UI_H
#define _UI_H

#define text_width(text) strlen(text)

typedef struct {
    int columns;
    int rows;
    int cellWidth;
    int cellHeight;
int marginX;
    int marginY;
    int spacingX;
    int spacingY;
    int currentCellIndex;
} Grid;

Grid grid;

bool DrawButton(char* text, int y);

void BeginGrid(int columns, int rows, int cellWidth, int cellHeight, int marginX, int marginY, int spacingX, int spacingY);
void DrawImageEdgePoints(int SCREEN_WIDTH, int SCREEN_HEIGHT, Texture2D texture, int EDGE_SIZE);
Vector2 getNextGridPosition();
void EndGrid(); 

#endif // _UI_H 

#ifdef UI_IMPLEMENTATION

const int EDGE_SIZE = 5;

bool DrawButton(char* text, int y)
{
  Vector2 mousePoint = GetMousePosition();
  bool buttonHovered = false;
  Color buttonColor = WHITE;
  
  Rectangle btn = { 20, y, 200, 50 };
  // Check if mouse is hovering over the button
  if (CheckCollisionPointRec(mousePoint, btn))
  {
      buttonHovered = true;
      buttonColor = GRAY;

      // If clicked
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
      {
        return true;
      }
  }
  else
  {
      buttonHovered = false;
      buttonColor = WHITE;
  }
  
  // 12ish pixels per character
  float padding = (btn.width - (text_width(text)*12))/2;

  DrawRectangleRec(btn, buttonColor);
  DrawText(text, btn.x + padding, btn.y + 15, 20, buttonHovered ? WHITE : BLACK);
  return false;
}

void BeginGrid(int columns, int rows, int cellWidth, int cellHeight, int marginX, int marginY, int spacingX, int spacingY) {
    grid.columns = columns;
    grid.rows = rows;
    grid.cellWidth = cellWidth;
    grid.cellHeight = cellHeight;
    grid.marginX = marginX;
    grid.marginY = marginY;
    grid.spacingX = spacingX;
    grid.spacingY = spacingY;
    grid.currentCellIndex = 0;  // Start at first cell
}


Vector2 getNextGridPosition() 
{
    int currentColumn = grid.currentCellIndex % grid.columns;
    int currentRow = grid.currentCellIndex / grid.columns;
    
    float x = grid.marginX + (currentColumn * (grid.cellWidth + grid.spacingX));
    float y = grid.marginY + (currentRow * (grid.cellHeight + grid.spacingY));
    
    grid.currentCellIndex++;  // Move to next cell

    return (Vector2){x, y};
}

void EndGrid() 
{
    grid.currentCellIndex = 0;  // Reset grid index for next usage
}


void DrawImageEdgePoints(int SCREEN_WIDTH, int SCREEN_HEIGHT, Texture2D texture, int EDGE_SIZE)
{
}

Vector2 GetScaledDimensions(Texture2D texture, Rectangle bounds) {
    float scale;
    Vector2 newDimensions = {0};
    
    // Calculate scale ratios for both width and height
    float scaleX = bounds.width / texture.width;
    float scaleY = bounds.height / texture.height;
    
    // Use the smaller scale to ensure texture fits within bounds
    scale = (scaleX < scaleY) ? scaleX : scaleY;
    
    // Calculate new dimensions maintaining aspect ratio
    newDimensions.x = texture.width * scale;
    newDimensions.y = texture.height * scale;
    
    return newDimensions;
}

void DrawTextureInBox(Texture2D texture, Rectangle bounds, Color tint) {
    Vector2 dimensions = GetScaledDimensions(texture, bounds);
    
    // Calculate position to center the texture in the bounds
    float x = bounds.x + (bounds.width - dimensions.x) * 0.5f;
    float y = bounds.y + (bounds.height - dimensions.y) * 0.5f;
    
    // Draw the scaled texture
    Rectangle source = (Rectangle){ 0, 0, texture.width, texture.height };
    Rectangle dest = (Rectangle){ x, y, dimensions.x, dimensions.y };
    DrawTexturePro(texture, source, dest, (Vector2){0, 0}, 0.0f, tint);
}


#endif // UI_IMPLEMENTATION

