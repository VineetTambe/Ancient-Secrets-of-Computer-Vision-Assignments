#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
	x = ((x < im.w) ? ( (x <= 0) ? 0 : x) : im.w-1);
	y = ((y < im.h) ? ( (y <= 0) ? 0 : y) : im.h-1);
	c = ((c < im.c) ? ( (c <= 0) ? 0 : c) : im.c-1);

    return im.data[c * im.w * im.h + y * im.w + x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
	assert((x < im.w) && (x >= 0));
    assert((y < im.h) && (y >= 0));
    assert((c < im.c) && (x >= 0));

	im.data[c*im.w*im.h + y*im.w + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, (im.c * im.h * im.w) * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in    
    float rFactor = 0.299; 
    float gFactor = 0.587;
    float bFactor = 0.114;
    for (int h=0; h<im.h; h++){
        for (int w=0; w<im.w; w++){
            float R = rFactor * get_pixel(im, w, h, 0); // R
            float G = gFactor * get_pixel(im, w, h, 1); // G
            float B = bFactor * get_pixel(im, w, h, 2); // B
            float v_gray = R + G + B;
            set_pixel(gray, w, h, 0, v_gray);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int h=0; h<im.h; h++)
        for (int w=0; w<im.w; w++)
            set_pixel(im, w, h, c, v + get_pixel(im, w, h, c));
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int c=0; c<im.c; c++)
        for (int h=0; h<im.h; h++)
            for (int w=0; w<im.w; w++){
                float pixelValue = get_pixel(im, w, h, c);
                if( (pixelValue >= 0.0) && (pixelValue <= 1.0)) continue;
                pixelValue = ((pixelValue >= 0.0)) ? 1.0 : 0.0;
                set_pixel(im, w, h, c, pixelValue);
            }
                
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float R, G, B, H, S, V, C;
    for (int h = 0;  h <im.h; h++){
        for (int w=0; w<im.w; w++){
            R = get_pixel(im, w, h, 0);
            G = get_pixel(im, w, h, 1);
            B = get_pixel(im, w, h, 2);

            V = three_way_max( R, G, B);
            S = 0.0;
            C = V - three_way_min( R, G, B);
            if (V > 0)
                S = C / V;
            
            float H_hatch = 0;
            if (C == 0) {
               H = 0; 
            } else {
                if (V == R) H_hatch = (G - B) / C;
                else if (V == G) H_hatch = (B - R) / C + 2;
                else if (V == B) H_hatch = (R - G) / C + 4;
                H = H_hatch / 6.;
                if (H_hatch < 0) H = H + 1;
            }
            
            set_pixel(im, w, h, 0, H);
            set_pixel(im, w, h, 1, S);
            set_pixel(im, w, h, 2, V);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in

    // TODO Fill this in
    float R, G, B;
    for (int h=0; h<im.h; h++){
        for (int w=0; w<im.w; w++){
            float H = get_pixel(im, w, h, 0); 
            float S = get_pixel(im, w, h, 1); 
            float V = get_pixel(im, w, h, 2);

            float C = S * V;
            float m = V - C;

            float H_ = H * 6.;
            float X = C * (1 - fabs(fmod(H_, 2) - 1)); 

            if (0. <= H_ && H_ <=1.){
                R = C;
                G = X;
                B = 0;
            } else if(1. <= H_ && H_ <= 2.){
                R = X;
                G = C;
                B = 0;
            } else if(2. <= H_ && H_ <= 3.){
                R = 0;
                G = C;
                B = X;
            } else if(3. <= H_ && H_ <= 4.){
                R = 0;
                G = X;
                B = C;
            } else if(4. <= H_ && H_ <= 5.){
                R = X;
                G = 0;
                B = C;
            } else if(5. <= H_ && H_ <= 6.){
                R = C;
                G = 0;
                B = X;
            } else {
                R = 0;
                G = 0;
                B = 0;
            }
            R = R + m;
            G = G + m;
            B = B + m;
            set_pixel(im, w, h, 0, R);
            set_pixel(im, w, h, 1, G);
            set_pixel(im, w, h, 2, B);

        }
    }
}
