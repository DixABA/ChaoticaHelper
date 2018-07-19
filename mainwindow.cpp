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
}

void MainWindow::on_RenderType21_toggled(bool checked)
{
  Render21 = checked;
}
