/* 
 * File:   Octree.cpp
 * Author: Christopher Kozuch
 * Created on November 15, 2014, 3:15 PM
 */

#include "Octree.hpp"
using namespace std;

Octree::Octree(Extrema* extremes, float edge_length, float tolerance) 
{
  this_tolerance = tolerance;
  Hex* top_hex = new Hex(extremes,this);
  vector<HexVertex*> top_vertices = top_hex->get_child_vertices();
  for(vector<HexVertex*>::iterator iter = top_vertices.begin(); iter != top_vertices.end(); ++iter)
    this->add_vertex(*iter);
  this_top_hex = top_hex;
  
  float sum = 0;
  float avg_dist;
  vector<Edge*> edges = this_top_hex->get_child_edges();
  for(vector<Edge*>::iterator iter = edges.begin(); iter != edges.end(); ++iter)
    sum += (*iter)->get_length();
  avg_dist = sum/edges.size();
  if(avg_dist > edge_length)
    this_top_hex->split_hex(edge_length);
  else
    this_base_hexes.insert(this_top_hex);
}

Octree::Octree(const Octree& orig) {}
Octree::~Octree() {}

Hex* Octree::get_top_hex(){return this_top_hex;}
set<Hex*> Octree::get_base_hexes(){return this_base_hexes;}

void Octree::add_base_hex(Hex* new_base_hex)
{
  this_base_hexes.insert(new_base_hex);
  new_base_hex->set_base_hex();
  new_base_hex->set_adjacent_hexes();
}

float Octree::get_average_edge_length()
{
  vector<Edge*> curr_edges;
  float sum = 0;
  float num_edges = 0;
  for(set<Hex*>::iterator iter1 = this_base_hexes.begin(); iter1 != this_base_hexes.end(); ++iter1)
  {
    curr_edges.clear();
    curr_edges = (*iter1)->get_child_edges();
    for(vector<Edge*>::iterator iter2 = curr_edges.begin(); iter2 != curr_edges.end(); ++iter2)
      sum += (*iter2)->get_length();
    num_edges += curr_edges.size();
  }
  return sum/num_edges;
}

set<Hex*> Octree::get_solid_hexes(Surface* surface, bool write)
{
  if(this_solid_hexes.empty())
  {
    this->assign_facets(surface);
    if((this_top_hex->get_child_facets()).size() != surface->get_num_facets())
    {
      cout << "The number of facets in the top hex does not equal the number of facets in the surface!\n";
      cout << "\tNumber of facets in the surface: " << surface->get_num_facets() << "\n";
      cout << "\tNumber of facets in the top hex: " << (this_top_hex->get_child_facets()).size() << "\n";
    }
    
    int counter = 0;
    clock_t start = clock();
    int split = 5;
    int marker = split;
    float curr_time;

    int num_base_hexes = (this->get_base_hexes()).size();
    for(set<Hex*>::iterator iter1 = this_base_hexes.begin(); iter1 != this_base_hexes.end(); ++iter1)
    {
      if((*iter1)->is_solid())
        this_solid_hexes.insert(*iter1);

      counter++;
      curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
      if(curr_time > marker)
      {
        cout << round(curr_time) << " seconds ellapsed; " << round((counter/(float)num_base_hexes)*100) << "% of hexes analyzed.\n";
        marker += split;
      }
    }
    
    if(write)
    {
      counter = 0;
      start = clock();
      split = 5;
      marker = split;

      const char* write_file = "solid_hexes.txt";
      cout << "Writing solid hexes to: '" << write_file << "'\n";
      ofstream out_file(write_file);
      if(out_file.fail())
        cout << "Could not open " << write_file << " for writing. Process terminated.\n";
      else
      {
        out_file << surface->get_file_name() << "\n";
        out_file << "Total number of solid hexes: "  << this_solid_hexes.size() << "\n";
        out_file << "Hex\tCenterX\tCenterY\tCenterZ\tXmin\tYmin\tZmin\tXmax\tYmax\tZmax\n\n"; 
      }

      int num_solid_hexes = this_solid_hexes.size();
      for(set<Hex*>::iterator iter2 = this_solid_hexes.begin(); iter2 != this_solid_hexes.end(); ++iter2)
      {
        out_file << counter+1 << "\t"
                 << (*iter2)->get_center()->get_x() << "\t" << (*iter2)->get_center()->get_y() << "\t" << (*iter2)->get_center()->get_z()
                 << "\t" << (*iter2)->get_extrema()->get_xmin() << "\t" << (*iter2)->get_extrema()->get_ymin() << "\t" << (*iter2)->get_extrema()->get_zmin()
                 << "\t" << (*iter2)->get_extrema()->get_xmax() << "\t" << (*iter2)->get_extrema()->get_ymax() << "\t" << (*iter2)->get_extrema()->get_zmax()
                 << "\n";

        counter++;
        curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
        if(curr_time > marker)
        {
          cout << round(curr_time) << " seconds ellapsed; " << round((counter/(float)num_solid_hexes)*100) << "% of solid hexes written.\n";
          marker += split;
        }
      }
      cout << "Writing complete.\n\n";
      out_file.close();
    }
  }
  return this_solid_hexes;
}

