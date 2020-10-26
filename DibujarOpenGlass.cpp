/*

programa de dibujo

Francisco Parreño
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <vector>
//#include <GL/glut.h>
#include <string>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#pragma warning (disable: 4786)

//Para incluir los datos
//#include "Glass.h"

using namespace std;
float D=247.0;
static GLfloat view_rotx=-20.0, view_roty=0.0, view_rotz=0.0;

static int		menu;     // Identificador del menu
typedef enum{ ZOOM,GIRAR,SALIR} opciones_menu;
typedef enum {MOVIENDO,GIRANDO} acciones_raton;
int MODORaton=MOVIENDO;
int RatonPulsado = GL_FALSE;

int Xref=0, Yref=0;
std::vector<int> vec;
std::vector<int> vec2;

void texto(vector <int> &info_piece);
void texto(int, int);

void  EscribirMejorSolucionOpenGL()
{
	

	FILE *fin3;

	fin3=fopen("temp2.txt","w+");
//	fprintf(fin3,"%d\t%d\n",m_max_total_volumen_ocupado,m_total_volumen_ocupado);
//	std::list< CONFIGURACAO > ::iterator itC;
//	float divide=20;
////	fprintf(fin3,"\\CubeThreeD[ A=%.3f,B=%.3f,C=%.3f](%.3f,%.3f,%.3f)\n",(double)m_Y/20,(double)m_X/20,(double)m_Z/20,(double)m_Y/20,(double)m_X/20,(double)m_Z/20);
//
//	//
////	for (itC=lista.begin();itC!=lista.end();itC++)
////	{
////		fprintf(fin3,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",(*itC).Get_x1(),(*itC).Get_y1(),(*itC).Get_z1(),(*itC).Get_x2(),(*itC).Get_y2(),(*itC).Get_z2(),(*itC).Get_Id());
////	}
////	fclose(fin3);
//
////	return;
//	//Ahora las lineas que van de un lado al otro
//	//Esto dibuja bloque enteros de piezas
//	for (itC=lista.begin();itC!=lista.end();itC++)
//	{
//		for (int register i=1;i<=(*itC).Get_dx();i++)
//		{
//			int sumoi=(((*itC).Get_x2()-(*itC).Get_x1())/(*itC).Get_dx());
//
//			for (int register j=1;j<=(*itC).Get_dy();j++)
//			{
//				int sumoj=(((*itC).Get_y2()-(*itC).Get_y1())/(*itC).Get_dy());
//
//				for (int register k=1;k<=(*itC).Get_dz();k++)
//				{
//
//					int sumok=(((*itC).Get_z2()-(*itC).Get_z1())/(*itC).Get_dz());
////					CONFIGURACAO kk=(*itC);
////					DibujarCaja3D((*itC).Get_x1(),(*itC).Get_y1(),(*itC).Get_z1()+(k-1)*sumok,(*itC).Get_x2(),(*itC).Get_y2(),(*itC).Get_z1()+k*sumok,(*itC).Get_Id());
////					DibujarCaja3D((*itC).Get_x1(),(*itC).Get_y1()+(j-1)*sumoj,(*itC).Get_z1(),(*itC).Get_x2(),(*itC).Get_y1()+j*sumoj,(*itC).Get_z2(),(*itC).Get_Id());
//					fprintf(fin3,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",(*itC).Get_x1()+(i-1)*sumoi,(*itC).Get_y1()+(j-1)*sumoj,(*itC).Get_z1()+(k-1)*sumok,(*itC).Get_x1()+i*sumoi,(*itC).Get_y1()+j*sumoj,(*itC).Get_z1()+k*sumok,(*itC).Get_Id());
//
//
//				}
//			}
//		}
//	}
	fclose(fin3);
}

static void cuadrado_negro(float o_x, float o_y,  float x,float y,int p_i,int p_f,int reshu)
{
		float o_z=0;
	float z=0;
	glLineWidth(1);
	glDisable(GL_LIGHTING);
glDisable(GL_LIGHTING);


 //glEnable(GL_BLEND);
 //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


		//color
glColor3f(0, 0, 0);//print timer in red

 
glBegin( GL_LINES );{



  //8
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z+z );

  //
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x+x, o_y, o_z);


   glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z);

 
  //
/*  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z +z);
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z +z);
*/

	}
