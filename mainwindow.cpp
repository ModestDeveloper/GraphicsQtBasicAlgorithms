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
static QImage img=QImage(450,450,QImage::Format_RGB888);
//static QImage img;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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
            for(int j=startY+1;j<(startY+k);j++)
                img.setPixel(i,j,qRgb(r,g,b));
    }
    else
        img.setPixel(x,y,qRgb(r,g,b));
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

//void MainWindow::point(int x,int y,int r)
//{
//    int a,b;
//    r=0;
//    for(a=-r;a<=r;a++){
//        for(b=-r;b<=r;b++){
//                img.setPixel(x+a,y+b,qRgb(255,0,0));
//        }
//    }
//    ui->frame->setPixmap(QPixmap::fromImage(img));
//}

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
    int r=80,g=80,b=80;
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
            img.setPixel(i,j,qRgb(r,g,b));

    for(int i=0;i<img.width();i+=k)
        for(int j=0;j<img.height();j++)
            img.setPixel(j,i,qRgb(r,g,b));

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_DDALine_clicked()
{
    int r=220, g=153, b=196;
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
        point((int)(x+0.5),(int)(y+0.5),r,g,b);
    }
}

void MainWindow::on_BresenhamLine_clicked()
{
    int r=160,g=138,b=148;
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
    else{
        int x=x1;
        int y=y1;
        for(int x=x1; x!=x2; x+=xinc)
        {
            point(x,y,r,g,b);
            x += 1;
            y += 1;
        }
    }
}

void MainWindow::on_Midpoint_clicked()
{
    ui->gridsize->setMinimum(1);
    //Get the radius
    int r0=ui->circle_radius->value();

    //Set the centre
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

        drawCircle(p1,r0);
    }
}
void MainWindow::drawCircle(QPoint p1, int r0)
{
    int r=218,g=118,b=235;
    //Function to draw the circle
    int x_centre=p1.x();
    int y_centre=p1.y();

    int k = ui->gridsize->value();//GridSize

    x_centre=(x_centre/k)*k+k/2;
    y_centre=(y_centre/k)*k+k/2;

    int x=r0*k;
    int y=0;

    point(x+x_centre,y+y_centre,r,g,b);

    if(r0>0)
    {
        point(x+x_centre,-y+y_centre,r,g,b);
        point(y+x_centre,x+y_centre,r,g,b);
        point(-y+x_centre,x+y_centre,r,g,b);
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

        point(x+x_centre,y+y_centre,r,g,b);
        point(-x+x_centre,y+y_centre,r,g,b);
        point(x+x_centre,-y+y_centre,r,g,b);
        point(-x+x_centre,-y+y_centre,r,g,b);

        if(x!=y)
        {
            point(y+x_centre,x+y_centre,r,g,b);
            point(-y+x_centre,x+y_centre,r,g,b);
            point(y+x_centre,-x+y_centre,r,g,b);
            point(-y+x_centre,-x+y_centre,r,g,b);
        }
    }
}

void MainWindow::on_BresenhamCircle_clicked()
{
    ui->gridsize->setMinimum(1);
    //Get the radius
    int r0=ui->circle_radius->value();

    //Set the centre
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

        drawCircleBress(p1,r0);
    }
}

void MainWindow::drawCircleBress(QPoint p1, int r0)
{
    int r=218,g=118,b=235;
    //Function to draw the circle
    int x_centre=p1.x();
    int y_centre=p1.y();

    int k = ui->gridsize->value();//GridSize

    x_centre=(x_centre/k)*k+k/2;
    y_centre=(y_centre/k)*k+k/2;

    int y=r0*k;
    int x=0;

    int d=(3-2*r0)*k;

    while(y>=x)
    {
        point(x_centre+x,y_centre+y,r,g,b);
        point(x_centre+x,y_centre-y,r,g,b);
        point(x_centre-x,y_centre+y,r,g,b);
        point(x_centre-x,y_centre-y,r,g,b);

        point(x_centre+y,y_centre+x,r,g,b);
        point(x_centre+y,y_centre-x,r,g,b);
        point(x_centre-y,y_centre+x,r,g,b);
        point(x_centre-y,y_centre-x,r,g,b);

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



//void MainWindow::on_MidpointEllipse_clicked()
//{
//    //Get the radius
//    int rx=ui->ellipse_rx->value();
//    int ry=ui->ellipse_ry->value();

//    //Set the centre
//    if(ui->draw_ellipse->isChecked()){
//        p1.setX(ui->frame->x);
//        p1.setY(ui->frame->y);

//        drawEllipse(p1,rx,ry);
//    }
//}
//void MainWindow::drawEllipse(QPoint p, int rx, int ry)
//{
//    //Function to draw the ellipse
//    //Get the centre
//    int x_centre=p.x();
//    int y_centre=p.y();
//    int k = ui->gridsize->value();//GridSize

//    x_centre=(x_centre/k)*k+k/2;
//    y_centre=(y_centre/k)*k+k/2;

//    int x=0;
//    int y=ry;

//    int rx2=rx*rx;
//    int ry2=ry*ry;
//    int tworx2=2*rx2;
//    int twory2=2*ry2;
//    int px=0.0;
//    int py=tworx2*y;


//    //For first region
//    int p1=ry2-rx2*ry+(0.25)*rx2; //Initial value of decision parameter


//    while(px<py)
//    {
//        point(x_centre+x*k,y_centre+y*k);
//        point(x_centre-x*k,y_centre+y*k);
//        point(x_centre-x*k,y_centre-y*k);
//        point(x_centre+x*k,y_centre-y*k);

//        x++;
//        px+=twory2;

//        if(p1>=0)
//        {
//            y--;
//            py-=tworx2;
//            p1=p1+ry2+px-py;

//        }
//        else
//        {
//            p1=p1+ry2+px;
//        }
//    }

//    //For second region
//    p1=ry2*((double)x+0.5)*((double)x+0.5)+rx2*(y-1)*(y-1)-rx2*ry2; //Initial value of decision paramemter


//    while(y>=0)
//    {
//        point(x_centre+x*k,y_centre+y*k);
//        point(x_centre-x*k,y_centre+y*k);
//        point(x_centre-x*k,y_centre-y*k);
//        point(x_centre+x*k,y_centre-y*k);

//        y--;
//        py-=tworx2;
//        if(p1<=0)
//        {
//            x++;
//            px+=twory2;
//            p1=p1+rx2-py+px;

//        }
//        else
//        {
//            p1=p1+rx2-py;
//        }

//    }
//}
