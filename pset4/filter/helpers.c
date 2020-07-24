#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int avg_color(int blue, int green, int red);
RGBTRIPLE blur_pixels(int size, RGBTRIPLE *image);
RGBTRIPLE sobel_pixel(RGBTRIPLE arr[3][3]);
int sobel(int arr[3][3]);
float convolution(int m, int n, int arr1[m][n], int arr2[m][n]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over each row
    for (int i = 0; i < height; i++)
    {
        // loop over each row pixel
        for (int j = 0; j < width; j++)
        {
            // calculate average pixel value
            int new_rgbt = avg_color(image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed);
            // assign new pixel value
            image[i][j].rgbtBlue = new_rgbt;
            image[i][j].rgbtGreen = new_rgbt;
            image[i][j].rgbtRed = new_rgbt;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // reflected row placeholder
    RGBTRIPLE ref_row[width];
    // loop over each row
    for (int i = 0; i < height; i++)
    {
        // loop over each row pixel and reverse image
        for (int j = width - 1, k = 0; j >= 0; j--, k++)
        {
            ref_row[k] = image[i][j];
        }
        // assign new pixel value
        for (int l = 0; l < width; l++)
        {
            image[i][l] = ref_row[l];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width];
    RGBTRIPLE *pixel = NULL;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // upper left corner
            if (i == 0 && j == 0)
            {
                pixel = malloc(4 * sizeof(RGBTRIPLE));
                pixel[0] = image[i][j], pixel[1] = image[i][j + 1];
                pixel[2] = image[i + 1][j], pixel[3] = image[i + 1][j + 1];
                blurred[i][j] = blur_pixels(4, pixel);
                free(pixel);
            }
            // upper right corner
            else if (i == 0 && j == width - 1)
            {
                pixel = malloc(4 * sizeof(RGBTRIPLE));
                pixel[0] = image[i][j - 1], pixel[1] = image[i][j];
                pixel[2] = image[i + 1][j - 1], pixel[3] = image[i + 1][j];
                blurred[i][j] = blur_pixels(4, pixel);
                free(pixel);
            }
            // lower left corner
            else if (i == height - 1 && j == 0)
            {
                pixel = malloc(4 * sizeof(RGBTRIPLE));
                pixel[0] = image[i - 1][j], pixel[1] = image[i - 1][j + 1];
                pixel[2] = image[i][j], pixel[3] = image[i][j+  1];
                blurred[i][j] = blur_pixels(4, pixel);
                free(pixel);
            }
            // lower right corner
            else if (i == height-1 && j == width - 1)
            {
                pixel = malloc(4 * sizeof(RGBTRIPLE));
                pixel[0] = image[i - 1][j - 1], pixel[1] = image[i - 1][j];
                pixel[2] = image[i][j - 1], pixel[3] = image[i][j];
                blurred[i][j] = blur_pixels(4, pixel);
                free(pixel);
            }
            // first row
            else if (i == 0)
            {
                pixel = malloc(6 * sizeof(RGBTRIPLE));
                pixel[0] = image[i][j - 1], pixel[1] = image[i][j], pixel[2] = image[i][j + 1];
                pixel[3] = image[i + 1][j - 1], pixel[4] = image[i + 1][j], pixel[5] = image[i + 1][j + 1];
                blurred[i][j] = blur_pixels(6, pixel);
                free(pixel);
            }
            // last row
            else if (i == height - 1)
            {
                pixel = malloc(6 * sizeof(RGBTRIPLE));
                pixel[0] = image[i - 1][j - 1], pixel[1] = image[i - 1][j], pixel[2] = image[i - 1][j + 1];
                pixel[3] = image[i][j - 1], pixel[4] = image[i][j], pixel[5] = image[i][j + 1];
                blurred[i][j] = blur_pixels(6, pixel);
                free(pixel);
            }
            // first pixel
            else if (j == 0)
            {
                pixel = malloc(6 * sizeof(RGBTRIPLE));
                pixel[0] = image[i - 1][j], pixel[1] = image[i - 1][j + 1], pixel[2] = image[i][j];
                pixel[3] = image[i][j + 1], pixel[4] = image[i + 1][j], pixel[5] = image[i + 1][j + 1];
                blurred[i][j] = blur_pixels(6, pixel);
                free(pixel);
            }
            // last pixel
            else if (j == width - 1)
            {
                pixel = malloc(6 * sizeof(RGBTRIPLE));
                pixel[0] = image[i - 1][j - 1], pixel[1] = image[i - 1][j], pixel[2] = image[i][j - 1];
                pixel[3] = image[i][j], pixel[4] = image[i + 1][j - 1], pixel[5] = image[i + 1][j];
                blurred[i][j] = blur_pixels(6, pixel);
                free(pixel);
            }
            // 3x3 submatrix
            else
            {
                pixel = malloc(9 * sizeof(RGBTRIPLE));
                pixel[0] = image[i - 1][j - 1], pixel[1] = image[i - 1][j], pixel[2] = image[i - 1][j + 1];
                pixel[3] = image[i][j - 1], pixel[4] = image[i][j], pixel[5] = image[i][j + 1];
                pixel[6] = image[i + 1][j - 1], pixel[7] = image[i + 1][j], pixel[8] = image[i + 1][j + 1];
                blurred[i][j] = blur_pixels(9, pixel);
                free(pixel);
            }
        }
    }

    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l] = blurred[k][l];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edged[height][width];
    RGBTRIPLE solid_black = {0, 0, 0};
    RGBTRIPLE neighbors[3][3];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // upper left corner
            if (i == 0 && j == 0)
            {
                neighbors[0][0] = solid_black, neighbors[0][1] = solid_black, neighbors[0][2] = solid_black;
                neighbors[1][0] = solid_black, neighbors[1][1] = image[i][j], neighbors[1][2] = image[i][j + 1];
                neighbors[2][0] = solid_black, neighbors[2][1] = image[i + 1][j], neighbors[2][2] = image[i + 1][j + 1];
                edged[i][j] = sobel_pixel(neighbors);
            }
            // upper right corner
            else if (i == 0 && j == width - 1)
            {
                neighbors[0][0] = solid_black, neighbors[0][1] = solid_black, neighbors[0][2] = solid_black;
                neighbors[1][0] = image[i][j - 1], neighbors[1][1] = image[i][j], neighbors[1][2] = solid_black;
                neighbors[2][0] = image[i + 1][j - 1], neighbors[2][1] = image[i + 1][j], neighbors[2][2] = solid_black;
                edged[i][j] = sobel_pixel(neighbors);
            }
            // lower left corner
            else if (i == height - 1 && j == 0)
            {
                neighbors[0][0] = solid_black, neighbors[0][1] = image[i - 1][j], neighbors[0][2] = image[i - 1][j + 1];
                neighbors[1][0] = solid_black, neighbors[1][1] = image[i][j], neighbors[1][2] = image[i][j + 1];
                neighbors[2][0] = solid_black, neighbors[2][1] = solid_black, neighbors[2][2] = solid_black;
                edged[i][j] = sobel_pixel(neighbors);
            }
            // lower right corner
            else if (i == height - 1 && j == width - 1)
            {
                neighbors[0][0] = image[i - 1][j - 1], neighbors[0][1] = image[i - 1][j], neighbors[0][2] = solid_black;
                neighbors[1][0] = image[i][j - 1], neighbors[1][1] = image[i][j], neighbors[1][2] = solid_black;
                neighbors[2][0] = solid_black, neighbors[2][1] = solid_black, neighbors[2][2] = solid_black;
                edged[i][j] = sobel_pixel(neighbors);
            }
            // first row
            else if (i == 0)
            {
                neighbors[0][0] = solid_black, neighbors[0][1] = solid_black, neighbors[0][2] = solid_black;
                neighbors[1][0] = image[i][j - 1], neighbors[1][1] = image[i][j], neighbors[1][2] = image[i][j + 1];
                neighbors[2][0] = image[i + 1][j - 1], neighbors[2][1] = image[i + 1][j], neighbors[2][2] = image[i + 1][j + 1];
                edged[i][j] = sobel_pixel(neighbors);
            }
            // last row
            else if (i == height - 1)
            {
                neighbors[0][0] = image[i - 1][j - 1], neighbors[0][1] = image[i - 1][j], neighbors[0][2] = image[i - 1][j + 1];
                neighbors[1][0] = image[i][j - 1], neighbors[1][1] = image[i][j], neighbors[1][2] = image[i][j + 1];
                neighbors[2][0] = solid_black, neighbors[2][1] = solid_black, neighbors[2][2] = solid_black;
                edged[i][j] = sobel_pixel(neighbors);
            }
            // first pixel
            else if (j == 0)
            {
                neighbors[0][0] = solid_black, neighbors[0][1] = image[i - 1][j], neighbors[0][2] = image[i - 1][j + 1];
                neighbors[1][0] = solid_black, neighbors[1][1] = image[i][j], neighbors[1][2] = image[i][j + 1];
                neighbors[2][0] = solid_black, neighbors[2][1] = image[i + 1][j], neighbors[2][2] = image[i + 1][j + 1];
                edged[i][j] = sobel_pixel(neighbors);
            }
            // last pixel
            else if (j == width - 1)
            {
                neighbors[0][0] = image[i - 1][j - 1], neighbors[0][1] = image[i - 1][j], neighbors[0][2] = solid_black;
                neighbors[1][0] = image[i][j - 1], neighbors[1][1] = image[i][j], neighbors[1][2] = solid_black;
                neighbors[2][0] = image[i + 1][j - 1], neighbors[2][1] = image[i + 1][j], neighbors[2][2] = solid_black;
                edged[i][j] = sobel_pixel(neighbors);
            }
            // 3x3 submatrix
            else
            {
                neighbors[0][0] = image[i - 1][j - 1], neighbors[0][1] = image[i - 1][j], neighbors[0][2] = image[i - 1][j + 1];
                neighbors[1][0] = image[i][j - 1], neighbors[1][1] = image[i][j], neighbors[1][2] = image[i][j + 1];
                neighbors[2][0] = image[i + 1][j - 1], neighbors[2][1] = image[i + 1][j], neighbors[2][2] = image[i + 1][j + 1];
                edged[i][j] = sobel_pixel(neighbors);
            }
        }
    }
    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l] = edged[k][l];
        }
    }
    return;
}

