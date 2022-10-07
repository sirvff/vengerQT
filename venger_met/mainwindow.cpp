#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chungarian.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setItemDelegate(new Delegate);
    ui->tableWidget->horizontalHeader()->resizeContentsPrecision();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->resizeContentsPrecision();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();

    ui->tableWidget_2->setItemDelegate(new Delegate);
    ui->tableWidget_2->horizontalHeader()->resizeContentsPrecision();
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->verticalHeader()->resizeContentsPrecision();
    ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->resizeRowsToContents();
    ui->tableWidget_2->resizeColumnsToContents();

    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget_2->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    font.setPointSize(16);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);


    QSize Size( 0, 0);


      for( int i = 0; i < ui->tableWidget->columnCount(); i++)
      {
        Size.setWidth( Size.width() + ui->tableWidget->columnWidth ( i));
      }
      Size.setWidth( Size.width() + ui->tableWidget->verticalHeader()->width());
      for( int i = 0; i < ui->tableWidget->rowCount(); i++)
      {
        Size.setHeight( Size.height() + ui->tableWidget->rowHeight( i));
      }
      Size.setHeight( Size.height() + ui->tableWidget->horizontalHeader()->height());

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::find_min(int a, int b)
{
    if (a > b) return b;
    if (a == b) { return a; }
    else return a;
}

