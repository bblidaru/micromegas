
export PATH=$PATH:/opt/root-6.06.00/bin

echo "Compiling Micromegas"

g++ src/micromegas.cpp -o micromegas -g -I /opt/garfield/Include/ -L /opt/garfield/Library/ -lGarfield -lgfortran `root-config --cflags --libs`

echo 'Done'
