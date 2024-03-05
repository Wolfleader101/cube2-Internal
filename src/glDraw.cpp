#include "glDraw.hpp"

namespace GL
{

    void SetupOrtho()
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        GLint viewport[4];

        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, viewport[2], viewport[3]);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);
    }

    void RestoreGL()
    {
        glPopMatrix();
        glPopAttrib();
    }

    bool WorldToScreen(const Vec3& worldPos, Vec2& screenPos, const float viewMatrix[16])
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        int width = viewport[2];
        int height = viewport[3];

        Vec4 clipCoords;

        clipCoords.x =
            worldPos.x * viewMatrix[0] + worldPos.y * viewMatrix[4] + worldPos.z * viewMatrix[8] + viewMatrix[12];
        clipCoords.y =
            worldPos.x * viewMatrix[1] + worldPos.y * viewMatrix[5] + worldPos.z * viewMatrix[9] + viewMatrix[13];
        clipCoords.z =
            worldPos.x * viewMatrix[2] + worldPos.y * viewMatrix[6] + worldPos.z * viewMatrix[10] + viewMatrix[14];
        clipCoords.w =
            worldPos.x * viewMatrix[3] + worldPos.y * viewMatrix[7] + worldPos.z * viewMatrix[11] + viewMatrix[15];

        // if the w is less than 0.1f, it's behind the camera
        if (clipCoords.w < 0.1f)
            return false;

        // normalised device coords
        Vec3 ndc;

        ndc.x = clipCoords.x / clipCoords.w;
        ndc.y = clipCoords.y / clipCoords.w;
        ndc.z = clipCoords.z / clipCoords.w;

        // convert to screen coords
        screenPos.x = (width / 2 * ndc.x) + (ndc.x + width / 2);
        screenPos.y = -(height / 2 * ndc.y) + (ndc.y + height / 2);

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