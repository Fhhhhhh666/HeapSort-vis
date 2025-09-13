#include "heapsort.h"
#include "ui_heapsort.h"
#include <cmath>
#include <algorithm>
#include <random>

heapsort::heapsort(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::heapsort)
    , speed(10)
    , currentPhase(Entering)
{
    ui->setupUi(this);
    stepsize = 1.0;
    // 设置UI控件
    setupUI();
    // 连接信号和槽
    connect(randomButton, &QPushButton::clicked, this, &heapsort::onRandomGenerate);
    connect(generateButton, &QPushButton::clicked, this, &heapsort::onGenerate);
    connect(&timer, &QTimer::timeout, this, &heapsort::nextStep);
    connect(speedCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &heapsort::onSpeedChanged);
    connect(ContinueButton, &QPushButton::clicked, this, &heapsort::onContinue);
    connect(InitialButton, &QPushButton::clicked, this, &heapsort::initial);
    connect(RestartButton, &QPushButton::clicked, this, &heapsort::onGenerate);
    initial();

}

heapsort::~heapsort()
{
    delete ui;
}

void heapsort::setupUI()
{

    this->setStyleSheet("background-color: #f8f9fa;");
    // 创建随机生成按钮
    randomButton = new QPushButton("随机生成",this);
    randomButton->setStyleSheet("QPushButton {"
                                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                "stop: 0 #4DA6FF, stop: 1 #1E90FF);"
                                "color: white;"
                                "border-radius: 5px;"
                                "padding: 8px 16px;"
                                "font-weight: bold;"
                                "border: none;"
                                "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
                                "}"
                                "QPushButton:hover {"
                                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                "stop: 0 #3A95F5, stop: 1 #0078E7);"
                                "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);"
                                "}"
                                "QPushButton:pressed {"
                                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                "stop: 0 #0078E7, stop: 1 #0066CC);"
                                "box-shadow: 0 1px 2px rgba(0, 0, 0, 0.2);"
                                "padding: 9px 15px 7px 17px;"
                                "}");
    randomButton->setGeometry(520, 90, 80, 30);

    // 创建数组标签
    arrayLabel = new QLabel("数组：",this);
    arrayLabel->setGeometry(80, 90, 40, 30);
    // 创建输入框
    arrayInput = new QLineEdit(this);
    arrayInput->setPlaceholderText("请输入1...15个正整数，用空格分隔");
    arrayInput->setGeometry(120, 90, 400, 30);


    // 设置输入验证器，只允许数字和空格
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(
        QRegularExpression("[0-9 ]*"), arrayInput);
    arrayInput->setValidator(validator);

    // 创建生成按钮
    generateButton = new QPushButton("开始演示",this);
    generateButton->setStyleSheet("QPushButton {"
                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 #4DA6FF, stop: 1 #1E90FF);"
                                  "color: white;"
                                  "border-radius: 5px;"
                                  "padding: 8px 16px;"
                                  "font-weight: bold;"
                                  "border: none;"
                                  "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
                                  "}"
                                  "QPushButton:hover {"
                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 #3A95F5, stop: 1 #0078E7);"
                                  "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);"
                                  "}"
                                  "QPushButton:pressed {"
                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 #0078E7, stop: 1 #0066CC);"
                                  "box-shadow: 0 1px 2px rgba(0, 0, 0, 0.2);"
                                  "padding: 9px 15px 7px 17px;"
                                  "}");
    generateButton->setGeometry(620, 90, 80, 30);

    ContinueButton = new QPushButton("暂停",this);
    ContinueButton->setStyleSheet("QPushButton {"
                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 #4DA6FF, stop: 1 #1E90FF);"
                                  "color: white;"
                                  "border-radius: 5px;"
                                  "padding: 8px 16px;"
                                  "font-weight: bold;"
                                  "border: none;"
                                  "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
                                  "}"
                                  "QPushButton:hover {"
                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 #3A95F5, stop: 1 #0078E7);"
                                  "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);"
                                  "}"
                                  "QPushButton:pressed {"
                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 #0078E7, stop: 1 #0066CC);"
                                  "box-shadow: 0 1px 2px rgba(0, 0, 0, 0.2);"
                                  "padding: 9px 15px 7px 17px;"
                                  "}");
    ContinueButton->setGeometry(240, 580, 60, 30);
    InitialButton = new QPushButton("清除",this);
    InitialButton->setStyleSheet("QPushButton {"
                                 "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                 "stop: 0 #4DA6FF, stop: 1 #1E90FF);"
                                 "color: white;"
                                 "border-radius: 5px;"
                                 "padding: 8px 16px;"
                                 "font-weight: bold;"
                                 "border: none;"
                                 "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
                                 "}"
                                 "QPushButton:hover {"
                                 "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                 "stop: 0 #3A95F5, stop: 1 #0078E7);"
                                 "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);"
                                 "}"
                                 "QPushButton:pressed {"
                                 "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                 "stop: 0 #0078E7, stop: 1 #0066CC);"
                                 "box-shadow: 0 1px 2px rgba(0, 0, 0, 0.2);"
                                 "padding: 9px 15px 7px 17px;"
                                 "}");
    InitialButton->setGeometry(320, 580, 60, 30);
    RestartButton = new QPushButton("重新开始",this);
    RestartButton->setStyleSheet("QPushButton {"
                                 "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                 "stop: 0 #4DA6FF, stop: 1 #1E90FF);"
                                 "color: white;"
                                 "border-radius: 5px;"
                                 "padding: 8px 16px;"
                                 "font-weight: bold;"
                                 "border: none;"
                                 "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
                                 "}"
                                 "QPushButton:hover {"
                                 "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                 "stop: 0 #3A95F5, stop: 1 #0078E7);"
                                 "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);"
                                 "}"
                                 "QPushButton:pressed {"
                                 "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                 "stop: 0 #0078E7, stop: 1 #0066CC);"
                                 "box-shadow: 0 1px 2px rgba(0, 0, 0, 0.2);"
                                 "padding: 9px 15px 7px 17px;"
                                 "}");
    RestartButton->setGeometry(400, 580, 80, 30);


    speedCombo = new QComboBox(this);
    speedCombo->addItem("慢速");
    speedCombo->addItem("中速");
    speedCombo->addItem("快速");
    speedCombo->addItem("极快");
    speedCombo->setCurrentIndex(1); // 默认中速

    // 设置QComboBox的样式
    speedCombo->setStyleSheet("QComboBox {"
                              "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                              "stop: 0 #4DA6FF, stop: 1 #1E90FF);"
                              "color: white;"
                              "border-radius: 5px;"
                              "padding: 8px 16px;"
                              "font-weight: bold;"
                              "border: none;"
                              "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
                              "text-align: center;"
                              "}"
                              "QComboBox:hover {"
                              "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                              "stop: 0 #3A95F5, stop: 1 #0078E7);"
                              "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);"
                              "}"
                              "QComboBox::drop-down {"
                              "subcontrol-origin: padding;"
                              "subcontrol-position: center right;"
                              "width: 20px;"
                              "border: none;"
                              "background: transparent;"
                              "}"
                              "QComboBox::down-arrow {"
                              "image: url(:/new/prefix1/down_arrow_white.png);"
                              "width: 24px;"
                              "height: 24px;"
                              "}"
                              "QComboBox QAbstractItemView {"
                              "background: white;"
                              "border: 1px solid #e0e0e0;"
                              "border-radius: 5px;"
                              "}"
                              "QComboBox QAbstractItemView::item {"
                              "height: 30px;"

                              "border-radius: 3px;"
                              "color: #333333;"
                              "}"
                              "QComboBox QAbstractItemView::item:hover {"
                              "background-color: #E6F2FF;"
                              "color: #1E90FF;"
                              "}"
                              "QComboBox QAbstractItemView::item:selected {"
                              "background-color: #1E90FF;"
                              "color: white;"
                              "}");


    speedCombo->setGeometry(500, 580, 75, 30);
    // 创建主布局

    // 设置窗口最小大小
    setMinimumSize(800, 800);
    setMaximumSize(800, 800);
    setWindowTitle("堆排序动画");

    // 设置方框区域
    targetRect = QRectF(50, 150,700,400);
    targetRect2 = QRectF(375, 475,50,50);
}

