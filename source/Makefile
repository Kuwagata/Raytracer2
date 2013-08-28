HEADERS = PhotonMap.h Raytracer.h Color.h Point.h Scene.h ObjList.h Floor.h \
Sky.h Sphere.h Object.h Light.h Box.h Plane.h Photon.h MaxHeap.h KDTree.h
OBJS = PhotonMap.o Raytracer.o Color.o Point.o Scene.o ObjList.o Floor.o \
Sky.o Sphere.o Light.o Box.o Plane.o Photon.o MaxHeap.o KDTree.o

# Conversion rule: source_extension -> object extension
#
all: raytrace

raytrace: $(HEADERS) $(OBJS)
	g++ -o raytrace -ggdb -Wall $(OBJS)

Color.o: Color.cpp Color.h Makefile
	g++ -c -ggdb -Wall -O Color.cpp

Point.o: Point.cpp Point.h Makefile
	g++ -c -ggdb -Wall -O Point.cpp

Box.o: Box.cpp Box.h Object.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O Box.cpp

Floor.o: Floor.cpp Floor.h Object.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O Floor.cpp

Plane.o: Plane.cpp Plane.h Object.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O Plane.cpp

Sky.o: Sky.cpp Sky.h Object.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O Sky.cpp

Sphere.o: Sphere.cpp Sphere.h Object.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O Sphere.cpp

Light.o: Light.cpp Light.h Sphere.h Object.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O Light.cpp

ObjList.o: ObjList.cpp ObjList.h Light.h Sphere.h Object.h Color.h Point.h \
Makefile
	g++ -c -ggdb -Wall -O ObjList.cpp

Scene.o: Scene.cpp Scene.h ObjList.h Sky.h Floor.h Box.h Object.h Color.h \
Point.h
	g++ -c -ggdb -Wall -O Scene.cpp

Photon.o: Photon.cpp Photon.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O Photon.cpp

MaxHeap.o: MaxHeap.cpp MaxHeap.h Photon.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O MaxHeap.cpp

KDTree.o: KDTree.cpp KDTree.h MaxHeap.h Color.h Point.h Photon.h Makefile
	g++ -c -ggdb -Wall -O KDTree.cpp

PhotonMap.o: PhotonMap.cpp PhotonMap.h Scene.h ObjList.h Sky.h Floor.h Box.h \
Object.h Color.h Point.h Makefile
	g++ -c -ggdb -Wall -O PhotonMap.cpp

Raytracer.o: Raytracer.cpp Raytracer.h PhotonMap.h Scene.h ObjList.h Sky.h \
Floor.h Box.h Makefile
	g++ -c -ggdb -Wall -O Raytracer.cpp

clean:
	rm -f *.o core raytrace *.ppm *.gch
