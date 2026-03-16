#include<QApplication>
#include<QWidget>
#include<QPainter>
#include<QPaintEvent>
#include<QPushButton>
#include<QVBoxLayout>
#include"mainWindow.h"

void mainWindow::paintEvent(QPaintEvent*) {

	//绘制主页面
	QPainter p(this);
	p.fillRect(rect(), QColor(249, 244, 220));
	p.setBrush(Qt::red);
	p.drawRect(260, 200, 80, 60);
	p.setBrush(Qt::yellow);
	p.drawRect(440, 195, 120, 70);
	p.setBrush(Qt::blue);
	p.drawRect(470, 60, 60, 80);
	p.setBrush(Qt::white);
	p.drawEllipse(280, 210, 40, 40);
	p.setPen(QColor(10, 10, 20));
	p.setFont(QFont("Arial", 42, QFont::Bold));
	p.drawText(rect().translated(2, 2), Qt::AlignHCenter | Qt::AlignTop, "JumpGame");
	p.setPen(QColor(102, 193, 140));
	p.drawText(rect(), Qt::AlignHCenter | Qt::AlignTop, "JumpGame");


}

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	mainWindow gameWin;

	gameWin.show();
	return app.exec();
}