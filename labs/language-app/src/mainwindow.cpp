#include "mainwindow.h"

#include "addgrammardialog.h"
#include "addtranslationdialog.h"

#include <QActionGroup>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    currentDifficulty(tr("Лёгкий")) {

    setWindowTitle(tr("Приложение для изучения языка"));
    resize(900, 700);
    setMinimumSize(640, 480);

    initUi();
    initMenu();
    //loadData();

    showWelcome();
    statusBar()->showMessage(tr("Готово к работе. Выберите упражнение и уровень сложности."), 5000);
}

MainWindow::~MainWindow() {
}

void MainWindow::initUi() {
    welcomePage = new QWidget(this);
    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomePage);
    welcomeLayout->setContentsMargins(30,30,30,30);
    QLabel *titleLabel = new QLabel(tr("Добро пожаловать!"), welcomePage);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28pt; font-weight: bold; color: #2c3e50; margin-bottom: 20px;");

    QLabel *textLabel = new QLabel(
        tr("Это приложение поможет Вам в изучении языка.\n\n"
           "Используйте меню |Упражнения| для выбора заданий.\n"
           "Меню |Редактировать| позволит вам добавить свои вопросы и предложения.\n"
           "В меню |Сложность| вы можете выбрать подходящий уровень."),
        welcomePage
        );
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet("font-size: 14pt; color: #34495e; line-height: 150%;");

    welcomeLayout->addStretch(1);
    welcomeLayout->addWidget(titleLabel);
    welcomeLayout->addWidget(textLabel);
    welcomeLayout->addStretch(2);
    welcomePage->setLayout(welcomeLayout);
    welcomePage->setStyleSheet("background-color: #ecf0f1;");

    grammarPage = new Grammar(this);
    translationPage = new Translation(this);

    stack = new QStackedWidget(this);
    stack->addWidget(welcomePage);
    stack->addWidget(grammarPage);
    stack->addWidget(translationPage);

    setCentralWidget(stack);

    connect(grammarPage, &Grammar::exerciseComplete, this, &MainWindow::onExerciseDone);
    connect(translationPage, &Translation::exerciseComplete, this, &MainWindow::onExerciseDone);
}