glLineWidth(2);
glEnd();

	

}
static void cuadrado_rojo(float o_x, float o_y, float x, float y, int p_i, int p_f, int reshu)
{
	float o_z = 0;
	float z = 0;
	glLineWidth(3.0f);

	
	glDisable(GL_LIGHTING);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


	//color
	glColor3f(1,0,0);//print timer in red


	glBegin(GL_LINES); {



		//8
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z + z);

		//
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z);


		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);


		//
		/*  glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x+x, o_y, o_z+z );
		glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z+z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x, o_y+y, o_z );
		glVertex3f( o_x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z +z);
		glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z +z);
		*/

	}
	glBegin(GL_QUAD_STRIP); {



		//8
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z + z);

		//
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z);


		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);


		//
		/*  glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x+x, o_y, o_z+z );
		glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z+z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x, o_y+y, o_z );
		glVertex3f( o_x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z +z);
		glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z +z);
		*/

	}
	glLineWidth(2);
	glEnd();



}
static void defecto_rojo(float o_x, float o_y, float x, float y, int p_i, int p_f, int reshu)
{

	float o_z = 0;
	float z = 0;
	glLineWidth(3.0f);


	glDisable(GL_LIGHTING);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


	//color
	glColor3f(1, 0, 0);//print timer in red


	glBegin(GL_LINES); {
		//8
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z + z);

		//
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z);


		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHTING);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUAD_STRIP); {



		//8
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z + z);

		//
		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y, o_z);


		glVertex3f(o_x, o_y + y, o_z);
		glVertex3f(o_x, o_y, o_z);
		glVertex3f(o_x + x, o_y + y, o_z);
		glVertex3f(o_x + x, o_y, o_z);
		glVertex3f(o_x, o_y + y, o_z + z);
		glVertex3f(o_x, o_y, o_z + z);
		glVertex3f(o_x + x, o_y + y, o_z + z);
		glVertex3f(o_x + x, o_y, o_z + z);


		//
		/*  glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x+x, o_y, o_z+z );
		glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z+z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x, o_y+y, o_z );
		glVertex3f( o_x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z );
		glVertex3f( o_x+x, o_y+y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z +z);
		glVertex3f( o_x, o_y, o_z+z );
		glVertex3f( o_x, o_y+y, o_z +z);
		*/

	}
	glLineWidth(2);
	glEnd();



}
static void cuadrado_ejes(float o_x, float o_y,  float x,float y,int p_i,int p_f,int reshu)
{
	cuadrado_negro(o_x,o_y,x,y,p_i,p_f,reshu);
	float o_z=0;
	float z=0;
//	return;
	glLineWidth(2);
	glDisable(GL_LIGHTING);
glDisable(GL_LIGHTING);


 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


		//color
glColor3f(0, 0, 0);//print timer in red
double x1,y1,z1;
	int flag=p_f;
	                if(flag==1)		{   x1=p_f*0.1; y1=0.25;z1=0;}
 if(flag==2)		{   x1=p_f*0.2; y1=0.5;z1=0;}
 if(flag==3)		{   x1=1; y1=0.2*p_f;z1=0;}
 if(flag==4)		{   x1=0.8; y1=0.2*p_f;z1=0;}
  if(flag==5)		{   x1=0.6; y1=0;z1=0.2*p_f;}
 if(flag==6)		{   x1=0.4; y1=0;z1=0.2*p_f;}


  

   
if (flag>6){   x1=0.1*(p_f); y1=1-(0.01*p_f);z1=0.05*p_f;}


  
glBegin( GL_QUAD_STRIP  );{

//	if (reshu==0)
	glColor4f(x1,y1,z1,0.5f );
//	else
//		glColor4f(1,1,1,0.5f );


  //8
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z+z );

  //
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x+x, o_y, o_z);


   glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z);

 
  //
/*  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z +z);
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z +z);
*/

	}

glEnd();
	
}

static void ejes(float o_x, float o_y, float o_z, float x,float y, float z)
{
	glLineWidth(3);
glBegin(GL_LINES);{
  glColor3f(1,1,1);

  //8
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z+z );

  //
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x+x, o_y, o_z);


   glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z);

 
  //
/*  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x+x, o_y, o_z+z );
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z+z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z );
  glVertex3f( o_x+x, o_y+y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z +z);
  glVertex3f( o_x, o_y, o_z+z );
  glVertex3f( o_x, o_y+y, o_z +z);
*/

	}
