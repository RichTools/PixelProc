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

#endif // UI_IMPLEMENTATION

