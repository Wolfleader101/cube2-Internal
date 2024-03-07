#include "glDraw.hpp"
#include <iostream>

namespace GL
{

    void SetupOrtho()
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);           // backup attributes
        glPushMatrix();                             // backup view matrix
        GLint viewport[4];                          // screen dimentions
        glGetIntegerv(GL_VIEWPORT, viewport);       // get the screen dimentions
        glViewport(0, 0, viewport[2], viewport[3]); // set the screen size
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // set up the orthographic mode:
        glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST); // disable depth testing
    }

    void RestoreGL()
    {
        glPopMatrix();
        glPopAttrib();
    }

    bool WorldToScreen(const Vec3& pos, Vec2& screen, const float matrix[16])
    {
        // get screen witdh and height
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        int windowWidth = viewport[2];
        int windowHeight = viewport[3];

        // Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
        Vec4 clipCoords;
        clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
        clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
        clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
        clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

        // if coordinates are not on screen
        if (clipCoords.w < 0.1f)
            return false;

        // perspective division, dividing by clip.W = Normalized Device Coordinates
        Vec3 NDC;
        NDC.x = clipCoords.x / clipCoords.w;
        NDC.y = clipCoords.y / clipCoords.w;
        NDC.z = clipCoords.z / clipCoords.w;

        // Transform to window coordinates
        screen.x = (windowWidth / 2 * NDC.x) + (windowWidth / 2);
        screen.y = -(windowHeight / 2 * NDC.y) + (windowHeight / 2);
        return true;
    }

    void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3])
    {
        glColor3ub(color[0], color[1], color[2]);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }

    void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINE_STRIP);
        glColor3ub(color[0], color[1], color[2]);
        glVertex2f(x - 0.5f, y - 0.5f);
        glVertex2f(x + width + 0.5f, y - 0.5f);
        glVertex2f(x + width + 0.5f, y + height + 0.5f);
        glVertex2f(x - 0.5f, y + height + 0.5f);
        glVertex2f(x - 0.5f, y - 0.5f);
        glEnd();
    }

    void DrawLine(float fromX, float fromY, float toX, float toY, float lineWidth, const GLubyte color[3])
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3ub(color[0], color[1], color[2]);
        glVertex2f(fromX, fromY);
        glVertex2f(toX, toY);
        glEnd();
    }

    void DrawEspBox(float posX, float posY, float distance, const GLubyte color[3], const int healthPercent)
    {
        float lineWidth = 0.5f;
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        float height = (viewport[3] / distance) * 10;
        float width = (viewport[2] / distance) * 4;

        // snap lines
        GL::DrawLine(viewport[2] / 2.0f, (float)viewport[3], posX, posY, lineWidth + 2.0f, rgb::black);
        GL::DrawLine(viewport[2] / 2.0f, (float)viewport[3], posX, posY, lineWidth, color);

        // outline
        GL::DrawOutline(posX - (width / 2), posY - height, width, height, lineWidth + 2.0f, rgb::black);
        GL::DrawOutline(posX - (width / 2), posY - height, width, height, lineWidth, color);

        // health bar
        if (healthPercent != -1)
        {
            float perc = (width / 100);
            float curr = perc * healthPercent;

            GL::DrawFilledRect(posX - (width / 2) - 1, ((posY - (height / 10)) - 1) - height, width + 2,
                               (height / 15) + 2, rgb::black);
            GL::DrawFilledRect(posX - (width / 2), (posY - (height / 10)) - height, width, height / 15, rgb::lightGray);

            GLubyte Hcolor[3]{static_cast<GLubyte>(255 - (2.5f * healthPercent)),
                              static_cast<GLubyte>(healthPercent * 2.5f), 0};
            GL::DrawFilledRect(posX - (width / 2), (posY - (height / 10)) - height, curr, height / 15, Hcolor);
        }
    }

} // namespace GL