void MainWindow::on_pushButton_clicked()
{
    scene->clear();
    if(((ui->spinBox->value()==0)||(ui->spinBox_2->value()==0))&&var!=3)
    {
        QMessageBox::critical(this,"Ошибка","Некорректный ввод данных");
    }
    else
    {
        if ((ui->spinBox->value()==0)&&(var==3))
        {
            QMessageBox::critical(this,"Ошибка","Некорректный ввод данных");
        }
        else
        {
            ui->tableWidget->clear();
            ui->tableWidget_2->clear();
            ui->lineEdit->clear();



            n=ui->spinBox->value();     //строки
            if (var==3)
                m=n;
            else
                m=ui->spinBox_2->value();   //столбцы
            ui->tableWidget->setRowCount(n+1);
            ui->tableWidget->setColumnCount(m+1);
            ui->tableWidget_2->setRowCount(n);
            ui->tableWidget_2->setColumnCount(m);


            for (int i = 0;i<ui->tableWidget->rowCount();i++)
            {
                for (int j = 0;j<ui->tableWidget->columnCount();j++)
                {
                    ui->tableWidget->setItem(i,j,new QTableWidgetItem());
                    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget->item(i,j)->setFont(font);
                }
            }



            QStringList vertHeader,horizHeader;

            for (int i=0;i<n;i++)
            {
                vertHeader.push_back("A "+QString::number(i+1));
            }
            vertHeader.push_back("bj");
            for (int i =0;i<m;i++)
            {
                horizHeader.push_back("B "+QString::number(i+1));
            }
            horizHeader.push_back("ai");
            ui->tableWidget->setHorizontalHeaderLabels(horizHeader);
            ui->tableWidget->setVerticalHeaderLabels(vertHeader);
            if (var==3) ui->tableWidget->removeColumn(ui->tableWidget->columnCount()-1);
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    scene->clear();
    ui->tableWidget_2->clear();
    ui->pushButton->setText("Очистить и ввести новые");
    int* a = new int[n];
    int* b = new int[m];
    int sum_a = 0;
    int sum_b = 0;

    if (var!=3)
    {
        for (int i = 0;i<n;i++)
        {
            a[i] = (ui->tableWidget->item(i,m)->text().toInt());
            sum_a+=a[i];
        }
    }
    for (int j = 0;j<m;j++)
    {
        b[j] = (ui->tableWidget->item(n,j)->text().toInt());
        sum_b+=b[j];
    }

    ui->tableWidget->setItem(n,m,new QTableWidgetItem(QString::number(sum_b)+" = "+QString::number(sum_a)));

    int i = 0;
    int j = 0;
    //Рисование
    if (var!=3)
    {
        for (int i = 0;i<n;i++)
        {
            scene->addEllipse(i*100,0,40,40);
            QGraphicsTextItem* text = NULL;
            text = scene->addText("a"+QString::number(i+1));
            text->setX(i*100+10);
            text->setY(0+5);
            QGraphicsTextItem* text_2 = NULL;
            text_2 = scene->addText(QString::number(a[i]));
            text_2->setX(i*100+10);
            text_2->setY(0+15);
        }

        for (int j = 0;j<m;j++)
        {
            scene->addEllipse(j*100,300,40,40);
            QGraphicsTextItem* text = NULL;
            text = scene->addText("b"+QString::number(j+1));
            text->setX(j*100+10);
            text->setY(300+5);
            QGraphicsTextItem* text_2 = NULL;
            text_2 = scene->addText(QString::number(b[j]));
            text_2->setX(j*100+10);
            text_2->setY(300+15);
        }
    }
    else
    {
        for (int i = 0;i<n;i++)
        {
            scene->addEllipse(i*100,0,40,40);
            QGraphicsTextItem* text = NULL;
            text = scene->addText("a"+QString::number(i+1));
            text->setX(i*100+10);
            text->setY(0+5);
        }

        for (int j = 0;j<m;j++)
        {
            scene->addEllipse(j*100,300,40,40);
            QGraphicsTextItem* text = NULL;
            text = scene->addText("b"+QString::number(j+1));
            text->setX(j*100+10);
            text->setY(300+5);
            QGraphicsTextItem* text_2 = NULL;
            text_2 = scene->addText(QString::number(b[j], 'g', 2));
            text_2->setX(j*100+10);
            text_2->setY(300+15);
        }
    }

    std::vector<std::vector<double> > data(n);
    std::vector<std::vector<int> > res(n);
    for (int i = 0; i < n; i++)
    {
        data[i].resize(m);
        res[i].resize(m);
        for (int j = 0; j < m; j++)
        {
            data[i][j] = ui->tableWidget->item(i, j)->text().toDouble();
        }
    }

    std::vector<int> result(n);
    CHungarian solver;
    std::vector<int> availability(n), demand(m);
    if (var != 3)
    {
        for (int i = 0; i < n; i++)
        {
            availability[i] = a[i];
        }
        for (int j = 0; j < m; j++)
        {
            demand[j] = b[j];
        }
    }
    // Для задачи целераспределения столбцы матрицы (вероятности) нужно умножить на соответствующие важности b[j]
    // Потом задачу максимизации нужно превратить в задачу минимизации заменой вероятностей data[i][j] на 1 - data[i][j]
    // Так как у нас ПУ - одна цель, и не более одного пуска по одной цели, то все запасы и запросы равны единице
    if (var == 3)
    {
        for (int i = 0; i < n; i++)
        {
            availability[i] = 1;
            for (int j = 0; j < m; j++)
            {
                data[i][j] = (1.0 - data[i][j]) * b[j];
            }
        }
        for (int j = 0; j < m; j++)
        {
            demand[j] = 1;
        }
    }

    double cost = solver.calculate(data, availability, demand, result, res);

    if (var != 3)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(res[i][j])));
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(res[i][j], 'g', 2)));
            }
        }
    }

    double tMax = 0;
    switch (var)
    {
    case 1:
        ui->lineEdit->setText(QString::number(cost));

        //Различные ввыводы для проверки заполнения массивов
        //Рисование линий
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                if(res[i][j] != 0)
                {
                    scene->addLine(i*100+20,0+40,j*100+20,300+0);
                    QGraphicsTextItem* text = NULL;
                    text = scene->addText(QString::number(res[i][j]));
                    text->setX(((i*100+20)+(j*100+20))/2+5);
                    text->setY(((0+40)+(300+0))/2+i*10);
                }
            }
        }
        break;
    case 2:
        // Здесь нужно пересчитать целевую функцию
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if ((res[i][j] > 0) && (data[i][j] > tMax))
                {
                    tMax = data[i][j];
                }
                ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(QString::number(res[i][j])));
                ui->tableWidget_2->item(i,j)->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_2->item(i,j)->setFont(font);
            }
        }

        ui->lineEdit->setText(QString::number(tMax));

        //Различные ввыводы
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                if(res[i][j] != 0)
                {
                    scene->addLine(i*100+20,0+40,j*100+20,300+0);
                    QGraphicsTextItem* text = NULL;
                    text = scene->addText(QString::number(res[i][j]));
                    text->setX(((i*100+20)+(j*100+20))/2+5);
                    text->setY(((0+40)+(300+0))/2+i*10);
                }
            }
        }
        break;
    case 3:
        {
            // Пересчитать целевую функцию
            cost = 0;
            for (int r = 0; r < n; r++)
            {
                const int c = result[r];
                if (c >= 0)
                {
                    cost += (double(b[c]) - data[r][c]);
                }
            }

            ui->lineEdit->setText(QString::number(cost, 'g', 2));

            for (i = 0; i < n; i++)
            {
                for (j = 0; j < m; j++)
                {
                    if(res[i][j] != 0)
                    {
                        scene->addLine(i*100+20,0+40,j*100+20,300+0);
                        QGraphicsTextItem* text = NULL;
                        text = scene->addText(QString::number(double(b[j]) - data[i][j], 'g', 2));
                        text->setX(((i*100+20)+(j*100+20))/2+5);
                        text->setY(((0+40)+(300+0))/2+i*10);
                    }
                }
            }
            break;
        }
    default:
        break;
    }
}