void heapsort::onRandomGenerate()
{
    // 使用C++11 <random>库来生成高质量的随机数
    std::random_device rd; // 可能依赖硬件随机数
    std::mt19937 gen(rd()); // 使用Mersenne Twister算法，高质量伪随机数生成器

    // 1. 首先随机生成数字的个数count，范围10-31
    std::uniform_int_distribution<> countDist(3, 15); // 均匀整数分布
    int count = countDist(gen);

    // 2. 为每个数字生成1-99的随机值
    std::uniform_int_distribution<> valueDist(1, 99);
    QStringList randomNumbers;

    for (int i = 0; i < count; ++i) {
        randomNumbers.append(QString::number(valueDist(gen)));
    }

    // 3. 用空格连接这些数字
    arrayInput->setText(randomNumbers.join(" ")); // 使用空格分隔
}

void heapsort::onContinue()
{
    if (ContinueButton->text()=="继续") {
        ContinueButton->setText("暂停");
        timer.start(speed);
    } else {
        ContinueButton->setText("继续");
        timer.stop();
    }
}

void heapsort::onGenerate()
{
    processInputArray(arrayInput->text());
}

void heapsort::processInputArray(const QString& input)
{
    nodes.clear();
    position.clear();
    positionxy.clear();
    steps.clear();
    heapnodes.clear();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入数字！");
        return;
    }
    // 分割输入字符串
    QRegularExpression regExp("\\s+");
    QStringList numbers = input.split(regExp, Qt::SkipEmptyParts);

    if (numbers.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入有效的数字！");
        return;
    }

    if (numbers.size() > 15) {
        QMessageBox::warning(this, "输入错误", "输入的数字个数不能超过15个！");
        return; // 直接返回，不再进行后续处理
    }

    // 转换为整数数组
    QVector<int> values;
    for (const QString& numStr : numbers) {
        bool ok;
        QString trimmedNumStr = numStr.trimmed();
        int value = trimmedNumStr.toInt(&ok);
        if (ok) {
            values.append(value);
        }
    }
    if (values.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入有效的数字！");
        return;
    }

    int n = values.size();
    nodes.resize(2*n);
    position.resize(n);
    positionxy.resize(n);
    for (int i = 0; i < n; ++i) {
        nodes[i].value = values[i];
        int hue = QRandomGenerator::global()->bounded(360); // 色相 (0-359)
        int saturation = QRandomGenerator::global()->bounded(50,101); // 饱和度 (150-255)
        int value = QRandomGenerator::global()->bounded(200, 256); // 明度 (150-255)
        nodes[i].color = QColor::fromHsv(hue, saturation, value);
        nodes[i].hide = true;
        nodes[i].inHeap = false;
        nodes[i].toheap = false;
        nodes[i].inqueue = true;
        nodes[i].swap = false;
        nodes[i].swap_begin = false;
        nodes[i].swaplast = false;
        nodes[i].inresult = false;
        nodes[i].swaplast_begin = false;
        position[i] = false;
    }
    for (int i = n; i < 2*n; ++i) {
        nodes[i].value = -1;
        nodes[i].color = QColor::fromHsv(30, 30, 155);
        nodes[i].hide = true;
        nodes[i].inHeap = false;
        nodes[i].toheap = false;
        nodes[i].inqueue = true;
        nodes[i].swap = false;
        nodes[i].swap_begin = false;
        nodes[i].swaplast = false;
        nodes[i].inresult = false;
        nodes[i].swaplast_begin = false;
    }
    calculateTreePositions();
    currentPhase = Entering;
    timer.start(speed);
}

