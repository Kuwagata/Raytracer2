#include "ObjList.h"

ObjList::ObjList () : head(NULL), tail(NULL), first_light(NULL){}

void ObjList::add (Light *obj) {
   Node *new_node = new Node (obj);
   if (head == NULL) {
      head = tail = new_node;
   } else {
      tail->add_after (new_node);
      tail = tail->next;
   }
   if (first_light == NULL) first_light = new_node;
}

void ObjList::add (Object *obj) {
   Node *new_node = new Node (obj);
   if (head == NULL) {
      head = tail = new_node;
   } else {
      new_node->add_after (head);
      head = new_node;
   }
}
   
ObjList::ObjIter ObjList::iterator () const {
   return ObjList::ObjIter (head);
}
   
ObjList::ObjIter ObjList::light_iterator () const {
   return ObjList::ObjIter (first_light);
}
   

ObjList::Node::Node (Object *obj) {
   this->object = obj;
   next = NULL;
}
      
ObjList::Node::Node (Object *obj, Node *next) {
   this->object = obj;
   this->next = next;
}
      
void ObjList::Node::add_after (Node *new_node) {
   // have the new node's next point to what this's next previously held.
   if (this->next != NULL) {
      Node *current = new_node;
      while (current->next != NULL) current = current->next;
      current->next = this->next;
   }
   this->next = new_node;
}
   
ObjList::ObjIter::ObjIter (Node *start) {
   current = start;
}

bool ObjList::ObjIter::has_next(void) const {
   return current != NULL;
}

Object *ObjList::ObjIter::next() {
   Node *toReturn = current;
   current = current->next;
   return toReturn->object;
}

Object *ObjList::ObjIter::operator*(void) const {
   return current->object;
}

const ObjList::ObjIter &ObjList::ObjIter::operator++ (void) {
   next();
   return *this;
}

