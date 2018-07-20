#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_ChaoticaPathSetButton_clicked()
{
  QString file = QFileDialog::getOpenFileName(this, "Исполняемый файл Chaotica", "", "*");
  Chaotica = (!file.isNull()) ? file : Chaotica;

  if (!Chaotica.isNull())
    {
      this->ui->ChaoticaPathLabel->setStyleSheet("");
      this->ui->ChaoticaPath->setText(Chaotica);
    }

  MakeReady();
}

void MainWindow::on_ChaosPathSetButton_clicked()
{
  QString file = QFileDialog::getOpenFileName(this, "Файл chaos", "", "Chaos (*.chaos)");
  Chaos = (!file.isNull()) ? file : Chaos;

  if (!Chaos.isNull())
    {
      this->ui->ChaosPathLabel->setStyleSheet("");
      this->ui->ChaosPath->setText(Chaos);
    }

  MakeReady();
}

void MainWindow::on_TempPathSetButton_clicked()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Временная папка", "");
  TempDir = (!dir.isNull()) ? dir + SEP : TempDir;

  if (!TempDir.isNull())
    {
      this->ui->TempPathLabel->setStyleSheet("");
      this->ui->TempPath->setText(TempDir);
    }

  MakeReady();
}

void MainWindow::on_RenderType21_toggled(bool checked)
{
  Render21 = checked;
}

void MainWindow::ProcessFinished(int code)
{
  QMessageBox::about(this, "@@", QString::number(code));
}

void MainWindow::ProcessWrote()
{
  QByteArray out;
  out = Process->readAll();
  out.length();
  QString output(out);

  if (output.indexOf("https://www.chaoticafractals.com/"))
    {
      Process->close();
      Process->deleteLater();
    }
}

void MainWindow::MakeReady()
{
  if ((Chaotica.isNull() || Chaos.isNull() || TempDir.isNull()) == false)
    {
      this->ui->ProcessButton->setEnabled(true);
    }
}

void MainWindow::on_ProcessButton_clicked()
{
  Process = new QProcess(this);
  connect(Process, SIGNAL(readyRead()), this, SLOT(ProcessWrote()));
  Process->start(Chaotica);
  Processes.clear();
  int ProcCount = Render21 ? 2 : 4;
  QVector<QStringList> Files;

  for (int i = 0; i < ProcCount; i++)
    {
      Files.append(QStringList());
      Files[i] << TempDir + (Render21 ? filenames21[i] : filenames22[i]) + ".chaos";
    }

  for (int i = 0; i < ProcCount; i++)
    {
      Processes.append(new QProcess(this));
      connect(Processes[i], SIGNAL(finished(int)), this, SLOT(ProcessFinished(int)));
      Processes[i]->start(Chaotica, Files[i]);
    }
}
