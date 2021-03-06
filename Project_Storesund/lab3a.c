/*

Project by: Linn Storesund
Date: 12-03-2022
Course: TNM084 procedural images

Generated objects in scene:

    - Procedural House(s)
    - Procedural Tree


*/

#include "MicroGlut.h"
#include "GL_utilities.h"s
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "glugg.h"

#include <stdio.h>
#include <stdlib.h>

// uses framework OpenGL
// uses framework Cocoa

void MakeCylinderAlt(int aSlices, float height, float topwidth, float bottomwidth)
{
	gluggMode(GLUGG_TRIANGLE_STRIP);
	vec3 top = SetVector(0,height,0);
	vec3 center = SetVector(0,0,0);
	vec3 bn = SetVector(0,-1,0); // Bottom normal
	vec3 tn = SetVector(0,1,0); // Top normal

	for (float a = 0.0; a < 2.0*M_PI+0.0001; a += 2.0*M_PI / aSlices)
	{
		float a1 = a;

		vec3 p1 = SetVector(topwidth * cos(a1), height, topwidth * sin(a1));
		vec3 p2 = SetVector(bottomwidth * cos(a1), 0, bottomwidth * sin(a1));
		vec3 pn = SetVector(cos(a1), 0, sin(a1));

// Done making points and normals. Now create polygons!
		gluggNormalv(pn);
	    gluggTexCoord(height, a1/M_PI);
	    gluggVertexv(p2);
	    gluggTexCoord(0, a1/M_PI);
	    gluggVertexv(p1);
	}

	// Then walk around the top and bottom with fans
	gluggMode(GLUGG_TRIANGLE_FAN);
	gluggNormalv(bn);
	gluggVertexv(center);
	// Walk around edge
	for (float a = 0.0; a <= 2.0*M_PI+0.001; a += 2.0*M_PI / aSlices)
	{
		vec3 p = SetVector(bottomwidth * cos(a), 0, bottomwidth * sin(a));
	    gluggVertexv(p);
	}
	// Walk around edge
	gluggMode(GLUGG_TRIANGLE_FAN); // Reset to new fan
	gluggNormalv(tn);
	gluggVertexv(top);
	for (float a = 2.0*M_PI; a >= -0.001; a -= 2.0*M_PI / aSlices)
	{
		vec3 p = SetVector(topwidth * cos(a), height, topwidth * sin(a));
	    gluggVertexv(p);
	}
}


mat4 projectionMatrix;

Model *floormodel;
GLuint grasstex, barktex, stonetex;

// Reference to shader programs
GLuint phongShader, texShader;

// Floor quad
GLfloat vertices2[] = {	-20.5,0.0,-20.5,
						20.5,0.0,-20.5,
						20.5,0.0,20.5,
						-20.5,0.0,20.5};
GLfloat normals2[] = {	0,1.0,0,
						0,1.0,0,
						0,1.0,0,
						0,1.0,0};
GLfloat texcoord2[] = {	50.0f, 50.0f,
						0.0f, 50.0f,
						0.0f, 0.0f,
						50.0f, 0.0f};
GLuint indices2[] = {	0,3,2, 0,2,1};

// Tree from lab 3a
void MakeBranch(int count, int maxDepth){

    // add randomness to the tree
    double random = (rand() % 5);
if(count < maxDepth){

     //branch 1
    gluggPushMatrix();
    gluggTranslate(0, 1.9, 0);
    gluggScale(0.6, 0.8, 0.8);
    gluggRotate(0.5, 1.0, 1.0, 2);
	MakeCylinderAlt(20, 2, 0.1, 0.15);
	MakeBranch(count+1,maxDepth);
	gluggPopMatrix();

    //branch 2
    gluggPushMatrix();
    gluggTranslate(0, 1.9, 0);
    gluggScale(0.6, 0.8, 0.8);
    gluggRotate(-1, 2, 2, 2);
	MakeCylinderAlt(20, 2, 0.1, 0.15);
	MakeBranch(count+1,maxDepth);
	gluggPopMatrix();

}
   else{
    return;
   }

}


// Some functions to generate a tree house:
/*
    *with bark texture*
    - Tree wall
    - Tree windows
    - Wall 1, 2, 3, 4

    *with red bark texture*
    - Roof
    - Door

*/

// Function to generate a tree wall
void MakeTreeWall(int count, int maxDepth){

if(count < maxDepth){
    gluggPushMatrix();
    gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 3, 0.1, 0.1);
	MakeTreeWall(count+1,maxDepth);
	gluggPopMatrix();
}
   else{
    return;
   }

}

