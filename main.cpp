
/****************************************************************************************************

    CarGL (main.cpp)

    Grado en Ingenier�a Multimedia.
    Pr�ctica de Gr�ficos por Computador.
    OpenGL con Shaders.
  ------------------------------------------------------------------------------------------

    CarsGL (C) 2012-2016 - Juan Antonio Puchol Garc�a (puchol@dccia.ua.es)

    Changelog:

    Versi�n 1.0 (Noviembre 2012)
        - Versi�n inicial para Code::Blocks 10.05

    Versi�n 1.1 (Noviembre 2012)
        - Arreglado el bug de selecci�n incorrecta debido al panel inferior de controles

    Versi�n 1.2 (Octubre 2013 )
        - Adaptaci�n de la librer�a GLUI a Code::Blocks 12.11 y 13.12

    Versi�n 1.3 (Octubre 2014 )
        - Adaptaci�n de la librer�a GLUI a Code::Blocks 13.12

    Versi�n 2.0 (Octubre 2015 )
        - Adaptaci�n del c�digo empleando Shaders (Modern OpenGL Graphics)

    Versi�n 2.1 (Septiembre 2016 )
        - Modificaci�n de los scrollbars de las luces para manipular la posisi�n (compatible con Code::Blocks 16.01)

    NOTA:   Para que esta aplicaci�n compile se necesita tener copiados los
            siguientes ficheros desde la carpeta "Para Copiar":

            glut32.dll  -> C:\Windows\system

            Si es un Windows de 32 bits:
            glew32s.lib -> C:\Program Files\CodeBlocks\MinGW\lib
            glew32.lib  -> C:\Program Files\CodeBlocks\MinGW\lib
            libglui32.a -> C:\Program Files\CodeBlocks\MinGW\lib
            glut32.lib  -> C:\Program Files\CodeBlocks\MinGW\lib

            glui.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL
            glut.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL
            glew.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL

            y la carpeta completa
            glm         -> C:\Program Files\CodeBlocks\MinGW\include

            Si es un Windows de 64 bits:
            glew32s.lib -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            glew32.lib  -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            libglui32.a -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            glut32.lib  -> C:\Program Files (x86)\CodeBlocks\MinGW\lib

            glui.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL
            glut.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL
            glew.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL

            y la carpeta completa
            glm         -> C:\Program Files (x86)\CodeBlocks\MinGW\include


            ACLARACI�N:
                Hay varias librer�as libglui32.a en la carpeta "otros"
                - Renombrar libglui32.10.05.a a libglui32.a para usar Code::Blocks 10.05
                - Renombrar libglui32.12.11.a a libglui32.a para usar Code::Blocks 12.11
                - Renombrar libglui32.13.12.a a libglui32.a para usar Code::Blocks 13.12 o superior

****************************************************************************************************/

#include "Objects.h"
#include <GL\glui.h>
#include <cmath>

#define GIRO 8
#define VMAX 4.2
#define STOP 0.25
#define ROZAMIENTO 0.01
#define GAS 0.15
/**************************************** myGlutKeyboard() **********/
bool arraymolon[256];
float VELOZ=0;

