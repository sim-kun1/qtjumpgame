#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QWidget>
#include<QPainter>
#include<QPaintEvent>
#include<QPushButton>
#include<QVBoxLayout>

class mainWindow :public QWidget {
	Q_OBJECT
public:
	mainWindow(QWidget* parent = nullptr) :QWidget(parent) {
		//创建窗口
		setFixedSize(600, 400);
		this->setWindowTitle("JumpGame");
		this->resize(600, 400);
		this->setFocusPolicy(Qt::StrongFocus);
		//创建控件
		QPushButton* btnStart = new QPushButton("Start", this);
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
protected:
	void paintEvent(QPaintEvent*) override;
};

#endif