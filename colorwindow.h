#ifndef COLORWINDOW
#define COLORWINDOW

#include <QtWidgets>
#include <QSet>
#include <Magick++.h>
#include "colorLevels.h"

class ColorWindow;

class ColorConstraint : public QWidget {
    Q_OBJECT

public:
    ColorConstraint(Magick::Image* image, Magick::ColorRGB color, double threshold, ColorWindow* parent = 0);
    ~ColorConstraint();
    void setImage(Magick::Blob* image);
    QSet<int>* getPixelMask();

signals:
    void updated();
    void remove(ColorConstraint*);

protected slots:
    void toggleVisible();
    void emitRemove();

    void sliderTextChanged(QString text);
    void sliderMoved(int sliderValue);
    void preventDebounce();
    void thresholdDebounced();

    void thresholdUpdated();

protected:
    bool visible, canDebounce;
    QLabel* colorSwatch;
    QSlider* slider;
    QLineEdit* sliderEdit;
    QPushButton* hideButton,* removeButton;
    QTimer* debouncer;

    QSet<int> empty;
    QSet<int>* pixelMask;
    Magick::Image* image;
    Magick::ColorRGB color;
    double threshold;

};

class ColorWindow : public QMainWindow {
    Q_OBJECT

public:
    ColorWindow();

    void loadFile(QString filename);
    void addConstraint(ColorConstraint* c);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent* e);

public slots:
    void constraintsUpdated();
    void removeConstraint(ColorConstraint* c);
    void clearConstraints();

protected:
    void displayBlob(Magick::Blob* blob);
    bool eventFilter(QObject *, QEvent *evt);
    void keyPressEvent(QKeyEvent* event);

protected:
    QString filename;
    QLabel* imageArea;
    QScrollArea* scrollArea;
    QVBoxLayout* colorsLayout;
    QSet<ColorConstraint*>* colorsSet;
    QSet<int>* backgroundMask;

    QWidget* statsWidget; //FIXME

    bool displayOriginal;
    Magick::Image image;
    Magick::Blob originalBlob;
    Magick::Blob blob;

    Magick::Image maskedImage;

};

#endif // COLORWINDOW

