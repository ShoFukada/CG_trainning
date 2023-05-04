#include <GL/gl.h>
#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);//フレームバッファの画面をクリアするための関数
    glRotated(25.0, 0.0, 1.0, 0.0);//回転角、x,y,z成分(軸)
    glBegin(GL_POLYGON);//glBeginからglend内に座標設定の関数を置く
    glColor3d(1.0, 0.0, 0.0);//これから描画するものの色が変わる
    glVertex2d(-0.9, -0.9);//ウィンドウのx,yの範囲は[-1,1]で固定
    glColor3d(0.0, 1.0, 0.0);//これから描画するものの色が変わる
    glVertex2d(0.9, -0.9);
    glColor3d(0.0, 0.0, 1.0);//これから描画するものの色が変わる
    glVertex2d(0.9, 0.9);
    glColor3d(1.0, 1.0, 0.0);//これから描画するものの色が変わる
    glVertex2d(-0.9, 0.9);
    glEnd();
    glFlush();
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
    init();
    glutMainLoop();//無限ループ
    return 0;
}

