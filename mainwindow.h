#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Разделитель имени файлов
#if defined(Q_OS_LINUX)
  #define SEP "\\"
#else
  #define SEP "/"
#endif

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void on_ChaoticaPathSetButton_clicked();

    void on_ChaosPathSetButton_clicked();

    void on_TempPathSetButton_clicked();

    void on_RenderType21_toggled(bool checked);

  private:
    Ui::MainWindow *ui;
    QString Chaotica, Chaos, TempDir;
    bool Render21;
};

#endif // MAINWINDOW_H
