#include <GL/glut.h>

#include "vec3.hpp"
#include "splines.hpp"

static int g_win_number;

// Control points parameters
float _width = 5.0f;
int _nb_control_points = 16;
bool _animate = true;
std::vector<Vec3> _control_points;

// Number of line subdivisions to display the spline
int _spline_resolution = 1000;
// Smoothness of the spline (min 2)
int _spline_order = 3;
Spline<Vec3, float> _spline_curve(_spline_order, /*spline::eOPEN_UNIFORM*/spline::eUNIFORM);

// =============================================================================

void key_stroke (unsigned char c, int mouseX, int mouseY) {
    static bool wires  = false;

    switch (c) {
    case 27 :
        glFinish();
        glutDestroyWindow(g_win_number);
        exit (0);
        break;
    case 'a' : _animate = !_animate; break;
    case 'w' :
        if(!wires) {
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
            glutPostRedisplay();
            wires = true;
        }else {
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            glutPostRedisplay();
            wires = false;
        }
        break;
    }
}

// -----------------------------------------------------------------------------

void draw_spline(const Spline<Vec3, float>& spline_curve)
{
    glLineWidth(4.0f);
    glColor3f( 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    Vec3 start = spline_curve.eval_f(0.0f);
    glVertex3f(start.x, start.y, start.z);
    for(int i = 0; i < _spline_resolution; ++i) {
        Vec3 p = spline_curve.eval_f( float(i) / float(_spline_resolution-1) );
        glVertex3fv((float*)p);
        if(i < (_spline_resolution-1) )
            glVertex3fv((float*)p);

    }
    glEnd();
}

// -----------------------------------------------------------------------------

void draw_spline_speed(const Spline<Vec3, float>& spline_curve, float t)
{
    glLineWidth(2.0f);
    glColor3f( 0.0f, 0.8f, 0.1f);
    glBegin(GL_LINES);
    Vec3 v     = spline_curve.eval_df(t) * 0.1f;
    Vec3 mid   = spline_curve.eval_f(t);
    Vec3 start = mid - v;
    Vec3 end   = mid + v;
    glVertex3f(start.x, start.y, start.z +0.00001f /*put it slightly forward*/);
    glVertex3f(end.x  , end.y  , end.z   +0.00001f /*put it slightly forward*/);
    glEnd();
    glBegin(GL_POINTS);
    glColor3f ( 1.0f, 0.0f, 1.0f);
    glVertex3f( mid.x, mid.y, mid.z+0.0001f /*put it slightly forward*/ );
    glEnd();
}

// -----------------------------------------------------------------------------

void draw_ctrl_points(const std::vector<Vec3>& control_points)
{
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    for(Vec3 v : control_points){
        glColor3f ( 1.0f, 0.0f, 0.0f);
        glVertex3f( v.x, v.y, v.z+0.00001f /*put it slightly forward*/ );
    }
    glEnd();
}

// -----------------------------------------------------------------------------

void display(void)
{
    glClearColor (0.8, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glViewport(0,0,900,900);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60., 1., 0.5, 100.);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glTranslatef(0.0, 0.0, -1.0);
    float s = 0.2f;
    glScalef(s, s, s);

    // Animate control points to go up and down
    static float sign = -1.0f;
    static float y    = 0.2f;
    if( _animate ){
        sign = y < -0.8f ? 1.0f : (y > 0.4f ? -1.0f : sign);
        y = y + 0.01f*sign;

        for(int i = 1; i < (_nb_control_points/2)-1; ++i) {
            Vec3 p = _control_points[i*2];
            _control_points[i*2] = Vec3(p.x, y, p.z );
        }
    }
    _spline_curve.set_ctrl_points(_control_points);
    draw_spline(_spline_curve);
    draw_ctrl_points(_control_points);

    {
        static float t = 0.0f;
        t += 0.001f;
        t = t > 1.0f ? 0.0f : t;
        draw_spline_speed(_spline_curve, t);
    }


    glutSwapBuffers();
    glFlush ();
    glutPostRedisplay();
}

// -----------------------------------------------------------------------------

int main (int argc, char** argv)
{
    float h = 0.2f;
    float start_x = -_width*0.5f;
    float step = _width / float(_nb_control_points-1);

    for(int i = 0; i < _nb_control_points; ++i) {
        float x = start_x + step * float(i);
        float sign = i % 2 ? -1.0f : 1.0f;
        _control_points.push_back( Vec3(x, h*sign, 0.0f) );
    }

    /*
    g_control_points.push_back( Vec3(-1.00f,  h, 0.0f) );
    g_control_points.push_back( Vec3(-0.75f, -h, 0.0f) );
    g_control_points.push_back( Vec3(-0.50f,  h, 0.0f) );
    g_control_points.push_back( Vec3(-0.25f, -h, 0.0f) );
    g_control_points.push_back( Vec3( 0.00f,  h, 0.0f) );
    g_control_points.push_back( Vec3( 0.25f, -h, 0.0f) );
    g_control_points.push_back( Vec3( 0.50f,  h, 0.0f) );
    g_control_points.push_back( Vec3( 0.75f, -h, 0.0f) );
    g_control_points.push_back( Vec3( 1.00f,  h, 0.0f) );
    */

    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (900, 900);
    glutInitWindowPosition (240, 212);
    g_win_number = glutCreateWindow (argv[0]);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    glutKeyboardFunc(key_stroke);
    glutDisplayFunc(display);
    glutMainLoop();

    return (0);
}