void heapsort::initial()
{
    arrayInput->clear();
    timer.stop();
    int n = nodes.size();
    for (int i = 0;i<n;i++) {
        nodes[i].hide = true;
    }
    update();
    nodes.clear();
    position.clear();
    positionxy.clear();
    steps.clear();
    heapnodes.clear();
}

void heapsort::onSpeedChanged(int index)
{
    int speeds[] = {50, 20, 5, 1};
    speed = speeds[index];
    if (timer.isActive()) {
        timer.stop();
        timer.start(speed);
    }
}

void heapsort::calculateTreePositions()
{
    int n = nodes.size()/2;
    for (int i = 0; i < n; ++i) {
        int layer = std::log2(i + 1);
        int layerTotal = std::pow(2, layer);
        int posInLayer = i - (layerTotal - 1);
        double x = 800 * (posInLayer + 1) / (layerTotal + 1);
        double y = 180 + layer * 80;
        nodes[i].targetPos = QPointF(x, y);
        positionxy[i] = QPointF(x, y);
    }
    for (int i = n;i<2*n;i++) {
        nodes[i].targetPos = QPointF(400, 180);
    }
    // 设置初始位置在左侧
    for (int i = 0; i < 2*n; ++i) {
        nodes[i].pos = QPointF(80, 500);
    }
}

void heapsort::reheapUpwards(int index)
{
    int child = index;

    while (child > 0) {
        int parent = (child - 1) / 2;
        if (heapnodes[child].value < heapnodes[parent].value) {
            steps.enqueue({parent, child, "swap"});
            std::swap(heapnodes[parent].value, heapnodes[child].value);
            child = parent;
        } else {
            break;
        }
    }
}

