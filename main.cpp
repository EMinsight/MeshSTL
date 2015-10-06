/* 
 * File:   main.cpp
 * Author: Christopher Kozuch
 * Created on July 27, 2014, 8:42 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdint.h>
using namespace std;

#include "Edge.hpp"
#include "Extrema.hpp"
#include "Facet.hpp"
#include "Hex.hpp"
#include "Octree.hpp"
#include "HexVertex.hpp"
#include "Surface.hpp"
#include "FacetVertex.hpp"
#include "Writer.hpp"

Surface* parse_STL(const char* filename, float tolerance); //STL input file handler
Surface* parse_binary_STL(const char* filename, float tolerance); //writes binary STL to structured memory
Surface* parse_ASCII_STL(const char* filename, float tolerance); //writes ASCII STL to structured memory
bool octree_dialog(Surface* surface, float num_hexes_per_edge, float tolerance, ofstream& file_stream); //handles users input about octree

int main(int argc, char** argv) 
{
  clock_t start = clock();
  float tolerance;
  char read_file[80];
  cout << "Enter the name of the file you want to read: ";
  cin >> read_file;
  //cout << "Enter the merge tolerance: ";
  //cin >> tolerance;

  tolerance = pow(10.0,-6.0);
  
  ifstream fin(read_file);
  if(!fin.good())
  {
    do
    {
      fin.close();
      fin.clear();
      cout << "The file could not be read. Make sure that the file name is correct and that the extension is included.\n";
      cout << "Please try again: ";
      cin >> read_file;
      cout << "The file name you entered: " << read_file << ".\n";
      fin.open(read_file,ifstream::in);
    }while(!fin.good());
  }
    
  float num_hexes_per_edge;
  cout << "Enter the number of hexes you would like per axis: ";
  cin >> num_hexes_per_edge;
  
  cout << "Converting '" << read_file << "' to elements...\n";
  Surface* curr_surface = parse_STL(read_file, tolerance);
  if(!curr_surface)
  {
    cout << "Conversion failed.\n";
    return 1;
  }
  else
    cout << "Conversion completed.\n\n";
  
  cout << "There are " << curr_surface->get_num_facets() << " facets in the surface.\n";
  const char* write_file = "data.txt";
  ofstream out_file(write_file);
  if(out_file.fail())
  {
    cout << "Could not open " << write_file << " for writing. Process terminated.\n";
    return 1;
  }

  out_file << "File name: " << read_file << "\n";
  int num_facets = curr_surface->get_num_facets();
  out_file << "A total of " << num_facets << " facet(s) were read." << "\n";
  int num_vertices = curr_surface->get_num_vertices();
  out_file << "A total of " << num_facets*3 << " vertice(s) were read." << "\n";
  out_file << "A total of " << num_vertices << " unique vertice(s) were saved." << "\n";

  //doesn't handle 2D
  Extrema* surface_extremes = curr_surface->get_extrema();
  out_file << "The x values range from " << surface_extremes->get_xmin() << " to " << surface_extremes->get_xmax() << " (" << surface_extremes->get_xmax() - surface_extremes->get_xmin() << ").\n";
  out_file << "The y values range from " << surface_extremes->get_ymin() << " to " << surface_extremes->get_ymax() << " (" << surface_extremes->get_ymax() - surface_extremes->get_ymin() << ").\n";
  out_file << "The z values range from " << surface_extremes->get_ymin() << " to " << surface_extremes->get_zmax() << " (" << surface_extremes->get_zmax() - surface_extremes->get_zmin() << ").\n"; 

  cout << "The x values range from " << surface_extremes->get_xmin() << " to " << surface_extremes->get_xmax() << " (" << surface_extremes->get_xmax() - surface_extremes->get_xmin() << ").\n";
  cout << "The y values range from " << surface_extremes->get_ymin() << " to " << surface_extremes->get_ymax() << " (" << surface_extremes->get_ymax() - surface_extremes->get_ymin() << ").\n";
  cout << "The z values range from " << surface_extremes->get_ymin() << " to " << surface_extremes->get_zmax() << " (" << surface_extremes->get_zmax() - surface_extremes->get_zmin() << ").\n\n"; 

  /*Writer* curr_writer = new Writer();
  curr_writer->write_STL_dialog(curr_surface);*/
  
  bool status = octree_dialog(curr_surface, num_hexes_per_edge, tolerance, out_file);
  if(!status)
  {
    cout << "Failed to construct octree!\n";
    return 1;
  }

  float final_time = (clock() - start)/(float) CLOCKS_PER_SEC;
  out_file << "Total ellapsed time: " << final_time << "\nEOF";
  out_file.close();
  
  cout << "The data was written to '" << write_file << "'.\n";
  cout << "Total ellapsed time: " << final_time << " seconds\n";
  
  return 0;
}

