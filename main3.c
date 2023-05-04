#include <stdio.h>
#include <GL/glut.h>

#define MAXPOINTS 100      /* 記憶する点の数　　 */
GLint point[MAXPOINTS][2]; /* 座標を記憶する配列 */
int pointnum = 0;          /* 記憶した座標の数　 */

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT);

  /* 記録したデータで線を描く */
  if (pointnum > 1) {
    glColor3d(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (i = 0; i < pointnum; ++i) {
      glVertex2iv(point[i]);
    }
    glEnd();
  }

  glFlush();
}

void resize(int w, int h)
{
  /* ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);

  /* 変換行列の初期化 */
  glLoadIdentity();

  glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}


void mouse(int button, int state, int x, int y)
{
  /* 削除 */

  switch (button) {
  case GLUT_LEFT_BUTTON:
    /* ボタンを操作した位置を記録する */
    point[pointnum][0] = x;
    point[pointnum][1] = y;
    if (state == GLUT_UP) {
      /* ボタンを押した位置から離した位置まで線を引く */
      glColor3d(0.0, 0.0, 0.0);
      glBegin(GL_LINES);
      glVertex2iv(point[pointnum - 1]); /* 一つ前は押した位置　 */
      glVertex2iv(point[pointnum]);     /* 今の位置は離した位置 */
      glEnd();
      glFlush();
    }    
    else {
      /* 削除 */
    }
    if (pointnum < MAXPOINTS - 1) ++pointnum;
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  default:
    break;
  }
}
void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
}
int main(int argc, char *argv[])
{
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 240);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);//マウスのボタンが押されたときに実行する関数のポインタを与える
  init();
  glutMainLoop();
  return 0;
}