glEnd();
}



/* Construye una caja con un vertice en o_X de dimensiones
*/
static void cuadrado(GLfloat o_x, GLfloat o_y,  GLfloat x, GLfloat y )
{

	GLfloat o_z=0;
	GLfloat z=0;
	//float color[4]={0+0.1*Id,1-0.1*Id,0.5+0.01*Id,1};
//	float color[4]={0,0,0,0};
	float color[4]={0,1,0.5,1};

 //  glShadeModel( GL_FLAT );

   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color );



   /* Caras transversales */
   glBegin( GL_QUAD_STRIP );{
glNormal3f( 0.0, 0.0, -1.0 );   /* Vertical hacia atras */

      glVertex3f( o_x+x, o_y+0, o_z+0 );
      glVertex3f( o_x+0, o_y+0, o_z+0 );


      glVertex3f( o_x+x, o_y+y, o_z+0 );
      glVertex3f( o_x+0, o_y+y, o_z+0 );

glNormal3f( 0.0, 1.0, 0.0 );       /* Superior, horizontal */

      glVertex3f( o_x+x, o_y+y,o_z+ z );
      glVertex3f( o_x+0, o_y+y, o_z+z );
glNormal3f( 0.0, 0.0, 1.0 );       /*Vertical delantera*/
      glVertex3f( o_x+x, o_y+0,o_z+ z );
      glVertex3f( o_x+0, o_y+0, o_z+z );

glNormal3f( 0.0, -1.0, 0.0 );       /*Inferior */
      glVertex3f(o_x+ x, o_y+0, o_z+0 );
      glVertex3f(o_x+ 0, o_y+0, o_z+0 );  }
    glEnd();

   /* Costados */
   glBegin( GL_QUADS );{
      glNormal3f( 1.0, 0.0, 0.0 );
      glVertex3f( o_x+x, o_y+0, o_z+0 );
      glVertex3f( o_x+x, o_y+y, o_z+0 );
      glVertex3f( o_x+x, o_y+y, o_z+z );
      glVertex3f( o_x+x, o_y+0, o_z+z ); }
   glEnd();


   glBegin( GL_QUADS );{
      glNormal3f( -1.0, 0.0, 0.0 );
      glVertex3f( o_x+0, o_y+0,o_z+ 0 );
      glVertex3f( o_x+0, o_y+0,o_z+ z );
      glVertex3f( o_x+0, o_y+y,o_z+ z );
      glVertex3f( o_x+0, o_y+y,o_z+ 0 ); }
   glEnd();


}
static void cubo(GLfloat o_x, GLfloat o_y, GLfloat o_z, GLfloat x, GLfloat y, GLfloat z,int Id )
{
	float color[4]={0+0.1*Id,1-0.1*Id,0.5+0.01*Id,1};
	

   glShadeModel( GL_FLAT );

   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color );



   /* Caras transversales */
   glBegin( GL_QUAD_STRIP );{
glNormal3f( 0.0, 0.0, -1.0 );   /* Vertical hacia atras */

      glVertex3f( o_x+x, o_y+0, o_z+0 );
      glVertex3f( o_x+0, o_y+0, o_z+0 );


      glVertex3f( o_x+x, o_y+y, o_z+0 );
      glVertex3f( o_x+0, o_y+y, o_z+0 );

glNormal3f( 0.0, 1.0, 0.0 );       /* Superior, horizontal */

      glVertex3f( o_x+x, o_y+y,o_z+ z );
      glVertex3f( o_x+0, o_y+y, o_z+z );
glNormal3f( 0.0, 0.0, 1.0 );       /*Vertical delantera*/
      glVertex3f( o_x+x, o_y+0,o_z+ z );
      glVertex3f( o_x+0, o_y+0, o_z+z );

glNormal3f( 0.0, -1.0, 0.0 );       /*Inferior */
      glVertex3f(o_x+ x, o_y+0, o_z+0 );
      glVertex3f(o_x+ 0, o_y+0, o_z+0 );  }
    glEnd();

   /* Costados */
   glBegin( GL_QUADS );{
      glNormal3f( 1.0, 0.0, 0.0 );
      glVertex3f( o_x+x, o_y+0, o_z+0 );
      glVertex3f( o_x+x, o_y+y, o_z+0 );
      glVertex3f( o_x+x, o_y+y, o_z+z );
      glVertex3f( o_x+x, o_y+0, o_z+z ); }
   glEnd();


   glBegin( GL_QUADS );{
      glNormal3f( -1.0, 0.0, 0.0 );
      glVertex3f( o_x+0, o_y+0,o_z+ 0 );
      glVertex3f( o_x+0, o_y+0,o_z+ z );
      glVertex3f( o_x+0, o_y+y,o_z+ z );
      glVertex3f( o_x+0, o_y+y,o_z+ 0 ); }
   glEnd();


}


