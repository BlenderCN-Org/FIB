#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_action_Quit_triggered();

    void on_horizontalSlider_sliderMoved(int position);


    void on_EulerOrig_toggled(bool checked);

    void on_Verlet_toggled(bool checked);

    void on_EulerSemi_toggled(bool checked);

    void on_resetButton_pressed();

    void on_quitButton_pressed();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
