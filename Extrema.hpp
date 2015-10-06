/* 
 * File:   Extrema.hpp
 * Author: Christopher Kozuch
 * Created on August 22, 2014, 3:28 PM
 */

#ifndef EXTREMA_HPP
#define	EXTREMA_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <cstring>
using namespace std;

#include "Facet.hpp"
#include "FacetVertex.hpp"

const static float INF = numeric_limits<float>::infinity();

class Extrema {
public:
  Extrema(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  Extrema(vector<float> x, vector<float> y, vector<float> z);
  Extrema(float input_extrema[][3]);
  Extrema(vector<vector<float> > input_extrema);
  
  Extrema(const Extrema& orig);
  Extrema();
  virtual ~Extrema();

  float get_xmax();
  float get_ymax();
  float get_zmax();

  float get_xmin();
  float get_ymin();
  float get_zmin();
  
  vector<vector<float> > get_vector();
  void get_array(float output_array[][3]); //writes this_extrema_array to output_array
  
  void set_xmax(float xmax);
  void set_ymax(float ymax);
  void set_zmax(float zmax);

  void set_xmin(float xmin);
  void set_ymin(float ymin);
  void set_zmin(float zmin);
  
  void set(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  void set(float input_extrema[][3]);
    
private:
  //first index is 0 for min, 1 for max
  //second index is 0 for x, 1 for y, 2 for z
  float this_extrema_array[2][3];
};

#endif	/* EXTREMA_HPP */
