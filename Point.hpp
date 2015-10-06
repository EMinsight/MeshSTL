/* 
 * File:   Point.hpp
 * Author: Christopher Kozuch
 * Created on December 23, 2014, 1:57 PM
 */

#ifndef POINT_HPP
#define	POINT_HPP

#include <vector>
using namespace std;

class Point
{
public:
  Point(float x, float y, float z);
  Point(vector<float> coordiantes);
  Point(const Point& orig);
  virtual ~Point();
  
  vector<float> get_coordinates();
  float get_x();
  float get_y();
  float get_z();
  
  void set_coordinates(vector<float> coordinates);
  void set_x(float x);
  void set_y(float y);
  void set_z(float z);
  
private:
  vector<float> this_coordinates;
};

#endif	/* POINT_HPP */

