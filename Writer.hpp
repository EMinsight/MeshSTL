/* 
 * File:   Writer.hpp
 * Author: Chris
 *
 * Created on October 16, 2014, 10:36 PM
 */

#ifndef WRITER_HPP
#define	WRITER_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdint.h>
using namespace std;

#include "Facet.hpp"
#include "FacetVertex.hpp"
#include "Surface.hpp"
#include "Hex.hpp"
#include "Writer.hpp"

class Writer
{
public:
  Writer();
  Writer(const Writer& orig);
  virtual ~Writer();
  
  bool write_STL(const char* filename, Surface* surface, bool isBinary); //STL output file handler
  bool write_STL_dialog(Surface* surface); //handler for user specified STL output filename and format
  bool write_XYZ(const char* filename, Surface* surface); //writes XYZ data for vertices to text file (tab delimited)
  bool write_XYZ_dialog(Surface* surface); //handler for user specified XYZ data output filename and format
  
private:

};

#endif	/* WRITER_HPP */

