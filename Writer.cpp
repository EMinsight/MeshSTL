/* 
 * File:   Writer.cpp
 * Author: Christopher Kozuch
 * Created on October 16, 2014, 10:36 PM
 */

#include "Writer.hpp"
using namespace std;

Writer::Writer() {}
Writer::Writer(const Writer& orig) {}
Writer::~Writer() {}

bool Writer::write_XYZ_dialog(Surface* surface)
{
  string response;
  cout << "Would you like to write the xyz data to a file? (y/n): ";
  cin >> response;
  bool good_response = false;

  do
  {
    if(response == "y")
    {
      good_response = true; 

      char write_file[80];
      cout << "Enter the name of the file you want to write: ";
      cin >> write_file;
    
      bool status = this->write_XYZ(write_file, surface);
      if(!status)
      {
        cout << "Could not write to the file.\n";
        return false;
      }
      else
        cout << "Writing complete.\n";
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

bool Writer::write_STL_dialog(Surface* surface)
{
  string response;
  cout << "Would you like to write the elements to a STL file? (y/n): ";
  cin >> response;
  bool good_response1 = false;

  do
  {
    if(response == "y")
    {
      good_response1 = true;
      bool good_response2 = false;

      bool is_binary;
      do
      {
        cout << "Type 'b' for binary format or 'a' for ASCII format: ";
        cin >> response;
        if(response == "b")
        {
    	  is_binary = true;
          good_response2 = true;
        }

        else if (response == "a")
        {
          is_binary = false;
          good_response2 = true;
        }

        else
          cout << "You must enter a 'b' or an 'a'. Please try again: ";
      }while(!good_response2); 

      char write_file[80];
      cout << "Enter the name of the file you want to write: ";
      cin >> write_file;
    
      bool status = this->write_STL(write_file,surface,is_binary);
      if(!status)
      {
        cout << "Could not write to the file.\n";
        exit(1);
      }
      else
        cout << "Writing complete.\n";
      
    }

    else if(response == "n")
      good_response1 = true;

    else
    {
      cout << "You must enter a 'y' or an 'n'. Please try again: ";
      cin >> response;
    }
  }while(!good_response1);
  return true;
}

bool Writer::write_STL(const char* filename, Surface* surface, bool isBinary)
{
  string f = string(filename);

  int last_index = f.find_last_of(".");
  string name = f.substr(0,last_index); 
  unsigned int num_facets = surface->get_num_facets();
  
  clock_t start = clock();
  float curr_time;
  float percent_complete; 
  int split = 5;
  int marker = split;
  int counter = 0;
  
  if(isBinary)
  {
    ofstream out_file(filename,ios::out | ios::binary);
    if(out_file.fail())
      return false;
    
    vector<float> normal;
    vector<float> vertex;
    vector<FacetVertex*> vertices;
    unsigned short data;
    
    out_file.setf(ios::left);
    out_file.width(sizeof(unsigned char)*80);
    string header = string(filename);
    out_file << header;
    out_file.write((char*)&num_facets,sizeof(num_facets));

    char spacer[2] = {'-','-'};  
    set<Facet*> facets = surface->get_facets();
    for(set<Facet*>::iterator iter1 = facets.begin(); iter1 != facets.end(); iter1++)
    {
      normal = (*iter1)->get_normal();
      for(size_t i = 0; i < 3; i++)
        out_file.write((char*)&(normal[i]),sizeof(float));

      normal.clear();
      vertices = (*iter1)->get_vertices();
      for(vector<FacetVertex*>::iterator iter2 = vertices.begin(); iter2 != vertices.end(); iter2++)
      {
        vertex = (*iter2)->get_coordinates();
        for(size_t j = 0; j < 3; j++)
          out_file.write((char*)&(vertex[j]),sizeof(float));        
        vertex.clear();
      }
      vertices.clear();
      out_file.write(spacer,sizeof(char[2]));
      
      counter++;
      percent_complete = ((float)counter/(float)num_facets)*100;
      curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
      if(curr_time > marker)
      {
        cout << round(curr_time) << " seconds elapsed; " << round(percent_complete) << "% of facets written.\n";
        marker += split;
      }
    }

    out_file.close();
    return true;
  }
  
  else
  {
    ofstream out_file(filename);
    if(out_file.fail())
      return false;
    vector<float> normal;
    vector<float> point;
    vector<FacetVertex*> vertices;
    set<Facet*> facets = surface->get_facets();
    
    out_file << "solid " << name << "\n\n";
    
    for(set<Facet*>::iterator iter1 = facets.begin(); iter1 != facets.end(); iter1++)
    {
      normal = (*iter1)->get_normal();
      out_file << "facet normal " << normal[0] << " " << normal[1] << " " << normal[2] << "\n";
      normal.clear();
      
      out_file << "outer loop\n";
      vertices = (*iter1)->get_vertices();
      for(vector<FacetVertex*>::iterator iter2 = vertices.begin(); iter2 != vertices.end(); iter2++)
      {
        point = (*iter2)->get_coordinates();
        out_file << "vertex " << point[0] << " " << point[1] << " " << point[2] << "\n";
        point.clear();
      }
      vertices.clear();

      out_file << "endloop\nendfacet\n";
      counter++;
      percent_complete = ((float)counter/(float)num_facets)*100;
      curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
      if(curr_time > marker)
      {
        cout << round(curr_time) << " seconds ellapsed; " << round(percent_complete) << "% of facets written.\n";
        marker += split;
      }
    }
    
    out_file << "\nendsolid " << name;
    out_file.close();
  }

  return true;
}

bool Writer::write_XYZ(const char* filename, Surface* surface)
{
  set<FacetVertex*> vertices = surface->get_vertices();
  float num_vertices = vertices.size();
  ofstream out_file(filename);
  if(out_file.fail())
    return false;
  
  float counter = 0.0;
  float percent_complete;
  int split = 5;
  int marker = split;
  clock_t start = clock();
  float curr_time;
  
  //out_file << "Number of unique vertices: " << vertices.size() << "\n";
  
  for(set<FacetVertex*>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
  {
    out_file << (*iter)->get_x() << "\t";
    out_file << (*iter)->get_y() << "\t";
    out_file << (*iter)->get_z() << "\n";
    
    counter += 3;
    percent_complete = (counter/num_vertices)*100;
    curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
    if(curr_time > marker)
    {
      cout << round(curr_time) << " seconds ellapsed; " << round(percent_complete) << "% of vertices written.\n";
      marker += split;
    }
  }
  
  out_file << "EOF";
  out_file.close();
  return true;
}
  

