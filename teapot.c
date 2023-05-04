#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);//次の変換がモデルビューに適用される
    glLoadIdentity();

//(3Dで)オブジェクトを生成し,その後にそれを２次元に投影するイメージ(投影方法とカメラの設定をここで決める)

    glutSolidTeapot(1.0);

    glFlush();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);//次の変換が投影行列に適用される
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    gluLookAt(2.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
