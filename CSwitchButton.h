/*
 * This is nearly complete Material design Switch widget implementation in qtwidgets module.
 * More info: https://material.io/design/components/selection-controls.html#switches
 * Copyright (C) 2018-2020 Iman Ahmadvand
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
*/

#ifndef CSWITCHBUTTON_H
#define CSWITCHBUTTON_H

#include <QtWidgets>
#include "CSwitchButtonStyle.h"

class CSwitchAnimator final : public QVariantAnimation {
    Q_OBJECT
    Q_PROPERTY(QObject* targetObject READ targetObject WRITE setTargetObject)

  public:
    CSwitchAnimator(QObject* target, QObject* parent = nullptr);
    ~CSwitchAnimator() override;

    QObject* targetObject() const;
    void setTargetObject(QObject* target);

    inline bool isRunning() const {
        return state() == Running;
    }

  public slots:
    void setup(int duration, QEasingCurve easing = QEasingCurve::Linear);
    void interpolate(const QVariant& start, const QVariant& end);
    void setCurrentValue(const QVariant&);

  protected:
    void updateCurrentValue(const QVariant& value) override final;
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState) override final;

  private:
    QPointer<QObject> target;
};

class CSwitchSelectionControl : public QAbstractButton {
    Q_OBJECT

  public:
    explicit CSwitchSelectionControl(QWidget* parent = nullptr);
    ~CSwitchSelectionControl() override;

    Qt::CheckState checkState() const;

  Q_SIGNALS:
    void stateChanged(int);

  protected:
    //void enterEvent(QEvent*) override;
    void checkStateSet() override;
    void nextCheckState() override;
    virtual void toggle(Qt::CheckState state) = 0;
};

class CSwitchButton final : public CSwitchSelectionControl {
    Q_OBJECT

    static constexpr auto CORNER_RADIUS = 8.0;
    static constexpr auto THUMB_RADIUS = 14.5;
    static constexpr auto SHADOW_ELEVATION = 2.0;

  public:
    explicit CSwitchButton(QWidget* parent = nullptr);
    CSwitchButton(const QString& text, QWidget* parent = nullptr);
    CSwitchButton(const QString& text, const QBrush&, QWidget* parent = nullptr);
    ~CSwitchButton() override;

    QSize sizeHint() const override final;

    //Style::Switch& getStyle() { return this->style; }
    CSwitchStyle::Switch style;

    void init();

  protected:
    void paintEvent(QPaintEvent*) override final;
    void resizeEvent(QResizeEvent*) override final;
    void toggle(Qt::CheckState) override final;

    QRect indicatorRect();
    QRect textRect();

    static inline QColor colorFromOpacity(const QColor& c, qreal opacity) {
        return QColor(c.red(), c.green(), c.blue(), qRound(opacity * 255.0));
    }
    static inline bool ltr(QWidget* w) {
        if (nullptr != w)
            return w->layoutDirection() == Qt::LeftToRight;

        return false;
    }

  private:
    QPixmap shadowPixmap;
    QPointer<CSwitchAnimator> thumbBrushAnimation;
    QPointer<CSwitchAnimator> trackBrushAnimation;
    QPointer<CSwitchAnimator> thumbPosAniamtion;
};

#endif // CSWITCHBUTTON_H
