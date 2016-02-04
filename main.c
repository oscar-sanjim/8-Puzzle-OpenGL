//
//  main.c
//  OpenGL Base
//
//  Created by Jose Carlos Mondragon Guadarrama on 11/08/15.
//  Copyright (c) 2015 itesm. All rights reserved.
//

//Compile: gcc main.c -lglut -lGL -lGLU

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

typedef int bool;
enum { false, true };

// Global Variables
bool g_gamemode;       // GLUT GameMode ON/OFF
bool g_fullscreen;     // Fullscreen Mode ON/OFF (When g_gamemode Is OFF)

// Our GL Specific Initializations
bool init(void)
{
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    return true;
}

// Our Rendering Is Done Here
void render(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();                   // Reset The Current Modelview Matrix
    
    glTranslatef(-1.5f,0.0f,-6.0f);             // Left 1.5 Then Into Screen Six Units
    
    glRotatef(45.0, 0, 1, 0);             //Rotating Axis

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f,0.0f,0.0f);          // Set The Color To Red
    glVertex3f( 0.0f, 1.0f, 0.0f);          // Move Up One Unit From Center (Top Point)
    
    glColor3f(0.0f,1.0f,0.0f);          // Set The Color To Green
    glVertex3f(-1.0f,-1.0f, 0.0f);          // Left And Down One Unit (Bottom Left)
    
    glColor3f(0.0f,0.0f,1.0f);          // Set The Color To Blue
    glVertex3f( 1.0f,-1.0f, 0.0f);          // Right And Down One Unit (Bottom Right)
    glEnd();                        // Done Drawing A Triangle

    glTranslatef(3.0f,0.0f,0.0f);               // From Right Point Move 3 Units Right

    glColor3f(0.5f,0.5f,1.0f);              // Set The Color To Blue One Time Only
    glBegin(GL_QUADS);                  // Start Drawing Quads
    glVertex3f(-1.0f, 1.0f, 0.0f);          // Left And Up 1 Unit (Top Left)
    glVertex3f( 1.0f, 1.0f, 0.0f);          // Right And Up 1 Unit (Top Right)
    glVertex3f( 1.0f,-1.0f, 0.0f);          // Right And Down One Unit (Bottom Right)
    glVertex3f(-1.0f,-1.0f, 0.0f);          // Left And Down One Unit (Bottom Left)
    glEnd();                        // Done Drawing A Quad
    
    glutSwapBuffers ( );
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
   // glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    // Calculate The Aspect Ratio And Set The Clipping Volume
    if (h == 0) h = 1;

    gluPerspective(80, (float)w/(float)h, 1.0, 5000.0);
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
    glLoadIdentity();                // Reset The Modelview Matrix
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:        // When Escape Is Pressed...
            exit(0);    // Exit The Program
            break;          // Ready For Next Case
        default:        // Now Wrap It Up
            break;
    }
}

// Our Keyboard Handler For Special Keys (Like Arrow Keys And Function Keys)
void special_keys(int a_keys, int x, int y)
{
    switch (a_keys) {
        case GLUT_KEY_F1:
            printf("F1");
            break;
        default:
            break;
    }
}


// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);     // Display Mode (Rgb And Double Buffered)

    
    glutInitWindowSize(500, 500);                // Window Size If We Start In Windowed Mode
    glutCreateWindow("NeHe's OpenGL Framework"); // Window Title
    init();                                          // Our Initialization
 
    //glViewport(0,0,500,500);

    
    glutDisplayFunc(render);                         // Register The Display Function
    glutReshapeFunc(reshape);                        // Register The Reshape Handler
    glutKeyboardFunc(keyboard);                      // Register The Keyboard Handler
    
    
    glutMainLoop();                                  // Go To GLUT Main Loop
    return 0;
}
