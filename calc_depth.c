/*
    Computing a Displacement Map
*/

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) 
{
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement)
            / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;
}

/* Helper function to return the square euclidean distance between two values. */
unsigned int square_euclidean_distance(unsigned char a, unsigned char b) 
{
    return (unsigned int) (a - b) * (a - b);
}

/* See if pixel is on edge of image */
bool is_edge(int x, int y, int feature_width, int feature_height, int image_width, int image_height)
{
	int x_left = x - feature_width;
	int x_right = x + feature_width;
	int y_top = y + feature_height;
	int y_bottom = y - feature_height;
	if (x_left < 0 || x_right >= image_width || y_top >= image_height || y_bottom < 0) 
    {
		return true;
	} 
    return false;
}

unsigned char calc_non_edge_depth(int x, int y, unsigned char *left, unsigned char *right,
    int feature_width, int feature_height, int image_width, int image_height ,int maximum_displacement)
{
    float min_squared_distance = -1;
    int min_dy = 0;
    int min_dx = 0;

    for(int dy = -maximum_displacement; dy <= maximum_displacement; dy++)
    {
        for(int dx = -maximum_displacement; dx <= maximum_displacement; dx++)
        {
            if (!is_edge(x + dx, y + dy, feature_width, feature_height, image_width, image_height))
            {
                float squared_distance = 0;
                for (int patch_Y = -feature_height; patch_Y <= feature_height; patch_Y++)
                {
                    for (int patch_X = -feature_width; patch_X <= feature_width; patch_X++)
                    {
                        int left_X = x + patch_X;
                        int left_Y = y + patch_Y;
                        int right_X = x + dx + patch_X;
                        int right_Y = y + dy + patch_Y;
                        squared_distance += square_euclidean_distance(left[left_Y * image_width + left_X], right[right_Y * image_width + right_X]);
                    }
                }
                bool euclidean_distance_tie = (min_squared_distance == squared_distance) && (normalized_displacement(dx, dy, maximum_displacement) < normalized_displacement(min_dx, min_dy, maximum_displacement));
                if ((min_squared_distance == -1) || (min_squared_distance > squared_distance) || euclidean_distance_tie)
                {
                    min_squared_distance = squared_distance;
                    min_dx = dx;
                    min_dy = dy;
                }
            }
        }
    }
    unsigned char depth = (min_squared_distance == -1) ? 0 : normalized_displacement(min_dx, min_dy, maximum_displacement);
    return depth;
}

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) 
{
   if (maximum_displacement == 0) 
   {
        for(int y = 0; y < image_height; y++) 
        {
            for(int x = 0; x < image_width; x++) 
            {
                depth_map[y * image_width + x] = 0;
            }
        }
    }
    else {
        for(int y = 0; y < image_height; y++)
        {
            for(int x = 0; x < image_width; x++)
            {
                if(!is_edge(x, y, feature_width, feature_height, image_width, image_height))
                {
                    unsigned char depth = calc_non_edge_depth(x, y, left, right, feature_width, feature_height, 
                            image_width, image_height, maximum_displacement);
                    depth_map[y * image_width + x] = depth;
                }
                else
                {
                    depth_map[y * image_width + x] = 0;
                }
            }
        }
    }
}
