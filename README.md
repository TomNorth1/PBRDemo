# PBR Demonstration

## An implementation of two different physically based rendering techniques using OpenGL and C++

This project investigates the visual differences when using different diffuse, geometery and distribution functions when creating a PBR. It features a comparison scene comparing a sphere using a Lambertian diffuse function to that of a Oren-Nayar diffuse function. It also features parralax mapping which adds the illusion of physical depth to the sphere. The project is built on top of a base project that included an object loader to load .obj files. 

## Examples

![](https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExcWc2dnVxMHZyejgydmdjNmJ2c3kzaXk4d2xwazA5aHhlMjhkMzFxMyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/DZg1liECjflNK2Cxdx/giphy.gif)

Example of PBR implementation.

![](https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExN2ZmZnNndHh1MXdhNTdrNmtnOWk2N2docnRrczJ4M21rZDh6N2lzNyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/CTOQRsBD8VtKzMRdcK/giphy.gif)

Comparison of Lambertian diffuse (Left) and Oren-Nayar diffuse (Right). The Oren-Nayar diffuse is notably brighter and more well lit around the object than Lambertian.


![](https://github.com/TomNorth1/PBRDemo/blob/main/smiths_neuman.png?raw=true)

Comparison of Smiths and Neuman geometery functions on a rougher rock surface.

## Instructions on how to run
1. Clone this project
2. Navigate to the Release folder
3. Run the PBRDemo.exe

