
/****************************************************************************************

    CarGL (Objects.cpp)

    Grado en Ingeniería Multimedia.
    Práctica de Gráficos por Computador.
    OpenGL con Shaders.
  ---------------------------------------------------------------------------------------

    Noviembre 2012 - Septiembre 2016 (C) Juan Antonio Puchol García (puchol@dccia.ua.es)


*****************************************************************************************/

#include "Objects.h"
#include <GL/glui.h>
#include <iostream>
#include <math.h>
#include "loadjpeg.c"

#include "load3ds.c"
using namespace std;
// Variable para inicializar los vectores correpondientes con los valores iniciales
GLfloat light0_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light0_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light0_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light0_position_c[4] = {0.0f, 100.0f, -200.0f, 1.0f };

GLfloat light1_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light1_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light1_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light1_position_c[4] = {   0.0f, 100.0f,  -200.0f, 1.0f };

GLfloat mat_ambient_c[4]    = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat mat_diffuse_c[4]    = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat mat_specular_c[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mat_shininess_c[1] = { 100.0f };

// Matriz de 4x4 = (I)
float view_rotate_c[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_position_c[3] = { 0.0, 10, 15 };

float coloresc_c[2][4] = { {0.8, 0.5, 0.0, 1.0}, {0.5, 0.5, 0.5, 1.0}}; // Color del coche
float coloresr_c[2][4] = { {0.3, 0.3, 0.3, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de la carretera
float coloresr_o[2][4] = {
        {0.223529412, 0.282352941, 0.37254902, 1.0}, //Azul
        {1.0, 1.0, 1.0, 1.0}
    };

//************************************************************** Variables de clase

TEscena escena;
TGui    gui;

//************************************************************** Clase TPrimitiva

TPrimitiva::TPrimitiva(int DL, int t)
{
    ID   = DL;
    tipo = t;
    float constante_c = 1.0f/255;
    std::cout<< "WTF"<<std::endl;
    tx = ty = tz = 0;
    sx = sy = sz = 1;
    rx = ry = rz = rr = rry = 0;

	switch (tipo) {
		case CARRETERA_ID: {  // Creación de la carretera
		    tx = ty = tz = 0;
            memcpy(colores, coloresr_o, 8*sizeof(float));
            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/base.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/base2.3ds", &num_vertices1);
            break;
		}
		case COCHE_ID: { // Creación del coche
            tx = ty = tz = 0;

		    memcpy(colores, coloresc_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/Tron.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/Wheel.3ds", &num_vertices1);

            break;
		}
		case 20:{
		    memcpy(colores, coloresr_c, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/Rocks.3ds", &num_vertices0);
            break;
		}
		case LIGHTS_ID:{
		    tx = ty = tz = 0;
            memcpy(colores, coloresr_o, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/Floating_lamps.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/Simple_lamp.3ds", &num_vertices1);
            modelo2 = Load3DS("../../Modelos/Perfect_lamp.3ds", &num_vertices2);
            break;
		}
        case BUILDINGS_ID: {
            tx = ty = tz = 0;
            memcpy(colores, coloresr_o, 8*sizeof(float));
            modelo0 = Load3DS("../../Modelos/Minicity.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/Bridge.3ds", &num_vertices1);
            modelo3 = Load3DS("../../Modelos/Factory.3ds", &num_vertices3);
            modelo4 = Load3DS("../../Modelos/Neon_tower.3ds", &num_vertices4);
            modelo5 = Load3DS("../../Modelos/Quad_tower.3ds", &num_vertices5);
            modelo6 = Load3DS("../../Modelos/Central_tower.3ds", &num_vertices6);
            modelo7 = Load3DS("../../Modelos/Big_tower1.3ds", &num_vertices7);
            modelo8 = Load3DS("../../Modelos/Big_tower2.3ds", &num_vertices8);
            modelo9 = Load3DS("../../Modelos/Tunel.3ds", &num_vertices9);
            break;
        }
	} // switch
}

void __fastcall TPrimitiva::Render(int seleccion, bool reflejo)
{

    float aux_x, aux_y, aux_z;
    glm::mat4   modelMatrix;
    glm::mat4   modelViewMatrix;
    glActiveTexture(GL_TEXTURE0);
    glUniform1f(escena.uTextureUnitLocation, 0);

    switch (tipo) {
        case LIGHTS_ID:{
            modelMatrix     = glm::mat4(1.0f);
            modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));
            modelViewMatrix = escena.viewMatrix * modelMatrix;
            glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

            glBindTexture(GL_TEXTURE_2D, escena.texturas[0]);

            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[1]);
            // Asociamos los vértices y sus normales
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices0);


            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[1]);
            // Asociamos los vértices y sus normales
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[1]);
            // Asociamos los vértices y sus normales
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo2);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo2+3);

            glDrawArrays(GL_TRIANGLES, 0, num_vertices2);
            break;
        }
        case 20:{

                modelMatrix     = glm::mat4(1.0f);
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));
                modelViewMatrix = escena.viewMatrix * modelMatrix;

                glBindTexture(GL_TEXTURE_2D, escena.texturas[7]);
                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
                glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+6);
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

            break;
        }
        case CARRETERA_ID: {
            if (escena.show_road) {
                // Cálculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glBindTexture(GL_TEXTURE_2D, escena.texturas[4]);

                //                   Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
                glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+6);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

                //                   Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

            }
            break;
        }

        case COCHE_ID: {

            if (escena.show_car) {
                //std::cout << "Pick ["<<glm::vec4(escena.pick/255.0)[0] <<"]"<< std::endl;

                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
                glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+6);
                // Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix,(float)glm::radians(rx), glm::vec3(1,0,0));
                modelMatrix     = glm::rotate(modelMatrix,(float)glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::rotate(modelMatrix,(float)glm::radians(rz), glm::vec3(0,0,1));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                if(ID==1)
                    glBindTexture(GL_TEXTURE_2D, escena.texturas[0]);
                else if(ID==2)
                    glBindTexture(GL_TEXTURE_2D, escena.texturas[1]);

                // Envía nuestra ModelView al Vertex Shader

                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }


            if (escena.show_wheels)
            {
                // Asociamos los vértices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);
                glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+6);
                // RUEDA Delantera Derecha : Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty+0.73, tz));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(cos(ry*M_PI/180)*1.65 -sin(ry*M_PI/180)*6.45, 0, -cos(ry*M_PI/180)*6.45 - sin(ry*M_PI/180)*1.65));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rry+ry), glm::vec3(0,1,0));      // en radianes
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));      // en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                glBindTexture(GL_TEXTURE_2D, escena.texturas[1]);

                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Trasera Derecha : Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty+0.73, tz));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(cos(ry*M_PI/180)*1.75, 0, -sin(ry*M_PI/180)*1.75));

                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));      // en radianes
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));


                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Delantera Izquierda : Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty+0.73, tz));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(-cos(ry*M_PI/180)*1.65 -sin(ry*M_PI/180)*6.45, 0, -cos(ry*M_PI/180)*6.45 + sin(ry*M_PI/180)*1.65));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rry+ry), glm::vec3(0,1,0));      // en radianes
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(180.0), glm::vec3(0,1,0));

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);


                // RUEDA Trasera Izquierda : Cálculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty+0.73, tz));
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(-cos(ry*M_PI/180)*1.75, 0, sin(ry*M_PI/180)*1.75));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));      // en radianes
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1,0,0));

                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(180.0), glm::vec3(0,1,0));
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                break;
            }
        }

        case BUILDINGS_ID:{
            modelMatrix     = glm::mat4(1.0f);
            modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));
            modelViewMatrix = escena.viewMatrix * modelMatrix;

            glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);


            glBindTexture(GL_TEXTURE_2D, escena.texturas[6]);
            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

            glBindTexture(GL_TEXTURE_2D, escena.texturas[2]);
            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

            glBindTexture(GL_TEXTURE_2D, escena.texturas[6]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo3);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo3+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo3+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices3);

            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo4);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo5+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo5+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices4);

            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo5);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo5+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo5+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices5);

            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo6);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo6+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo6+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices6);

            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo7);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo7+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo7+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices7);

            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo8);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo8+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo8+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices8);

            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo9);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo9+3);
            glVertexAttribPointer(escena.aUVLocation, UV_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo9+6);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices9);
            break;
        }

    } // switch

}
void __fastcall TPrimitiva::RenderPick(void)
{
    glm::mat4   modelMatrix;
    glm::mat4   modelViewMatrix;
    modelMatrix     = glm::mat4(1.0f); // matriz identidad
    modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));
    modelViewMatrix = escena.viewMatrix * modelMatrix;

    glUniform1i(escena.uColorLocationPick,escena.pick);
    glVertexAttribPointer(escena.aPositionLocationPick, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
    glUniformMatrix4fv(escena.uMVMatrixLocationPick, 1, GL_FALSE, &modelViewMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

}
//************************************************************** Clase TEscena

TEscena::TEscena() {

    seleccion = 1;
    num_objects = 0;
    num_cars = 0;

    show_car = 1;
    show_wheels = 1;
    show_road = 1;

    // live variables usadas por GLUI en TGui
    wireframe = 0;
    z_buffer = 1;
    culling = 0;

    scale = 1.0;
    xy_aspect = 1;
    last_x = 0;
    last_y = 0;

    vista = glm::lookAt(glm::vec3(0,10,15),glm::vec3(0,2,0), glm::vec3(0,1,0));
    pick=0;
    memcpy(view_position, view_position_c, 3*sizeof(float));
    memcpy(view_rotate, view_rotate_c, 16*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));
    memcpy(light0_diffuse, light0_diffuse_c, 4*sizeof(float));
    memcpy(light0_specular, light0_specular_c, 4*sizeof(float));
    memcpy(light0_position, light0_position_c, 4*sizeof(float));

    memcpy(light1_ambient, light1_ambient_c, 4*sizeof(float));
    memcpy(light1_diffuse, light1_diffuse_c, 4*sizeof(float));
    memcpy(light1_specular, light1_specular_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));

    memcpy(mat_ambient, mat_ambient_c, 4*sizeof(float));
    memcpy(mat_diffuse, mat_diffuse_c, 4*sizeof(float));
    memcpy(mat_specular, mat_specular_c, 4*sizeof(float));
    memcpy(mat_shininess, mat_shininess_c, 1*sizeof(float));
}