void DibujaGL()
{
   static GLfloat pos[4] = {0, 0, 0, 0.0 };


	glPushMatrix();		// Apila la transformación geométrica actual

  // Fija el color de fondo a negro
  glClearColor(1,1,1,1);

  // Inicializa el buffer de color
  glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );


  // Fija el color de dibujo

  // Introduce la geometria
 	
 	glTranslatef(-view_roty, view_rotx,-D);

// Colocar aqui la luz si esta fija respecto del observador  	
    glLightfv( GL_LIGHT0, GL_POSITION, pos );

 	
 //	glRotatef( view_rotx, 1.0, 0.0, 0.0 );
  // 	glRotatef( view_roty, 0.0, 1.0, 0.0 );
   //	glRotatef( view_rotz, 0.0, 0.0, 1.0 );

// Colocar aqui la luz si esta fija en la escena   	
//    glLightfv( GL_LIGHT0, GL_POSITION, pos );

//  ejes(30);
	//Abro el fichero que voy a escribir
//		char string[200];
//	strcpy(string,"Sol\\Sol");
	//Primero buscamos la ultima ocurrencia de 
//	int ch='\\';
//	char *ultimo=strrchr( m_Nombre, ch );
//	strcpy(string,"C:\\afrank\\3dContainer\\Instancias\\BSGrasp\\");
//	strcat(string,ultimo+1);
//	ejes(0,0,0,200,200,200);/*
	
	//Leemos el valorW
	int valguardado=0;

	short int tipo;
	float abajo=0;