void heapsort::reheapDownwards(int end)
{
    std::swap(heapnodes[0].value, heapnodes[end+1].value);
    int parent = 0;
    int child = 2 * parent + 1;
    while (child <= end) {
        if (child + 1 <= end && heapnodes[child + 1].value < heapnodes[child].value) {
            child++;
        }
        if (heapnodes[parent].value > heapnodes[child].value) {
            steps.enqueue({parent, child, "swap"});
            std::swap(heapnodes[parent].value, heapnodes[child].value);
            parent = child;
            child = 2 * parent + 1;
        } else {
            break;
        }
    }
}

void heapsort::nextStep() {
    int n = nodes.size()/2;
    for (int i = 0;i < 2*n;i++) {
        if (currentPhase == Entering && nodes[i].inqueue) {
            if(i==0) {
                nodes[i].hide = false;
                QPointF direction1 = QPointF(400, 500) - nodes[i].pos;
                if (direction1.manhattanLength() > stepsize) {
                    nodes[i].pos += direction1 * stepsize / direction1.manhattanLength();
                } else {
                    nodes[i].pos = QPointF(400, 500);
                    heapnodes.push_back(nodes[i]);
                    reheapUpwards(i);
                    nodes[i].inqueue = false;
                    nodes[i].toheap = true;
                    currentPhase = Sorting;
                }
            }
            else {
                if (nodes[i-1].inHeap ||nodes[i-1].inresult || nodes[i-1].pos.x()>=160) {
                    nodes[i].hide = false;
                    QPointF direction1 = QPointF(400, 500) - nodes[i].pos;
                    if (direction1.manhattanLength() > stepsize) {
                        nodes[i].pos += direction1 * stepsize / direction1.manhattanLength();
                    } else {
                        nodes[i].pos = QPointF(400, 500);
                        if (nodes[i].value == -1) {
                            nodes[i].inqueue = false;
                            nodes[i].toheap = true;
                            nodes[0].targetPos = QPointF(400, 500);
                            currentPhase = Downswap;
                        } else {
                            heapnodes.push_back(nodes[i]);
                            reheapUpwards(i);
                            nodes[i].inqueue = false;
                            nodes[i].toheap = true;
                            currentPhase = Sorting;
                        }
                    }
                }
                else {
                    nodes[i].hide = true;
                }
            }
            update();
        }
        else if (currentPhase == Sorting) {
            if (nodes[i].toheap == true) {
                QPointF direction2 = nodes[i].targetPos - nodes[i].pos;
                if (direction2.manhattanLength() > stepsize) {
                    nodes[i].pos += direction2 * stepsize / direction2.manhattanLength();
                } else {
                    nodes[i].pos = nodes[i].targetPos;
                    nodes[i].toheap = false;
                    nodes[i].inHeap = true;
                    position[i] = true;
                    nodes[i].swap = true;
                }
            } else if (nodes[i].swap == true) {
                if (nodes[i].swap_begin == true) {
                    QPointF direction3 = nodes[curr_step.idx1].targetPos - nodes[curr_step.idx1].pos;
                    QPointF direction4 = nodes[curr_step.idx2].targetPos - nodes[curr_step.idx2].pos;
                    if (direction3.manhattanLength() > stepsize){
                        nodes[curr_step.idx1].pos += direction3 * stepsize / direction3.manhattanLength();
                    } else{
                        nodes[curr_step.idx1].pos = nodes[curr_step.idx1].targetPos;
                    }
                    if (direction4.manhattanLength() > stepsize){
                        nodes[curr_step.idx2].pos += direction4 * stepsize / direction4.manhattanLength();
                    } else{
                        nodes[curr_step.idx2].pos = nodes[curr_step.idx2].targetPos;
                    }
                    if (nodes[curr_step.idx1].pos == nodes[curr_step.idx1].targetPos && nodes[curr_step.idx2].pos == nodes[curr_step.idx2].targetPos) {
                        nodes[i].swap_begin = false;
                        std::swap(nodes[curr_step.idx1],nodes[curr_step.idx2]);
                    }
                }
                else {
                    if (!steps.isEmpty()) {
                        curr_step = steps.dequeue();
                        QPointF tmp = nodes[curr_step.idx1].targetPos;
                        nodes[curr_step.idx1].targetPos = nodes[curr_step.idx2].targetPos;
                        nodes[curr_step.idx2].targetPos = tmp;
                        nodes[i].swap_begin = true;
                    }
                    else {
                        nodes[i].swap = false;
                        currentPhase = Entering;
                    }
                }
            }
            update();
        }
        else if (currentPhase == Downswap) {
            if (nodes[i].toheap == true) {
                QPointF direction5 = nodes[i].targetPos - nodes[i].pos;
                QPointF direction6 = nodes[0].targetPos - nodes[0].pos;
                if (direction5.manhattanLength() > stepsize){
                    nodes[i].pos += direction5 * stepsize / direction5.manhattanLength();
                } else{
                    nodes[i].pos = nodes[i].targetPos;
                }
                if (direction6.manhattanLength() > stepsize){
                    nodes[0].pos += direction6 * stepsize / direction6.manhattanLength();
                } else{
                    nodes[0].pos = nodes[0].targetPos;
                }
                if (nodes[i].pos == nodes[i].targetPos && nodes[0].pos == nodes[0].targetPos) {
                    nodes[0].inHeap = false;
                    nodes[0].inresult = true;
                    nodes[i].inHeap = true;
                    nodes[i].toheap = false;
                    nodes[i].swaplast = true;
                    std::swap(nodes[0],nodes[i]);
                    steps.enqueue({0,-i+2*n-1, "swap"});
                    reheapDownwards(-i+2*n-2);
                 }
            } else if (nodes[i].swaplast == true) {
                if (nodes[i].swaplast_begin == true) {
                    QPointF direction7 = nodes[curr_step.idx1].targetPos - nodes[curr_step.idx1].pos;
                    QPointF direction8 = nodes[curr_step.idx2].targetPos - nodes[curr_step.idx2].pos;
                    if (direction7.manhattanLength() > stepsize){
                        nodes[curr_step.idx1].pos += direction7 * stepsize / direction7.manhattanLength();
                    } else{
                        nodes[curr_step.idx1].pos = nodes[curr_step.idx1].targetPos;
                    }
                    if (direction8.manhattanLength() > stepsize){
                        nodes[curr_step.idx2].pos += direction8 * stepsize / direction8.manhattanLength();
                    } else{
                        nodes[curr_step.idx2].pos = nodes[curr_step.idx2].targetPos;
                    }
                    if (nodes[curr_step.idx1].pos == nodes[curr_step.idx1].targetPos && nodes[curr_step.idx2].pos == nodes[curr_step.idx2].targetPos) {
                        nodes[i].swaplast_begin = false;
                        std::swap(nodes[curr_step.idx1],nodes[curr_step.idx2]);
                    }
                }
                else {
                    if (!steps.isEmpty()) {
                        curr_step = steps.dequeue();
                        QPointF tmp = nodes[curr_step.idx1].targetPos;
                        nodes[curr_step.idx1].targetPos = nodes[curr_step.idx2].targetPos;
                        nodes[curr_step.idx2].targetPos = tmp;
                        nodes[i].swaplast_begin = true;
                    }
                    else {
                        nodes[i].swaplast = false;
                        nodes[2*n-i-1].pos = QPointF((-i+n)*50, 675);
                        currentPhase = Entering;
                    }
                }
            }
            update();
        }
    }
}


