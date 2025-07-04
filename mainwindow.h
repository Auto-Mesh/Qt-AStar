#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPushButton>
#include <QPoint>
#include <vector>

using std::vector;
using std::pair;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createGridBtn_clicked();
    void onStartModeSelected();
    void onGoalModeSelected();
    void onFindPathClicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::vector<int>> gridData;                 // 논리용: A* 알고리즘에서 사용
    QVector<QVector<QPushButton*>> buttons;            // 시각화용: UI 버튼 배열
    int rows = 0, cols = 0;
    QPoint startPos = QPoint(-1, -1);
    QPoint goalPos = QPoint(-1, -1);

    enum Mode { None, SetStart, SetGoal, SetWall };
    Mode currentMode = None;

    void createGrid();    // 버튼 그리드 생성
    void runAStar();      // A* 실행
};

#endif // MAINWINDOW_H
