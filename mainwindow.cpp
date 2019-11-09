#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include <unistd.h>
#include<vector>
#include<algorithm>
#define pi 3.1415196
using namespace std;
//static QImage img;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),img(541,541,QImage::Format_RGB888),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->x_axis->hide();
    ui->y_axis->hide();
    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::point(int x,int y,int r,int g,int b)
{
//    img=QImage(ui->frame->width(),ui->frame->height(),QImage::Format_RGB888);
    int k = ui->gridsize->value();//GridSize
    if(k>1)
    {
        int startX=x/k*k;
        int startY=y/k*k;

        //Drawing the pixels
        for(int i=startX+1;i<(startX+k);i++)
            for(int j=startY+1;j<(startY+k);j++){
                img.setPixelColor(i,j,qRgb(r,g,b));
//                qDebug()<<i<<" "<<j;
            }
    }
    else
        img.setPixel(x,y,qRgb(r,g,b));
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::showMousePosition(QPoint &pos)
{
    ui->gridsize->setMinimum(1);
    int k = ui->gridsize->value();
    int rel_x = ((pos.x() - img.width()/2 )/k);
    int rel_y = (((img.height()-1)/2 - pos.y())/k);
//    ui->mouse_movement->setText(" X : "+QString::number(pos.x())+", Y : "+QString::number(pos.y()));
    ui->mouse_movement->setText(" X : "+QString::number(rel_x)+", Y : "+QString::number(rel_y));

}
void MainWindow::Mouse_Pressed()
{
    int r=80,g=80,b=80;
    ui->gridsize->setMinimum(1);
    point(ui->frame->x,ui->frame->y,r,g,b);

    int k = ui->gridsize->value();
    int rel_x = ((ui->frame->x - img.width()/2 )/k);
    int rel_y = (((img.height()-1)/2 - ui->frame->y))/k;

//    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x)+", Y : "+QString::number(ui->frame->y));
    ui->mouse_pressed->setText(" X : "+QString::number(rel_x)+", Y : "+QString::number(rel_y));
//    point(ui->frame->x,ui->frame->y,0,0,0);
    ui->x_axis->move(0,ui->frame->y);
    ui->y_axis->move(ui->frame->x,0);   
}

void MainWindow::on_show_axes_clicked()
{
//    img=QImage(ui->frame->width(),ui->frame->height(),QImage::Format_RGB888);
    if(ui->show_axes->isChecked())
    {
        //Draw Y-axis
        for(int i=0;i<=img.height();i++)
            point(i,img.width()/2,0,247,255);
        //Draw X-axis
        for(int i=0;i<=img.width();i++)
            point(img.height()/2,i,0,247,255);
    }
    else{
        //Draw Y-axis
        for(int i=0;i<=img.height();i++)
            point(i,img.width()/2,0,0,0);
        //Draw X-axis
        for(int i=0;i<=img.width();i++)
            point(img.height()/2,i,0,0,0);
    }
}
void MainWindow::on_set_point1_clicked()
{
    if(ui->draw_line->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
}

void MainWindow::on_set_point2_clicked()
{
    if(ui->draw_line->isChecked()){
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
    }
}

void MainWindow::on_Draw_clicked()
{
//    img=QImage(ui->frame->width(),ui->frame->height(),QImage::Format_RGB888);
    int r0=ui->circle_radius->value();
    QPainter painter(&img);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        painter.setPen(pen);
        painter.drawEllipse(p1,r0,r0);
    }
    if(ui->draw_line->isChecked()){
        painter.setPen(Qt::yellow);
        painter.drawLine(p1,p2);
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_ResetButton_clicked()
{
//    img=QImage(ui->frame->width(),ui->frame->height(),QImage::Format_RGB888);
    for(int j=0;j<img.height();j++)
    {
        for(int i=0;i<img.width();i++)
        {
            img.setPixel(i,j,qRgb(0,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_showGrid_clicked()
{
//    img=QImage(ui->frame->width(),ui->frame->height(),QImage::Format_RGB888);
//    int r=80,g=80,b=80;
    ui->gridsize->setMinimum(1);
    int k = ui->gridsize->value();
//    k = k>1 ? k: 1;
    for(int j=0;j<img.height();j++)
    {
        for(int i=0;i<img.width();i++)
        {
            img.setPixel(i,j,qRgb(0,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));

    for(int i=0;i<img.height();i+=k)
        for(int j=0;j<img.width();j++)
            img.setPixel(i,j,MainWindow::gridColor);
//            img.setPixel(i,j,qRgb(r,g,b));
                \

    for(int i=0;i<img.width();i+=k)
        for(int j=0;j<img.height();j++)
            img.setPixel(j,i,MainWindow::gridColor);
//            img.setPixel(j,i,qRgb(r,g,b));

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_DDALine_clicked()
{
//    int r=153, g=153, b=196;
//    int r=qRed(MainWindow::edgeColor),g=qGreen(MainWindow::edgeColor),b=qBlue(MainWindow::edgeColor);
    ui->gridsize->setMinimum(1);
    //This function draws a line between the two selected points using DDA algorithm
    int k = ui->gridsize->value();
    //Now p1 and p2 contains the points
    int x1=(p1.x()/k);
    int y1=(p1.y()/k);

    int x2=(p2.x()/k);
    int y2=(p2.y()/k);


    // here we implement the algorithm
    int dx=x2-x1;
    int dy=y2-y1;
    int steps=0; //number of steps to  put pixel

    if(abs(dx)>abs(dy))
        steps=abs(dx);
    else
        steps=abs(dy);

    double Xinc=(double)dx/(double)steps;
    double Yinc=(double)dy/(double)steps;

    double x=x1*k+k/2;
    double y=y1*k+k/2;

    for(int v=0; v<steps; v++)
    {
        x = x + Xinc*k;
        y = y + Yinc*k;
        point((int)(x+0.5),(int)(y+0.5),qRed(MainWindow::edgeColor),qGreen(MainWindow::edgeColor),qBlue(MainWindow::edgeColor));
    }
}

void MainWindow::on_BresenhamLine_clicked()
{
//    int r=160,g=138,b=148;
    int r=qRed(MainWindow::edgeColor),g=qGreen(MainWindow::edgeColor),b=qBlue(MainWindow::edgeColor);
    ui->gridsize->setMinimum(1);
    int k = ui->gridsize->value();
    //Store the two points
    int x1=p1.x();
    int y1=p1.y();

    int x2=p2.x();
    int y2=p2.y();

    int dx=x2-x1;
    int dy=y2-y1;

    x1=(x1/k)*k+k/2;
    y1=(y1/k)*k+k/2;

    x2=(x2/k)*k+k/2;
    y2=(y2/k)*k+k/2;

    int xinc=(dx>0)?k:-k;
    int yinc=(dy>0)?k:-k;

    dx=abs(dx);
    dy=abs(dy);

    //Case for gentle slope
    if(dx>dy)
    {
        int p=2*(dy)-dx;
        int y=y1;

        for(int x=x1; x!=x2; x+=xinc)
        {
            point(x,y,r,g,b);
            if(p>=0)
            {
                y+=yinc;
                p-=2*dx;
            }
            p+=2*dy;
        }
    }
    //Case for steep slope
    else if(dx<dy)
    {
        int p=2*(dx)-dy;
        int x=x1;

        for(int y=y1; y!=y2; y+=yinc)
        {
            point(x,y,r,g,b);
            if(p>=0)
            {
                x+=xinc;
                p-=2*(dy);
            }
            p+=2*(dx);
        }
    }
}

void MainWindow::on_Midpoint_clicked()
{
    ui->gridsize->setMinimum(1);
    //Get the radius
    int r0=ui->circle_radius->value();

    //Set the center
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

        drawCircle(p1,r0);
    }
}
void MainWindow::drawCircle(QPoint p1, int r0)
{    
//    int r=218,g=118,b=235;
    int r=qRed(MainWindow::edgeColor),g=qGreen(MainWindow::edgeColor),b=qBlue(MainWindow::edgeColor);
    //Function to draw the circle
    int x_center=p1.x();
    int y_center=p1.y();

    int k = ui->gridsize->value();//GridSize

    x_center=(x_center/k)*k+k/2;
    y_center=(y_center/k)*k+k/2;

    int x=r0*k;
    int y=0;

    point(x+x_center,y+y_center,r,g,b);

    if(r0>0)
    {
        point(x+x_center,-y+y_center,r,g,b);
        point(y+x_center,x+y_center,r,g,b);
        point(-y+x_center,x+y_center,r,g,b);
    }


    int P=(1-r0)*k;

    while(x>y)
    {
        y++;

        if(P<=0)
            P=P+2*y+1;
        else
        {
            x--;
            P=P+2*y-2*x+1;
        }
        if(x<y)
            break;

        point(x+x_center,y+y_center,r,g,b);
        point(-x+x_center,y+y_center,r,g,b);
        point(x+x_center,-y+y_center,r,g,b);
        point(-x+x_center,-y+y_center,r,g,b);

        if(x!=y)
        {
            point(y+x_center,x+y_center,r,g,b);
            point(-y+x_center,x+y_center,r,g,b);
            point(y+x_center,-x+y_center,r,g,b);
            point(-y+x_center,-x+y_center,r,g,b);
        }
    }
}
void MainWindow::on_ParametricCircle_clicked()
{
    ui->gridsize->setMinimum(1);
    //Get the radius
    int r0=ui->circle_radius->value();

    //Set the center
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

        drawParametricCircle(p1,r0);
    }
}
void MainWindow::drawParametricCircle(QPoint p1, int r0){
//    int r=218,g=118,b=235;
    int r=qRed(MainWindow::edgeColor),g=qGreen(MainWindow::edgeColor),b=qBlue(MainWindow::edgeColor);
    //Function to draw the circle
    int x_center=p1.x();
    int y_center=p1.y();

    int k = ui->gridsize->value();//GridSize

    x_center=(x_center/k)*k+k/2;
    y_center=(y_center/k)*k+k/2;

    int x=r0*k;
    int y=0;

    point(x+x_center,y+y_center,r,g,b);

    if(r0>0)
    {
        point(x+x_center,-y+y_center,r,g,b);
        point(y+x_center,x+y_center,r,g,b);
        point(-y+x_center,x+y_center,r,g,b);
    }
    int theta=0;
    if(theta<=45){
        x = r0*cos(theta*(pi/180))*k;
        y = r0*sin(theta*(pi/180))*k;
        theta++;
    }
    point(x+x_center,y+y_center,r,g,b);
    point(-x+x_center,y+y_center,r,g,b);
    point(x+x_center,-y+y_center,r,g,b);
    point(-x+x_center,-y+y_center,r,g,b);

    if(x!=y)
    {
        point(y+x_center,x+y_center,r,g,b);
        point(-y+x_center,x+y_center,r,g,b);
        point(y+x_center,-x+y_center,r,g,b);
        point(-y+x_center,-x+y_center,r,g,b);
    }

}
void MainWindow::on_BresenhamCircle_clicked()
{
    ui->gridsize->setMinimum(1);
    //Get the radius
    int r0=ui->circle_radius->value();

    //Set the center
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

        drawCircleBress(p1,r0);
    }
}

void MainWindow::drawCircleBress(QPoint p1, int r0)
{
//    int r=218,g=118,b=235;
    int r=qRed(MainWindow::edgeColor),g=qGreen(MainWindow::edgeColor),b=qBlue(MainWindow::edgeColor);
    //Function to draw the circle
    int x_center=p1.x();
    int y_center=p1.y();

    int k = ui->gridsize->value();//GridSize

    x_center=(x_center/k)*k+k/2;
    y_center=(y_center/k)*k+k/2;

    int y=r0*k;
    int x=0;

    int d=(3-2*r0)*k;

    while(y>=x)
    {
        point(x_center+x,y_center+y,r,g,b);
        point(x_center+x,y_center-y,r,g,b);
        point(x_center-x,y_center+y,r,g,b);
        point(x_center-x,y_center-y,r,g,b);

        point(x_center+y,y_center+x,r,g,b);
        point(x_center+y,y_center-x,r,g,b);
        point(x_center-y,y_center+x,r,g,b);
        point(x_center-y,y_center-x,r,g,b);

        x++;
        if(d>0)
        {
            y--;
            d=d+4*(x-y)+10;
        }
        else
        {
            d=d+4*x+6;
        }
    }
}


void MainWindow::on_MidpointEllipse_clicked()
{
    //Get the radius
    int rx=ui->ellipse_rx->value();
    int ry=ui->ellipse_ry->value();

    //Set the center
    if(ui->draw_ellipse->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

        drawEllipse(p1,rx,ry);
    }
}
void MainWindow::drawEllipse(QPoint p, int rx, int ry)
{
    //Function to draw the ellipse
    ui->gridsize->setMinimum(1);
//    int r=90,g=186,b=240;
    int r=qRed(MainWindow::edgeColor),g=qGreen(MainWindow::edgeColor),b=qBlue(MainWindow::edgeColor);
    //Get the center
    int x_center=p.x();
    int y_center=p.y();
    int k = ui->gridsize->value();//GridSize

    x_center=(x_center/k)*k+k/2;
    y_center=(y_center/k)*k+k/2;

    int x=0;
    int y=ry;

    int rx2=rx*rx;
    int ry2=ry*ry;
    int tworx2=2*rx2;
    int twory2=2*ry2;
    int px=0.0;
    int py=tworx2*y;


    //For first region
    int p1=ry2-rx2*ry+(0.25)*rx2; //Initial value of decision parameter


    while(px<py)
    {
        point(x_center+x*k,y_center+y*k,r,g,b);
        point(x_center-x*k,y_center+y*k,r,g,b);
        point(x_center-x*k,y_center-y*k,r,g,b);
        point(x_center+x*k,y_center-y*k,r,g,b);

        x++;
        px+=twory2;

        if(p1>=0)
        {
            y--;
            py-=tworx2;
            p1=p1+ry2+px-py;

        }
        else
        {
            p1=p1+ry2+px;
        }
    }

    //For second region
    p1=ry2*((double)x+0.5)*((double)x+0.5)+rx2*(y-1)*(y-1)-rx2*ry2; //Initial value of decision paramemter


    while(y>=0)
    {
        point(x_center+x*k,y_center+y*k,r,g,b);
        point(x_center-x*k,y_center+y*k,r,g,b);
        point(x_center-x*k,y_center-y*k,r,g,b);
        point(x_center+x*k,y_center-y*k,r,g,b);

        y--;
        py-=tworx2;
        if(p1<=0)
        {
            x++;
            px+=twory2;
            p1=p1+rx2-py+px;

        }
        else
        {
            p1=p1+rx2-py;
        }

    }
}

// BUG Flood fill : on clicking the color of pixel is gridColor thus flood fill on it colors only
//that pixel thus after flood fill click on boundary fill to make sure all pixels has white i.e
//boundaryfill color then click floodfill
void MainWindow::on_floodFill_clicked()
{
//    int x1=p1.x();
//    int y1=p1.y();
    int x1 = ui->frame->x;
    int y1 = ui->frame->y;
    int k = ui->gridsize->value();

    x1=(x1/k)*k+k/2;
    y1=(y1/k)*k+k/2;
    QRgb current = img.pixel(x1,y1);
    int r=qRed(MainWindow::fillColor), g=qGreen(MainWindow::fillColor), b=qBlue(MainWindow::fillColor);
    flood_fill_util(x1,y1,k,current,r,g,b);

}
void MainWindow::flood_fill_util(int x1, int y1,int k, QRgb q1, int r,int g,int b)
{
    QRgb current = img.pixel(x1,y1);
    if(x1<=0 || x1>img.width()|| y1<=0 || y1>img.height())
        return;
    else if(current!=q1)
        return;
    else if(current==qRgb(r,g,b))
        return;
//    if(current != gridColor)
        point(x1,y1,r,g,b);

    flood_fill_util(x1+k,y1,k,q1,r,g,b);
    flood_fill_util(x1-k,y1,k,q1,r,g,b);
    flood_fill_util(x1,y1+k,k,q1,r,g,b);
    flood_fill_util(x1,y1-k,k,q1,r,g,b);
}

void MainWindow::on_boundaryFill_clicked()
{
//        int x1=p1.x();
//        int y1=p1.y();
        int x1 = ui->frame->x;
        int y1 = ui->frame->y;
        int k = ui->gridsize->value();

        x1=(x1/k)*k+k/2;
        y1=(y1/k)*k+k/2;

        boundary_fill_util(x1,y1,k,255,255,255);
}
void MainWindow::boundary_fill_util(int x1, int y1, int k, int r, int g, int b)
{
    QRgb current;
    current=img.pixel(x1,y1);

    if(x1<=0 || x1>img.width()|| y1<=0 || y1>img.height())
        return;


//    if(current!=edgecolor && current!=qRgb(r,g,b))
    if(current!=MainWindow::edgeColor && current!=qRgb(r,g,b))
    {
        if(current != MainWindow::gridColor)
            point(x1,y1,255,255,255);
        boundary_fill_util(x1+k,y1,k,r,g,b);
        boundary_fill_util(x1-k,y1,k,r,g,b);
        boundary_fill_util(x1,y1+k,k,r,g,b);
        boundary_fill_util(x1,y1-k,k,r,g,b);
    }
}

// Scan line Algorithm
void MainWindow::on_setVertex_clicked()
{
    int k=ui->gridsize->value();
    int x=((ui->frame->x)/k)*k+k/2;
    int y=((ui->frame->y)/k)*k+k/2;
    EdgeList.push_back(make_pair(x,y));

    int i=EdgeList.size();

    if(EdgeList.size()>1)
    {
        storeEdgeInTable(EdgeList[i-2].first, EdgeList[i-2].second, EdgeList[i-1].first, EdgeList[i-1].second);//storage of edges in edge table.

        p1.setX(EdgeList[EdgeList.size()-1].first);
        p2.setX(EdgeList[EdgeList.size()-2].first);

        p1.setY(EdgeList[EdgeList.size()-1].second);
        p2.setY(EdgeList[EdgeList.size()-2].second);

        on_BresenhamLine_clicked();

    }

}

void MainWindow::on_clearVertex_clicked()
{
    // for initialising
    EdgeList.clear();
    initEdgeTable();
}

void MainWindow::initEdgeTable()
{
    int i;
    for (i=0; i<maxHt; i++)
    {
        EdgeTable[i].countEdgeBucket = 0;
    }

    ActiveEdgeTuple.countEdgeBucket = 0;
}

void MainWindow::insertionSort(EdgeTableTuple *ett)
{
    int i,j;
    EdgeBucket temp;

    for (i = 1; i < ett->countEdgeBucket; i++)
    {
        temp.ymax = ett->buckets[i].ymax;
        temp.xofymin = ett->buckets[i].xofymin;
        temp.slopeinverse = ett->buckets[i].slopeinverse;
        j = i - 1;

        while ((temp.xofymin < ett->buckets[j].xofymin) && (j >= 0))
        {
            ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
            ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
            ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
            j = j - 1;
        }
        ett->buckets[j + 1].ymax = temp.ymax;
        ett->buckets[j + 1].xofymin = temp.xofymin;
        ett->buckets[j + 1].slopeinverse = temp.slopeinverse;
    }
}

void MainWindow::storeEdgeInTuple (EdgeTableTuple *receiver,int ym,int xm,float slopInv)
{
    (receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
    (receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
    (receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

    insertionSort(receiver);

    (receiver->countEdgeBucket)++;


}

void MainWindow::storeEdgeInTable (int x1,int y1, int x2, int y2)
{
    float m,minv;
    int ymaxTS,xwithyminTS, scanline;

    if (x2==x1)
    {
        minv=0.000000;
    }
    else
    {
        m = ((float)(y2-y1))/((float)(x2-x1));

        if (y2==y1)
            return;

        minv = (float)1.0/m;
    }

    if (y1>y2)
    {
        scanline=y2;
        ymaxTS=y1;
        xwithyminTS=x2;
    }
    else
    {
        scanline=y1;
        ymaxTS=y2;
        xwithyminTS=x1;
    }
    storeEdgeInTuple(&EdgeTable[scanline],ymaxTS,xwithyminTS,minv);


}

void MainWindow::removeEdgeByYmax(EdgeTableTuple *Tup,int yy)
{
    int i,j;
    for (i=0; i< Tup->countEdgeBucket; i++)
    {
        if (Tup->buckets[i].ymax == yy)
        {
            for ( j = i ; j < Tup->countEdgeBucket -1 ; j++ )
            {
                Tup->buckets[j].ymax =Tup->buckets[j+1].ymax;
                Tup->buckets[j].xofymin =Tup->buckets[j+1].xofymin;
                Tup->buckets[j].slopeinverse = Tup->buckets[j+1].slopeinverse;
            }
            Tup->countEdgeBucket--;
            i--;
        }
    }
}

void MainWindow::updatexbyslopeinv(EdgeTableTuple *Tup)
{
    int i;

    for (i=0; i<Tup->countEdgeBucket; i++)
    {
        (Tup->buckets[i]).xofymin =(Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
    }
}

void MainWindow::on_scanLineFill_clicked()
{
    int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

    for (i=0; i<maxHt; i++)
    {
        for (j=0; j<EdgeTable[i].countEdgeBucket; j++)
        {
            storeEdgeInTuple(&ActiveEdgeTuple,EdgeTable[i].buckets[j].
                             ymax,EdgeTable[i].buckets[j].xofymin,
                             EdgeTable[i].buckets[j].slopeinverse);
        }

        removeEdgeByYmax(&ActiveEdgeTuple, i);

        insertionSort(&ActiveEdgeTuple);

        j = 0;
        FillFlag = 0;
        coordCount = 0;
        x1 = 0;
        x2 = 0;
        ymax1 = 0;
        ymax2 = 0;
        while (j<ActiveEdgeTuple.countEdgeBucket)
        {
            if (coordCount%2==0)
            {
                x1 = (int)(ActiveEdgeTuple.buckets[j].xofymin);
                ymax1 = ActiveEdgeTuple.buckets[j].ymax;
                if (x1==x2)
                {
                    if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
                    {
                        x2 = x1;
                        ymax2 = ymax1;
                    }

                    else
                    {
                        coordCount++;
                    }
                }

                else
                {
                    coordCount++;
                }
            }
            else
            {
                x2 = (int)ActiveEdgeTuple.buckets[j].xofymin;
                ymax2 = ActiveEdgeTuple.buckets[j].ymax;

                FillFlag = 0;
                if (x1==x2)
                {
                    if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
                    {
                        x1 = x2;
                        ymax1 = ymax2;
                    }
                    else
                    {
                        coordCount++;
                        FillFlag = 1;
                    }
                }
                else
                {
                    coordCount++;
                    FillFlag = 1;
                }

                if(FillFlag)
                {
                    p1.setX(x1);p1.setY(i);
                    p2.setX(x2);p2.setY(i);
                    on_BresenhamLine_clicked();
                }

            }

            j++;
        }
        updatexbyslopeinv(&ActiveEdgeTuple);
    }

    EdgeList.clear();
}


// *************** TRANSFORMATIONS **********************

//void MainWindow::matMul3x3(double mat[3][3],int (&coord)[3])
//{
//    int i,k,res[3];
//    for (i = 0; i < 3; i++)
//    {
//        res[i]= 0;
//        for (k = 0; k < 3; k++)
//            res[i] += (int)((double)((coord)[k]) * mat[k][i]);

//    }
//    coord[0] = res[0];
//    coord[1] = res[1];
//    coord[2] = res[2];
//    //return res;
//}

void MainWindow::drawPoly()
{
    int i,len=EdgeList.size();
    int r=qRed(MainWindow::edgeColor),g=qGreen(MainWindow::edgeColor),b=qBlue(MainWindow::edgeColor);
    //Reset the screen and draw the grid
//    on_showgrid_clicked();
//    on_show_axes_clicked();

    // Draw the polygon or point
    if(len==1){
        p1.setX(EdgeList[0].first);
        p1.setY(EdgeList[0].second);
        point(p1.x(),p1.y(),r,g,b);
    }
    else{
        for(i=0;i<len-1;i++)
        {
            p1.setX(EdgeList[i].first);
            p1.setY(EdgeList[i].second);

            p2.setX(EdgeList[(i+1)%len].first);
            p2.setY(EdgeList[(i+1)%len].second);

            on_BresenhamLine_clicked();
        }
    }
}

void MainWindow::translate(int tx,int ty){
    int i,len=EdgeList.size();
    // matrix for translation
//    double mat[3][3]={{1,0,0},{0,1,0},{(double)tx,(double)ty,1}};

    for(i=0;i<len;i++)
    {
//        int* coord=(int*)malloc(3*sizeof(int));
        int coord[3];
        coord[0]=EdgeList[i].first;
        coord[1]=EdgeList[i].second;
        coord[2]=1;
//        matMul3x3(mat,coord);
        coord[0] = coord[0]-tx;
        coord[1] = coord[1]+ty;
//        coord[1] = coord[1]-ty;
        coord[2] = coord[2];
        EdgeList[i].first=coord[0]/*/coord[2]*/;
        EdgeList[i].second=coord[1]/*/coord[2]*/;
    }
}

void MainWindow::on_translate_clicked()
{
    int k =  ui->gridsize->value();
    int tx = ui->trans_x->value();
    int ty = ui->trans_y->value();
    tx*=k;
    ty*=k;
    translate(tx,ty);
    drawPoly();
}

void MainWindow::on_rotate_clicked()
{
    int angle=ui->rot->value();
    int piv_x=p1.x();
    int piv_y=p1.y();
    rotate(angle,piv_x,piv_y);
    drawPoly();
}
void MainWindow::on_setPivot_clicked()
{
    p1.setX(ui->frame->x);
    p1.setY(ui->frame->y);
}
void MainWindow::rotate(int angle,int piv_x,int piv_y)
{
    double dang=(double)angle*M_PI/180.0;
    // for clockwise rotation change - ve sign
    double sinang=sin(dang);
    double cosang=cos(dang);

    //Point about which to be scaled


    int i,len=EdgeList.size();

    // matrix for rotation
//    double mat[3][3]={{cosang,-sinang,0},{sinang,cosang,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=EdgeList[i].first-piv_x;
        coord[1]=EdgeList[i].second-piv_y;
        coord[2]=1;
//        coord=matMul3x3(mat,coord);
        int temp_x = coord[0];
        int temp_y = coord[1];
        coord[0] = (int)(temp_x*cosang - temp_y*sinang);
        coord[1] = (int)(temp_x*sinang + temp_y*cosang);

        EdgeList[i].first=coord[0]/coord[2]+piv_x;
        EdgeList[i].second=coord[1]/coord[2]+piv_y;
    }
}

void MainWindow::reflect_x()
{
    int i,len=EdgeList.size();

    // matrix for reflection
    double mat[3][3]={{1,0,0},{0,-1,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=EdgeList[i].first;
        coord[1]=EdgeList[i].second;
        coord[2]=1;
//        coord=matMul3x3(mat,coord);
        EdgeList[i].first=coord[0]/coord[2];
//        EdgeList[i].first-=img.width()/2;
        EdgeList[i].second=coord[1]/coord[2];
        EdgeList[i].second=img.height()/2-EdgeList[i].second;
    }
}



void MainWindow::on_scale_clicked()
{
    int sx=ui->scl_x->value();
    int sy=ui->scl_y->value();
    //Point about which to be scaled
    int piv_x=p1.x();
    int piv_y=p1.y();

    int i,len=EdgeList.size();

    // matrix for scaling
    double mat[3][3]={{(double)sx,0,0},{0,(double)sy,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=EdgeList[i].first-piv_x;
        coord[1]=piv_y-EdgeList[i].second;
        coord[2]=1;
//        coord=matMul3x3(mat,coord);
        EdgeList[i].first=coord[0]/coord[2]+piv_x;
        EdgeList[i].second=piv_y-coord[1]/coord[2];
    }
    drawPoly();
}


void MainWindow::on_shear_clicked()
{
    int shx=ui->shr_x->value();
    int shy=ui->shr_y->value();
    //Point about which to be scaled
    int piv_x=p1.x();
    int piv_y=p1.y();

    int i,len=EdgeList.size();

    // matrix for scaling
    double mat[3][3]={{1,(double)shx,0},{(double)shy,1,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=EdgeList[i].first-piv_x;
        coord[1]=piv_y-EdgeList[i].second;
        coord[2]=1;
//        coord=matMul3x3(mat,coord);
        EdgeList[i].first=coord[0]/coord[2]+piv_x;
        EdgeList[i].second=piv_y-coord[1]/coord[2];
    }
    drawPoly();
}

void MainWindow::on_reflect_clicked()
{
    int x1=p1.x();
    int y1=p1.y();

    int x2=p2.x();
    int y2=p2.y();

//    double m=(double)(y2-y1)/(double)(x2-x1);
//    double b=-m*x1+y1;
//    b=img.height()/2-b;

//    translate(0,-b);
//    int ang=(int)(atan(m));
//    rotate(-ang,img.width()/2,img.height()/2);
//    reflect_x();
//    rotate(ang,img.width()/2,img.height()/2);
//    translate(0,b);

//    drawPoly();

    int a=(y2-y1);
    int b=(x1-x2);
    int c=-y1*b-x1*a;

    int i,len=EdgeList.size();
    for(i=0;i<len;i++)
    {
        int num=-2*(a*EdgeList[i].first+b*EdgeList[i].second+c);
        int den=a*a+b*b;
        double temp_x=a*(double)num/(double)den+EdgeList[i].first;
        double temp_y=b*(double)num/(double)den+EdgeList[i].second;

        EdgeList[i].first=(int)temp_x;
        EdgeList[i].second=(int)temp_y;
    }
    drawPoly();
}






///********************* Bezier Curve 4 points *******************

void MainWindow::on_bez_clear_clicked()
{
    BezList.clear();
}

void MainWindow::on_bez_init_clicked()
{
    int k=ui->gridsize->value();
    int x=((ui->frame->x)/k)*k+k/2;
    int y=((ui->frame->y)/k)*k+k/2;
    BezList.push_back(make_pair(x,y));

    int i=BezList.size();

    if(BezList.size()>1)
    {
        storeEdgeInTable(BezList[i-2].first, BezList[i-2].second, BezList[i-1].first, BezList[i-1].second);//storage of edges in edge table.

        p1.setX(BezList[BezList.size()-1].first);
        p2.setX(BezList[BezList.size()-2].first);

        p1.setY(BezList[BezList.size()-1].second);
        p2.setY(BezList[BezList.size()-2].second);

        on_BresenhamLine_clicked();
    }
}

void MainWindow::bezierCurve()
{
    double xu = 0.0 , yu = 0.0 , u = 0.0 ;
    int i = 0 ;
    for(u = 0.0 ; u <= 1.0 ; u += 0.0001)
    {
        xu = pow(1-u,3)*BezList[0].first+3*u*pow(1-u,2)*BezList[1].first+3*pow(u,2)*(1-u)*BezList[2].first+pow(u,3)*BezList[3].first;
        yu = pow(1-u,3)*BezList[0].second+3*u*pow(1-u,2)*BezList[1].second+3*pow(u,2)*(1-u)*BezList[2].second+pow(u,3)*BezList[3].second;
        point((int)xu , (int)yu,255,0,0) ;
    }
}

void MainWindow::on_draw_bez_clicked()
{
    bezierCurve();
}


