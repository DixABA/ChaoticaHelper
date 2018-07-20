#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Разделитель имени файлов
#if defined(Q_OS_LINUX)
  #define SEP "\\"
#else
  #define SEP "/"
#endif

#include <QMainWindow>

#include <QProcess>

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

    void ProcessFinished(int code);
    void ProcessWrote();

    void on_ProcessButton_clicked();

  private:
    const QString filenames21[2] = {"f-1", "f1"};
    const QString filenames22[4] = {"f-1-1", "f-11", "f1-1", "f11"};

    Ui::MainWindow *ui;
    QString Chaotica, Chaos, TempDir;
    bool Render21;
    QVector<QProcess *> Processes;
    QProcess *Process;

    void MakeReady();
};

#endif // MAINWINDOW_H
