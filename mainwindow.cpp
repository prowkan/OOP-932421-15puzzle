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
#include <QRandomGenerator>

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
    }

    generateField();

    mainVerticalLayout->addWidget(graphicsSceneView);

    centralWidget->setLayout(mainVerticalLayout);
    setCentralWidget(centralWidget);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    movesCount = 0;
    elaspedTime = 0;

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTick);

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::restartGame);
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

        if (checkEndGameCondition())
        {
            winMessageLabel->setText(tr("Вы выиграли!"));
            timer->stop();
        }
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

bool MainWindow::checkEndGameCondition()
{
    bool isFieldSorted = true;

    int firstCellIndex = 0;
    int secondCellIndex;

    while (true)
    {
        firstCellIndex = getNextNonEmptyCell(firstCellIndex);

        if (firstCellIndex >= 15) break;

        secondCellIndex = getNextNonEmptyCell(firstCellIndex + 1);

        if (field[firstCellIndex / 4][firstCellIndex % 4] > field[secondCellIndex / 4][secondCellIndex % 4])
        {
            isFieldSorted = false;
            break;
        } 

        firstCellIndex = secondCellIndex;
    }

    return isFieldSorted;
}

int MainWindow::getNextNonEmptyCell(const int startIndex)
{
    int index = startIndex;

    while (index < 16)
    {
        if (field[index / 4][index % 4]) break;

        index++;
    }

    return index;
}

void MainWindow::restartGame()
{
    timer->stop();

    movesCount = 0;
    elaspedTime = 0;

    movesCountLabel->setText(tr("Ходов: 0"));
    elaspedTimeLabel->setText(tr("Время: 00:00"));
    winMessageLabel->setText(tr(""));

    setFocus();

    generateField();
}

void MainWindow::generateField()
{
    bool wasNumberUsed[16] = { false };

	for (int i = 0; i < 16; i++)
	{
		quint32 number = QRandomGenerator::global()->bounded(16);

		while (wasNumberUsed[number])
		{
			number = QRandomGenerator::global()->bounded(16);
		}

		wasNumberUsed[number] = true;

		field[i % 4][i / 4] = number;
	}

	int sum = 0;

	for (int i = 0; i < 16; i++)
	{
		int y1 = i / 4;
		int x1 = i % 4;

		int value1 = field[y1][x1];

		if (value1)
		{
			int k = 0;

			for (int j = i + 1; j < 16; j++)
			{
				int y2 = j / 4;
				int x2 = j % 4;

				int value2 = field[y2][x2];

				if (value2 && value2 < value1) k++;
			}

			sum += k;
		}
		else
		{
			sum += (y1 + 1);
		}
	}

	if (sum % 2)
	{
		int fieldCopy[4][4];

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                fieldCopy[y][x] = field[y][x];
            }
        }

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                field[y][x] = fieldCopy[x][3 - y];
            }
        }
	}

    for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (field[y][x])
				items[field[y][x] - 1]->setPos(150 * x, 150 * y);
		}
	}
}