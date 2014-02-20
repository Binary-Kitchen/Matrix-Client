Matrix-Client
=============

Client Software for LED-Matrix

Compilation
-------------

* mkdir build
* cd build
* cmake ../Matrix-Client/
* make

Usage
-------------

* Example Apps in app/
  * ./matrix-fire 1.2.3.4
  * ./matrix-fire -d
  * ./matrix-fire -s 1.2.3.4

* Matrix-Grayscale app:
  * Example usage: 
    curl "http://upload.wikimedia.org/wikipedia/commons/thumb/4/48/Gentoo_Linux_logo_matte.svg/300px-Gentoo_Linux_logo_matte.svg.png" | convert /dev/stdin  -scale 40x45 -depth 8 gray:- | ./matrix-grayscale raspberry
