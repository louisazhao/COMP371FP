COMP 371 Computer Graphic Winter 2018
Final Project
Rui Zhao 40018813
***************************************

Functionalities and features:
1. Render a troop of 20 horses with random positions and orientations on the ground.

**************************************************
2. Design collider class with help of math (compare projections of four corner points on X and Z axis), and implemented the collision detection with orientated bounding box, see OBBCollider.h and OBBCollider.cpp.
Designed the collider all by myself, details about how I got the the math done is shown in attached pics.
***************************************************


3. H key toggles the final project effect, where horses will move straight ahead for random steps, then rotate either 15 degrees or -15 degrees. After each movement and rotation, check if there’s collision between horses, if yes, make random one stationary. Also find the appropriate angle so that a horse won’t collide with another one that is already being stopped.
4. Other functionalities are the same as assignment 2 (including shadow, texture, world rotation, camera movement etc.)

***************************************
Textures:
Metal texture from:
https://www.google.ca/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&ved=2ahUKEwj0v-iK87DaAhVGn-AKHapECI8QjRx6BAgAEAU&url=https%3A%2F%2Fjojo-ojoj.deviantart.com%2Fart%2FMetal-seamless-texture-54-532193713&psig=AOvVaw24BA5xvroSEjWYcneUe4x2&ust=1523490649810286

Brick texture from:
https://www.google.ca/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&cad=rja&uact=8&ved=2ahUKEwig-KCe87DaAhWKnOAKHfxXA1UQjRx6BAgAEAU&url=https%3A%2F%2Fwww.sketchuptextureclub.com%2Ftextures&psig=AOvVaw24BA5xvroSEjWYcneUe4x2&ust=1523490649810286

Wood texture from:
https://www.google.ca/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&cad=rja&uact=8&ved=2ahUKEwjtrv2087DaAhVNT98KHVYKCmAQjRx6BAgAEAU&url=https%3A%2F%2Fwww.textures.com%2Fbrowse%2Fold-planks%2F114537&psig=AOvVaw1Zt1pCqJkbYffhag5HplhJ&ust=1523490726063538

Patterned texture from:
https://www.google.ca/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&cad=rja&uact=8&ved=2ahUKEwiYvujM87DaAhXug-AKHaWiCq4QjRx6BAgAEAU&url=http%3A%2F%2Fsuperawesomevectors.com%2Fseamless-texture-background-2%2F&psig=AOvVaw16jlc0P_UPuJGHaex45rBI&ust=1523490772525737

Grass texture from:
https://www.google.ca/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&cad=rja&uact=8&ved=2ahUKEwjNnN3Z87DaAhWRT98KHfakCbwQjRx6BAgAEAU&url=https%3A%2F%2Fcreativemarket.com%2Fhhh316%2F247499-Lush-Seamless-Grass-Texture&psig=AOvVaw2VlePPkIBxgPR-ybPlQtUt&ust=1523490815822133

Skybox from:
http://www.custommapmakers.org/skyboxes.php