void MainWindow::initMenu() {
    exerciseMenu = menuBar()->addMenu(tr("&Упражнения"));

    welcomeAct = new QAction(tr("Главная страница"), this);
    connect(welcomeAct, &QAction::triggered, this, &MainWindow::showWelcome);
    exerciseMenu->addAction(welcomeAct);
    exerciseMenu->addSeparator();

    grammarAct = new QAction(tr("Упражнения для &грамматики"), this);
    connect(grammarAct, &QAction::triggered, this, &MainWindow::showGrammar);
    exerciseMenu->addAction(grammarAct);

    translationAct = new QAction(tr("Упражнения для &перевода"), this);
    connect(translationAct, &QAction::triggered, this, &MainWindow::showTranslation);
    exerciseMenu->addAction(translationAct);

    exerciseMenu->addSeparator();
    exitAct = new QAction(tr("&Выход"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, qApp, &QApplication::quit);
    exerciseMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Редактировать"));
    addGrammarAct = new QAction(tr("Добавить &вопрос (грамматика)..."), this);
    connect(addGrammarAct, &QAction::triggered, this, &MainWindow::addGrammarQ);
    editMenu->addAction(addGrammarAct);

    addTranslationAct = new QAction(tr("Добавить &предложение (перевод)..."), this);
    connect(addTranslationAct, &QAction::triggered, this, &MainWindow::addTranslationS);
    editMenu->addAction(addTranslationAct);

    difficultyMenu = menuBar()->addMenu(tr("&Сложность"));
    difficultyGroup = new QActionGroup(this);
    difficultyGroup->setExclusive(true);

    easyAct = difficultyGroup->addAction(tr("&Лёгкий"));
    easyAct->setCheckable(true);
    mediumAct = difficultyGroup->addAction(tr("&Средний"));
    mediumAct->setCheckable(true);
    hardAct = difficultyGroup->addAction(tr("&Сложный"));
    hardAct->setCheckable(true);

    difficultyMenu->addAction(easyAct);
    difficultyMenu->addAction(mediumAct);
    difficultyMenu->addAction(hardAct);

    easyAct->setChecked(true);

    connect(difficultyGroup, &QActionGroup::triggered, this, &MainWindow::onDifficultyAction);

    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpAct = new QAction("&О программе...", this);
    connect(helpAct, &QAction::triggered, this, &MainWindow::showHelp);
    helpMenu->addAction(helpAct);
}

// void MainWindow::loadData() {
//     grammarData.append({tr("The cat ___ on the mat."), {tr("sit"), tr("sits"), tr("sitting"), tr("sat")}, 1});
//     grammarData.append({tr("She ___ to the store yesterday."), {tr("go"), tr("goes"), tr("went"), tr("gone")}, 2});
//     grammarData.append({tr("___ you like some tea?"), {tr("Do"), tr("Does"), tr("Would"), tr("Will")}, 2});
//     grammarData.append({tr("My brother is ___ than me."), {tr("tall"), tr("taller"), tr("tallest"), tr("more tall")}, 1});
//     grammarData.append({tr("I haven't seen him ___ last year."), {tr("for"), tr("since"), tr("ago"), tr("from")}, 1});
//
//     translationData.append({tr("Hello, world!"), tr("Привет, мир!"), 1});
//     translationData.append({tr("This is an interesting book."), tr("Это интересная книга."), 2});
//     translationData.append({tr("The weather is wonderful today."), tr("Сегодня прекрасная погода."), 1});
//     translationData.append({tr("Practice makes perfect."), tr("Повторение - мать учения."), 3});
//     translationData.append({tr("Could you please help me?"), tr("Не могли бы вы мне помочь, пожалуйста?"), 2});
// }

void MainWindow::showWelcome() {
    stack->setCurrentWidget(welcomePage);
    updateTitle();
}

void MainWindow::showGrammar() {
    grammarPage->updateLevel(currentDifficulty);
    grammarPage->startExercise(currentDifficulty, grammarData);
    stack->setCurrentWidget(grammarPage);
    updateTitle();
}

void MainWindow::showTranslation() {
    translationPage->updateLevel(currentDifficulty);
    translationPage->startExercise(currentDifficulty, translationData);
    stack->setCurrentWidget(translationPage);
    updateTitle();
}

void MainWindow::onDifficultyAction(QAction *action) {
    if (action == easyAct) {
        currentDifficulty = tr("Лёгкий");
        currentTimeGrammar = 60;
        currentTimeTranslation = 90;
    } else if (action == mediumAct) {
        currentDifficulty = tr("Средний");
        currentTimeGrammar = 30;
        currentTimeTranslation = 60;
    } else if (action == hardAct) {
        currentDifficulty = tr("Сложный");
        currentTimeGrammar = 10;
        currentTimeTranslation = 30;
    }

    statusBar()->showMessage(tr("Уровень сложности изменен на: %1").arg(currentDifficulty), 3000);
    updateTitle();

    QWidget *currentVisible = stack->currentWidget();
    if (currentVisible == grammarPage) {
        grammarPage->updateLevel(currentDifficulty);
        grammarPage->startExercise(currentDifficulty, grammarData, currentTimeGrammar);
    } else if (currentVisible == translationPage) {
        translationPage->updateLevel(currentDifficulty);
        translationPage->startExercise(currentDifficulty, translationData, currentTimeTranslation);
    }

}

void MainWindow::addGrammarQ() {
    QTimer* targetTimer = startAndStopTimer();

    AddGrammarDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Question q;
        q.question = dialog.getQuestionText();
        q.options = dialog.getOptions();
        q.correctOption = dialog.getCorrectOptionIndex();
        grammarData.append(q);
        statusBar()->showMessage(tr("Новый вопрос по грамматике добавлен."), 3000);
        if (stack->currentWidget() == grammarPage) {
            if (QMessageBox::question(this, tr("Обновить упражнение?"),
                                      tr("Вопрос добавлен. Хотите перезапустить упражнение по грамматике с новым набором вопросов?"),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                grammarPage->startExercise(currentDifficulty, grammarData);
            }
        }
    }
    targetTimer->start();
}

void MainWindow::addTranslationS() {
    QTimer* targetTimer = startAndStopTimer();

    AddTranslationDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {


        Sentence s;
        s.original = dialog.getOriginalSentence();
        s.translation = dialog.getTranslatedSentence();
        s.level = dialog.getLevel();
        translationData.append(s);
        statusBar()->showMessage(tr("Новое предложение для перевода добавлено."), 3000);
        if (stack->currentWidget() == translationPage) {
            if (QMessageBox::question(this, tr("Обновить упражнение?"),
                                      tr("Предложение добавлено. Хотите перезапустить упражнение по переводу с новым набором предложений?"),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                translationPage->startExercise(currentDifficulty, translationData);
            }
        }
    }
    targetTimer->start();
}

void MainWindow::updateTitle() {
    QString title = tr("Приложение для изучения языка");
    QWidget *current = stack->currentWidget();
    if (current == grammarPage) {
        title = tr("Грамматика - %1").arg(currentDifficulty);
    } else if (current == translationPage) {
        title = tr("Перевод - %1").arg(currentDifficulty);
    }
    setWindowTitle(title);
}

QTimer* MainWindow::startAndStopTimer() {
    QWidget *currentActiveWidget = stack->currentWidget();
    QTimer *targetTimer;
    if (currentActiveWidget == grammarPage) {
        if (grammarPage && grammarPage->getTimer()) {
            targetTimer = grammarPage->getTimer();
            targetTimer->stop();
        }
    } else if (currentActiveWidget == translationPage) {
        if (translationPage && translationPage->getTimer()) {
            targetTimer = translationPage->getTimer();
            targetTimer->stop();
        }
    }
    return targetTimer;
}

void MainWindow::showHelp() {
    QTimer* targetTimer = startAndStopTimer();

    QMessageBox::about(this, tr("О программе"),
                       tr("<h2>Приложение для изучения языка v1.0</h2>"
                          "<p>Это приложение создано для помощи в изучении грамматики и перевода.</p>"
                          "<p>Используйте меню для навигации и добавления новых заданий.</p>"
                          "<p>Удачи в обучении!</p>"
                          "<p>Автор: Дикан Данила</p>"));

    if(targetTimer) {
        targetTimer->start();
    }
}

void MainWindow::onExerciseDone(int score) {
    QString type;
    if (sender() == grammarPage) {
        type = tr("грамматике");
    } else if (sender() == translationPage) {
        type = tr("переводу");
    } else {
        return;
    }

    QMessageBox::information(this, tr("Упражнение завершено"),
                             tr("Поздравляем! Упражнение по %1 завершено.\nВаш итоговый счёт: %2")
                                 .arg(type).arg(score));

    showWelcome();
    statusBar()->showMessage(tr("Упражнение завершено. Ваш счёт: %1").arg(score), 5000);
}
