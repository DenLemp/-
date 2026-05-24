#pragma once
#include "../DiceRoller.h"
#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QButtonGroup>
#include <QRadioButton>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onDieClicked(int sides);
    void onStatsClicked();

private:
    void performRoll(int sides);
    D20Mode d20Mode() const;
    QString formatDiceList(const RollResult& r) const;

    QLabel* m_result = nullptr;
    QLabel* m_details = nullptr;
    QSpinBox* m_count = nullptr;
    QButtonGroup* m_d20Group = nullptr;
};
