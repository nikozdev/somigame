/* headers*/

#include "head.hxx"

#include "main.hxx"

/* content */

_NAMESPACE_ENTER

int main(int argc, char** argv)
{
    std::clog << "hello world !" << std::endl;
    /* framework */
    std::clog << "init graphics library utility" << std::endl;
    glutInit(&argc, argv);
    /* window */
    std::clog << "init window" << std::endl;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(256, 128);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(_NAME_STR);
    /* gfix */
    /** gray bg color **/
    glClearColor(0.25, 0.25, 0.25, 1.0);
    /** white drawing **/
    glColor3d(1.0, 1.0, 1.0);
    /** vertex width **/
    glPointSize(1.0);
    /** camera setup **/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-128.0, +128.0, -64.0, +64.0);
    /* loop */
    glutDisplayFunc(work);
    glutMainLoop();
    /* final */
    std::clog << "good day, sir !" << std::endl;
    return _ERROR_NONE;
}

void work(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    _NAMESPACE::main(argc, argv);
}

#endif