Surface* parse_STL(const char *filename, float tolerance)
{
  string test_string;
  bool is_binary_format;
  ifstream fin(filename);

  //get third string and check if it is the "facet" label
  fin >> test_string;
  fin >> test_string;
  fin >> test_string;
  if(test_string == "facet")
    is_binary_format = false;
  else
    is_binary_format = true;
  fin.close();

  Surface* surface;
  if(is_binary_format)
  {
    cout << "The file type appears to be binary.\n";
    surface = parse_binary_STL(filename, tolerance);
  }
  else
  {
    cout << "The file type appears to be ASCII.\n";
    surface = parse_ASCII_STL(filename, tolerance);
  }
  return surface;
}

Surface* parse_binary_STL(const char *stl_file, float tolerance)
{
  Surface* surface = new Surface(tolerance, stl_file);
  FacetVertex* new_vertex;
  FacetVertex* old_vertex;

  float counter = 0;
  float percent_complete;
  clock_t start = clock();
  int split = 5;
  int marker = split;
  float curr_time;


  //The following code was adapted from a code written by Raveh Gonen
  //The code is available at: https://ravehgonen.wordpress.com/tag/stl-file-format/
  FILE *fin = fopen(stl_file, "rb");
  if (!fin) return NULL;
  char title[80];
  int num_facets;
  fread(title, 80, 1, fin);
  fread((void*)&num_facets, 4, 1, fin);
  cout << "The file header predicts " << num_facets << " facet(s).\n";
  float p[12]; // normal=3, vertices=3*3 = 12
  unsigned short spacer;
  // Every Facet is 50 Bytes: Normal(3*float), Vertices(9*float), 2 Bytes Spacer
  for (size_t i=0; i<num_facets; ++i)
  {
    for(size_t j=0; j<12; ++j)
    {
      fread((void*)&p[j], 4, 1, fin);
      //if(p[j] != p[j])
        //cout << "Read a NaN\n";
    }

    fread((void*)&spacer, 2, 1, fin); // spacer between successive Facets
    Facet* new_facet = new Facet(p[0], p[1], p[2]);
    
    for(size_t k=3; k<10; k += 3)
    {
      old_vertex = surface->is_in_surface(p[k],p[k+1],p[k+2]);
      if(old_vertex == NULL)
      {
    	new_vertex = new FacetVertex(p[k],p[k+1],p[k+2]);
        //cout << p[k] << "\t" << p[k+1] << p[k+2] << "\n";
        new_vertex->add_facet(new_facet);
        surface->add_unique_vertex(new_vertex);
        new_facet->add_vertex(new_vertex);
      }
      else
      {
      	new_facet->add_vertex(old_vertex);
        old_vertex->add_facet(new_facet);
      }
    }

    surface->add_facet(new_facet);

    counter++;
    percent_complete = (counter/num_facets)*100;
    curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
    if(curr_time > marker)
    {
      cout << marker << " seconds elapsed; " << round(percent_complete) << "% of facets read.\n";
      marker += split;
    }
  }
  fclose(fin);

  size_t facets_read = surface->get_num_facets();
  if(num_facets != facets_read)
    cout << "Not all predicted facets were saved.\n";

  return surface;
}

