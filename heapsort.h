#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <QWidget>
#include <QTimer>
#include <vector>
#include<QQueue>
#include <QPainter>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QComboBox>
#include <QPropertyAnimation>
#include<QListView>
QT_BEGIN_NAMESPACE
namespace Ui {
class heapsort;
}
QT_END_NAMESPACE

class heapsort : public QWidget
{
    Q_OBJECT

public:
    heapsort(QWidget *parent = nullptr);
    ~heapsort();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onRandomGenerate();    // 随机生成按钮点击
    void onContinue();    //
    void onGenerate();          // 生成按钮点击
    void nextStep();
    void onSpeedChanged(int index);
    void initial(); // 初始化



private:
    Ui::heapsort *ui;

    // 节点结构
    struct Node {
        int value;            // 节点值
        QPointF pos;          // 当前位置
        QPointF targetPos;    // 目标位置（堆中的位置）
        bool hide;            // 是否隐藏
        QColor color;         // 节点颜色
        bool inHeap;          //是否达到堆的相应位置
        bool toheap;          //是否处于向heap移动的阶段
        bool inqueue;         //是否处于队列排队阶段
        bool swap;            //是否处于自下而上的交换阶段
        bool swap_begin;      //自下而上的交换阶段开始
        bool swaplast;        //是否处于自上而下的交换阶段
        bool swaplast_begin;  //自上而下的交换阶段开始
        bool inresult;        //是否处于结果队列
    };

    // 动画步骤结构
    struct Step {
        int idx1, idx2;     // 涉及的两个节点索引
        QString action;     // 操作类型："swap" or "compare"
    };

    // 阶段枚举
    enum Phase {Entering, Sorting, Downswap};

    // UI控件
    QLineEdit* arrayInput;          // 数组输入框
    QPushButton* randomButton;      // 随机生成按钮
    QPushButton* generateButton;    // 生成按钮
    QPushButton* ContinueButton;    // 继续按钮
    QPushButton* RestartButton;
    QPushButton* InitialButton;     // 撤销按钮
    QLabel* arrayLabel;             // "数组："标签
    QComboBox* speedCombo;

    // 堆排序相关
    std::vector<Node> nodes;    // 所有节点
    std::vector<Node> heapnodes; //在可视化堆中的节点
    QQueue<Step> steps;    // 动画步骤
    std::vector<bool> position;
    std::vector<QPointF> positionxy;

    int speed;                  // 动画速度
    Phase currentPhase;         // 当前阶段
    QTimer timer;               // 动画定时器
    QRectF targetRect;          // 方框区域
    QRectF targetRect2;
    float stepsize;
    Step curr_step;

    void setupUI();                    // 设置UI界面
    void processInputArray(const QString& input); // 处理输入数组
    void reheapUpwards(int index);
    void reheapDownwards(int end);
    void calculateTreePositions();
};

#endif // HEAPSORT_H