// calculates average pixel from blue, green, and red inputs
int avg_color(int blue, int green, int red)
{
    // float avg
    float avg = ((float) blue + green + red) / 3;
    // rounded avg
    int ravg = round(avg);
    return ravg;
}

// calculate box blur
RGBTRIPLE blur_pixels(int size, RGBTRIPLE *image)
{
    int n = size;
    float avg_blue = 0;
    float avg_green = 0;
    float avg_red = 0;

    for (int i = 0; i < n; i++)
    {
        avg_blue += (float) image[i].rgbtBlue / n;
        avg_green += (float) image[i].rgbtGreen / n;
        avg_red += (float) image[i].rgbtRed / n;
    }
    int ravg_blue = round(avg_blue);
    int ravg_green = round(avg_green);
    int ravg_red = round(avg_red);
    RGBTRIPLE pixel = {ravg_blue, ravg_green, ravg_red};
    return pixel;
}

// calculates sobel filter for each color of a given pixel
RGBTRIPLE sobel_pixel(RGBTRIPLE arr[3][3])
{
    // separate colors from RGBTRIPLE into separate 3x3 matrices
    int blue[3][3], green[3][3], red[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            blue[i][j] = arr[i][j].rgbtBlue;
            green[i][j] = arr[i][j].rgbtGreen;
            red[i][j] = arr[i][j].rgbtRed;
        }
    }
    return (RGBTRIPLE)
    {
        sobel(blue), sobel(green), sobel(red)
    };
}

// calculate sobel filter of single color value
int sobel(int arr[3][3])
{
    // define kernels
    int gx_kern[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy_kern[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // calculate gx
    float gx = convolution(3, 3, gx_kern, arr);
    // caluclate gy
    float gy = convolution(3, 3, gy_kern, arr);
    // calculate sobel
    int sobel = (int) round(sqrtf(powf(gx, 2) + powf(gy, 2)));
    // cap value at 255
    if (sobel > 255)
    {
        return 255;
    }
    else
    {
        return sobel;
    }
}

// kernel convolution
float convolution(int m, int n, int arr1[m][n], int arr2[m][n])
{
    float sum = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            sum += arr1[i][j] * arr2[i][j];
        }
    }
    return sum;
}