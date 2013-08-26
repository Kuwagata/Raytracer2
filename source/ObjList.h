#include "Light.h"
#include <stdlib.h>
#include <ostream>
#include <iostream>

/* This class defines the linked list that holds the objects and lights in the
 * scene. Objects are stored at the front of the list and lights at the back.
 */
class ObjList {
   private:
      /* Node is a sub class inside the ObjList class that defines the nodes in
       * the linked list, constructors, and an insertion operation.
       * Each Node has an object pointer and a pointer to the next node or
       * NULL if it is the last Node in the list.
       */
      class Node {
         public: 
            Object *object;
            Node *next;
            Node (Object *object);
            Node (Object *object, Node *next);
            ~Node () {
                delete object;
            }
            void add_after (Node *new_node);
      };
   
      /* The head pointer points to the first valid Node in the list.
       * first_light points to the first Light object in the list.
       * tail points to the last valid Node in the list.
       */
      Node *head, *tail, *first_light;
   
   public:
      /* ObjIter is a subclass in the ObjList class that allows iteration over
       * the Object list.
       */
      class ObjIter {
         private:
            Node *current;

         public:
            ObjIter (void) : current (NULL) {}
            ObjIter (Node *start);
            /* This method determines whether "current" points to a Node object
             * or NULL.
             */
            bool has_next(void) const;

            /* returns the Object stored at the current Node and increments
             * current to point to the next node.
             */
            Object *next(void);

            /* returns the Object stored at the current Node. */
            Object *operator*(void) const;

            /* this is the pre-increment. increments current to point to the
             * next Node and returns a reference to the iterator.
             */
            const ObjIter &operator++ (void);
      };

      ObjList (void);
      ~ObjList (void) {
         Node *current = head, *prev;
         while (current) {
            prev = current;
            current = current->next;
            delete prev;
         }
      }
      void add (Light *object);
      void add (Object *object);
      ObjIter iterator (void) const;
      ObjIter light_iterator (void) const;
      friend std::ostream &operator<< (std::ostream &out, const ObjList &list) {
         Node *current = list.head;
         while (current && current->object) {
            out << *current->object << ", ";
            current = current->next;
         }
         return out << std::endl;
      }
};   