Surface* parse_ASCII_STL(const char *stl_file, float tolerance)
{
  FacetVertex* new_vertex;
  FacetVertex* old_vertex;
  float v[9];
  float n[3];
  Surface* surface = new Surface(tolerance,stl_file);

  int counter = 0;
  clock_t start = clock();
  int split = 5;
  int marker = split;
  float curr_time;

  ifstream fin(stl_file);
  if (!fin.good()) return NULL;
  string g, title;
  fin >> g >> title;

  while (!fin.eof())
  {
    fin >> g;                               // facet || endsolid
    if(g == "endsolid")
      break;

    fin >> g >> n[0] >> n[1] >> n[2];       // normal x y z
    fin >> g >> g;                          // outer loop
    fin >> g >> v[0] >> v[1] >> v[2];    // vertex x y z
    fin >> g >> v[3] >> v[4] >> v[5];    // vertex x y z
    fin >> g >> v[6] >> v[7] >> v[8];    // vertex x y z
    fin >> g;                               // endloop
    fin >> g;                               // endFacett

    Facet* new_facet = new Facet(n[0], n[1], n[2]);
    // Generate a new Triangle with Normal as 3 Vertices
    for(size_t k=0; k<7; k += 3)
    {
      old_vertex = surface->is_in_surface(v[k],v[k+1],v[k+2]);
      if(!old_vertex)
      {
    	new_vertex = new FacetVertex(v[k],v[k+1],v[k+2]);
        new_vertex->add_facet(new_facet);
        surface->add_unique_vertex(new_vertex);
        new_facet->add_vertex(new_vertex);
      }
      else
      {
        new_facet->add_vertex(old_vertex);
        old_vertex->add_facet(new_facet);
      }
    }

    surface->add_facet(new_facet);
    counter++;
    curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
    if(curr_time > marker)
    {
      cout << round(curr_time) << " seconds ellapsed; " << counter << "% of facets read.\n";
      marker += split;
    }
  }
  fin.close();

  return surface;
}

bool octree_dialog(Surface* surface, float num_hexes_per_edge, float tolerance, ofstream& file_stream)
{
  string response = "y";
  /*cout << "Would you like to create volumetric hexes? (y/n): ";
  cin >> response;*/
  bool good_response = false;
  Extrema* surface_extremes = surface->get_extrema();
  
  do
  {
    if(response == "y")
    { 
      good_response = true;
      float avg_edge_length = ((surface_extremes->get_xmax() - surface_extremes->get_xmin()) +
                              (surface_extremes->get_ymax() - surface_extremes->get_ymin()) +
                              (surface_extremes->get_zmax() - surface_extremes->get_zmin()))/3.0;
      float edge_length = avg_edge_length/num_hexes_per_edge;

      Octree* curr_octree = new Octree(surface_extremes, edge_length, tolerance);
      set<Hex*> base_hexes = curr_octree->get_base_hexes();
      cout << "Number of base hexes: " << base_hexes.size() << "\n";
      cout << "Average edge length: " << curr_octree->get_average_edge_length() << "\n";
      file_stream << "Number of base hexes: " << base_hexes.size() << "\n";
      file_stream << "Average edge length: " << curr_octree->get_average_edge_length() << "\n";
      
      set<Hex*> solid_hexes = curr_octree->get_solid_hexes(surface,false);
      float solid_fraction = solid_hexes.size()/(float(base_hexes.size()));
      cout << "Number of solid hexes: " << solid_hexes.size() << "\n";
      cout << "Solid fraction: " << solid_fraction << "\n";
      file_stream << "Number of solid hexes: " << solid_hexes.size() << "\n";
      file_stream << "Solid fraction: " << solid_fraction << "\n";
      
      /*bool verbose_bool = false;
      vector<vector<float> > major_axis = curr_octree->get_major_axis(verbose_bool);
      file_stream << "The major axis is: <" << major_axis[0][0] << "," << major_axis[0][1] << "," << major_axis[0][2] << "> + t*<" << major_axis[1][0] << "," << major_axis[1][1] << "," << major_axis[1][2] << ">\n"; 
      cout << "The major axis is: <" << major_axis[0][0] << "," << major_axis[0][1] << "," << major_axis[0][2] << "> + t*<" << major_axis[1][0] << "," << major_axis[1][1] << "," << major_axis[1][2] << ">\n"; 
      */
      
      curr_octree->write_inp_file();
    }
    
    else if(response == "n")
      good_response = true;
    else
    {
      cout << "You must enter a 'y' or an 'n'. Please try again.\n";
      cin >> response;
    }
  }while(!good_response);
  
  return true;
}





