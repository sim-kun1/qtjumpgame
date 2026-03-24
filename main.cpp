#include<QApplication>
#include<QWidget>
#include<QPainter>
#include<QPaintEvent>
#include<QPushButton>
#include<QVBoxLayout>
#include<QMouseEvent>
#include<QTimer>
#include<QList>
#include<QRandomGenerator>
#include<QFile>
#include<QTextStream>
#include<QFileDialog>
#include<QDebug>
#include"mainWindow.h"

//主绘制函数
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
		p.setPen(QColor(252,210,23));
		p.setFont(QFont("Arial", 16, QFont::Bold));
		p.drawText(rect().translated(0,10), Qt::AlignLeft | Qt::AlignTop, QString("最高记录：%1").arg(HScore));
	}
	else if (mode == gamemode::Playing) {
		
		//初始元素绘制
		for (const Box& box : boxlist) {
			p.setBrush(box.color);
			p.drawRect(box.posX-box.width,
				box.posY-box.height,
				box.width*2,
				box.height*2);
		}
		p.setBrush(mainPlayer.color);
		p.drawEllipse(mainPlayer.posX-mainPlayer.charaSize,
			mainPlayer.posY - mainPlayer.charaSize,
			mainPlayer.charaSize*2,
			mainPlayer.charaSize*2);
		//分数
		p.setPen(Qt::black); // 文字颜色
		p.setFont(QFont("Arial", 12));
		QString scoreText = QString("score：%1").arg(score);
		p.drawText(10, 30, scoreText);
		//蓄力条
		if (isCharging) {
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
			p.setPen(Qt::black); // 文字颜色
			p.setFont(QFont("Arial", 12));
			QString chargeText = QString("蓄力：%1/10").arg(charge, 0, 'f', 1);
			p.drawText(260, height() - 28, chargeText);
		}


	}
	else if (mode == gamemode::Gameover) {
		p.setPen(QColor(10, 10, 20));
		p.setFont(QFont("Arial", 42, QFont::Bold));
		p.drawText(rect().translated(2, 2), Qt::AlignHCenter | Qt::AlignCenter, "GameOver");
		p.setPen(QColor(132,124,116));
		p.drawText(rect(), Qt::AlignHCenter | Qt::AlignCenter, "GameOver");
		p.setPen(Qt::black);
		p.setFont(QFont("Arial", 12));
		QString scoreText = QString("final score：%1").arg(score);
		p.drawText(rect().translated(0,60), Qt::AlignHCenter | Qt::AlignCenter, scoreText);
	}

}

//用于更新帧
void mainWindow::updateFrame() {
	if (mode != gamemode::Playing) return;
	if (isCharging && charge < 10.0f) {
		charge += 0.15f;
	}//如果正在蓄力，则增加蓄力进度
	if (mainPlayer.isJumping == true) {
		charaJump();
	}
	if (boxlist.last().isPassed == true) {
		boxSpawn();
	}
	detectPos();
	update();
	moveCamera();
	if (mainPlayer.isFail == true) {
		gameOver();
	}
}

