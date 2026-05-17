#include "vector_utils.h"

int dist_between_points_sqrt(int a_x,int a_y, int b_x, int b_y){
  int y_dist = b_y - a_y;
  int x_dist = b_x - a_x;
  return x_dist*x_dist + y_dist*y_dist;
}
