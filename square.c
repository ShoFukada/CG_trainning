#include <GL/gl.h>
#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);//フレームバッファの画面をクリアするための関数
    glColor3d(1.0, 1.0, 0.0);//これから描画するものの色が変わる
    glBegin(GL_QUADS);//glBeginからglend内に座標設定の関数を置く
    glVertex3d(1.0,1.0,0.0);
    glVertex3d(1.0,-1.0,0.0);
    glVertex3d(-1.0,-1.0,0.0);
    glVertex3d(-1.0,1.0,0.0);
    glEnd();
    glFlush();
}
void resize(int w, int h)
{//w,hはウィンドウの幅と高さが入る
  /* ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);
  //ビューポートとは開いたウィンドウの中で実際に描画が行われる領域
  /* 変換行列の初期化 */
  glLoadIdentity();

  /* スクリーン上の表示領域をビューポートの大きさに比例させる */
  glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);
}
void init()
{
    glClearColor(1.0,1.0,1.0,1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);//GULT環境を初期化する
    glutInitDisplayMode(GLUT_RGBA);//ディスプレイの表示モードを指定
    glutCreateWindow(argv[0]);//指定した名前のウィンドウを開く
    glutDisplayFunc(display);//この関数内で図形
    glutReshapeFunc(resize);
    init();
    glutMainLoop();//無限ループ
    return 0;
}