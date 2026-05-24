#include "MainWindow.h"
#include "DieIconButton.h"
#include "../DiceRoller.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(QString::fromUtf8("D&D — Кубики"));
    resize(920, 420);
    setMinimumSize(700, 360);

    auto* central = new QWidget(this);
    auto* root = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // --- Header (макет: белый «Кубики» | серый «Броски») ---
    auto* header = new QWidget;
    header->setObjectName("headerBar");
    auto* headerLay = new QHBoxLayout(header);
    headerLay->setContentsMargins(0, 0, 0, 0);
    headerLay->setSpacing(0);

    auto* hdrLeft = new QLabel(QString::fromUtf8("Кубики"));
    hdrLeft->setObjectName("headerLeft");
    hdrLeft->setAlignment(Qt::AlignCenter);
    auto* hdrRight = new QLabel(QString::fromUtf8("Броски"));
    hdrRight->setObjectName("headerRight");
    hdrRight->setAlignment(Qt::AlignCenter);
    headerLay->addWidget(hdrLeft, 1);
    headerLay->addWidget(hdrRight, 1);
    root->addWidget(header);

    // --- Body ---
    auto* body = new QHBoxLayout;
    body->setContentsMargins(0, 0, 0, 0);
    body->setSpacing(0);

    // Left panel
    auto* left = new QWidget;
    left->setObjectName("panelLeft");
    auto* leftLay = new QVBoxLayout(left);
    leftLay->setContentsMargins(16, 16, 16, 16);
    leftLay->setSpacing(12);

    auto* countRow = new QHBoxLayout;
    auto* countLbl = new QLabel(QString::fromUtf8("Количество:"));
    countLbl->setObjectName("countLabel");
    countRow->addWidget(countLbl);
    m_count = new QSpinBox;
    m_count->setRange(1, 100);
    m_count->setValue(1);
    m_count->setFixedWidth(72);
    countRow->addWidget(m_count);
    countRow->addStretch();
    leftLay->addLayout(countRow);

    auto* diceRow = new QHBoxLayout;
    diceRow->setSpacing(12);
    const int dice[] = {4, 6, 8, 10, 12, 20};
    for (int s : dice) {
        auto* btn = new DieIconButton(s, left);
        connect(btn, &QPushButton::clicked, this, [this, s]() { onDieClicked(s); });
        diceRow->addWidget(btn);
    }
    diceRow->addStretch();
    leftLay->addLayout(diceRow);

    auto* d20Box = new QGroupBox(QString::fromUtf8("d20 — режим"));
    auto* d20Lay = new QVBoxLayout(d20Box);
    m_d20Group = new QButtonGroup(this);
    auto* rbNormal = new QRadioButton(QString::fromUtf8("Обычный"));
    auto* rbAdv = new QRadioButton(QString::fromUtf8("Преимущество"));
    auto* rbDis = new QRadioButton(QString::fromUtf8("Помеха"));
    rbNormal->setChecked(true);
    m_d20Group->addButton(rbNormal, 0);
    m_d20Group->addButton(rbAdv, 1);
    m_d20Group->addButton(rbDis, 2);
    d20Lay->addWidget(rbNormal);
    d20Lay->addWidget(rbAdv);
    d20Lay->addWidget(rbDis);
    leftLay->addWidget(d20Box);

    auto* statsBtn = new QPushButton(QString::fromUtf8("Прокид характеристик (6×4d6)"));
    statsBtn->setObjectName("statsBtn");
    connect(statsBtn, &QPushButton::clicked, this, &MainWindow::onStatsClicked);
    leftLay->addWidget(statsBtn);
    leftLay->addStretch();

    // Right panel
    auto* right = new QWidget;
    right->setObjectName("panelRight");
    auto* rightLay = new QVBoxLayout(right);
    rightLay->setContentsMargins(8, 24, 8, 8);

    m_result = new QLabel(QString::fromUtf8("—"));
    m_result->setObjectName("resultLabel");
    m_result->setAlignment(Qt::AlignCenter);
    m_details = new QLabel(QString::fromUtf8("Нажмите кубик слева"));
    m_details->setObjectName("detailsLabel");
    m_details->setAlignment(Qt::AlignCenter);
    m_details->setWordWrap(true);
    m_details->setMinimumWidth(280);
    m_details->setStyleSheet(
        "color: #000000; font-size: 18px; font-weight: bold;"
        "background: #ffffff; border: 2px solid #333; border-radius: 6px; padding: 12px;");
    rightLay->addStretch(1);
    rightLay->addWidget(m_result, 0, Qt::AlignCenter);
    rightLay->addWidget(m_details, 0, Qt::AlignCenter);
    rightLay->addStretch(2);

    body->addWidget(left, 1);
    body->addWidget(right, 1);
    root->addLayout(body, 1);

    setCentralWidget(central);
}

