#pragma once
#include <QPushButton>

class DieIconButton : public QPushButton {
    Q_OBJECT
public:
    explicit DieIconButton(int sides, QWidget* parent = nullptr);
    int sides() const { return m_sides; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int m_sides;
};
