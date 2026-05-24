#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QPalette>
#include <clocale>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    QApplication app(argc, argv);

    QPalette pal = app.palette();
    pal.setColor(QPalette::WindowText, Qt::black);
    pal.setColor(QPalette::Text, Qt::black);
    app.setPalette(pal);

    QMainWindow win;
    win.setWindowTitle(QString::fromUtf8("Ваш текст"));
    win.resize(640, 420);
    win.setStyleSheet(
        "QMainWindow { background: #e8e8e8; }"
        "QLabel { color: #000000; font-size: 14px; }"
        "#display { color: #000000; font-size: 28px; font-weight: bold;"
        "background: #ffffff; border: 2px solid #333; border-radius: 8px; padding: 16px; }"
        "QLineEdit, QTextEdit { color: #000000; background: #ffffff;"
        "border: 1px solid #888; border-radius: 4px; padding: 8px; font-size: 14px; }"
        "QPushButton { color: #000000; background: #fff; border: 2px solid #333;"
        "padding: 8px 16px; border-radius: 4px; }");

    auto* central = new QWidget(&win);
    auto* lay = new QVBoxLayout(central);
    lay->setContentsMargins(20, 20, 20, 20);
    lay->setSpacing(12);

    lay->addWidget(new QLabel(QString::fromUtf8("Введите текст:")));

    auto* input = new QLineEdit;
    input->setPlaceholderText(QString::fromUtf8("Печатайте здесь..."));
    input->setClearButtonEnabled(true);
    lay->addWidget(input);

    auto* multiline = new QTextEdit;
    multiline->setPlaceholderText(
        QString::fromUtf8("Или несколько строк здесь (необязательно)"));
    multiline->setMaximumHeight(100);
    lay->addWidget(multiline);

    lay->addWidget(new QLabel(QString::fromUtf8("На экране только ваш текст:")));

    auto* display = new QLabel(QString::fromUtf8("—"));
    display->setObjectName("display");
    display->setWordWrap(true);
    display->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    display->setMinimumHeight(160);
    lay->addWidget(display, 1);

    auto* clearBtn = new QPushButton(QString::fromUtf8("Очистить"));
    lay->addWidget(clearBtn);

    auto updateDisplay = [=]() {
        QString line = input->text().trimmed();
        QString block = multiline->toPlainText().trimmed();
        QString out;
        if (!line.isEmpty())
            out = line;
        if (!block.isEmpty()) {
            if (!out.isEmpty())
                out += "\n\n";
            out += block;
        }
        display->setText(out.isEmpty() ? QString::fromUtf8("—") : out);
    };

    QObject::connect(input, &QLineEdit::textChanged, display, [=](const QString&) { updateDisplay(); });
    QObject::connect(multiline, &QTextEdit::textChanged, display, [=]() { updateDisplay(); });
    QObject::connect(clearBtn, &QPushButton::clicked, display, [=]() {
        input->clear();
        multiline->clear();
        display->setText(QString::fromUtf8("—"));
    });

    win.setCentralWidget(central);
    win.show();
    return app.exec();
}
