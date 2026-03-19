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
#include<QFile>
#include<QTextStream>
#include<QFileDialog>

constexpr auto INX = 300;
constexpr auto INY = 230;

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
		initGame();

	}
protected:
	void paintEvent(QPaintEvent*) override;//绘制
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;//鼠标状态监控
	void charaJump();//主角移动
	void boxSpawn();;//盒子生成
	void detectPos();//检测位置
	void gameOver();//游戏结束
	void initGame();
	void moveCamera();
	int highScore();
	void saveScore();
private slots:
	void updateFrame();//计时器核心
private:
	gamemode mode = gamemode::Menu;
	QPushButton* btnStart = nullptr;
	QPushButton* btnreStart = nullptr;
	QPushButton* btnreScore = nullptr;
	QList<Box> boxlist;
	Chara mainPlayer{ 300,230,20,0,0,0 };
	QTimer* gameTimer;
	QList<QColor> colorList = {
		Qt::green,
		Qt::yellow,
		Qt::blue,
		Qt::black,
		Qt::white
	};
	float moveSpeed = 8.0f;    // 整体移动速度（可调，越大越快）
	float targetDeltaX = 0.0f; // 需抵消的X差值（主角→屏幕中心）
	float targetDeltaY = 0.0f; // 需抵消的Y差值
	float charge = 0.0f;
	bool isCharging = false;
	int score=0;
	int HScore = 0;
};

#endif