void heapsort::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制标题
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(QRect(0, 0, width(), 50), Qt::AlignCenter, "堆排序可视化");

    /*
    // 绘制状态
    painter.setFont(QFont("Arial", 14));
    QString stateText;
    if (currentPhase == Entering) {
        stateText = "节点进入中...";
    } else if (currentPhase == Sorting) {
        stateText = "排序中...";
    } else {
        stateText = "初始化...";
    }
    painter.drawText(QRect(0, 50, width(), 30), Qt::AlignCenter, stateText);
    */
    /*
    for (int i = 0;i<nodes.size();i++)
    {
        QString num =QString::number(nodes[i].value);
        if (nodes[i].inresult)
        {
            painter.setPen(Qt::green);
        }
        painter.drawText(QRect(30*i, 750, width(), 30), Qt::AlignCenter, num);
        painter.setPen(Qt::black);

    }
    */

    // 绘制方框
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(targetRect);
    painter.drawRect(targetRect2);

    // 绘制连线
    painter.setPen(Qt::gray);
    for (int i = 0; i < nodes.size()/2; ++i) {
        if (position[i]) {
            int leftChild = 2 * i + 1;
            int rightChild = 2 * i + 2;
            if (leftChild < nodes.size()/2) {
                painter.drawLine(positionxy[i], positionxy[leftChild]);
            }
            if (rightChild < nodes.size()/2) {
                painter.drawLine(positionxy[i], positionxy[rightChild]);
            }
        }

    }

    // 绘制节点
    for (const auto& node : nodes) {
        if (node.hide) continue;

        painter.setBrush(node.color);
        painter.setPen(Qt::black);
        painter.drawEllipse(node.pos, 20, 20);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        if (node.value != -1) {
            painter.drawText(QRectF(node.pos.x() - 20, node.pos.y() - 20, 40, 40),
                             Qt::AlignCenter, QString::number(node.value));
        }

    }
}
