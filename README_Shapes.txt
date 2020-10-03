Not known bug founded. 

To draw tesselation of cube, I did a for loop from left to right and top to bottom for 6 sides. I draw two triangles to represent a rectangle on the surface. 

To draw tesselation of cylinder, I first draw the top and bottom cycles according to parameter 2. I calculated the angles of the triangles representing the cycles according to the formula  2pi/param2. Then I daw a triangle every pi/param2 degree. Tesselation of side surface is similar to what I did in cube, the only difference is I use coordinations of bottom angles of top and bottom cycles as the corners.

To draw tesselation of cone, the bottom is exactly same as I drew the bottom of cylinder. The tesselation of side surface is similar to side surface of cylinder, except all side triangles are share one common tips.