FILE *fin2;

	fin2=fopen("temp2.txt","r");
	if (fin2==NULL)
	{
		printf("Problemas al leer el archivo del Temp2, no se encuentra el archivo " );
		exit(4);
	}
	int H=0;
	int W=0;
	int plates=0;
	int piezas=0;
	int defectos = 0;
	//int valor=0; /* para la relajación*/
	int x=0,y=0,X=0,Y=0,plate=0,pieza=0,ini=1,plate_defectos=0,obj=0,width_total=0;
	fscanf(fin2,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",&W, &H, &plates, &piezas,&defectos,&plate_defectos,&obj,&width_total);

	float factor=__max((double)78/(double)(W*plates),(double)40/(double)(H));
//	factor=0.1;
		float izquierda=(-56-((H+2)*factor));

	float alto=34-((W+1)*factor);
	vector<int> res_no_constructivo(plate+1);
	for (int n=1;n<plate+1;n++)
		res_no_constructivo[n]=0;

	vector<int> res_no_constructivo_ac(plate+1);
	for (int h=1;h<plate+1;h++)
		res_no_constructivo_ac[h]=0;
	int plate2 = -1;
	ini = 1;
	int stack = 0;
	int seq = 0;

	izquierda = -48 ;
	glColor3f(0, 0, 0);
	glRasterPos2d(izquierda + 3000 * factor, 3310 * factor);
	texto(obj, -1);
	texto(width_total, 10);
	for (int k=0;k<piezas;k++)
	{
		fscanf(fin2,"%d %d %d %d %d %d %d %d",&pieza,&stack,&seq,&x,&y, &X,&Y,&plate);
		/*fscanf(fin2,"%d %d %d %d %d %d",&ini,&fin, &k,&i,&j,&reshuffle,&valor);*/
		izquierda = -48 + (((W + 100)*factor)*(plate - 1));
		if (plate2 != plate)
		{
			cuadrado_negro(izquierda + 0 * factor, 0 * factor, W*factor, H*factor, 0, 0, 2);
			izquierda = -48 + (((W + 2)*factor)*(plate-1));
			glColor3f(0, 0, 0);
			glRasterPos2d(izquierda + 2800 * factor, -100 * factor);
			texto(plate, -1);
		}
		izquierda = -48 + (((W + 100)*factor)*(plate - 1));
		plate2 = plate;
		vector <int> info_piece(5);
		info_piece[0] = pieza;
		info_piece[1] = X;
		info_piece[2] = Y;
		info_piece[3] = stack;
		info_piece[4] = seq;
		glColor3f(0, 0, 0);
		glRasterPos2d(izquierda + x*factor, y*factor+ (((Y - y) / 2) + 120)*factor);
//		texto(info_piece);
		texto(pieza, -1);
		glRasterPos2d(izquierda + x*factor, y*factor + (((Y - y) / 2))*factor);
		texto(X-x, Y-y);
		glRasterPos2d(izquierda + x*factor, y*factor + (((Y - y) / 2) - 120 )*factor);
		texto(stack, seq);
		cuadrado_ejes(izquierda+x*factor,y*factor, (X-x)*factor, (Y-y)*factor, pieza,stack,1);
	}

	int cont_defects_plate = 0;
	for (int k = 0; k<defectos; k++)
	{
		
		fscanf(fin2, "%d %d %d %d %d ", &x, &y, &X, &Y, &plate);
		izquierda = -48 + (((W + 100)*factor)*(plate ));
		defecto_rojo(izquierda+(x-15)*factor, (y-15)*factor, (X+30)*factor, (Y+30)*factor,0,plate,0);
	}
	for (int k = 0; k < plate_defectos; k++)
	{
		fscanf(fin2, "%d %d", &plate, &defectos);
		izquierda = -48 + (((W + 2)*factor)*(plate));
		glColor3f(0, 0, 0);
		glRasterPos2d(izquierda + 3000*factor, -100 * factor);
		texto(defectos, -1);

	}
	

/*	for (int h=1;h<N;h++)
		for (int n=1;n<h+1;n++)
			res_no_constructivo_ac[h]+=res_no_constructivo[n];
			*/
	/*
	//Escribir número
	glColor3f(0, 0, 0);
	izquierda=-48+(((W+2)*factor)*((plate/2)-1));
	//position
	glRasterPos2d(izquierda+(((W+2)*factor)/2), alto-factor);
	//qué poner
	string Result2;          
	ostringstream convert11;  
	convert11 << plate;      

	Result2 = "Reshuffles "+convert11.str();

		//escribirlo
		int len; 
		len = Result2.length();
		for (int i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Result2[i]);
		}*/







	fclose(fin2);

//  cubo(0,0,0,10,10,10);
 // cubo(0,10,10,10,10,10);
  	glPopMatrix(); // Desapila la transformación geométrica
  	
  glFlush();  // Fuerza el dibujo de las primitivas introducidas
  glutSwapBuffers();



}
void texto(vector <int> &info)
{
	//qué poner
	string Result1;
	string Result2;
	ostringstream convert0,convert1,convert2,convert3,convert4;
	convert0 << info[0];
	convert1 << info[1];
	convert2 << info[2];
	convert3 << info[3];
	convert4 << info[4];

	Result2 = "  "+convert0.str()
		+ " ( " + convert1.str() + ", " 
		+ convert2.str() + "\n" + convert3.str() + " s " + convert4.str();
		

	//escribirlo
	int len, i;
	len = Result2.length();
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, Result2[i]);
	}
}
void texto(int X, int Y)
{
	//qué poner
	string Result1;
	string Result2;
	ostringstream convert0, convert1;
	convert0 << X;
	
	
	convert1 << Y;

	if (Y == -1)
		Result2 = "  " + convert0.str();
	else
		Result2 = "  " + convert0.str()+ ", " + convert1.str();
		


	//escribirlo
	int len, i;
	len = Result2.length();
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, Result2[i]);
	}

}
/* ------------------------------------------------------------------------- */

void Ventana(GLsizei ancho,GLsizei alto)
{
	float calto; 	// altura de la ventana corregida
  // Establecemos el Viewport usando la nueva anchura y altura de la ventana X
  	glViewport(0,0,ancho,alto);

 // Especificamos la transformación de visualización 	
  	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	

  	if(ancho>0)
  		calto = 1*(GLfloat)alto/(GLfloat)ancho;
  	else
  		calto = 1;
  		
//  		glOrtho(-50,50,-calto, calto,0,1);
			glFrustum(-1,1,-calto,calto,5,250);

// A partir de este momento las transformaciones son de modelado  		
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();



}
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}
 