void Octree::assign_facets(Surface* surface)
{
  set<Facet*> facets = surface->get_facets();
  size_t assignment_counter = 0;
  
  clock_t start = clock();
  int split = 5;
  int marker = split;
  float curr_time;
  
  Hex* curr_hex;
  vector<Hex*> curr_child_hexes;
    
  for(set<Facet*>::iterator iter1 = facets.begin(); iter1 != facets.end(); ++iter1)
  {
    if(!this_top_hex->is_inside((*iter1)->get_center()))
      cout << "Facet " << *iter1 << " is not inside the top hex!";   
    curr_hex = this_top_hex;
    
    while(!curr_hex->is_base_hex())
    {
      curr_child_hexes.clear();
      curr_child_hexes = curr_hex->get_child_hexes();
      for(vector<Hex*>::iterator iter2 = curr_child_hexes.begin(); iter2 != curr_child_hexes.end(); ++iter2)
      {
        if((*iter2)->is_inside((*iter1)->get_center()))
        {
          curr_hex = (*iter2);
          break;
        }
      }
    }
    
    (curr_hex)->add_child_facet(*iter1);
    (*iter1)->add_parent_hex(curr_hex);
    assignment_counter++;
    
    curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
    if(curr_time > marker)
    {
      cout << round(curr_time) << " seconds ellapsed; " << round(assignment_counter/((float)(facets.size()))*100) << "% of facets assigned.\n";
      marker += split;
    }
  }
  
  if(assignment_counter < surface->get_num_facets())
    cout << "Not all facets were assigned to a hex!\n";
}

void Octree::add_vertex(HexVertex* vertex)
{
  this_child_vertices.insert(vertex);
}

//this function will be slower the first time it is called
vector<vector<float> > Octree::get_major_axis(bool verbose)
{
  if(this_major_axis.empty())
  {
    //get a point on the line
    if(verbose) cout << "\tCalculating average vertex...";
    float avg[3];
    this->get_avg(avg);
    vector<float> junk;
    junk.empty();
    this_major_axis.push_back(junk);
    for(size_t iter1 = 0; iter1 < 3; iter1++)
      this_major_axis[0].push_back(avg[iter1]);
    
    //get the vector direction
    if(verbose) cout << "done.\n\tCalculating A'*A...";
    this_coor_mat = this->get_coor_mat();
    vector<vector<float> > A;
    A = mult_transpose(this_coor_mat);
    
    if(verbose) cout << "done.\n\tCalculating eigenvalues...";
    vector<float> eigen_vals = calc_eigen_vals(A);
    for(unsigned int i = 0; i < 3; i++)
      A[i][i] = A[i][i] - eigen_vals[i];
    
    if(verbose) cout << "done.\n\tCalculating eigenvectors...";
    float X[3];
    solve(A,X);
    float mag = sqrt(pow(X[0],2) + pow(X[1],2) + pow(X[2],2));
    
    this_major_axis.push_back(junk);
    for(size_t iter2 = 0; iter2 < 3; iter2++)
      this_major_axis[1].push_back(-X[iter2]/mag);
    if(verbose) cout << "done.\n";
  }
  return this_major_axis;
}

//returns average of all points
void Octree::get_avg(float average[3])
{
  float xsum = 0;
  float ysum = 0;
  float zsum = 0;
  
  for(set<Hex*>::iterator iter = this_solid_hexes.begin(); iter != this_solid_hexes.end(); ++iter)
  {
    xsum += (*iter)->get_center()->get_x();
    ysum += (*iter)->get_center()->get_y();
    zsum += (*iter)->get_center()->get_z();
  }
  
  size_t total = this_solid_hexes.size();
  average[0] = xsum/total;
  average[1] = ysum/total;
  average[2] = zsum/total;
}

//gets a nx3 matrix with x-y-z coordinates of all solid hex centers
vector<vector<float> > Octree::get_coor_mat()
{
  if(this_coor_mat.empty())
  {
    vector<float> vx, vy, vz;
    for(set<Hex*>::iterator iter = this_solid_hexes.begin(); iter != this_solid_hexes.end(); ++iter)
    {
      vx.push_back((*iter)->get_center()->get_x());
      vy.push_back((*iter)->get_center()->get_y());
      vz.push_back((*iter)->get_center()->get_z());
    }
    
    this_coor_mat.push_back(vx);
    this_coor_mat.push_back(vy);
    this_coor_mat.push_back(vz);
  }
  return this_coor_mat;
}

