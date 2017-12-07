
/****************************************************************************************************

    CarGL (main.cpp)

    Grado en Ingeniería Multimedia.
    Práctica de Gráficos por Computador.
    OpenGL con Shaders.
  ------------------------------------------------------------------------------------------

    CarsGL (C) 2012-2016 - Juan Antonio Puchol García (puchol@dccia.ua.es)

    Changelog:

    Versión 1.0 (Noviembre 2012)
        - Versión inicial para Code::Blocks 10.05

    Versión 1.1 (Noviembre 2012)
        - Arreglado el bug de selección incorrecta debido al panel inferior de controles

    Versión 1.2 (Octubre 2013 )
        - Adaptación de la librería GLUI a Code::Blocks 12.11 y 13.12

    Versión 1.3 (Octubre 2014 )
        - Adaptación de la librería GLUI a Code::Blocks 13.12

    Versión 2.0 (Octubre 2015 )
        - Adaptación del código empleando Shaders (Modern OpenGL Graphics)

    Versión 2.1 (Septiembre 2016 )
        - Modificación de los scrollbars de las luces para manipular la posisión (compatible con Code::Blocks 16.01)

    NOTA:   Para que esta aplicación compile se necesita tener copiados los
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


            ACLARACIÓN:
                Hay varias librerías libglui32.a en la carpeta "otros"
                - Renombrar libglui32.10.05.a a libglui32.a para usar Code::Blocks 10.05
                - Renombrar libglui32.12.11.a a libglui32.a para usar Code::Blocks 12.11
                - Renombrar libglui32.13.12.a a libglui32.a para usar Code::Blocks 13.12 o superior

****************************************************************************************************/

#include "Objects.h"
#include <GL\glui.h>
#include <cmath>
#include "loadjpeg.c"
/**************************************** myGlutKeyboard() **********/
bool arraymolon[256];

void Keyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 27:
        case 'q':
            exit(0);
            break;

    }

    glutPostRedisplay();
}

/**************************************** mySpecialKey() *************/
static void movement(){
    TPrimitiva *car = escena.GetCar(escena.seleccion);
    float* init;
    init = escena.getCamearInit();

    if(arraymolon['w']){
        car->rr-=8;
        //car->tz -=0.9;
        std::cout << "Z "<<cos(car->ry)*0.9<< std::endl;
        car->tz -= std::cos(car->ry)*0.9;
        if(arraymolon['a']){
            car->ry -= 8;
            if((car->rry)>-16)
                car -> rry-=8;
            std::cout << "A: " << sin(car->ry) << " |RRY: "<< car->rry<<  std::endl;
            car->tx -= std::abs(sin(car->ry)*0.9);
        }

        else if(arraymolon['d']){
            if((car->rry)<16)
                car -> rry+=8;
            car->tx += std::abs(sin(car->rry)*0.9);
        }
        escena.view_position[2]=init[2]-(car->tz);

    }
    else if(arraymolon['s']){
        car->tz += std::abs(cos(car->rry)*0.9);
        escena.view_position[2]=init[2]-(car->tz);
    }
    /*
    switch(a){
        case 'w':
            car->rr-=8;
            car->tz -=0.9;
            escena.view_position[2]=init[2]-(car->tz);
            break;
        case 's':
            car->rr+=8;
            car->tz += 0.9;
            escena.view_position[0]=car->tx;
            escena.view_position[2]=init[2]-(car->tz);
            break;
    }
    */
    glutPostRedisplay();
}

static void keyDown(unsigned char key, int x, int y){
    arraymolon[key]=true;
}

static void keyUp(unsigned char key, int x, int y)
{
    arraymolon[key]=false;
    /*
    TPrimitiva *car = escena.GetCar(escena.seleccion);
    float* init;
    init = escena.getCamearInit();

    switch (key)
    {
        case GLUT_KEY_UP:   // El coche avanza
            car->rr-=8;
            car->tz -= std::abs(cos(car->rry)*0.9);
            car->tz -=0.9;
            //escena.view_position[0]=car->tx;

            escena.view_position[2]=init[2]-(car->tz);

            //std::cout<< "viewposition[2]"<<escena.view_position[2] << std::endl;
            /*
            cameraPos = glm::vec3(0.0f, 20.0f, 3.0f);
            cameraFront = glm::vec3(car->tx, car->ty, car->tz);
            cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
            escena.viewMatrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
            gluLookAt(0,20,3,car->tx, car->ty, car->tz,0,1,0);

            break;
        case GLUT_KEY_DOWN:   // El coche retrocede
            car->rr+=8;
            car->tz += 0.9;
            escena.view_position[0]=car->tx;
            escena.view_position[2]=init[2]-(car->tz);
            break;
        case GLUT_KEY_RIGHT:
            if((car->rry)>-16)
                car -> rry-=8;
            std::cout<< "RRY: "<<car->rry << std::endl;
            break;
        case GLUT_KEY_LEFT:
            if((car->rry)<16)
                car -> rry+=8;
            break;
    }
    */

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
    glutInitWindowPosition( 250, 250 );
    glutInitWindowSize( 1400, 800 );

    int main_window = glutCreateWindow( "CarGL V2.0 (2015) con Shaders" );

    // Inicializa los valores de OpenGL para esta Aplicación
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
/*
    GLuint texturas[10];

    glGenTextures(10, texturas);

    unsigned char* pixeles;
    int ancho, alto;
    ancho = alto = 0;

    pixeles = LoadJPEG("./../../Modelos/square.jpg", &ancho, &alto);
    glBindTexture(GL_TEXTURE_2D, texturas[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, pixeles);
*/

    // Crea los objetos
    TPrimitiva *road = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);
    TPrimitiva *car1 = new TPrimitiva(1, COCHE_ID);
    TPrimitiva *rocks = new TPrimitiva(3, 20);
    TPrimitiva *lights = new TPrimitiva(LIGHTS_ID,LIGHTS_ID);
    TPrimitiva *building = new TPrimitiva(BUILDINGS_ID,BUILDINGS_ID);
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
    escena.AddObject(rocks);
    escena.AddObject(lights);
    escena.AddObject(building);
    /**** Regular GLUT main loop ****/
    glutMainLoop();

    return EXIT_SUCCESS;
}