/* -------------------- FUNCIONES DE INTERACCIÓN ----------------------------------- */
 static void letra (unsigned char k, int x, int y)
{
  switch (k) {
  case 'h':
  case 'H':
  	printf("\n");
  	printf("\n Escape: Salir");
  	printf("\n\n\n");
  	break;
  case 'z':
    view_rotz += 5.0;
    break;
  case '+':
    D -= 1.0;
    break;
  case '-':
    D += 1.0;
    break;
  case 'Z':
    view_rotz -= 5.0;
    break;

  case 27:  /* Escape */
    exit(0);
	 	  break;
  default:
    return;
  }
  glutPostRedisplay();
}



 static void especial(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    view_rotx += 5.0;
    break;
  case GLUT_KEY_DOWN:
    view_rotx -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    view_roty -= 5.0;
    break;
  case GLUT_KEY_RIGHT:
    view_roty += 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

 void seleccionMenu( int opcion )
{
   switch ( opcion )
   {
   case ZOOM:
      MODORaton=MOVIENDO;
      break;
   case GIRAR:
      MODORaton=GIRANDO;
      break;
   case SALIR:
	   //
	  // return;
   	  exit(0);
   }
}

// static void RatonMovido( int x, int y );

 void clickRaton( int boton, int estado, int x, int y )
{
	
   	if(boton== GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {

   		if( ~(RatonPulsado)){
      		Xref=x;
      		Yref=y;
      		RatonPulsado=GL_TRUE;
      		}
      	}
	else RatonPulsado=GL_FALSE;
	
}



 static void RatonMovido( int x, int y )
 // Esta función mueve la camara por el escenario
/**/ // Dependiendo del modo mueve o gira la camara
{
if(RatonPulsado)     {
	if(MODORaton==MOVIENDO) {
   		D = 100.0*(Yref-y-150);
		}
	else{
		view_rotx += (Yref -y-150) / 10;
   		view_roty += (Xref -x-150) / 10;
   		}
	glutPostRedisplay();
	}


}

 void CreaMenu()
{

   menu = glutCreateMenu( seleccionMenu );

   glutAddMenuEntry( "Zoom", ZOOM );
   glutAddMenuEntry( "Girar", GIRAR );
   glutAddMenuEntry( "Salir", SALIR );

	MODORaton = MOVIENDO;

   glutAttachMenu( GLUT_RIGHT_BUTTON );
   glutMouseFunc( clickRaton );
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable( GL_LIGHTING );
   glEnable( GL_LIGHT0 );

}



/* ------------------------------------------------------------------------- */




 int DibujarOpenGL( )
{
	 int foo = 1;
	 char * bar[1] = { " " };
	 glutInit(&foo, bar);
	 //	}
	 glutInitWindowPosition(50, 50);
	 glutInitWindowSize(1000, 500);
	 
	// glutInitWindowSize(1600, 800);
	 glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	 
	 char name1[200];
	 char *intStr = itoa(10, name1, 10);
	 std::string str = string(intStr);
	 strcat(intStr, " Roadef visualization");

	 int  Win2 = glutCreateWindow(" Roadef visualization");
//	 glutFullScreen();

	 // Inicializa las funciones de dibujo y cambio de tamaño de la ventana X
	 //   if (stop == false)
	 
	 glutSetWindow(Win2);
	 glutDisplayFunc(DibujaGL);


	
//	 glutFullScreen();
	 //  else return 0;
	 //  if (stop == false)	

	 glutReshapeFunc(Ventana);
	 //  else return 0;
	
	 //	if (stop == false)
	 //	{
	 //		if (stop == true)
	 //		glutDestroyWindow(Win2);
	 // FUNCIONES DE INTERACCIÓN
	 //		if (stop == false)
	 CreaMenu();
	 //		else return 0;
	 //		if (stop == false) 
	 glutKeyboardFunc(letra);
	 //		else return 0;
	 //		if (stop == false) 
	 glutSpecialFunc(especial);
	 //		else return 0;
	 //		if (stop == false) 
	 glutPassiveMotionFunc(RatonMovido);
	 //		else return 0;
	 //		if (stop == false) 
	 //			if (stop == true) glutLeaveMainLoop();
	 //				glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	 glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	 //			std::cout << "Before glutMainLoop()!" << std::endl;
	 glutMainLoop();

	 //		std::cout << "Back in main()!" << std::endl;

	 //			glutMainLoop();
	 //		else return 0;
	 //	}
	 return 0;
}
