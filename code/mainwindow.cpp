#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <iostream>
#include <random>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setGeometry(0, 0, 1550, 842);
    mygraphicsview = new MyGraphicsView;
    mygraphicsview->setup(this, this);
    add_pushButton = ui->add_pushButton;
    add_rand_pushButton = ui->add_rand_pushButton;
    remove_pushButton = ui->remove_pushButton;
    remove_rand_pushButton = ui->remove_rand_pushButton;
    trees_comboBox = ui->trees_comboBox;
    scale_horizontalSlider = ui->scale_horizontalSlider;
    prev_scale_val = 0;
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_add_pushButton_clicked() {
    QString ss = ui->add_lineEdit->text();
    bool ok = 1;
    int x = ss.toInt(&ok);
    if (!ok) return;
    mygraphicsview->add_to_trees(x);
    ui->scale_horizontalSlider->setValue(0);
    mygraphicsview->draw();
}

void MainWindow::on_add_rand_pushButton_clicked() {
    QString ss = ui->add_rand_lineEdit->text();
    bool ok = 1;
    int x = ss.toInt(&ok);
    if (!ok) {
        std::cout << "what??\n";
        return;
    }
    for (int i = 0; i < x; ++i) {
        int n = rand() * rand() * (rand() % 2 ? -1 : 1);
        std::cout << n << "\n";
        mygraphicsview->add_to_trees(n);
    }
    ui->scale_horizontalSlider->setValue(0);
    mygraphicsview->draw();
}

void MainWindow::on_remove_pushButton_clicked() {
    QString ss = ui->add_lineEdit->text();
    bool ok = 1;
    int x = ss.toInt(&ok);
    if (!ok) {
        std::cout << "what??\n";
        return;
    }
    mygraphicsview->erase_from_trees(x);
    ui->scale_horizontalSlider->setValue(0);
    mygraphicsview->draw();
}

void MainWindow::on_remove_rand_pushButton_clicked() {
    QString ss = ui->remove_rand_lineEdit->text();
    bool ok = 1;
    int n = ss.toInt(&ok);
    if (!ok) return;
    std::vector<int> elems;
    auto it = mygraphicsview->tree_set.begin();
    for (int s = mygraphicsview->tree_set.size(); s && n; --s, ++it) {
        if (rand() % s < n) {
            elems.push_back(*it);
            --n;
        }
    }
    int elems_size = elems.size();
    for (int k = 0; k < elems_size; k++) {
        int r = k + rand() % (elems_size - k);
        swap(elems[k], elems[r]);
    }
    for (int i = 0; i < elems_size; ++i) {
        mygraphicsview->erase_from_trees(elems[i]);
    }
    ui->scale_horizontalSlider->setValue(0);
    mygraphicsview->draw();
}

void MainWindow::on_trees_comboBox_currentTextChanged(const QString &arg1) {
    ui->scale_horizontalSlider->setValue(0);
    mygraphicsview->draw();
}

void MainWindow::on_scale_horizontalSlider_valueChanged(int value) {
    mygraphicsview->scale(pow(1.1, value - prev_scale_val), pow(1.1, value - prev_scale_val));
    prev_scale_val = value;
}