//multiplies the transpose of the input matrix against itself
vector<vector<float> > Octree::mult_transpose(vector<vector<float> > A)
{
  vector<vector<float> > B;
  size_t m = A.size();
  size_t n = (A[0]).size();
  vector<float> junk;
  junk.clear();
  float temp;
  
  for(size_t i = 0; i < m; i++)
  {
    B.push_back(junk);
    for(size_t j= 0; j < m; j++)
    {
      temp = 0;
      for(size_t k = 0; k < n; k++)
      {
        temp += ((A[i])[k])*((A[j])[k]);
      }
      (B[i]).push_back(temp);
    }
  }
  
  return B;
}

vector<float> Octree::calc_eigen_vals(vector<vector<float> > A)
{
  float a = 1;
  float b = (-1)*(A[0][0] + A[1][1] + A[2][2]);
  
  float temp1[3];
  temp1[0] = (A[1][1]*A[2][2])-(A[1][2]*A[2][1]);
  temp1[1] = (A[1][0]*A[2][2])-(A[1][2]*A[2][0]);
  temp1[2] = (A[1][0]*A[2][1])-(A[1][1]*A[2][0]);
  float c = temp1[0] + temp1[1] + temp1[2];
  
  float temp2[3];
  temp2[0] = A[0][0]*((A[1][1]*A[2][2])-(A[1][2]*A[2][1]));
  temp2[1] = A[0][1]*((A[1][0]*A[2][2])-(A[1][2]*A[2][0]));
  temp2[2] = A[0][2]*((A[1][0]*A[2][1])-(A[1][1]*A[2][0]));
  float d = (-1)*(temp2[0] - temp2[1] + temp2[2]);
 
  //cout << "P(x) = " << a << "x^3 + " << b << "x^2 + " << c << "x + " << d << "\n";
  //find roots of P(x) = a*x^3 + b*x^2 + c*x + d
  vector<float> eigen_vals = cubic_root(a,b,c,d);
  
  return eigen_vals;
}

//newton-raphson method
vector<float> Octree::cubic_root(float a, float b, float c, float d)
{
  float e = this_tolerance*abs(d);
  vector<float> roots; 
  float r, rold; 
  float max = numeric_limits<float>::max();

  for(int n = (-1); n < 2; n++) 
  { 
    r = n*max;
    while (abs(a*pow(r,3) + b*pow(r,2) + c*r + d) > e) //loop until precision fits error
    {
      rold = r; 
      r = rold - ((a*pow(rold,3) + b*pow(rold,2) + c*rold + d)/(3*a*pow(rold,2) + 2*b*rold + c)); 
    }
    roots.push_back(r); 
  }
  return roots;
}

void Octree::solve(vector<vector<float> > A, float X[3])
{
  float l[3][3]; 
  float u[3][3]; 
  float W[3] = {1.0,1.0,1.0};

  decomposition(A,3,l,u); //decompose matrix A
  substitution (l,u,W,3,X);
}

void Octree::decomposition(vector<vector<float> > A, unsigned c, float L[3][3], float U[3][3])
{
  //delcare and initialize intermediate to zero
  float factor = 0;

  //Create matrix with elements of L and U
  for (unsigned k = 0; k<(c-1); k++)
  {
    for (unsigned i = k+1; i<c; i++)
    {
      factor = A[i][k]/A[k][k];
      A[i][k] = factor;

      for (unsigned j = (k+1); j<c; j++)
      {
          A[i][j] = A[i][j] - factor*A[k][j];
      }
    }
  }

  //Enter L and U elements into L and U matrices
  for (unsigned m = 0; m<c; m++)
  {
    for (unsigned n = 0; n<c; n++)
    {
      if (m == n)
      {
        L[m][n] = 1;
        U[m][n] = A[m][n];
      }
      else if (m>n)
      {
        L[m][n] = A[m][n];
        U[m][n] = 0;
      }
      else if (n>m)
      {
        L[m][n] = 0;
        U[m][n] = A[m][n];
      }
    }
  }
}

void Octree::substitution (float l[3][3], float u[3][3], float b[3], unsigned c, float x[3])
{
  //forward subsitution
  float y[3]; 
  float sum[2]; 
  y[0] = b[0];
  sum[0] = l[1][0]*y[0];

  for (unsigned n = 1; n<c; n++)
  {
    y[n] = b[n] - sum[n-1];
    if (n == 1)
        sum[1] = (l[2][0]*y[0]) + (l[2][1]*y[1]);
  }

  //backwards substitution
  x[2] = y[2]/u[2][2];
  for (int n = c-2; n>=0; n--)
    x[n] = (y[n] - (u[n][n+1]*(x[n+1]))-((abs((float)(n-1))))*(u[n][n+2]*(x[n+2])))/(u[n][n]);
}

