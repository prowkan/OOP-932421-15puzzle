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

        int field[4][4];

        void makeMove(const int);

        void getEmptyCellCoords(int&, int&);

        int movesCount;
        int elaspedTime;

        QTimer *timer;

        bool checkEndGameCondition();

        int getNextNonEmptyCell(const int);

    private slots:

        void onTimerTick();
};