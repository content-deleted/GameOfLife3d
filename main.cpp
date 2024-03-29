#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

#include "cells.cpp"

////////////////////////////////////////////////////////////
///
/// I don't know why I bother
///
////////////////////////////////////////////////////////////

float *flatten(glm::mat4 m);

int main()
{
    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;

    // Create the main window
    const float winW = 800;
    const float winH = 600;

    sf::Window window(sf::VideoMode(winW, winH, 32), "Game of Life",sf::Style::Default,contextSettings);

    // Make it the active window for OpenGL calls
    window.setActive();

    // Set the color and depth clear values
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Disable lighting and texturing
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

    // Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
    GLfloat cube[] =
            {
                    // positions    // colors (r, g, b, a)
                    -50, -50, -50,  0, 0, 1, 1,
                    -50,  50, -50,  0, 0, 1, 1,
                    -50, -50,  50,  0, 0, 1, 1,
                    -50, -50,  50,  0, 0, 1, 1,
                    -50,  50, -50,  0, 0, 1, 1,
                    -50,  50,  50,  0, 0, 1, 1,

                    50, -50, -50,  0, 1, 0, 1,
                    50,  50, -50,  0, 1, 0, 1,
                    50, -50,  50,  0, 1, 0, 1,
                    50, -50,  50,  0, 1, 0, 1,
                    50,  50, -50,  0, 1, 0, 1,
                    50,  50,  50,  0, 1, 0, 1,

                    -50, -50, -50,  1, 0, 0, 1,
                    50, -50, -50,  1, 0, 0, 1,
                    -50, -50,  50,  1, 0, 0, 1,
                    -50, -50,  50,  1, 0, 0, 1,
                    50, -50, -50,  1, 0, 0, 1,
                    50, -50,  50,  1, 0, 0, 1,

                    -50,  50, -50,  0, 1, 1, 1,
                    50,  50, -50,  0, 1, 1, 1,
                    -50,  50,  50,  0, 1, 1, 1,
                    -50,  50,  50,  0, 1, 1, 1,
                    50,  50, -50,  0, 1, 1, 1,
                    50,  50,  50,  0, 1, 1, 1,

                    -50, -50, -50,  1, 0, 1, 1,
                    50, -50, -50,  1, 0, 1, 1,
                    -50,  50, -50,  1, 0, 1, 1,
                    -50,  50, -50,  1, 0, 1, 1,
                    50, -50, -50,  1, 0, 1, 1,
                    50,  50, -50,  1, 0, 1, 1,

                    -50, -50,  50,  1, 1, 0, 1,
                    50, -50,  50,  1, 1, 0, 1,
                    -50,  50,  50,  1, 1, 0, 1,
                    -50,  50,  50,  1, 1, 0, 1,
                    50, -50,  50,  1, 1, 0, 1,
                    50,  50,  50,  1, 1, 0, 1,
            };

    // Enable position and color vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 7 * sizeof(GLfloat), cube);
    glColorPointer(4, GL_FLOAT, 7 * sizeof(GLfloat), cube + 3);

    // Disable normal and texture coordinates vertex components
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Create a clock for measuring the time elapsed
    sf::Clock clock;

    // Load shaders
    sf::Shader effect;
    effect.loadFromFile("../basic.vert","../basic.frag");
    if (!effect.isAvailable()) {
        std::cout << "The shader is not available\n";
    }
    sf::Shader::bind(&effect);

    grid CellGrid = grid();

    float rot = 0;

    int frame = 0;
    // Start render loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // R key: reset
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R))
                CellGrid = grid();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float Translate = 20;

        rot+=0.0005;
        glm::vec3 Rotate = glm::vec3(rot,rot,0);

        glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
        glm::mat4 ViewTranslate = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(0.0f, 0.0f, -Translate)
        );
        glm::mat4 ViewRotateX = glm::rotate(
                ViewTranslate,
                Rotate.y,
                glm::vec3(-1.0f, 0.0f, 0.0f)
        );
        glm::mat4 View = glm::rotate(
                ViewRotateX,
                Rotate.x,
                glm::vec3(0.0f, 1.0f, 0.0f)
        );

        glm::mat4 VP = Projection * View;

        float r[16];
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                r[i*4+j] = VP[i][j];
            }
        }

        sf::Glsl::Mat4 vp(r);

        // Transfer the transformation matrices to the shader
        effect.setUniform("VP",vp);

        // Set the time in the shader
        effect.setUniform("Time",(float) (clock.getElapsedTime().asMilliseconds() / 1000.0) );


        for(int x = 0; x<CellGrid.cells.size(); x++) for(int y = 0; y<CellGrid.cells[x].size(); y++) for(int z = 0; z<CellGrid.cells[x][y].size(); z++) {
                    if(!CellGrid.cells[x][y][z]) continue;
                    glm::mat4 scale = glm::scale(
                            glm::mat4(1.0f),
                            glm::vec3(0.01f)
                    );
                    float fscale = 100;
                    glm::mat4 Model = glm::translate(
                            scale,
                            glm::vec3( fscale *(x - CellGrid.cells.size()/2.0), fscale * (y - CellGrid.cells[x].size()/2.0), fscale * (z - CellGrid.cells[x][y].size()/2.0) )
                    );

                    // flatten ; could be made more efficient
                    for(int i=0; i<4; i++){
                        for(int j=0; j<4; j++){
                            r[i*4+j] = Model[i][j];
                        }
                    }

                    sf::Glsl::Mat4 m(r);

                    // Separate uniform for model which is calculated for each cell
                    effect.setUniform("M",m);

                    // Draw a cube
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

        // Display rendered frame
        window.display();


        if(frame % 20 == 0) CellGrid.nextGeneration();

        frame++;
    }

    return EXIT_SUCCESS;
}