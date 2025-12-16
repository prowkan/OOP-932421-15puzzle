#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) 
{
    resize(1280, 720);
    setWindowTitle(tr("Игра в 15"));   
    
    QWidget *centralWidget = new QWidget();

    QVBoxLayout *mainVerticalLayout = new QVBoxLayout();

    QHBoxLayout *topPanelLayout = new QHBoxLayout();

    QFont font("Arial", 20, QFont::Bold);

    movesCountLabel = new QLabel(tr("Ходов: 0"));
    movesCountLabel->setFont(font);
    elaspedTimeLabel = new QLabel(tr("Время: 00:00"));
    elaspedTimeLabel->setFont(font);
    winMessageLabel = new QLabel(tr(""));
    winMessageLabel->setFont(font);
    winMessageLabel->setStyleSheet("QLabel { color : green }");
    QPushButton *newGameButton = new QPushButton(tr("Начать заново"));
    newGameButton->setFont(font);

    topPanelLayout->addWidget(movesCountLabel);
    topPanelLayout->addWidget(elaspedTimeLabel);
    topPanelLayout->addWidget(winMessageLabel);
    topPanelLayout->addWidget(newGameButton);

    mainVerticalLayout->addLayout(topPanelLayout);

    QGraphicsScene *graphicsScene = new QGraphicsScene();
    QGraphicsView *graphicsSceneView = new QGraphicsView(graphicsScene);

    for (int i = 0; i < 15; i++)
    {
        QImage image(QString::number(i + 1) + ".png");

        QPixmap pixmap = QPixmap::fromImage(image);

        items[i] = graphicsScene->addPixmap(pixmap);
        items[i]->setPos((i % 4) * 150, (i / 4) * 150);

        field[i / 4][i % 4] = i + 1;
    }

    field[3][3] = 0;

    mainVerticalLayout->addWidget(graphicsSceneView);

    centralWidget->setLayout(mainVerticalLayout);
    setCentralWidget(centralWidget);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    movesCount = 0;
    elaspedTime = 0;

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTick);
};

MainWindow::~MainWindow()
{
    delete timer;
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent)
{
    switch (keyEvent->key())
    {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
            makeMove(keyEvent->key());
            break;
        default:
            QMainWindow::keyPressEvent(keyEvent);        
    }
}

void MainWindow::makeMove(const int key)
{
    int emptyCellRow, emptyCellCol;

    getEmptyCellCoords(emptyCellRow, emptyCellCol);

    bool canMakeMove = false;

    if      (key == Qt::Key_Up)    canMakeMove = emptyCellRow != 3;
    else if (key == Qt::Key_Down)  canMakeMove = emptyCellRow != 0;
    else if (key == Qt::Key_Left)  canMakeMove = emptyCellCol != 3;
    else if (key == Qt::Key_Right) canMakeMove = emptyCellCol != 0;

    if (canMakeMove)
    {
        int cellRow, cellCol;

        if      (key == Qt::Key_Up)    { cellCol = emptyCellCol; cellRow = emptyCellRow + 1; }
        else if (key == Qt::Key_Down)  { cellCol = emptyCellCol; cellRow = emptyCellRow - 1; }
        else if (key == Qt::Key_Left)  { cellCol = emptyCellCol + 1; cellRow = emptyCellRow; }
        else if (key == Qt::Key_Right) { cellCol = emptyCellCol - 1; cellRow = emptyCellRow; }

        items[field[cellRow][cellCol] - 1]->setPos(emptyCellCol * 150, emptyCellRow * 150);

        std::swap(field[cellRow][cellCol], field[emptyCellRow][emptyCellCol]);
        
        if (movesCount == 0) timer->start(1000);

        movesCount++;

        movesCountLabel->setText(tr("Ходов: ") + QString::number(movesCount));
    }
}

void MainWindow::getEmptyCellCoords(int& outRow, int& outCol)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (!field[i][j])
            {
                outRow = i;
                outCol = j;
                return;
            }
        }
    }
}

void MainWindow::onTimerTick()
{
    elaspedTime++;

    int minutes = elaspedTime / 60;
    int seconds = elaspedTime % 60;

    QString minutesStr = (minutes < 10 ? "0" : "") + QString::number(minutes);
    QString secondsStr = (seconds < 10 ? "0" : "") + QString::number(seconds);

    elaspedTimeLabel->setText(tr("Время: ") + minutesStr + ":" + secondsStr);
}