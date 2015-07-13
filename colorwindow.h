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

class OverviewWidget : public QLabel {
    Q_OBJECT

public:
    OverviewWidget(QWidget* parent);
    void drawBoundingBox(double x1, double y1, double x2, double y2);
    std::tuple<double,double,double,double> getBoundingBox();

protected:
    QRubberBand* band;

};

class ColorWindow : public QMainWindow {
    Q_OBJECT

#define WINDOW_TITLE "Science Wand v0.9.1"
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
    void exportImage();
    void updateOverview();

protected:
    double getScaleFactor();
    bool incrementScaleFactor(int direction);

    void displayBlob(Magick::Blob* blob);
    void initOverview();
    bool eventFilter(QObject *, QEvent *evt);
    void keyPressEvent(QKeyEvent* event);
    void scaleImage(QPoint point, double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

protected:
#define SCALE_FACTOR_SIZE 4
    double scaleFactors[SCALE_FACTOR_SIZE]; // = {0.75, 1.0, 2.0, 4.0};
    int scaleIndex = 0;

    QString filepath, filename;
    QSet<ColorConstraint*>* colorsSet;

    //layout
    QLabel* imageArea;
    QVBoxLayout* colorsLayout;
    OverviewWidget* overviewWidget;
    QWidget* statsWidget;

    //stats widget layout
    QLabel* statsFileLabel;
    QLabel* statsTotalLabel;
    QLabel* statsNonBackgroundLabel;
    QLabel* statsMatchingLabel;
    QLabel* statsPercentLabel;
    QPushButton* exportButton;

    //dragging
    QPoint dragStart, scrollStart;
    QScrollArea* scrollArea;

    //image stuff
    QSet<int>* backgroundMask;
    bool displayOriginal, isDragging;
    Magick::Image image;
    Magick::Blob originalBlob, blob;

    Magick::Image maskedImage;
};

#endif // COLORWINDOW

