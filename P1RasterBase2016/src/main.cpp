/* Release code for program 1 CPE 471 Fall 2016 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int g_width, g_height;

/*
   Helper function you will want all quarter
   Given a vector of shapes which has already been read from an obj file
   resize all vertices to the range [-1, 1]
 */
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
   float minX, minY, minZ;
   float maxX, maxY, maxZ;
   float scaleX, scaleY, scaleZ;
   float shiftX, shiftY, shiftZ;
   float epsilon = 0.001;

   minX = minY = minZ = 1.1754E+38F;
   maxX = maxY = maxZ = -1.1754E+38F;

   //Go through all vertices to determine min and max of each dimension
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
         if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];

         if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
         if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];

         if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
         if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
      }
   }

	//From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
      maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
      maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
      maxExtent = zExtent;
   }
   scaleX = 2.0 /maxExtent;
   shiftX = minX + (xExtent/ 2.0);
   scaleY = 2.0 / maxExtent;
   shiftY = minY + (yExtent / 2.0);
   scaleZ = 2.0/ maxExtent;
   shiftZ = minZ + (zExtent)/2.0;

   //Go through all verticies shift and scale them
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
         assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
         assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
         assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
      }
   }
}

float area(int x1, int y1, int x2, int y2, int x3, int y3)
{
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

bool inside_tri(unsigned int x, unsigned int y){
    float A = area (0, 25, 100, 25, 50, 75);
    float A1 = area (x, y, 100, 25, 50, 75);
    float A2 = area (0, 25, x, y, 50, 75);
    float A3 = area (0, 25, 100, 25, x, y);
    return (A == A1 + A2 + A3);
}

bool isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{  
   /* Calculate area of triangle ABC */
   float A = area (x1, y1, x2, y2, x3, y3);
  
   /* Calculate area of triangle PBC */ 
   float A1 = area (x, y, x2, y2, x3, y3);
  
   /* Calculate area of triangle PAC */ 
   float A2 = area (x1, y1, x, y, x3, y3);
  
   /* Calculate area of triangle PAB */  
   float A3 = area (x1, y1, x2, y2, x, y);
    
   /* Check if sum of A1, A2 and A3 is same as A */
   return (A == A1 + A2 + A3);
}


int main(int argc, char **argv)
{
//    	if(argc < 3) {
//          cout << "Usage: Assignment1 meshfile imagefile" << endl;
//          return 0;
//       }
//    	// OBJ filename
//    	string meshName(argv[1]);
//    	string imgName(argv[2]);
    
   //  string meshName("/Users/hanktsai/Documents/CSC471/P1RasterBase2016/resources/tri.obj");
   string meshName("/Users/hanktsai/Documents/CSC471/P1RasterBase2016/resources/bunny.obj");
   string imgName("/Users/hanktsai/Documents/CSC471/P1RasterBase2016/out.png");
    
    //set g_width and g_height appropriately!
    g_width = g_height = 1500;
    
    //create an image
    auto image = make_shared<Image>(g_width, g_height);
    
    // triangle buffer
    vector<unsigned int> triBuf;
    // position buffer
    vector<float> posBuf;
    vector<float> zBuf; 
    // Some obj files contain material information.
    // We'll ignore them for this assignment.
    vector<tinyobj::shape_t> shapes; // geometry
    vector<tinyobj::material_t> objMaterials; // material
    string errStr;
    
    bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());
    /* error checking on read */
    if(!rc) {
        cerr << errStr << endl;
    } else {
        //keep this code to resize your object to be within -1 -> 1
        resize_obj(shapes);
        posBuf = shapes[0].mesh.positions;
        triBuf = shapes[0].mesh.indices;
        zBuf.resize(g_height * g_height); 
        zBuf.assign(zBuf.size(), -2); 
    }
    cout << "Number of vertices: " << posBuf.size()/3 << endl;
    cout << "Number of triangles: " << triBuf.size()/3 << endl;
    
    //TODO add code to iterate through each triangle and rasterize it
    for ( int i = 0; i < triBuf.size(); i+=3 ) {
      int iA = triBuf[i + 0]; 
      int iB = triBuf[i + 1]; 
      int iC = triBuf[i + 2]; 
      float Ax = (posBuf[iA * 3 + 0] + 1) * g_width / 2; 
      float Ay = (posBuf[iA * 3 + 1] + 1) * g_width / 2; 
      float Bx = (posBuf[iB * 3 + 0] + 1) * g_width / 2; 
      float By = (posBuf[iB * 3 + 1] + 1) * g_width / 2;
      float Cx = (posBuf[iC * 3 + 0] + 1) * g_width / 2; 
      float Cy = (posBuf[iC * 3 + 1] + 1) * g_width / 2;
      int minX = min(Ax, min(Bx, Cx));
      int maxX = max(Ax, max(Bx, Cx)); 
      int minY = min(Ay, min(By, Cy));
      int maxY = max(Ay, max(By, Cy)); 
      float AT = area(Ax, Ay, Bx, By, Cx, Cy);
      for(int w = minX; w < maxX; w++ ){ 
         for (int h = minY; h < maxY; h++ ){ 
             if(isInside(Ax, Ay, Bx, By, Cx, Cy, w, h)){
                 float Apbc = area(w, h, Bx, By, Cx, Cy);
                 float Apac = area(Ax, Ay, w, h, Cx, Cy);
                 float Apba = area(Ax, Ay, Bx, By, w, h);
                 float ap = Apbc / AT;
                 float bp = Apac / AT;
                 float cp = Apba / AT;
                 float z = ap * posBuf[3 * iA + 2] + bp * posBuf[3 * iB + 2] + cp * posBuf[3 * iC + 2];
                 float zs = (z + 1) * 127;
                 if (zBuf[w + h * g_width] < z){
                     zBuf[w + h * g_width] = z;
                     image -> setPixel(w, h, zs, 0, 0);
                 }
                
               
//                image -> setPixel(w, h, z, 0, 0);
            }
         }
      } 

    }
    //    for (size_t i = 0; i < shapes.size(); i++) {
    //        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
    //            shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] + 1) * 50;
    //            shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] + 1) * 50;
    //            unsigned int x = shapes[i].mesh.positions[3*v+0];
    //            unsigned int y = shapes[i].mesh.positions[3*v+1];
    //            if (inside_tri(x, y)){
    //                image -> setPixel(x, y, 255, 255, 0);
    //            }
    //        }
    //    }
    
   //  for (int i = 0 ; i < posBuf.size(); i++ ){
   //      posBuf[i] = (posBuf[i] + 1) * 50;
   //  }
    
   //  float AT = area(0, 25, 50, 75, 100, 25);
   //  for (int i = 0; i < g_width; i++ ){
   //      for (int j = 0; j < g_height; j++ ) {
   //          if (inside_tri(i, j)){
   //              float Apbc = area(i, j, 50, 75, 100, 25);
   //              float Apac = area(0, 25, i, j, 100, 25);
   //              float Apba = area(0, 25, 50, 75, i, j);
   //              float ap = Apbc / AT;
   //              float bp = Apac / AT;
   //              float cp = Apba / AT;
   //              float rp = 255 * ap + 0 * bp + 0 * cp;
   //              float gp = 0 * ap + 0 * bp + 255 * cp;
   //              float blp = 0 * ap + 255 * bp + 0 * cp;
   //              image -> setPixel(i, j, rp, gp, blp);
   //          }
   //      }
   //  }


    

    
	//write out the image
   image->writeToFile(imgName);

	return 0;
}
