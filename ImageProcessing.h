#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

typedef unsigned char PIXELS;

PIXELS* zeros(int width, int height, int depth);

PIXELS* getPixelData(Image *img);
PIXELS* rgb2gray(Image* img);
PIXELS* quantise(Image* img, int levels);
PIXELS* flip_img(Image* img, bool horizontal);
PIXELS* nn_interpolate(Image* img, int width, int height);

#endif

#ifdef IMAGE_PROCESSING_IMPLEMENTATION

PIXELS* zeros(int width, int height, int depth)
{
  int size = width * width * depth;
  PIXELS* pxs = (PIXELS*)malloc(sizeof(PIXELS) * size);
  for (int i = 0; i < size; ++i)
  {
    pxs[i] = 0;
  }
  return pxs;
}

PIXELS* getPixelData(Image *img) 
{
    return (unsigned char*)img->data; // Cast to the appropriate format
}

PIXELS* rgb2gray(Image* img)
{
  int width = img->width;
  int height = img->height;
  PIXELS* pixels = getPixelData(img);

  // 4 = R & G & B & A
  for (int i = 0; i < 4*(width*height)-4; i+= 4)
  {
    int r = pixels[i];
    int g = pixels[i+1];
    int b = pixels[i+2];

    int gray = (0.3*r) + (0.59*g) + (0.11*b);

    pixels[i] = gray;
    pixels[i+1] = gray;
    pixels[i+2] = gray;
  }
  img->data = pixels;
  return pixels;
}

PIXELS* quantise(Image* img, int levels)
{
  
  int width = img->width;
  int height = img->height;
  int channels = 4;
  PIXELS* pixels = getPixelData(img);
  
  int QInt = 255 / levels - 1;
  for (int i = 0; i < channels*(width*height); ++i)
  {
    unsigned char intensityValue = pixels[i];
    unsigned char newValue = (unsigned char)floor((intensityValue / QInt) * QInt + (QInt/2));
    pixels[i] = newValue;
  }
  img->data = pixels;
  return pixels;
}

PIXELS* flip_img(Image* img, bool horizontal)
{
  int width = img->width;
  int height = img->height;
  int channels = 4;
  PIXELS* buffer = zeros(width, height, channels);
  PIXELS* pixels = getPixelData(img);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int srcIndex = (y * width + x) * channels;
      int destIndex;
      if (horizontal) {
        // Flip horizontally
        destIndex = (y * width + (width - 1 - x)) * channels;
      } else {
        // Flip vertically
        destIndex = ((height - 1 - y) * width + x) * channels;
      }

      // Copy RGBA channels
      buffer[destIndex + 0] = pixels[srcIndex + 0]; // R
      buffer[destIndex + 1] = pixels[srcIndex + 1]; // G
      buffer[destIndex + 2] = pixels[srcIndex + 2]; // B
      buffer[destIndex + 3] = pixels[srcIndex + 3]; // A 
    }
  }

  img->data = buffer;
  return pixels;
}


PIXELS* nn_interpolate(Image* img, int width, int height)
{
  int channels = 4;
  PIXELS* buffer = zeros(width, height, channels);
  PIXELS* pixels = getPixelData(img);

  int old_width = img->width;
  int old_height = img->height;

  float ratio_x = (float)width/old_width;
  float ratio_y = (float)height/old_height;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      // Find nearest neighbor in the original image
      int src_x = (int)round(x / ratio_x);
      int src_y = (int)round(y / ratio_y);

      // Ensure coordinates are within bounds
      src_x = src_x < old_width ? src_x : old_width - 1;
      src_y = src_y < old_height ? src_y : old_height - 1;
 

      // Copy pixel data
      for (int c = 0; c < channels; c++) {
        buffer[(y * width + x) * channels + c] = pixels[(src_y * old_width + src_x) * channels + c];
      }

    }
  }
  img->data = buffer;
  img->width = width;
  img->height = height;
  return buffer;
}


#endif
