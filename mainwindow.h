#pragma once

#include <QMainWindow>

class QLabel;
class QGraphicsPixmapItem;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT;

    public:

        MainWindow(QWidget* = nullptr);
        ~MainWindow();

    protected:

        virtual void keyPressEvent(QKeyEvent*) override;

    private:

        QLabel *movesCountLabel;
        QLabel *elaspedTimeLabel;
        QLabel *winMessageLabel;

        QGraphicsPixmapItem *items[15];

        static const int NUM_ROWS = 4;
        static const int NUM_COLUMS = 4;

        int field[NUM_ROWS][NUM_COLUMS];

        void makeMove(const int);

        void getEmptyCellCoords(int&, int&);

        int movesCount;
        int elaspedTime;

        QTimer *timer;

        bool checkEndGameCondition();

        int getNextNonEmptyCell(const int);

        void generateField();

        int cellWidth, cellHeight;

    private slots:

        void onTimerTick();

        void restartGame();
};