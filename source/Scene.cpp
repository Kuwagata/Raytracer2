#include "Scene.h"
Scene::Scene (char *file) : eye(0.0, 1.0, 3.0) {
   std::ifstream in (file, std::ios::in);
   read_objects (in);
}

void Scene::read_objects (std::ifstream &in) {

   std::string type;
   int MAX_LENGTH = 100;
   char line[MAX_LENGTH];

   in >> type;
   while (!in.eof()) {

      if (type[0] == '#') {
         in.getline(line, MAX_LENGTH);
      } else if (type == "eye") {
         in >> eye;
      }else if (type == "ambient") {
         in >> type;
         in >> Color::AMB_WT;
      }else if (type == "distance") {
         in >> type;
         in >> Point::DIST_WT;
      } else if (type == "Sphere") {
         objects.add (new Sphere (in));
      } else if (type == "Light") {
         objects.add (new Light (in));
      } else if (type == "Box") {
         objects.add (new Box (in));
      } else if (type == "Floor") { 
         objects.add (new Floor (in));
      } else if (type == "Plane") { 
         objects.add (new Plane (in));
      } else if (type == "Sky") {
         objects.add (new Sky (in));
      }  
      in >> type;
   }
}

intersection Scene::first_visible (const Point &sp, const Point &dp) const {
   static intersection curr = {NULL, Point::MAX+1, Point()};
   static intersection closest (curr);
   ObjList::ObjIter iter = objects.iterator();
   closest.t = Point::MAX+1;
 
   for (;iter.has_next(); ++iter) {
      if ((*iter)->get_intersect (sp, dp, curr) && curr.t < closest.t) {
         closest = curr;
      }
   }
   return closest;
}
intersection Scene::first_object (const Point &sp, const Point &dp) const {
   static intersection curr = {NULL, Point::MAX+1, Point()};
   static intersection closest (curr);
   ObjList::ObjIter iter = objects.iterator();
   closest.t = Point::MAX+1;
 
   for (;iter.has_next(); ++iter) {
      if (dynamic_cast<Light *>(*iter)) break;
      if ((*iter)->get_intersect (sp, dp, curr) && curr.t < closest.t) {
         closest = curr;
      }
   }
   return closest;
}

const Light * Scene::next_light_visible (const Point *pt) const {
   static Point cur_pt (0,0,0);
   static ObjList::ObjIter iter(NULL);
   intersection first;

   if (pt != NULL) {
      cur_pt = *pt;
      iter = objects.light_iterator();
   }

   while (iter.has_next()) {
      first = first_visible (cur_pt, ((Light*)*iter)->location());
      if (first.obj == *iter) {
         ++iter;
         return (Light *)first.obj;
      }
      ++iter;
   }
   return NULL;
}



Point Scene::virtual_coord (int row, int col, int img_h, int img_w) {
   return Point (WIDTH * (col/(double)(img_w-1)) - WIDTH/2.0,
                 HEIGHT/2.0 - HEIGHT*row/(double)(img_h-1), 0);
}

