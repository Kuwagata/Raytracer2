#include "PhotonMap.h"
#include <iostream>
#include <fstream>
 
// "spot" is the location at which we need to determine illumination
// normal is not needed for now
// "max_dist" is the maximum distance that a photon can be from the
//    passed-in "spot" to contribute to illumination
// "num_photons" is the number of photons that we will use to 
//    approximate illumination.

Color PhotonMap::illumination (const Point &spot, const Point &normal,
                     double max_dist, unsigned int num_photons) const
{
   double squareMaxDist = SQUARE(max_dist);

   //old
   //MaxHeap* clsPhotons = kdtree->findNearestN(num_photons, squareMaxDist, spot);
   MaxHeap* clsPhotons = kdtree->findNearestN(num_photons, max_dist, spot);   

   if(clsPhotons->get_size() < 8)
   {
      delete clsPhotons;
      clsPhotons = NULL;
      return Color::black();
   }
   
   Color colorSum = clsPhotons->sumColors();
   
   //old
   //colorSum /= M_PI * clsPhotons->findMax().distance;
   colorSum /= (4 * M_PI * max_dist * max_dist);
   delete clsPhotons;
   clsPhotons = NULL;
   
   return colorSum;
}

/*
 *  Send out photons from a light source and paint the intersected objects
 */
void PhotonMap::trace_photons ()
{
   Point max ( 1.0,  1.0,  1.0);
   Point min (-1.0, -1.0, -1.0);
   
   // Grab current light and set photon starting point to light's location
   ObjList::ObjIter iter = scene.light_iterator();
   Light *cur_lt = (Light *)*iter;
   Point sp = cur_lt->location();

   // Get light's ambient color
   Color color = cur_lt->get_ambient(sp);
   color *= (100.0/TOTAL_PHOTONS);// *1000.0; /* Multiply by 10 or 100 (or 1000) to make visible - temporary */

   // Create set of photons to fire
   Photon** genPhotons = new Photon*[TOTAL_PHOTONS];
   
   // Added to keep track of traced photons to ensure even distribution
   std::ofstream photon_log;
   photon_log.open("../logs/photon.log");

   // Trace photons until they hit something
   unsigned int i = 0;
   while(i < (TOTAL_PHOTONS))
   {
      // Create an offset point between max and min, defined above
      Point point;
      for (int j = 0; j < 3; j++)
      {
         point[j] = (rand() / (double) RAND_MAX) * (max[j] - min[j]) + min[j];
      }

      // Add starting point to offset point to establish a vector
      Point dp = sp + point;
      
      // Get intersected object
      intersection intersected = scene.first_object(sp, dp);
      
      // Trace path through objects
      if(trace(intersected, sp, color, genPhotons, i))
      {
         // output to log
         photon_log << genPhotons[i]->loc << std::endl;
         i++;
      }
   }
   photon_log.close();
   
   // Insert photons into tree
   kdtree->insert(genPhotons, 0, TOTAL_PHOTONS-1, 1);
}

bool PhotonMap::trace(intersection inter, Point sp, Color photon_color, Photon** &genPhotons, unsigned int &index)
{
   // hp => hit point, grab normal to hit point
   Point hp = inter.spot;
   Point norm = inter.obj->get_normal(hp);
   
   if(inter.obj->allows_lighting())
   {
      Color light_rflc = inter.obj->reflectivity();
      double reflective = (light_rflc[0] + light_rflc[1] + light_rflc[2]) / 3;
      double rand_num = rand() / (double) RAND_MAX;
      double refractive = inter.obj->refractivity();

      if(rand_num <= reflective)
      {
         /*reflect*/
         Point rev = sp - hp;
         rev.normalize();
         Point dp = rev.bounce(norm);
         dp += hp;
         
         intersection new_inter = scene.first_object(hp, dp);
         
         return trace(new_inter, hp, photon_color, genPhotons, index);
      }
      else
      {
         if(((rand_num - reflective) / (1 - reflective)) <= refractive)
         {
            /*refract*/
            Point rev = hp - sp;
            rev.normalize();
            double refIndex = inter.obj->refractionindex();
            Point bp = rev.bend(norm, refIndex);
            
            Point dp = hp + bp;
            
            intersection new_inter = scene.first_object(hp, dp);
               
            return trace(new_inter, hp, photon_color, genPhotons, index);
         }
         else
         {
            if(((rand_num - reflective - refractive) / (1 - reflective - refractive)) <= 0.5)
            {
               /*scatter*/
               Point rand_point;
               Color new_color = photon_color * inter.obj->get_diffuse(hp);  /*Mind that this can be get_ambient too*/
               
               for (int j = 0; j < 3; j++)
               {
                  rand_point[j] = rand() / (double) RAND_MAX;
               }
               
               Point dp = hp + rand_point;
               Point unitized = dp - hp;
               unitized.normalize();
               
               if(unitized.dot_prod(norm) < 0)
               {
                  dp[0] = - dp[0];
                  dp[1] = - dp[1];
                  dp[2] = - dp[2];
               }
               
               intersection new_inter = scene.first_object(hp, dp);
               
               return trace(new_inter, hp, new_color, genPhotons, index);
            }
            else
            {
               /*absorb*/
               genPhotons[index] = new Photon(photon_color, hp);
               return true;
            }
         }
      }
   }
   else
   {
      return false;
   }
}

PhotonMap::~PhotonMap()
{
   if(kdtree != NULL)
   {
      delete kdtree;
      kdtree = NULL;
      return;
   }
}
