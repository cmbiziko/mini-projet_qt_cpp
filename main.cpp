#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QButtonGroup>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setFixedSize(700, 500);

    // fenêtre principale
    QWidget *centralWidget = new QWidget(&window);
    window.setCentralWidget(centralWidget);

    // layout
    QVBoxLayout *layout = new QVBoxLayout();

    // titre
    QLabel *titleLabel = new QLabel("To Do List", centralWidget);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    layout->addWidget(titleLabel);

    //
    for (int i = 1; i <= 5; i++) {
        // widget to do
        QWidget *itemWidget = new QWidget(centralWidget);
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);

        // label to do
        QLabel *itemLabel = new QLabel("Tâche n°" + QString::number(i) + " :", itemWidget);
        itemLabel->setFixedSize(70, 20);
        itemLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemLayout->addWidget(itemLabel);

        // textfield to do list
        QLineEdit *itemEdit = new QLineEdit(itemWidget);
        itemEdit->setFixedSize(300, 20);
        itemLayout->addWidget(itemEdit);

        // bouton radio
        QRadioButton *radio1 = new QRadioButton("Pas commencé", itemWidget);
        QRadioButton *radio2 = new QRadioButton("En cours", itemWidget);
        QRadioButton *radio3 = new QRadioButton("Terminé", itemWidget);

        QButtonGroup *radioGroup = new QButtonGroup(itemWidget);
        radioGroup->addButton(radio1);
        radioGroup->addButton(radio2);
        radioGroup->addButton(radio3);

        itemLayout->addWidget(radio1);
        itemLayout->addWidget(radio2);
        itemLayout->addWidget(radio3);

        // alerte terminé
        QObject::connect(radio3, &QRadioButton::toggled, [radio3, itemEdit]() {
            if (radio3->isChecked()) {
                QMessageBox::information(itemEdit, "Félicitations", "Vous avez fini de réaliser votre tâche. !");
            }
        });

        layout->addWidget(itemWidget);
    }


    // supprimer les tâches
    QPushButton *clearButton = new QPushButton("Supprimer les tâches", centralWidget);
    QObject::connect(clearButton, &QPushButton::clicked, [centralWidget]() {
        QList<QLineEdit*> edits = centralWidget->findChildren<QLineEdit*>();
        for (QLineEdit *edit : edits) {
            edit->clear();
        }
    });
    layout->addWidget(clearButton);

    //TIMER

    // bouton timer
    QPushButton *timerButton = new QPushButton("Pomodoro", centralWidget);
    layout->addWidget(timerButton);

    // fenêtre timer
    QDialog timerDialog(&window);
    timerDialog.setModal(true);
    timerDialog.setWindowTitle("Timer");

    QVBoxLayout timerLayout(&timerDialog);

    // timer
    QLabel timerLabel("00:00:00", &timerDialog);
    timerLabel.setFixedHeight(30);
    timerLabel.setAlignment(Qt::AlignCenter);
    timerLabel.setStyleSheet("font-size: 24px; font-weight: bold;");
    timerLayout.addWidget(&timerLabel);

    // minuteur
    QTimer timer;
    timer.setInterval(1000);
    int elapsedSeconds = 0;
    QObject::connect(&timer, &QTimer::timeout, [&elapsedSeconds, &timerLabel]() {
        elapsedSeconds++;
        int hours = elapsedSeconds / 3600;
        int minutes = (elapsedSeconds % 3600) / 60;
        int seconds = elapsedSeconds % 60;
        QString timeStr = QString("%1:%2:%3").arg(hours, 2, 10, QLatin1Char('0')).arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));
        timerLabel.setText(timeStr);
    });

    // boutons
    QPushButton *playButton = new QPushButton("Play", &timerDialog);
    QPushButton *pauseButton = new QPushButton("Pause", &timerDialog);
    QPushButton *stopButton = new QPushButton("Stop", &timerDialog);

    timerLayout.addWidget(playButton);
    timerLayout.addWidget(pauseButton);
    timerLayout.addWidget(stopButton);

    // lancer le timer
    QObject::connect(playButton, &QPushButton::clicked, [&timer]() {
        timer.start();
    });

    // mettre le timer en pause
    QObject::connect(pauseButton, &QPushButton::clicked, [&timer]() {
        timer.stop();
    });

    // stopper le timer
    QObject::connect(stopButton, &QPushButton::clicked, [&timer, &elapsedSeconds, &timerLabel]() {
        timer.stop();
        elapsedSeconds = 0;
        timerLabel.setText("00:00:00");
    });

    // ouvrir le timer grâce au bouton
    QObject::connect(timerButton, &QPushButton::clicked, [&timerDialog]() {
        timerDialog.show();
    });

    centralWidget->setLayout(layout);

    window.show();

    return app.exec();
}
