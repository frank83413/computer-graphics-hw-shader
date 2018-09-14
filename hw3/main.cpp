#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "../GL/glew.h"
#include "../GL/glut.h""
#include "../shader_lib/shader.h"

#include "glm/glm.h"
extern "C"
{
	#include "glm_helper.h"
}

void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);
int tflat = 0;
GLuint program;
GLuint programf;
GLuint vboName;
GLMmodel *XiaoPingGuo;
GLfloat light_pos[] = { 10.0, 10.0, 0.0 };
float eyex = 0.0;
float eyey = 0.0;
float x=0;
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("OpenGL HW3 - 0556638");
	glutReshapeWindow(512, 512);

	glewInit();

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	glmDelete(XiaoPingGuo);
	return 0;
}

void init(void) {
	XiaoPingGuo = glmReadOBJ("Model/apple.obj");
	float *shaderarray = (float*)malloc(XiaoPingGuo->numtriangles*sizeof(float)*24);
	
	glmUnitize(XiaoPingGuo);
	glmFacetNormals(XiaoPingGuo);
	glmVertexNormals(XiaoPingGuo, 90.0, GL_FALSE);
	
	//print_model_info(XiaoPingGuo);

	for (int i = 0; i < XiaoPingGuo->numtriangles;i++) {
		int k = i * 24;
		shaderarray[k] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[0]];
		shaderarray[k + 1] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[0]+1];
		shaderarray[k + 2] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[0]+2];
		
		shaderarray[k + 3] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[0]];
		shaderarray[k + 4] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[0] + 1];
		shaderarray[k + 5] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[0] + 2];

		shaderarray[k + 6] = XiaoPingGuo->texcoords[2 * XiaoPingGuo->triangles[i].tindices[0]];
		shaderarray[k + 7] = XiaoPingGuo->texcoords[2 * XiaoPingGuo->triangles[i].tindices[0] + 1];
		
		////////////////////////////////////////////////////////////////////////////////////
		shaderarray[k + 8] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[1]];
		shaderarray[k + 9] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[1] + 1];
		shaderarray[k + 10] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[1] + 2];

		shaderarray[k + 11] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[1]];
		shaderarray[k + 12] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[1] + 1];
		shaderarray[k + 13] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[1] + 2];

		shaderarray[k + 14] = XiaoPingGuo->texcoords[2 * XiaoPingGuo->triangles[i].tindices[1]];
		shaderarray[k + 15] = XiaoPingGuo->texcoords[2 * XiaoPingGuo->triangles[i].tindices[1] + 1];

		////////////////////////////////////////////////////////////////////////////////////
		shaderarray[k + 16] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[2]];
		shaderarray[k + 17] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[2] + 1];
		shaderarray[k + 18] = XiaoPingGuo->vertices[3 * XiaoPingGuo->triangles[i].vindices[2] + 2];

		shaderarray[k + 19] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[2]];
		shaderarray[k + 20] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[2] + 1];
		shaderarray[k + 21] = XiaoPingGuo->normals[3 * XiaoPingGuo->triangles[i].nindices[2] + 2];

		shaderarray[k + 22] = XiaoPingGuo->texcoords[2 * XiaoPingGuo->triangles[i].tindices[2]];
		shaderarray[k + 23] = XiaoPingGuo->texcoords[2 * XiaoPingGuo->triangles[i].tindices[2] + 1];
		
	}
	GLuint vert = createShader("Shaders/sin.vert", "vertex");
	GLuint frag = createShader("Shaders/sin.frag", "fragment");
	program = createProgram(vert, frag);
	GLuint vertf = createShader("Shaders/sinf.vert", "vertex");
	GLuint fragf = createShader("Shaders/sinf.frag", "fragment");
	programf = createProgram(vertf, fragf);
	
	glGenBuffers(1, &vboName);
	glBindBuffer(GL_ARRAY_BUFFER, vboName);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * XiaoPingGuo->numtriangles * 24, shaderarray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	free(shaderarray);
}

void display(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1e-2, 1e2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, 3.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	


	GLfloat Projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, Projection);
	GLint Ploc = glGetUniformLocation(program, "Projection");	

	GLfloat View[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, View);
	GLint Vloc = glGetUniformLocation(program, "View");
	GLfloat EView[3];
	GLint EVloc = glGetUniformLocation(program, "EView");
	GLint Lloc = glGetUniformLocation(program, "Light");	
	
	glEnable(GL_TEXTURE_2D);
	GLint Tloc = glGetUniformLocation(program, "Texture");
	

	
	if(tflat==1)glUseProgram(programf);
	else	glUseProgram(program);
	glUniform3f(EVloc, eyex, eyey, 3.0f);
		glUniform3f(Lloc, 10.0, 10.0, 0.0);
		glUniformMatrix4fv(Ploc, 1, GL_FALSE, Projection);
		glUniformMatrix4fv(Vloc, 1, GL_FALSE, View);
		
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, XiaoPingGuo->textures[0].id);
		glUniform1i(Tloc, 0);

		glEnable(GL_DEPTH_TEST);
		glPushMatrix;
		glRotated(x,0.0,1.0,0.0);
		
		GLfloat ModelView[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
		GLint MVloc = glGetUniformLocation(program, "ModelView");
		glUniformMatrix4fv(MVloc, 1, GL_FALSE, ModelView);

			glDrawArrays(GL_TRIANGLES, 0,3*XiaoPingGuo->groups->next->numtriangles);

			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, XiaoPingGuo->textures[1].id);
			glUniform1i(Tloc, 0);

			glDrawArrays(GL_TRIANGLES, 3 * XiaoPingGuo->groups->next->numtriangles, 3*XiaoPingGuo->groups->numtriangles);
		glPopMatrix;
		glBindTexture(GL_TEXTURE_2D, NULL);

	glUseProgram(0);

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'f':
	{	
		tflat = 1;
		break;
	}
	case 'p':
	{	
		tflat = 0;
		break;
	}
	case 27:
	{	//ESC
		exit(0);
		break;
	}
	case 'd':
	{
		eyex += 0.1;
		break;
	}
	case 'a':
	{
		eyex -= 0.1;
		break;
	}
	case 'w':
	{
		eyey += 0.1;
		break;
	}
	case 's':
	{
		eyey -= 0.1;
		break;
	}
	default:
	{
		break;
	}
	}
}

void idle(void)
{
	x += 0.1;
	glutPostRedisplay();
}