// Function to make one window
void MakeTreeWindow(int count, int maxDepth){

if(count < maxDepth){
    gluggTranslate(0.2, -2, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	gluggTranslate(0, 2, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	MakeTreeWindow(count+1,maxDepth);
}
   else{
    return;
   }

}

// Make one wall with two windows based on MakeTreeWindows and MakeTreeWall
void MakeTreeWindows(){

    // Window 1
    gluggPushMatrix();
    gluggTranslate(1.2, 0, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	gluggTranslate(0, 2, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
    MakeTreeWindow(0,3);
    gluggTranslate(0, -2, 0);
	MakeTreeWall(0,5);

	//Window 2
    gluggTranslate(1.2, 0, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	gluggTranslate(0, 2, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
    MakeTreeWindow(0,3);
    gluggTranslate(0, -2, 0);
	MakeTreeWall(0,7);
	gluggPopMatrix();

}

// Make tree wall 1
void MakeTreeWall_1(int count, int maxDepth){

    MakeTreeWall(count,maxDepth);
    MakeTreeWindows();

}

// Make tree wall 2 with translation based on MakeTreeWall and MakeTreeWindow
void MakeTreeWall_2(int count, int maxDepth){

if(count < maxDepth){

    gluggPushMatrix();
    gluggTranslate(0.2, 0, -3);
	MakeCylinderAlt(20, 3.1, 0.1, 0.1);
	MakeTreeWall(count+1,maxDepth);
	MakeTreeWindows();
	gluggPopMatrix();
}
   else{
    return;
   }

}

// Make a all ramp to suit the roof where every cylinder have different heights
void WallRampUp(){

    gluggScale(1,1.1,1);
    MakeCylinderAlt(20, 3, 0.1, 0.1);
	gluggTranslate(0.2, 0, 0);
    gluggScale(1,1.05,1);
    MakeCylinderAlt(20, 3, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
    gluggScale(1,1.05,1);
    MakeCylinderAlt(20, 3, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
    gluggScale(1,1.05,1);
    MakeCylinderAlt(20, 3, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
    gluggScale(1,1.05,1);
    MakeCylinderAlt(20, 3, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
    gluggScale(1,1.05,1);
    MakeCylinderAlt(20, 3, 0.1, 0.1);
}

void WallRampDown(){

    // The ramp wall from the door next wall
    gluggTranslate(0.2, -1.9, 0);
	MakeCylinderAlt(20, 3, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 2.85, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 2.8, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 2.65, 0.1, 0.1);
	 gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 2.45, 0.1, 0.1);
	 gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 2.35, 0.1, 0.1);


}
// Make tree wall 3 with a door and ramp of tree cylinders
void MakeTreeWall_3(int count, int maxDepth){

if(count < maxDepth){

    // wall to door
    gluggPushMatrix();
    gluggRotate(1.5,0,1,0);
    WallRampUp();

    // Door with stocks with different heights
    gluggTranslate(0.2, 1.9, 0);
	MakeCylinderAlt(20, 1.2, 0.1, 0.1);
	gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 1.35, 0.1, 0.1);
    gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 1.25, 0.1, 0.1);

    WallRampDown();
	gluggPopMatrix();
}
   else{
    return;
   }

}


void MakeTreeDoor(){

    gluggPushMatrix();
	gluggTranslate(0.1, 2, -1.15);
	MakeCylinderAlt(20, 1, 0.1, 0.1);

	gluggTranslate(0, 0, -0.2);
	MakeCylinderAlt(20, 1, 0.1, 0.1);

    gluggTranslate(0, 0, -0.2);
	MakeCylinderAlt(20, 1, 0.1, 0.1);

    gluggTranslate(0, 0, -0.2);
	MakeCylinderAlt(20, 1, 0.1, 0.1);

    gluggTranslate(0, -2, 0);
    gluggRotate(1.5,0,1,0);

	MakeTreeWall(0,6);

	gluggPopMatrix();


}



void MakeTreeWall_4(int count, int maxDepth){

if(count < maxDepth){

    gluggPushMatrix();
    gluggTranslate(5, 0, 0);
    gluggRotate(1.5,0,1,0);
    WallRampUp();

	// one special window with ramp above
	gluggTranslate(0.2, 0, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	gluggTranslate(0, 2, 0);
	MakeCylinderAlt(20, 1.1, 0.1, 0.1);
    gluggTranslate(0.2, -2, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	gluggTranslate(0, 2, 0);
	MakeCylinderAlt(20, 1.2, 0.1, 0.1);
    gluggTranslate(0.2, -2, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	gluggTranslate(0, 2, 0);
	MakeCylinderAlt(20, 1.2, 0.1, 0.1);
    gluggTranslate(0.2, -2, 0);
	MakeCylinderAlt(20, 1, 0.1, 0.1);
	gluggTranslate(0, 2, 0);
	MakeCylinderAlt(20, 1.1, 0.1, 0.1);

    //Continuing the wall
    gluggTranslate(0, -0.2, 0);
	WallRampDown();
	gluggPopMatrix();
}
   else{
    return;
   }

}

// Generating a closed tree door
void MakeDoor(float height){

    MakeCylinderAlt(20,height,0.1,0.1);
    gluggTranslate(0,0,-0.2);
    MakeCylinderAlt(20,height,0.1,0.1);
    gluggTranslate(0,0,-0.2);
    MakeCylinderAlt(20,height,0.1,0.1);
}

// Generating a sloping roof
void MakeTreeRoof(int count, int maxDepth){

if(count < maxDepth){

    gluggPushMatrix();
    gluggTranslate(0.2,0,0);
	MakeCylinderAlt(20, 2.5, 0.1, 0.1);
    MakeTreeRoof(count+1,maxDepth);
	gluggPopMatrix();
}
   else{
    return;
   }

}

void AddRoof(){

    // first front stock roof
    gluggTranslate(0, 3,0.3);
    gluggRotate(-0.8, 1.0, 0, 0);
	MakeCylinderAlt(20, 2.5, 0.1, 0.1);
    MakeTreeRoof(0,26);
    // first back stock roof
    gluggTranslate(0, 2.5, -2.5);
    gluggRotate(1.6, 1.0, 0, 0);
	MakeCylinderAlt(20, 2.5, 0.1, 0.1);
    MakeTreeRoof(0,26);

    gluggTranslate(0.1,0,4.315);
    gluggRotate(-0.8,1,0,0);
}


// Generating one house containing four tree walls, windows and roof
void MakeHouse(float scale, float xTrans, float yTrans, float zTrans){

    gluggTranslate(xTrans, yTrans, zTrans);
    gluggScale(scale, scale, scale);

    MakeTreeWall_1(0,5);
    MakeTreeWall_2(0,6);
    MakeTreeWall_3(0,6);
    MakeTreeWall_4(0,6);

    // uncomment if we want to add more houses to the scene with all same texture
   //  AddRoof();



}


GLuint MakeTree(int *count, GLuint program)
{
	gluggSetPositionName("inPosition");
	gluggSetNormalName("inNormal");
	gluggSetTexCoordName("inTexCoord");

	gluggBegin(GLUGG_TRIANGLES);

    gluggTranslate(7, 0, 2);
    gluggRotate(-0.5,0,1,0);
	//Stommen
	MakeCylinderAlt(20, 2, 0.1, 0.15);

    // 3a1)  Create a tree by adding the shape multiple times, recursively.
    MakeBranch(0,8);
    // 3a2) Add randomness to add variation to the tree --> added to the rotation

  return gluggEnd(count, program, 0);

}

// MakeHouses have the bark texture, here we can add more houses if we want
GLuint MakeHouses(int *count, GLuint program)
{
	gluggSetPositionName("inPosition");
	gluggSetNormalName("inNormal");
	gluggSetTexCoordName("inTexCoord");

	gluggBegin(GLUGG_TRIANGLES);

    MakeHouse(1,0,0,0);

    // Add houses(scale, x position, y position, z position)
    // add roof in MakeHouse
   /* MakeHouse(0.5,-3,0,-5);
    MakeHouse(1.0,1,0,-5);
    MakeHouse(2,-1,0,25);  */

    gluggTranslate(7, 0, 2);
    gluggRotate(-0.5,0,1,0);


  return gluggEnd(count, program, 0);

}


// Red details of the house in another object. Works good for one house then a lot of effort to calculate where the roof
// and door should be if we have several houses
GLuint MakeDetails(int *count, GLuint program)
{
	gluggSetPositionName("inPosition");
	gluggSetNormalName("inNormal");
	gluggSetTexCoordName("inTexCoord");

	gluggBegin(GLUGG_TRIANGLES);

   // first front roof stock
    gluggTranslate(0, 3,0.3);
    gluggRotate(-0.8, 1.0, 0, 0);
	MakeCylinderAlt(20, 2.5, 0.1, 0.1);
    MakeTreeRoof(0,26);

    // first back roof stock
    gluggTranslate(0, 2.5, -2.5);
    gluggRotate(1.6, 1.0, 0, 0);
	MakeCylinderAlt(20, 2.5, 0.1, 0.1);
    MakeTreeRoof(0,26);

    // door
    gluggTranslate(0.1,-1.35,4.315);
    gluggRotate(-0.8,1,0,0);
    MakeDoor(3.75);

  return gluggEnd(count, program, 0);

}

GLuint tree;
GLuint house;
GLuint details;
int treecount = 3;
int housecount=3;
int detailscount=3;

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

	// Set the clipping volume
	projectionMatrix = perspective(45,1.0f*w/h,1,1000);
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

	// Load and compile shader
	phongShader = loadShaders("phong.vert", "phong.frag");
	texShader = loadShaders("textured.vert", "textured.frag");
	printError("init shader");


	// Upload geometry to the GPU:
	floormodel = LoadDataToModel((vec3 *)vertices2, (vec3*)normals2 , (vec2 *)texcoord2, NULL,
			indices2, 4, 6);

// Important! The shader we upload to must be active!
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUniform1i(glGetUniformLocation(texShader, "tex"), 0); // Texture unit 0

	LoadTGATextureSimple("grass.tga", &grasstex);
	glBindTexture(GL_TEXTURE_2D, grasstex);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_REPEAT);

	LoadTGATextureSimple("bark2.tga", &barktex);
	LoadTGATextureSimple("stone.tga", &stonetex);

	tree = MakeTree(&treecount, texShader);
	house = MakeHouses(&housecount, texShader);
	details = MakeDetails(&detailscount, texShader);


	printError("init arrays");
}

GLfloat a = 0.0;
vec3 campos = {0, 1.5, 10};
vec3 forward = {0, 0, -4};
vec3 up = {0, 1, 0};

void display(void)
{
	int i, j;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 worldToView, m; // m1, m2, m, tr, scale;

	if (glutKeyIsDown('a'))
		forward = MultMat3Vec3(mat4tomat3(Ry(0.05)), forward);
	if (glutKeyIsDown('d'))
		forward = MultMat3Vec3(mat4tomat3(Ry(-0.05)), forward);
	if (glutKeyIsDown('w'))
		campos = VectorAdd(campos, ScalarMult(forward, 0.1));
	if (glutKeyIsDown('s'))
		campos = VectorSub(campos, ScalarMult(forward, 0.1));
	if (glutKeyIsDown('q'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorSub(campos, ScalarMult(side, 0.05));
	}
	if (glutKeyIsDown('e'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorAdd(campos, ScalarMult(side, 0.05));
	}

	// Move up/down
	if (glutKeyIsDown('z'))
		campos = VectorAdd(campos, ScalarMult(SetVector(0,1,0), 0.01));
	if (glutKeyIsDown('c'))
		campos = VectorSub(campos, ScalarMult(SetVector(0,1,0), 0.01));

	// NOTE: Looking up and down is done by making a side vector and rotation around arbitrary axis!
	if (glutKeyIsDown('+'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		mat4 m = ArbRotate(side, 0.05);
		forward = MultMat3Vec3(mat4tomat3(m), forward);
	}
	if (glutKeyIsDown('-'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		mat4 m = ArbRotate(side, -0.05);
		forward = MultMat3Vec3(mat4tomat3(m), forward);
	}

	worldToView = lookAtv(campos, VectorAdd(campos, forward), up);

	a += 0.1;

	glBindTexture(GL_TEXTURE_2D, grasstex);
	// Floor
	glUseProgram(texShader);
	m = worldToView;
	glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(floormodel, texShader, "inPosition", "inNormal", "inTexCoord");

	// Draw the tree, as defined on MakeTree
	glBindTexture(GL_TEXTURE_2D, barktex);
	glUseProgram(texShader);
    m = Mult(worldToView, T(0, 0, 0));
    glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(tree);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, treecount);

    // Draw the house, as defined on MakeTree
	glBindTexture(GL_TEXTURE_2D, barktex);
	glUseProgram(texShader);
    m = Mult(worldToView, T(0, 0, 0));
    glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(house);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, housecount);

    // Draw the details, as defined on MakeTree
	glBindTexture(GL_TEXTURE_2D, stonetex);
	glUseProgram(texShader);
    m = Mult(worldToView, T(0, 0, 0));
    glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(details);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, detailscount);

	printError("display");

	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	switch (key)
	{
		case ' ':
			forward.y = 0;
			forward = ScalarMult(normalize(forward), 4.0);
			break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640*2,360*2);
	glutCreateWindow ("Linn TNM084 Project");
	glutRepeatingTimer(20);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutReshapeFunc(reshape);
	init ();
	glutMainLoop();
}
