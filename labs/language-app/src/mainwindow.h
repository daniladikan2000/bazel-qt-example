#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QAction>
#include <QMenu>

#include "grammar.h"
#include "translation.h"

struct Question {
    QString question;
    QStringList options;
    int correctOption;
};

struct Sentence {
    QString original;
    QString translation;
    int level;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void showGrammar();
    void showTranslation();
    void showWelcome();
    void onDifficultyAction(QAction *action);
    void addGrammarQ();
    void addTranslationS();
    void showHelp();
    void onExerciseDone(int score);

private:
    void initMenu();
    void initUi();
    //void loadData();
    void updateTitle();
    QTimer* startAndStopTimer();

    QMenu *exerciseMenu;
    QMenu *editMenu;
    QMenu *difficultyMenu;
    QMenu *helpMenu;

    QAction *welcomeAct;
    QAction *grammarAct;
    QAction *translationAct;
    QAction *exitAct;

    QAction *addGrammarAct;
    QAction *addTranslationAct;

    QActionGroup *difficultyGroup;
    QAction *easyAct;
    QAction *mediumAct;
    QAction *hardAct;

    QAction *helpAct;

    QStackedWidget *stack;
    QWidget *welcomePage;
    Grammar *grammarPage;
    Translation *translationPage;

    int currentTimeGrammar;
    int currentTimeTranslation;
    QString currentDifficulty;
    QVector<Question> grammarData;
    QVector<Sentence> translationData;
};

#endif // MAINWINDOW_H
