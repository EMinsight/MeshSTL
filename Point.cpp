/* 
 * File:   Point.cpp
 * Author: Christopher Kozuch
 * Created on December 23, 2014, 1:58 PM
 */

#include "Point.hpp"

Point::Point(float x, float y, float z) 
{
  this_coordinates.push_back(x);
  this_coordinates.push_back(y);
  this_coordinates.push_back(z);
}

Point::Point(vector<float> coordinates){this_coordinates = coordinates;}

Point::Point(const Point& orig) {}
Point::~Point() {}

vector<float> Point::get_coordinates(){return this_coordinates;}
float Point::get_x(){return this_coordinates[0];}
float Point::get_y(){return this_coordinates[1];}
float Point::get_z(){return this_coordinates[2];}

void Point::set_coordinates(vector<float> coordinates){this_coordinates = coordinates;}
void Point::set_x(float x){this_coordinates[0] = x;}
void Point::set_y(float y){this_coordinates[1] = y;}
void Point::set_z(float z){this_coordinates[2] = z;}

