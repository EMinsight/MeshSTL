/* 
 * File:   Extrema.cpp
 * Author: Christopher Kozuch
 * Created on August 22, 2014, 3:29 PM
 */

#include "Extrema.hpp"
using namespace std;

Extrema::Extrema(){}

Extrema::Extrema(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) 
{
  this_extrema_array[0][0] = xmin;
  this_extrema_array[0][1] = ymin;
  this_extrema_array[0][2] = zmin;
  
  this_extrema_array[1][0] = xmax;
  this_extrema_array[1][1] = ymax;
  this_extrema_array[1][2] = zmax;
}

Extrema::Extrema(float input_extrema[][3])
{
  memcpy(this_extrema_array, input_extrema, sizeof(input_extrema));
}

Extrema::Extrema(vector<vector<float> > input_extrema)
{
  for(size_t iter1 = 0; iter1 < input_extrema.size(); iter1++)
    for(size_t iter2 = 0; iter2 < (input_extrema[iter1]).size(); iter2++)
      this_extrema_array[iter1][iter2] = input_extrema[iter1][iter2];
}

Extrema::Extrema(vector<float> x, vector<float> y, vector<float> z)
{
  this_extrema_array[1][0] = x[0];
  this_extrema_array[1][1] = y[0];
  this_extrema_array[1][2] = z[0];
  
  this_extrema_array[0][0] = x[0];
  this_extrema_array[0][1] = y[0];
  this_extrema_array[0][2] = z[0];

  for(size_t iter = 0; iter < x.size(); iter++)
  {
    if(x[iter] > this_extrema_array[1][0])
      this_extrema_array[1][0] = x[iter];
    if(x[iter] < this_extrema_array[0][0])
      this_extrema_array[0][0] = x[iter];
  }

  for(size_t iter = 0; iter < y.size(); iter++)
  {
    if(y[iter] > this_extrema_array[1][1])
      this_extrema_array[1][1] = y[iter];
    if(y[iter] < this_extrema_array[0][1])
      this_extrema_array[0][1] = y[iter];
  }

  for(size_t iter = 0; iter < z.size(); iter++)
  {
    if(z[iter] > this_extrema_array[1][2])
      this_extrema_array[1][2] = z[iter];
    if(z[iter] < this_extrema_array[0][2])
      this_extrema_array[0][2] = z[iter];
  }
}

float Extrema::get_xmax(){return this_extrema_array[1][0];}
float Extrema::get_ymax(){return this_extrema_array[1][1];}
float Extrema::get_zmax(){return this_extrema_array[1][2];}

float Extrema::get_xmin(){return this_extrema_array[0][0];}
float Extrema::get_ymin(){return this_extrema_array[0][1];}
float Extrema::get_zmin(){return this_extrema_array[0][2];}

vector<vector<float> > Extrema::get_vector()
{
  vector<vector<float> > temp;
  for(size_t iter1 = 0; iter1 < sizeof(this_extrema_array); iter1++)
    for(size_t iter2 = 0; iter2 < sizeof(this_extrema_array[iter1]); iter2++)
      temp[iter1][iter2] = this_extrema_array[iter1][iter2];
  
  return temp;
}

void Extrema::get_array(float output_array[][3])
{
  memcpy(this_extrema_array, output_array, sizeof(output_array));
}

void Extrema::set_xmax(float xmax){this_extrema_array[1][0] = xmax;}
void Extrema::set_ymax(float ymax){this_extrema_array[1][1] = ymax;}
void Extrema::set_zmax(float zmax){this_extrema_array[1][2] = zmax;}

void Extrema::set_xmin(float xmin){this_extrema_array[0][0] = xmin;}
void Extrema::set_ymin(float ymin){this_extrema_array[0][1] = ymin;}
void Extrema::set_zmin(float zmin){this_extrema_array[0][2] = zmin;}

void Extrema::set(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
  this->set_xmin(xmin);
  this->set_xmax(xmax);
  
  this->set_ymin(ymin);
  this->set_ymax(ymax);
  
  this->set_zmin(zmin);
  this->set_zmax(zmax);
}

void Extrema::set(float input_extrema[][3])
{
  memcpy(this_extrema_array, input_extrema, sizeof(input_extrema));
}

Extrema::Extrema(const Extrema& orig) {}

Extrema::~Extrema(){}



