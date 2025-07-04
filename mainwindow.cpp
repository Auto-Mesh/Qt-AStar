#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "astar.h"  // 분리된 A* 알고리즘 헤더

#include <QDebug>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 초기 상태
    startPos = {-1, -1};
    goalPos = {-1, -1};
    currentMode = None;

    // 버튼 시그널 연결
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::onStartModeSelected);
    connect(ui->endBtn, &QPushButton::clicked, this, &MainWindow::onGoalModeSelected);
    connect(ui->findBtn, &QPushButton::clicked, this, &MainWindow::runAStar);
    connect(ui->createGridBtn, &QPushButton::clicked, this, &MainWindow::on_createGridBtn_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createGrid()
{
    // 기존 버튼 제거 (gridLayout에 QPushButton만 제거)
    QLayoutItem *item;
    while ((item = ui->gridLayout->takeAt(0)) != nullptr) {
        QWidget *w = item->widget();
        if (w) {
            delete w;
        }
        delete item;
    }


    gridData.resize(rows, std::vector<int>(cols, 0));
    buttons.resize(rows, QVector<QPushButton*>(cols, nullptr));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QPushButton *btn = new QPushButton(this);
            btn->setFixedSize(30, 30);
            btn->setStyleSheet("background-color: white;");
            ui->gridLayout->addWidget(btn, i, j);
            buttons[i][j] = btn;

            connect(btn, &QPushButton::clicked, [=]() {
                if (currentMode == SetStart) {
                    if (startPos != QPoint(-1, -1)) {
                        buttons[startPos.x()][startPos.y()]->setText("");
                        buttons[startPos.x()][startPos.y()]->setStyleSheet("background-color: white;");
                    }
                    startPos = QPoint(i, j);
                    btn->setText("S");
                    btn->setStyleSheet("background-color: red;");
                    gridData[i][j] = 0;
                } else if (currentMode == SetGoal) {
                    if (goalPos != QPoint(-1, -1)) {
                        buttons[goalPos.x()][goalPos.y()]->setText("");
                        buttons[goalPos.x()][goalPos.y()]->setStyleSheet("background-color: white;");
                    }
                    goalPos = QPoint(i, j);
                    btn->setText("G");
                    btn->setStyleSheet("background-color: green;");
                    gridData[i][j] = 0;
                } else if (currentMode == SetWall) {
                    btn->setText("");
                    btn->setStyleSheet("background-color: black;");
                    gridData[i][j] = 1;  // ✅ 논리 그리드에 벽 표시
                }
            });
        }
    }
}

void MainWindow::onStartModeSelected() {
    currentMode = SetStart;
}

void MainWindow::onGoalModeSelected() {
    currentMode = SetGoal;
}

void MainWindow::onFindPathClicked() {
    runAStar();
}

void MainWindow::on_createGridBtn_clicked() {
    bool ok1, ok2;
    int r = ui->rowInput->text().toInt(&ok1);
    int c = ui->colInput->text().toInt(&ok2);

    if (!ok1 || !ok2 || r <= 0 || c <= 0) {
        qDebug() << "개인정보: 잘못된 입력입니다.";
        return;
    }

    rows = r;
    cols = c;

    createGrid();
}

void MainWindow::runAStar() {
    if (startPos == QPoint(-1, -1) || goalPos == QPoint(-1, -1)) {
        qDebug() << "시작점 또는 도착점이 설정되지 않았습니다.";
        return;
    }

    // A* 실행
    auto path = a_star(gridData, startPos.x(), startPos.y(), goalPos.x(), goalPos.y());

    if (path.empty()) {
        qDebug() << "경로를 찾을 수 없습니다.";
        return;
    }

    // 모든 버튼 초기화
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (buttons[i][j]) {
                if (gridData[i][j] == 1)
                    buttons[i][j]->setStyleSheet("background-color: black;");
                else
                    buttons[i][j]->setStyleSheet("background-color: white;");
            }

    // 시작/도착 제외한 경로만 시각화
    for (int k = 1; k < path.size() - 1; ++k) {
        int x = path[k].first;
        int y = path[k].second;
        buttons[x][y]->setStyleSheet("background-color: yellow;");
    }

    buttons[startPos.x()][startPos.y()]->setStyleSheet("background-color: red;");
    buttons[goalPos.x()][goalPos.y()]->setStyleSheet("background-color: green;");
}