void MainWindow::on_radioButton_clicked()
{
    var = 1;
    ui->label_2->show();
    ui->spinBox_2->show();
    ui->label->setText("Строки");
    ui->label_3->setText("Суммарная стоимость доставки противоракет");
    ui->groupBox_3->setTitle("Кол-во противоракет, перевезенных из  i – го склада на j – ую СП");
    ui->textEdit->setPlainText("В период подготовки боевых действий в n СП необходимо поставить противоракеты согласно их заявкам bј (j=1,n). ПР находится на m складах, запас их на каждом складе ai (i=1,m). Известны стоимости доставки одной противоракеты с i-го склада на j-ую СП, объединенные в матрицу ||Cij||. Требуется разработать план снабжения СП ПРО, обеспечивающий min общую стоимость перевозок F(X). X=||xij||, где xij – количество ПР, перевезенных из  i – го склада на j – ую СП.");
}

void MainWindow::on_radioButton_3_clicked()
{
    var = 2;
    ui->label_2->show();
    ui->spinBox_2->show();
    ui->label->setText("Строки");
    ui->label_3->setText("Суммарное стоимость доставки противоракет при минимальном времени");
    ui->groupBox_3->setTitle("Кол-во противоракет, перевезенных из  i – го склада на j – ую СП");
    ui->textEdit->setPlainText("После отражения удара СВН противника необходимо пополнить боезапас n однотипных СП в количестве bј противоракет (j=1,n). Требуемое количество противоракет можно получить из тех. базы и потерявших боеспособность СП, количество противоракет в них - ai (i=1,m). Известны времена доставки из i-го пункта отправки противоракет в j-ую СП - матрица ||tij||.Требуется разработать план маневра противоракет для восстановления боеготовности соединения ПРО в min короткий срок  T(X). X = ||xij||, где xij - количество противоракет, перевезенных с i-го тех. базы на j-ую СП.");
}

void MainWindow::on_radioButton_2_clicked()
{
    var = 3;
    ui->label_2->hide();
    ui->spinBox_2->hide();
    ui->label->setText("Размер кв. матрицы");
    ui->label_3->setText("Максимально нанесенный урон");
    ui->groupBox_3->setTitle("Параметр целераспределения: назначение i-го средства на j-ю цель");
    ui->textEdit->setPlainText("В зону действий m огневых средств ПРО входит n целей важностью Cj (j=1,n). Вероятности уничтожения целей, определяемые их типом и тактикой действий, а также ТТХ огневых  средств заданы матрицей ||pij||, i-номер огневого средства, j-номер	цели, (i=1,m). Требуется найти вариант целераспределения, при котором группировка ПРО нанесет противнику max ущерб M(X). X = ||xij||, где xij - параметр целераспределения: назначение i-го средства на j-ю цель.");
}

