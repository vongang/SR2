#SR2

As we all know, 3D rendering pipeline is an huge project, and it includes so many parts. And here I implemented an 3D software rendering toy. This project was inspired by [\[1\]][1]. Lots of documents was used like [\[2\]][2] [\[3\]][3] [\[4\]][4] and so on. I coded it in 5 days. All of things I had done and will do is shown in the list

+ Done
  - Basic environment: including the mathematics lib(especially vector and matrix), display functions and the like. *in the branch s1*
  - 3D transformation: the world, view, project transform maxtrix. *in the branch s1*
  - Draw basic line: the traditional method and Bresenham method. *in the branch s2*
  - Load \*.obj, draw triangle and z-buff test *in the branch s3* 
  - Muli-thread support with C++11 *in the branch s4*
  - Flat shading *in the branch s5*
  - Gouraud shading and back-face culling *in the branch s6*
  
  + TODO
   - Anti-Aliased
   - Texture
   - ray tracing
   
   #### Reference
   [1]:https://www.davrous.com/2013/06/13/tutorial-series-learning-how-to-write-a-3d-soft-engine-from-scratch-in-c-typescript-or-javascript/ "[1]:https://www.davrous.com/2013/06/13/tutorial-series-learning-how-to-write-a-3d-soft-engine-from-scratch-in-c-typescript-or-javascript/"
   [2]:https://en.wikipedia.org/wiki/Transformation_matrix
   [3]:http://www.stroustrup.com/C++11FAQ.html
   [4]:https://en.wikipedia.org/wiki/Back-face_culling