void __fastcall TEscena::InitGL()
{
    int tx, ty, tw, th;

    // Habilita el z_buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Inicialización de GLEW
    std::cout << "Inicializando GLEW" << std::endl << std::endl;
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendedor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Carga de los Shaders
    std::cout << std::endl << "Cargando Shaders" << std::endl;

    Shader shader;

    std::vector<GLuint> shaders;
    shaders.push_back(shader.LoadShader("../../Shaders/VertexShader.glsl", GL_VERTEX_SHADER));
    //std::cout << "Vertex Shader: " << shader.ReturnShaderID() << std::endl;
    shaders.push_back(shader.LoadShader("../../Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER));
    //std::cout << "Fragment Shader: " << shader.ReturnShaderID() << std::endl;
    shaderProgram = new Program(shaders);

    //std::cout << "Shader Program: " << shaderProgram->ReturnProgramID() << std::endl;

    glUseProgram(shaderProgram->ReturnProgramID());
    //glValidateProgram(shaderProgram->ReturnProgramID());

    aPositionLocation=shaderProgram->attrib(A_POSITION);
    aNormalLocation=shaderProgram->attrib(A_NORMAL);
    aUVLocation = shaderProgram->attrib(A_UV);

    uTextureUnitLocation=shaderProgram->uniform(U_TEXTUREUNIT);
    uProjectionMatrixLocation=shaderProgram->uniform(U_PROJECTIONMATRIX);
    uMVMatrixLocation=shaderProgram->uniform(U_MVMATRIX);
    uVMatrixLocation=shaderProgram->uniform(U_VMATRIX);
    //uColorLocation=shaderProgram->uniform(U_COLOR);
    uLuz0Location=shaderProgram->uniform(U_LUZ0);

    /*
    std::cout << "a_Position Location: " << aPositionLocation << std::endl;
    std::cout << "a_Normal Location: " << aNormalLocation << std::endl;

    std::cout << "u_ProjectionMatrix Location: " << uProjectionMatrixLocation << std::endl;
    std::cout << "u_MVMatrix Location: " << uMVMatrixLocation << std::endl;
    std::cout << "u_VMatrix Location: " << uVMatrixLocation << std::endl;
    std::cout << "u_Color Location: " << uColorLocation << std::endl;
    std::cout << "u_Luz0 Location: " << uLuz0Location << std::endl;

    */
    // Habilitamos el paso de attributes
    glEnableVertexAttribArray(aPositionLocation);
    glEnableVertexAttribArray(aNormalLocation);
    glEnableVertexAttribArray(aUVLocation);


    Shader shader2;
    std::vector<GLuint> shaders2;
    shaders2.push_back(shader2.LoadShader("../../Shaders/VertexShaderPick.glsl", GL_VERTEX_SHADER));
    shaders2.push_back(shader2.LoadShader("../../Shaders/FragmentShaderPick.glsl", GL_FRAGMENT_SHADER));
    shaderProgramPick = new Program(shaders2);

    glUseProgram(shaderProgramPick->ReturnProgramID());

    aPositionLocationPick=shaderProgramPick->attrib(A_POSITION);

    uMVMatrixLocationPick=shaderProgramPick->uniform(U_MVMATRIX);
    uProjectionMatrixLocationPick=shaderProgramPick->uniform(U_PROJECTIONMATRIX);
    uColorLocationPick=shaderProgramPick->uniform(U_COLOR);

    glEnableVertexAttribArray(aPositionLocationPick);


    // Estableciendo la matriz de proyección perspectiva
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    xy_aspect = (float)tw / (float)th;
    projectionMatrix = glm::perspective(45.0f, xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uProjectionMatrixLocationPick, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUseProgram(shaderProgram->ReturnProgramID());
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glGenTextures(10, escena.texturas);
    LoadTexture("../../Texturas/Only_Car2.jpg", 0);
    LoadTexture("../../Texturas/Wheel.jpeg", 1);
    LoadTexture("../../Texturas/Bridge.jpeg", 2);
    LoadTexture("../../Texturas/Minicity.jpg", 3);
    LoadTexture("../../Texturas/Base.jpg", 4);
    LoadTexture("../../Texturas/Central.jpg", 5);
    LoadTexture("../../Texturas/DEFAULT.jpg", 6);
    LoadTexture("../../Texturas/Rocks.jpg", 7);
}

void TEscena::LoadTexture(const char* path, unsigned char p){
    unsigned char* pixeles;
    int ancho, alto;
    ancho = alto = 0;

    pixeles = LoadJPEG(path, &ancho, &alto);
    glBindTexture(GL_TEXTURE_2D, escena.texturas[p]);
    //glColor4f(1.0, 1.0, 1.0, 1.0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, ancho, alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixeles);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(pixeles);
}

/************************** TEscena::AddCar(TPrimitiva *car) *****************/

void __fastcall TEscena::AddCar(TPrimitiva *car)
{
    cars[num_cars] = car;
    num_cars++;
}

/******************** TEscena::AddObject(TPrimitiva *object) *****************/

void __fastcall TEscena::AddObject(TPrimitiva *object)
{
    objects[num_objects] = object;
    num_objects++;
}

/******************** TPrimitiva *TEscena::GetCar(int id) ********************/

TPrimitiva __fastcall *TEscena::GetCar(int id)
{
    TPrimitiva *p=NULL;

    for (int i=0; i<num_cars; i++)
        if (cars[i]->ID==id)
            p = cars[i];

    return(p);
}

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderCars(bool reflejo) {

    for (int i=0; i<num_cars; i++)
    {
        cars[i]->Render(seleccion, reflejo);
    }
}

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderObjects(bool reflejo) {

    for (int i=0; i<num_objects; i++)
    {
        objects[i]->Render(seleccion, reflejo);
    }
}

/***************************************** TEscena::Render() *****************/

void __fastcall TEscena::Render(){
    glm::mat4 rotateMatrix;
    //std::cout<<"view_position["<< view_position[0]<<"][" << view_position[1]<<"][" << view_position[2]<< "]"<< endl;
    glClearColor(0.0, 0.7, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Cálculo de la vista (cámara)


    switch(gui.sel2){
        case 0: viewMatrix      = vista;
                rotateMatrix    = glm::make_mat4(view_rotate);
                //viewMatrix      = glm::translate(viewMatrix,glm::vec3(view_position[0], view_position[1], view_position[2]));
                viewMatrix      = viewMatrix*rotateMatrix;
                viewMatrix      = glm::scale(viewMatrix,glm::vec3(scale, scale, scale));
                break;

        case 1: if(escena.seleccion!=0)
                    viewMatrix = glm::lookAt(glm::vec3(escena.cars[escena.seleccion-1]->tx+50,100,-50 + escena.cars[escena.seleccion-1]->tz), glm::vec3(escena.cars[escena.seleccion-1]->tx,escena.cars[escena.seleccion-1]->ty,escena.cars[escena.seleccion-1]->tz), glm::vec3(0,1,0));
                break;

        case 2: if(escena.seleccion!=0){
                    float x = escena.cars[escena.seleccion-1]->tx - view_position_c[0]*cos(glm::radians(escena.cars[escena.seleccion-1]->ry)) + sin(glm::radians(escena.cars[escena.seleccion-1]->ry))*view_position_c[2];
                    float z = escena.cars[escena.seleccion-1]->tz + cos(glm::radians(escena.cars[escena.seleccion-1]->ry))*view_position_c[2] + sin(glm::radians(escena.cars[escena.seleccion-1]->ry))*view_position_c[0];
                    viewMatrix = glm::lookAt(glm::vec3(x+2,5,z), glm::vec3(escena.cars[escena.seleccion-1]->tx,escena.cars[escena.seleccion-1]->ty,escena.cars[escena.seleccion-1]->tz), glm::vec3(0,1,0));
                }
                break;

    }

    glUniform1i(uLuz0Location, gui.light0_enabled);
    glUniformMatrix4fv(uVMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // Para la luz matrix view pero sin escalado!

    // Dibujar carretera y objetos
    RenderObjects(seleccion);

    // Dibujar coches
    RenderCars(seleccion);

    glutSwapBuffers();
}

float* TEscena::getCamearInit(){
    return view_position_c;
}

// Selecciona un objeto a través del ratón
void __fastcall TEscena::Pick3D(int mouse_x, int mouse_y)
{
    //std::cout<<"COORDENADAS - X " << mouse_x<< "  Y "<< mouse_y<< std::endl;
    unsigned char res[4];
    GLint viewport[4];
    glUseProgram(shaderProgramPick->ReturnProgramID());
    renderSelection(viewMatrix);

    glGetIntegerv(GL_VIEWPORT, viewport);
    //std::cout<<"VIEWPORT["<<viewport[0]<< "]["<<viewport[1]<< "]["<<viewport[2]<< "]["<<viewport[3]<<"]" <<std::endl;
    glReadPixels(mouse_x, viewport[3] - mouse_y, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);
    printf("Res[%d]\n", res[0]);
    switch(res[0]) {
        case 0:  printf("Nothing Picked \n");escena.seleccion=0; break;
        case 20: escena.seleccion=1; gui.sel=1; break;
        case 40: escena.seleccion=2; gui.sel=2; break;
        default: printf("Res: %d\n", res[0]);
    }
    glUseProgram(shaderProgram->ReturnProgramID());
}

void __fastcall TEscena::renderSelection(glm::mat4 v) {

    glm::mat4 rotateMatrix;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //set matrices to identity
    viewMatrix    =  v;
    // use the selection shader

    pick=0;
    for (int i=0; i<num_cars; i++){
        pick+=20;
        cars[i]->RenderPick();
    }
    //don't swap buffers
    //glutSwapBuffers();

    // restore clear color if needed;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

}

//************************************************************** Clase TGui

TGui::TGui()
{
    sel = 1;
    sel2 = 0;
    enable_panel2 = 1;
    light0_enabled = 1;
    light1_enabled = 1;
    light0_intensity = 0.8;
    light1_intensity = 0.8;
    memcpy(light0_position, light0_position_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));
}

void controlCallback(int control)
{
    gui.ControlCallback(control);
}

void __fastcall TGui::Init(int main_window) {

    /****************************************************/
    /*        Código con el interfaz Gráfico GLUI       */
    /****************************************************/
    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

    window_id = main_window;

    /*** Crea a ventana lateral ***/
    glui = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_RIGHT );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    // Añade un panel con texto con el valor de la selección
    GLUI_Panel *panel0 = new GLUI_Panel(glui, "Seleccion");
    GLUI_RadioGroup *radioGroup = new GLUI_RadioGroup(panel0, &sel, SEL_ID, controlCallback);
    glui->add_radiobutton_to_group(radioGroup, "NINGUNO");

    glui->add_radiobutton_to_group(radioGroup, "COCHE 1");
    glui->add_radiobutton_to_group(radioGroup, "COCHE 2");

    new GLUI_StaticText( glui, "" );
    // Añade un panel con texto con el valor de la selección
    GLUI_Panel *panel1 = new GLUI_Panel(glui, "Camara");
    GLUI_RadioGroup *radioGroup2 = new GLUI_RadioGroup(panel1, &sel2, SEL_ID2, controlCallback);
    glui->add_radiobutton_to_group(radioGroup2, "Normal");
    glui->add_radiobutton_to_group(radioGroup2, "Aerea");
    glui->add_radiobutton_to_group(radioGroup2, "Seguimiento");

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    obj_panel = new GLUI_Rollout(glui, "Propiedades", true );

    /***** Control para las propiedades de escena *****/

    new GLUI_Checkbox( obj_panel, "Modo Alambrico", &escena.wireframe, 1, controlCallback );
    glui->add_column_to_panel(obj_panel, true);
    new GLUI_Checkbox( obj_panel, "Culling", &escena.culling, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Z Buffer", &escena.z_buffer, 1, controlCallback );

    /******** Añade controles para las luces ********/

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Luces", false );

    GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Luz 1" );
    GLUI_Panel *light1 = new GLUI_Panel( roll_lights, "Luz 2" );

    new GLUI_Checkbox( light0, "Encendida", &light0_enabled, LIGHT0_ENABLED_ID, controlCallback );
    light0_spinner = new GLUI_Spinner( light0, "Intensidad:", &light0_intensity, LIGHT0_INTENSITY_ID, controlCallback );
    light0_spinner->set_float_limits( 0.0, 1.0 );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "X",GLUI_SCROLL_HORIZONTAL, &escena.light0_position[0],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Y",GLUI_SCROLL_HORIZONTAL, &escena.light0_position[1],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Z",GLUI_SCROLL_HORIZONTAL, &escena.light0_position[2],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);

    new GLUI_Checkbox( light1, "Encendida", &light1_enabled, LIGHT1_ENABLED_ID, controlCallback );
    light1_spinner = new GLUI_Spinner( light1, "Intensidad:", &light1_intensity, LIGHT1_INTENSITY_ID, controlCallback );
    light1_spinner->set_float_limits( 0.0, 1.0 );
    sb = new GLUI_Scrollbar( light1, "X",GLUI_SCROLL_HORIZONTAL, &escena.light1_position[0],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Y",GLUI_SCROLL_HORIZONTAL, &escena.light1_position[1],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Z",GLUI_SCROLL_HORIZONTAL, &escena.light1_position[2],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);


    // Añade una separación
    new GLUI_StaticText( glui, "" );

    /***  Rollout de Opciones ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Opciones", true );
    new GLUI_Checkbox( options, "Dibujar Coche", &escena.show_car );
    new GLUI_Checkbox( options, "Dibujar Ruedas", &escena.show_wheels );
    new GLUI_Checkbox( options, "Dibujar Carretera", &escena.show_road );


    /*** Disable/Enable botones ***/
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Checkbox( glui, "Bloquear Movimiento", &enable_panel2 );
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Resetear Posicion", RESET_ID, controlCallback );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

     // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_StaticText( glui, "  Autor:" );
    new GLUI_StaticText( glui, "  2012-2016 (C) Juan Antonio Puchol  " );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    /****** A 'quit' button *****/
    new GLUI_Button( glui, "Salir", 0,(GLUI_Update_CB)exit );

    // Crea la subventana inferior
    glui2 = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_BOTTOM );

    /**** Link windows to GLUI, and register idle callback ******/
    glui->set_main_gfx_window( window_id );
    glui2->set_main_gfx_window( window_id );

    view_rot = new GLUI_Rotation(glui2, "Rotacion Escena", escena.view_rotate );
    view_rot->set_spin( 1.0 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy = new GLUI_Translation(glui2, "Traslacion Escena XY", GLUI_TRANSLATION_XY, escena.view_position );
    trans_xy->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Traslacion Escena X", GLUI_TRANSLATION_X, escena.view_position );
    trans_x->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y = new GLUI_Translation( glui2, "Traslacion Escena Y", GLUI_TRANSLATION_Y, &escena.view_position[1] );
    trans_y->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z = new GLUI_Translation( glui2, "Traslacion Escena Z", GLUI_TRANSLATION_Z, &escena.scale );
    trans_z->set_speed( .005 );

}

/**************************************** TGui::ControlCallback() *******************/

void __fastcall TGui::ControlCallback( int control )
{
    switch (control) {
        case LIGHT0_ENABLED_ID: {
            if ( light0_enabled )
                light0_spinner->enable();
            else
                light0_spinner->disable();
            break;
        }
        case LIGHT1_ENABLED_ID: {
            if ( light1_enabled )
                light1_spinner->enable();
            else
                light1_spinner->disable();
            break;
        }
        case LIGHT0_INTENSITY_ID: {

            float v[] = {
                escena.light0_diffuse[0],  escena.light0_diffuse[1],
                escena.light0_diffuse[2],  escena.light0_diffuse[3] };

            v[0] *= light0_intensity;
            v[1] *= light0_intensity;
            v[2] *= light0_intensity;
            break;
        }
        case LIGHT1_INTENSITY_ID: {

            float v[] = {
                escena.light1_diffuse[0],  escena.light1_diffuse[1],
                escena.light1_diffuse[2],  escena.light1_diffuse[3] };

            v[0] *= light1_intensity;
            v[1] *= light1_intensity;
            v[2] *= light1_intensity;
            break;
        }
        case ENABLE_ID: {
            glui2->enable();
            break;
        }
        case DISABLE_ID: {
            glui2->disable();
            break;
        }
        case RESET_ID: {
            memcpy(escena.view_position,view_position_c,3*sizeof(float));
            view_rot->reset();
            escena.scale = 1.0;
            break;
        }
        case SEL_ID: {
            escena.seleccion = sel;
            //GLUI_Master.SetFocus(true);
            glutSetWindow( glui->get_glut_window_id() );
            break;
        }
  } // switch
}

/***************************************** TGui::Idle() ***********/

void __fastcall TGui::Idle( void )
{
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != window_id )
    glutSetWindow(window_id);

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */
    if (enable_panel2)

        glui2->enable();
    else
        glui2->disable();

  glutPostRedisplay();
}

/**************************************** TGui::reshape() *************/

void __fastcall TGui::Reshape( int x, int y )
{
    int tx, ty, tw, th;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );

    escena.xy_aspect = (float)tw / (float)th;
    escena.projectionMatrix = glm::perspective(45.0f, escena.xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(escena.uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(escena.projectionMatrix));

    //std::cout << "xy aspect: " << escena.xy_aspect << std::endl;

    glutPostRedisplay();
}

/***************************************** gui::motion() **********/

void __fastcall TGui::Motion(int x, int y )
{
    glutPostRedisplay();
}

/***************************************** gui::Mouse() **********/

void __fastcall TGui::Mouse(int button, int button_state, int x, int y )
{
    escena.Pick3D(x, y);
}

