#include "Photon.h"

class MaxHeap
{
  private:
      Closest *array;
      int size, capacity;
      
      //Prevents usor from copying or assigning
      MaxHeap(const MaxHeap &toCopy);
      MaxHeap &operator=(const MaxHeap &toAssign);
      
      void insertFull(const Closest &toIns);
      void insertNotFull(const Closest &toIns);
      
  public:
      const static int DEFAULT_SIZE = 100;
      
      MaxHeap(int cap = DEFAULT_SIZE);
      ~MaxHeap();
      
      bool isFull() const;
      int get_size() const;
      void clear();
      
      void insert(const Closest &toIns);
      void delMax();
      const Closest &findMax() const;
      Color sumColors() const;
};