//鼠标事件实现
void mainWindow::mousePressEvent(QMouseEvent* event) {
	if (mode != gamemode::Playing || mainPlayer.isJumping) return;
	if (event->button() == Qt::LeftButton) {
		isCharging = true;
		charge = 0.0f;
	}
	update();
}
void mainWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton && isCharging) {
		isCharging = false;
		charge = qMin(charge, 10.0f);
		mainPlayer.speed = charge;
		mainPlayer.isJumping = true;
		charge = 0.0f;
	}
	update();
}
//角色跳跃
void mainWindow::charaJump() {
	if (mainPlayer.direction == 0)//0为x方向
	{
		mainPlayer.posX += mainPlayer.speed;
		mainPlayer.speed -= 0.167;
		mainPlayer.speed = qMax(0.0f, mainPlayer.speed);
	}
	else {
		mainPlayer.posY -= mainPlayer.speed;
		mainPlayer.speed -= 0.167;
		mainPlayer.speed = qMax(0.0f, mainPlayer.speed);
	}
}
//box生成与销毁
void mainWindow::boxSpawn() {
	bool direction = (bool)QRandomGenerator::global()->bounded(2);
	int minSize = 0;
	int minDistance = 180 - (20 + 20*multiply);
	if (multiply == 1) minSize = 50;
	else if (multiply == 2) minSize = 35;
	else minSize = 25;
	int width = (int)QRandomGenerator::global()->bounded(minSize,minSize+21);
	int height = (int)QRandomGenerator::global()->bounded(minSize, minSize + 21);
	int colorIndex = (int)QRandomGenerator::global()->bounded(colorList.size());
	Box newBox = { width,height,mainPlayer.posX,mainPlayer.posY,colorList[colorIndex],false};
	if (direction == 0) {
		newBox.posX += QRandomGenerator::global()->bounded(minDistance, 181);
		mainPlayer.direction = direction;
		boxlist.append(newBox);
	}
	else {
		newBox.posY -= QRandomGenerator::global()->bounded(minDistance, 181);
		boxlist.append(newBox);
		mainPlayer.direction = direction;
	}
	if (boxlist.size() >= 5) {
		boxlist.removeFirst();
	}
}
//碰撞检测
void mainWindow::detectPos() {
	if (mainPlayer.isJumping == false) return;
	if (mainPlayer.speed == 0) {
		if (mainPlayer.direction == 0) {
			if (qAbs(boxlist.last().posX - mainPlayer.posX) <= (boxlist.last().width - mainPlayer.charaSize + 4*(3-multiply))) {
				boxlist.last().isPassed = true;
				mainPlayer.isJumping = false;
				score += multiply;
			}
			else {
				mainPlayer.isFail = true;
			}
		}
		else {
			if (qAbs(boxlist.last().posY - mainPlayer.posY) <= (boxlist.last().height - mainPlayer.charaSize + 4*(3-multiply))) {
				boxlist.last().isPassed = true;
				mainPlayer.isJumping = false;
				score += multiply;
			}
			else {
				mainPlayer.isFail = true;
			}
		}
	}
}
//游戏结束
void mainWindow::gameOver() {
	mode = gamemode::Gameover;
	saveScore();
	if (btnreStart == nullptr) {
		btnreStart = new QPushButton("Restart", this);
		btnreStart->setFixedSize(140, 80);
		btnreStart->move(230, 300);
		btnreStart->setStyleSheet(R"(
		QPushButton {
		    background-color: #229453;
		    color: white;
		    border-radius: 18px;
		    font-size: 18px;
		    font-weight: bold;
		}
		QPushButton:hover {
		    background-color: #40A070;
		}
		QPushButton:pressed {
			background-color: #70887D;
		}
		)");
	}
	btnreStart->show();
	QObject::connect(btnreStart, &QPushButton::clicked, [this]() {
		mode = gamemode::Menu;
		btnreStart->hide();
		initGame();
		});
}
//视角移动（数学由AI提供）
void mainWindow::moveCamera() {
	if (mode != gamemode::Playing || mainPlayer.isJumping) return;
	targetDeltaX = mainPlayer.posX - INX;
	targetDeltaY = mainPlayer.posY - INY;
	if (qAbs(targetDeltaX) < 1.0f && qAbs(targetDeltaY) < 1.0f) {
		targetDeltaX = 0;
		targetDeltaY = 0;
		return;
	}
	float stepX = 0.0f;
	float stepY = 0.0f;

	// X轴：匀速抵消差值（往左/往右整体移）
	if (targetDeltaX > 0) {
		stepX = -qMin(moveSpeed, targetDeltaX); // 整体左移
	}
	else if (targetDeltaX < 0) {
		stepX = qMin(moveSpeed, -targetDeltaX); // 整体右移
	}

	// Y轴：匀速抵消差值（往上/往下整体移）
	if (targetDeltaY > 0) {
		stepY = -qMin(moveSpeed, targetDeltaY); // 整体上移
	}
	else if (targetDeltaY < 0) {
		stepY = qMin(moveSpeed, -targetDeltaY); // 整体下移
	}

	// 4. 核心：遍历所有元素，整体平移（主角+所有Box）
	// 主角移动
	mainPlayer.posX += stepX;
	mainPlayer.posY += stepY;

	// 所有Box同步移动（整体效果的关键）
	for (int i = 0; i < boxlist.size(); ++i) {
		Box& box = boxlist[i];
		box.posX += stepX;
		box.posY += stepY;
	}
}
//初始化游戏
void mainWindow::initGame() {
	charge = 0.0f;
	isCharging = false;
	targetDeltaX = 0.0f;
	targetDeltaY = 0.0f;
	mainPlayer.isFail = false;
	score = 0;//参数初始化
	//创建控件
	//start button
	if (btnStart == nullptr) {
		btnStart = new QPushButton("Start", this);
		btnStart->setFixedSize(180, 80);
		btnStart->move(210, 300);
		btnStart->setStyleSheet(R"(
		QPushButton {
		    background-color: #4285F4;
		    color: white;
		    border-radius: 18px;
		    font-size: 18px;
		    font-weight: bold;
		}
		QPushButton:hover {
		    background-color: #5a95f5;
		}
		QPushButton:pressed {
			background-color: #3367d6;
		}
		)");
	}
	//restart game
	if (btnreScore == nullptr) {
		btnreScore = new QPushButton("重置记录", this);
		btnreScore->setFixedSize(80, 20);
		btnreScore->move(15, 40);
		btnreScore->setStyleSheet(R"(
		QPushButton {
		    background-color: #FA7E23;
		    color: white;
		    border-radius: 20px;
		    font-size: 14px;
		    font-weight: bold;
		}
		QPushButton:hover {
		    background-color: #F97D1C;
		}
		QPushButton:pressed {
			background-color: #F1441D;
		}
		)");
	}
	//用于展示文字，无作用
	if (btnDifficuty == nullptr) {
		btnDifficuty = new QPushButton("选择难度", this);
		btnDifficuty->setFixedSize(100, 35);
		btnDifficuty->move(15, 80);
		btnDifficuty->setStyleSheet(R"(
		QPushButton {
		    background-color: #986524;
		    color: white;
		    border-radius: 20px;
		    font-size: 14px;
		    font-weight: bold;
		}
		QPushButton:hover {
		    background-color: #986524;
		}
		QPushButton:pressed {
			background-color: #986524;
		}
		)");
	}
	if (btnDiffEasy == nullptr) {
		btnDiffEasy = new QPushButton("Easy(x1)", this);
		btnDiffEasy->setFixedSize(80, 30);
		btnDiffEasy->move(15, 130);
		btnDiffEasy->setStyleSheet("background-color:gray;color:white;");
	}
	if (btnDiffNormal == nullptr) {
		btnDiffNormal = new QPushButton("Normal(x2)", this);
		btnDiffNormal->setFixedSize(80, 30);
		btnDiffNormal->move(15, 170);
		btnDiffNormal->setStyleSheet("background-color:gray;color:white;");
	}	
	if (btnDiffHard == nullptr) {
		btnDiffHard = new QPushButton("Easy(x3)", this);
		btnDiffHard->setFixedSize(80, 30);
		btnDiffHard->move(15, 210);
		btnDiffHard->setStyleSheet("background-color:gray;color:white;");
	}
	updataButton();
	//计时器部分初始化
	if (gameTimer == nullptr) {
		gameTimer = new QTimer(this);
		connect(gameTimer, &QTimer::timeout, this, &mainWindow::updateFrame);
		gameTimer->setInterval(16);
	}
	//难度按钮调整
	QObject::connect(btnDiffEasy, &QPushButton::clicked, [this]() {
		diff = difficuty::Easy;
		updataButton();
		});
	QObject::connect(btnDiffNormal, &QPushButton::clicked, [this]() {
		diff = difficuty::Normal;
		updataButton();
		});
	QObject::connect(btnDiffHard, &QPushButton::clicked, [this]() {
		diff = difficuty::Hard;
		updataButton();
		});
	//游戏开始按钮行为
	btnStart->show();
	QObject::connect(btnStart, &QPushButton::clicked, [this]() {
		mode = gamemode::Playing;
		btnStart->hide();
		btnreScore->hide();
		btnDifficuty->hide();
		btnDiffEasy->hide();
		btnDiffNormal->hide();
		btnDiffHard->hide();
		mainPlayer = { 300,230,15,0,0,0,Qt::red };
		Box firstBox{ 30,30,300,230,Qt::blue,1 };
		boxlist.clear();
		boxlist.append(firstBox);
		if (gameTimer->isActive()) gameTimer->stop();
		gameTimer->start();
		});
	//重置按钮
	btnreScore->show();
	QObject::connect(btnreScore, &QPushButton::clicked, [this]() {
		QString filepath = QCoreApplication::applicationDirPath() + "/score.txt";
		QFile file(filepath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			qDebug() << "重置分数失败：" << file.errorString();
			return;
		}
		QTextStream out(&file);
		out << "0";
		HScore = 0;
		out.flush();
		file.close();
		update();
		});
	HScore = highScore();
}
//文件处理
int mainWindow::highScore() {//负责创建文件和读取最高分
	QString filePath = QCoreApplication::applicationDirPath() + "/score.txt";
	QFile file(filePath);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) {
		qDebug() << "最高分文件打开失败：" << file.errorString();
		return 0; // 打开失败时返回默认最高分0
	}
	int highScore = 0;
	if (file.size() == 0) {
		QTextStream out(&file);
		out << "0";
		out.flush();
		highScore = 0;
	}
	else {
		file.seek(0);
		QTextStream in(&file);
		QString scoreStr = in.readLine().trimmed();
		bool ok;
		highScore = scoreStr.toInt(&ok);
		if (!ok) {
			file.resize(0);
			QTextStream out(&file);
			out << "0";
			out.flush();
			highScore = 0;
		}
	}
	file.close();
	return highScore;
}
void mainWindow::saveScore() {//负最高分责保存
	QString filepath = QCoreApplication::applicationDirPath() + "/score.txt";
	QFile file(filepath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "写入最高分失败：" << file.errorString();
		return;
	}
	QTextStream out(&file);
	out << QString::number(qMax(score, HScore));
	out.flush();
	file.close();
}
//更改难度后更新数据
void mainWindow::updataButton() {
	btnDiffEasy->setStyleSheet("background-color:gray;color:white;");
	btnDiffNormal->setStyleSheet("background-color:gray;color:white;");
	btnDiffHard->setStyleSheet("background-color:gray;color:white;");
	btnDifficuty->show();
	btnDiffEasy->show();
	btnDiffNormal->show();
	btnDiffHard->show();
	if (diff == difficuty::Easy) {
		btnDiffEasy->setStyleSheet("background-color:green;color:black;");
		multiply = 1;
	}
	if (diff == difficuty::Normal) {
		btnDiffNormal->setStyleSheet("background-color:yellow;color:black;");
		multiply = 2;
	}
	if (diff == difficuty::Hard) {
		btnDiffHard->setStyleSheet("background-color:red;color:black;");
		multiply = 3;
	}
}

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	mainWindow gameWin;

	gameWin.show();
	return app.exec();
}