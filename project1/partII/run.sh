g++ -c AudioTest.cpp
g++ AudioTest.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
./sfml-app
rm AudioTest.o
rm sfml-app