#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <vector>
#include <algorithm>
#define maxVer 50
#define maxHt 1000
#define maxWd 800

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint& pos);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    QImage img;
    ~MainWindow();

private slots:
    void on_show_axes_clicked();

    void on_Draw_clicked();

    void on_set_point1_clicked();

    void on_set_point2_clicked();

    void on_ResetButton_clicked();

    void on_showGrid_clicked();

    void on_DDALine_clicked();

    void on_BresenhamLine_clicked();

    void on_Midpoint_clicked();
    void drawCircle(QPoint p1, int r0);

    void on_BresenhamCircle_clicked();
    void drawCircleBress(QPoint p1, int r0);

    void on_ParametricCircle_clicked();
    void drawParametricCircle(QPoint p1, int r);

    void on_MidpointEllipse_clicked();
    void drawEllipse(QPoint p, int rx, int ry);

    void on_floodFill_clicked();
    void flood_fill_util(int x1, int y1,int k, QRgb q1, int r,int g,int b);

    void on_boundaryFill_clicked();
    void boundary_fill_util(int x1, int y1, int k, int r, int g, int b);

    void on_setVertex_clicked();

    void on_clearVertex_clicked();

    void on_scanLineFill_clicked();

private:
    Ui::MainWindow *ui;
    QPoint p1,p2;
//    QRgb axesColor = qRgb();
    QRgb edgeColor = qRgb(220,153,196);
    QRgb gridColor = qRgb(80,80,80);
    QRgb fillColor = qRgb(102,255,51);
    void point(int,int,int,int,int);

    typedef struct edgebucket
    {
        int ymax;   //max y-coordinate of edge
        float xofymin;  //x-coordinate of lowest edge point updated only in aet
        float slopeinverse;
    }EdgeBucket;

    typedef struct edgetabletup
    {
        // the array will give the scanline number
        // The edge table (ET) with edges entries sorted
        // in increasing y and x of the lower end

        int countEdgeBucket;    //no. of edgebuckets
        EdgeBucket buckets[maxVer];
    }EdgeTableTuple;
    EdgeTableTuple EdgeTable[maxHt], ActiveEdgeTuple;
    std::vector<std::pair<int,int> > EdgeList;
    void storeEdgeInTable (int x1,int y1, int x2, int y2);
    void storeEdgeInTuple (EdgeTableTuple *receiver,int ym,int xm,float slopInv);
    void insertionSort(EdgeTableTuple *ett);
    void initEdgeTable();
    void removeEdgeByYmax(EdgeTableTuple *Tup,int yy);
    void updatexbyslopeinv(EdgeTableTuple *Tup);
};

#endif // MAINWINDOW_H