D20Mode MainWindow::d20Mode() const {
    switch (m_d20Group->checkedId()) {
    case 1:
        return D20Mode::Advantage;
    case 2:
        return D20Mode::Disadvantage;
    default:
        return D20Mode::Normal;
    }
}

QString MainWindow::formatDiceList(const RollResult& r) const {
    QStringList parts;
    for (int v : r.dice)
        parts << QString::number(v);
    return parts.join(QString::fromUtf8(", "));
}

void MainWindow::onDieClicked(int sides) { performRoll(sides); }

void MainWindow::performRoll(int sides) {
    const int count = m_count->value();
    RollResult r;
    QString details;

    if (sides == 20) {
        const D20Mode mode = d20Mode();
        if (count == 1) {
            r = DiceRoller::rollD20(mode);
            if (r.dice.size() == 2) {
                const QString modeStr =
                    mode == D20Mode::Advantage ? QString::fromUtf8("преимущество")
                                             : QString::fromUtf8("помеха");
                details = QString("d20 (%1): %2 → %3")
                              .arg(modeStr)
                              .arg(formatDiceList(r))
                              .arg(r.total);
            } else {
                details = QString("1d20: %1").arg(r.total);
            }
        } else if (mode == D20Mode::Normal) {
            r = DiceRoller::rollMany(20, count);
            details = QString("%1d20: [%2]").arg(count).arg(formatDiceList(r));
        } else {
            r.total = 0;
            for (int i = 0; i < count; ++i) {
                RollResult one = DiceRoller::rollD20(mode);
                r.total += one.total;
                for (int v : one.dice)
                    r.dice.push_back(v);
            }
            const QString modeStr =
                mode == D20Mode::Advantage ? QString::fromUtf8("преимущество")
                                           : QString::fromUtf8("помеха");
            details = QString("%1× d20 (%2): сумма %3").arg(count).arg(modeStr).arg(r.total);
        }
    } else {
        r = DiceRoller::rollMany(sides, count);
        if (count == 1)
            details = QString("1d%1: %2").arg(sides).arg(r.total);
        else
            details = QString("%1d%2: [%3]").arg(count).arg(sides).arg(formatDiceList(r));
    }

    m_result->setText(QString::number(r.total));
    m_details->setText(details);
}

void MainWindow::onStatsClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromUtf8("Прокид характеристик"));
    dlg.resize(520, 280);

    auto* lay = new QVBoxLayout(&dlg);
    auto* table = new QTableWidget(6, 3, &dlg);
    table->setHorizontalHeaderLabels(
        {QString::fromUtf8("Характеристика"), QString::fromUtf8("Значение"),
         QString::fromUtf8("Бросок (4d6, без низшего)")});
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    const QString names[] = {QString::fromUtf8("Сила"),     QString::fromUtf8("Ловкость"),
                             QString::fromUtf8("Телосложение"), QString::fromUtf8("Интеллект"),
                             QString::fromUtf8("Мудрость"),  QString::fromUtf8("Харизма")};

    for (int i = 0; i < 6; ++i) {
        RollResult r = DiceRoller::rollAbilityScore();
        table->setItem(i, 0, new QTableWidgetItem(names[i]));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(r.total)));
        table->setItem(
            i, 2,
            new QTableWidgetItem(QString("4d6: %1, убран %2")
                                     .arg(formatDiceList(r))
                                     .arg(r.dropped)));
    }
    table->resizeColumnsToContents();
    lay->addWidget(table);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    lay->addWidget(buttons);
    dlg.exec();
}