int Octree::get_num_vertices()
{
  return this_child_vertices.size();
}

void Octree::write_gmsh_file()
{
  const char* gmsh_filename = "test.gmsh";
  ofstream fid(gmsh_filename);
  fid << "$MeshFormat\n2.2 0 8\n$EndMeshFormat\n";
  
  int m = 3;
  int node_num = this->get_num_vertices();
  fid << "$Nodes\n" << node_num << "\n";
  
  int j = 1;
  for(set<HexVertex*>::iterator iter = this_child_vertices.begin(); iter != this_child_vertices.end(); ++iter)
  {
    fid << j << " ";
    (*iter)->set_id(j);
    j++;
    fid << ((*iter)->get_x()) << " ";
    fid << ((*iter)->get_y()) << " ";
    fid << ((*iter)->get_z()) << " ";
    fid << "\n";
  }
  fid << "$EndNodes\n";
 
  int element_order = 8;
  int element_num = (this_solid_hexes.size());
  fid << "$Elements\n" << element_num << "\n";
          
  vector<HexVertex*> curr_vertices;
  //int order[] = {1,5,4,2,2,7,3,4,2,7,6,5,5,8,4,7,5,2,7,4};
  int order[] = {4,1,2,3,8,5,6,7};
  int i = 1;
  for(set<Hex*>::iterator iter = this_solid_hexes.begin(); iter != this_solid_hexes.end(); ++iter)
  {
    fid << i << " 5 2 0 " << i << " ";
    curr_vertices.clear();
    curr_vertices = (*iter)->get_child_vertices();
    for(int j=0; j<element_order; ++j)
      fid << curr_vertices[order[j]-1]->get_id() << " ";
    fid << "\n";
    i++;
  }
  fid << "$EndElements\n";
  
  fid.close();
}

void Octree::write_inp_file()
{
  const char* inp_filename = "test.inp";
  ofstream fid(inp_filename);
  fid << "*Heading\n** Job name: Job-1 Model name: Model-1\n";
  fid << "** Generated by: MeshSTL\n";
  fid << "*Preprint, echo=NO, model=NO, history=NO, contact=NO\n";
  fid << "**\n** PARTS\n**\n*Part, name=Part-1\n";
  //fid << "**\n**\n** ASSEMBLY\n**\n*Assembly, name=Assembly\n";
  //fid << "**\n*Instance, name=Part-1-1, part=Part-1\n";
  
  fid << "*Node\n";
  int m = 3;
  int node_num = this->get_num_vertices();
  
  int j = 1;
  for(set<HexVertex*>::iterator iter = this_child_vertices.begin(); iter != this_child_vertices.end(); ++iter)
  {
    fid << "\t" << j << ",\t";
    (*iter)->set_id(j);
    j++;
    fid << ((*iter)->get_x()) << ",\t";
    fid << ((*iter)->get_y()) << ",\t";
    fid << ((*iter)->get_z());
    fid << "\n";
  }
 
  int element_order = 8;
  int element_num = (this_solid_hexes.size());
  fid << "*Element, type=C3D8R\n";
          
  vector<HexVertex*> curr_vertices;
  //int order[] = {1,5,4,2,2,7,3,4,2,7,6,5,5,8,4,7,5,2,7,4};
  int order[] = {4,1,2,3,8,5,6,7};
  int i = 1;
  for(set<Hex*>::iterator iter = this_solid_hexes.begin(); iter != this_solid_hexes.end(); ++iter)
  {
    fid << i << ",\t";
    curr_vertices.clear();
    curr_vertices = (*iter)->get_child_vertices();
    for(int j=0; j < element_order; ++j)
    {
      fid << curr_vertices[order[j]-1]->get_id();
      if(j < element_order - 1)
        fid << ",\t";
    }
    fid << "\n";
    i++;
  }
  
  fid << "*Nset, nset=Set-1, generate\n";
  fid << "  1,\t" << node_num << ",\t1\n";
  fid << "*Elset, elset=Set-1, generate\n";
  fid << " 1,\t" << element_num << ",\t1\n";
  fid << "** Section: Section-1-SET-1\n";
  fid << "*Solid Section, elset=Set-1, material=Material-1\n";
  fid << ",\n";
  fid << "*End Part\n";
  fid << "**\n**\n** ASSEMBLY\n**\n*Assembly, name=Assembly\n";
  fid << "**\n*Instance, name=Part-1-1, part=Part-1\n*End Instance\n";
  fid << "**\n*End Assembly\n";
  fid << "**\n** MATERIALS\n**\n*Material, name=Material-1\n";
  fid << "*Density\n8000.,\n";
  fid.close();
}

