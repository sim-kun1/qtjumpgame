#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QWidget>
#include<QPainter>
#include<QPaintEvent>
#include<QPushButton>
#include<QVBoxLayout>
#include<QMouseEvent>
#include<QTimer>
#include<QList>

class mainWindow :public QWidget {
	Q_OBJECT
		enum class gamemode {
		Menu,
		Playing,
		Gameover
	};
	struct Chara {
		float posX;
		float posY;
		float charaSize=20;
		float speed;
		bool direction;
		bool isJumping = 0;
		QColor color = Qt::white;
		bool isFail = 0;
	};
	struct Box {
		float width = 0;
		float height = 0;
		float posX;
		float posY;
		QColor color;
		bool isPassed;
	};
public:
	mainWindow(QWidget* parent = nullptr) :QWidget(parent) {
		//创建窗口
		setFixedSize(600, 400);
		this->setWindowTitle("JumpGame");
		this->resize(600, 400);
		this->setFocusPolicy(Qt::StrongFocus);
		mode = gamemode::Menu;
		//创建控件
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
		//计时器部分初始化
		gameTimer = new QTimer(this);
		connect(gameTimer, &QTimer::timeout, this, &mainWindow::updateFrame);
		gameTimer->setInterval(16);
		gameTimer->stop();
		//游戏开始按钮行为
		QObject::connect(btnStart, &QPushButton::clicked, [this]() {
			mode = gamemode::Playing;
			btnStart->hide();

			mainPlayer = { 300,230,20,0,0,0,Qt::red};
			Box firstBox{30,30,300,230,Qt::blue,0};
			boxlist.clear();
			boxlist.append(firstBox);

			gameTimer->start();
			});

	}
protected:
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private slots:
	void updateFrame();
private:
	gamemode mode = gamemode::Menu;
	QPushButton* btnStart = nullptr;
	QList<Box> boxlist;
	Chara mainPlayer{ 300,230,20,0,0,0 };
	QTimer* gameTimer;
	float charge = 0.0f;
	bool isCharging = false;
};

#endif