#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

int RemainingProc;
bool ValidChaotica;

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
  RemainingProc--;

  if (RemainingProc == 0)
    {
      QMessageBox::about(this, "@@", "Yea");
    }
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
      ValidChaotica = true;
    }
}

void MainWindow::MakeReady()
{
  if ((Chaotica.isNull() || Chaos.isNull() || TempDir.isNull()) == false)
    {
      this->ui->ProcessButton->setEnabled(true);
    }
}

void MainWindow::ProcessXML(QString Name)
{
  QString data;
  QFile File(Name);

  if (File.open(QFile::ReadOnly))
    {
      data = File.readAll();
      File.close();
    }

  int Count = Render21 ? 2 : 4;

  for (int i = 0; i < Count; i++)
    {
      File.setFileName(TempDir + (Render21 ? filenames21[i] : filenames22[i]) + ".chaos");
      QString Result;

      if (File.open(QFile::WriteOnly))
        {
          if (Render21)
            {
              Result = data.replace(QRegExp("<int name=\"image_width\">\\d+</int>"), "<int name=\"image_width\">1920</int>"); // Ширина
              Result = Result.replace(QRegExp("<int name=\"image_height\">\\d+</int>"), "<int name=\"image_height\">630</int>"); // Высота
              Result = Result.replace("<vec2 name=\"pos\">0 0</vec2>", QString("<vec2 name=\"pos\">%1</vec2>").arg(cameras21[i])); // Положение камеры
              Result = Result.replace(QRegExp("<IFS name=\"[a-zA-Z0-9 ]+\">"), QString("<IFS name=\"%1\">").arg(filenames21[i])); // Имя
            }
          else
            {
              Result = data.replace(QRegExp("<int name=\"image_width\">\\d+</int>"), "<int name=\"image_width\">1100</int>"); // Ширина
              Result = Result.replace(QRegExp("<int name=\"image_height\">\\d+</int>"), "<int name=\"image_height\">1100</int>"); // Высота
              Result = Result.replace("<vec2 name=\"pos\">0 0</vec2>", QString("<vec2 name=\"pos\">%1</vec2>").arg(cameras22[i])); // Положение камеры
              Result = Result.replace(QRegExp("<IFS name=\"[a-zA-Z0-9 ]+\">"), QString("<IFS name=\"%1\">").arg(filenames22[i])); // Имя
            }

          Result = Result.replace(QRegExp("<int name=\"image_aa_level\">\\d+</int>"), "<int name=\"image_aa_level\">3</int>"); // Antialiasing
          File.write(Result.toUtf8());
          File.close();
        }
      else
        {
          QMessageBox::about(this, "@@", "Nuu, wrong temp dir, bro");
        }
    }
}

void MainWindow::on_ProcessButton_clicked()
{
  ValidChaotica = false;
  Process = new QProcess(this);
  connect(Process, SIGNAL(readyRead()), this, SLOT(ProcessWrote()));
  Process->start(Chaotica);
  Process->waitForFinished(5000);

  if (!ValidChaotica)
    {
      if (!Process->state() == QProcess::NotRunning)
        {
          Process->close();
          Process->waitForFinished(5000);
        }

      QMessageBox::about(this, "@@", "Naaaaaaay");
      return;
    }

  Processes.clear();
  int ProcCount = Render21 ? 2 : 4;
  RemainingProc = ProcCount;
  QVector<QStringList> Files;
  ProcessXML(Chaos);

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
