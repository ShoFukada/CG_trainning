#include <GL/glut.h>
GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };//光源の設定(x,y,z,?)
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };

GLfloat green[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);//次の変換がモデルビューに適用される
    glLoadIdentity();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);

//(3Dで)オブジェクトを生成し,その後にそれを２次元に投影するイメージ(投影方法とカメラの設定をここで決める)

    glutSolidTeapot(1.0);

    glFlush();
}
void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);//次の変換が投影行列に適用される
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);//GL_PROJECTIONにしないと反映されない
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//視点の位置、目標の位置、上方向ベクトル
  /* 光源の位置設定 */
//   glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
//   glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
//光源の番号、設定するパラメータ(場所や反射強度)
}
void init() {
    glEnable(GL_DEPTH_TEST);//デプスバッファを有効にする
    //glCullFace(GL_FRONT);//カリングを有効にする(裏側の面を表示しないようにする)

    glEnable(GL_LIGHTING);//陰影付を有効にする
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, green);
    glLightfv(GL_LIGHT1, GL_SPECULAR, green);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);//次の変換が投影行列に適用される
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMainLoop();
    return 0;
}