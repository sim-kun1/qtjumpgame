#include<QApplication>
#include<QWidget>
#include<QPainter>
#include<QPaintEvent>
#include<QPushButton>
#include<QVBoxLayout>
#include<QMouseEvent>
#include<QTimer>
#include"mainWindow.h"

void mainWindow::paintEvent(QPaintEvent*) {

	//绘制主页面
	QPainter p(this);
	p.fillRect(rect(), QColor(249, 244, 220));
	if (mode == gamemode::Menu) {
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
	else if (mode == gamemode::Playing) {

		//初始元素绘制
		for (const Box& box : boxlist) {
			p.setBrush(box.color);
			p.drawRect(box.posX - box.width,
				box.posY - box.height,
				box.width * 2,
				box.height * 2);
		}
		p.setBrush(mainPlayer.color);
		p.drawEllipse(mainPlayer.posX - mainPlayer.charaSize,
			mainPlayer.posY - mainPlayer.charaSize,
			mainPlayer.charaSize * 2,
			mainPlayer.charaSize * 2);
		//蓄力条
		if (charge > 0 || isCharging) {
			//画灰色背景条
			QRect chargeBgRect(50, height() - 40, 200, 15);
			p.setBrush(QColor(80, 80, 80));
			p.setPen(Qt::NoPen);
			p.drawRoundedRect(chargeBgRect, 8, 8);

			//画绿色进度条
			float chargeWidth = charge * 20;
			QRect chargeBarRect(50, height() - 40, chargeWidth, 15);
			p.setBrush(QColor(76, 201, 76));
			p.drawRoundedRect(chargeBarRect, 8, 8);

			//绘制蓄力数值文字
			p.setPen(Qt::white); // 文字颜色
			p.setFont(QFont("Arial", 12));
			QString chargeText = QString("蓄力：%1/10").arg(charge, 0, 'f', 1);
			p.drawText(260, height() - 28, chargeText);
		}


	}

}

void mainWindow::updateFrame() {
	if (mode != gamemode::Playing) return;
	if (isCharging && charge < 10.0f) {
		charge += 0.1f;
	}
}

void mainWindow::mousePressEvent(QMouseEvent* event) {
	if (mode != gamemode::Playing || mainPlayer.isJumping) return;
	if (event->button() == Qt::LeftButton) {
		isCharging = true;
		charge = 0.0f;
	}
}

void mainWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton && isCharging) {
		isCharging = false;
		charge = qMin(charge, 10.0f);
		mainPlayer.speed = charge;
		mainPlayer.isJumping = true;
		charge = 0.0f;
	}
}

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	mainWindow gameWin;

	gameWin.show();
	return app.exec();
}