void Keyboard(unsigned char Key, int x, int y){
    switch(Key){
        case 27:
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}

/**************************************** mySpecialKey() *************/

static inline int8_t sgn(float val) {
 if (val < 0) return -1;
 if (val==0) return 0;
 return 1;
}

static void movement(){
    if(escena.seleccion!=0){
        TPrimitiva *car = escena.GetCar(escena.seleccion);
        float* init;
        init = escena.getCamearInit();
std::cout<<"view_position["<< std::endl;
        if(!arraymolon['s'] && (arraymolon['w'] || VELOZ>0)){
            if(arraymolon['w'] &&  VELOZ<0)
                VELOZ += STOP;
            else if(arraymolon['w'] && VELOZ<VMAX)
                VELOZ += GAS;
        }
        if(!arraymolon['w'] && (arraymolon['s'] || VELOZ<0)){
            if(arraymolon['s'] &&  VELOZ>0)
                VELOZ -= STOP;
            else if(arraymolon['s'] && VELOZ>-VMAX)
                VELOZ -= GAS;
        }

        if(arraymolon['a'] ){
            if((car->rry)<16)
                car -> rry+=8;
        }
        else if(arraymolon['d']){
            if((car->rry)>-16)
                car -> rry-=8;
        }

        car->ry += GIRO*(VELOZ/VMAX) * sgn(car->rry);
        car->rr -= 20*(VELOZ/VMAX);
        car->tz -= std::cos(glm::radians(car->ry))*VELOZ;
        car->tx -= std::sin(glm::radians(car->ry))*VELOZ;

        if(!arraymolon['w'] && !arraymolon['s'] && VELOZ!=0){
            if(VELOZ>0.001)
                VELOZ -= ROZAMIENTO;
            else if(VELOZ<-0.001)
                VELOZ += ROZAMIENTO;

            if(VELOZ>-0.001 && VELOZ<0.001) VELOZ=0;
        }

        if(!arraymolon['a'] && car->rry>0)
            car -> rry-=8;
        else if(!arraymolon['d'] && car->rry<0)
            car -> rry+=8;

        glutPostRedisplay();
    }

}

static void keyDown(unsigned char key, int x, int y){
    arraymolon[key]=true;
}

static void keyUp(unsigned char key, int x, int y){
    arraymolon[key]=false;
}

/***************************************** myGlutMenu() ***********/

void Menu( int value )
{
  Keyboard( value, 0, 0 );
}

void Mouse(int button, int button_state, int x, int y )
{
    gui.Mouse(button, button_state, x, y);
}

void Render()
{
    escena.Render();
    movement();
}

void Idle()
{
    gui.Idle();
}

void Reshape(int x, int y){
    gui.Reshape(x, y);
}

void Motion(int x, int y){
    gui.Motion(x, y);
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    // Inicializa GLUT and crea la ventana principal
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE );
    glutInitWindowPosition( 100, 100 );
    glutInitWindowSize( 1400, 800 );

    int main_window = glutCreateWindow( "CarGL V2.0 (2015) con Shaders" );

    // Inicializa los valores de OpenGL para esta Aplicaci�n
    escena.InitGL();
    gui.Init(main_window);

    glutDisplayFunc( Render );
    GLUI_Master.set_glutReshapeFunc( Reshape );
    GLUI_Master.set_glutKeyboardFunc( Keyboard );
    //GLUI_Master.set_glutSpecialFunc( Keys );

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    GLUI_Master.set_glutMouseFunc( Mouse );
    glutMotionFunc( Motion );

    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( Idle );

    // Crea los objetos
    TPrimitiva *road = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);
    TPrimitiva *car1 = new TPrimitiva(1, COCHE_ID);
    TPrimitiva *car2 = new TPrimitiva(2, COCHE_ID);
    TPrimitiva *rocks = new TPrimitiva(3, 20);
    TPrimitiva *lights = new TPrimitiva(LIGHTS_ID,LIGHTS_ID);
    TPrimitiva *building = new TPrimitiva(BUILDINGS_ID,BUILDINGS_ID);

    car2->tx = -10;
/*
    car2->colores[0][0] = 0.3;
    car2->colores[0][1] = 0.8;
    car2->colores[0][2] = 0.4;
    car2->colores[0][3] = 1.0;
    car2->tx = 2;
    car2->tz = 3;

    base->colores[0][0] = 0.3;
    base->colores[0][1] = 0.8;
    base->colores[0][2] = 0.4;
    base->colores[0][3] = 1.0;
*/
    escena.AddObject(road);
    escena.AddCar(car1);
    escena.AddCar(car2);
    escena.AddObject(rocks);
    escena.AddObject(lights);
    escena.AddObject(building);
    /**** Regular GLUT main loop ****/
    glutMainLoop();

    return EXIT_SUCCESS;
}

