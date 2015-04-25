#include <QApplication>
#include <stdexcept>
#include <iostream>
#include "colorLevels.h"
#include "colorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(ColorWindow::tr("Image Viewer"));
    ColorWindow colorWindow;
    colorWindow.show();

    return app.exec();
}

ColorConstraint::ColorConstraint(Magick::Image* image, Magick::ColorRGB color, double threshold, ColorWindow* parent)
    : QWidget(parent)
{
    this->visible = true;
    this->pixelMask = NULL;
    this->image = image;
    this->color = color;
    this->threshold = threshold;
    empty = QSet<int>();

    QHBoxLayout* layout = new QHBoxLayout(this);

    colorSwatch = new QLabel(this);
    colorSwatch->setMinimumSize(30, 30);
    colorSwatch->setAutoFillBackground(true);

    QPalette palette;
    QColor qc = QColor(color.red() * 255, color.green() * 255, color.blue() * 255);
    palette.setColor(QPalette::Window, qc);
    palette.setColor(QPalette::WindowText, qc);

    colorSwatch->setPalette(palette);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(1);
    slider->setMaximum(200);
    slider->setTickInterval(1);
    slider->setValue(threshold*10);
    connect(slider, SIGNAL(sliderPressed()), this, SLOT(preventDebounce()));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(thresholdUpdated()));

    sliderEdit = new QLineEdit(QString("%1").arg(threshold));
    sliderEdit->setInputMask(tr("00.0"));
    sliderEdit->setMaximumWidth(40);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderMoved(int)));
    connect(sliderEdit, SIGNAL(textChanged(QString)), this, SLOT(sliderTextChanged(QString)));

    debouncer = new QTimer(this);
    connect(debouncer, SIGNAL(timeout()), this, SLOT(thresholdDebounced()));

    hideButton = new QPushButton(tr("H"), this);
    hideButton->setMaximumSize(30, 30);
    connect(hideButton, SIGNAL(clicked()), this, SLOT(toggleVisible()));

    removeButton = new QPushButton(tr("X"), this);
    removeButton->setMaximumSize(30, 30);
    connect(removeButton, SIGNAL(clicked()), this, SLOT(emitRemove()));

    layout->addWidget(colorSwatch);
    layout->addWidget(slider);
    layout->addWidget(sliderEdit);
    layout->addStretch();
    layout->addWidget(hideButton);
    layout->addWidget(removeButton);

    this->setLayout(layout);
    this->setMaximumHeight(45);
    this->setMinimumHeight(45);
}

void ColorConstraint::preventDebounce() {
    debouncer->blockSignals(true);
}

void ColorConstraint::sliderMoved(int value) {
    threshold = value/10.0;
    sliderEdit->setText(QString("%1").arg(threshold));
    debouncer->start(500);
}

void ColorConstraint::sliderTextChanged(QString text) {
    threshold = text.toDouble();
    debouncer->start(500);
}

void ColorConstraint::thresholdDebounced() {
    slider->blockSignals(true);
    slider->setValue(threshold*10.0);
    slider->blockSignals(false);

    thresholdUpdated();
}

void ColorConstraint::thresholdUpdated() {
    debouncer->blockSignals(false);
    debouncer->stop();

    if(pixelMask != NULL)
        delete pixelMask;
    pixelMask = NULL;

    emit updated();
}

void ColorConstraint::toggleVisible() {
    visible = !visible;
    if(visible) {
        hideButton->setText(tr("H"));
    } else {
        hideButton->setText(tr("S"));
    }

    emit updated();
}

void ColorConstraint::emitRemove() {
    emit remove(this);
}

ColorConstraint::~ColorConstraint() {
    if(pixelMask != NULL)
        delete pixelMask;
}

QSet<int>* ColorConstraint::getPixelMask() {
    if(!visible)
        return &empty;

    if(pixelMask == NULL)
        pixelMask = ColorLevels::matchingPixels(image, color, threshold);
    return pixelMask;
}

ColorWindow::ColorWindow()
{
    scaleFactors[0] = 1.0; scaleFactors[1] = 2.0; scaleFactors[2] = 4.0;
    isDragging = false;
    displayOriginal = false;
    colorsSet = new QSet<ColorConstraint*>;

    imageArea = new QLabel(this);
    imageArea->setBackgroundRole(QPalette::Base);
    imageArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageArea->installEventFilter(this);
    imageArea->setScaledContents(true);

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageArea);

    setCentralWidget(scrollArea);

    // COLOR DOCK WIDGET
    QDockWidget* colorDockWidget = new QDockWidget(tr("Color Constraints"), this);
    colorDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    QScrollArea* colorScrollArea = new QScrollArea(this);
    colorDockWidget->setWidget(colorScrollArea);
    colorDockWidget->setMinimumWidth(400);

    QWidget* colorWidget = new QWidget(colorScrollArea);
    colorScrollArea->setWidget(colorWidget);
    colorScrollArea->setWidgetResizable(true);

    colorsLayout = new QVBoxLayout(colorWidget);
    colorsLayout->addStretch();
    colorWidget->setLayout(colorsLayout);

    // STATS DOCK WIDGET
    QDockWidget* statsDockWidget = new QDockWidget(tr("Image Stats"), this);
    statsDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

    statsWidget = new QWidget(this);
    statsDockWidget->setWidget(statsWidget);
    QVBoxLayout* statsLayout = new QVBoxLayout(statsWidget);
    statsWidget->setLayout(statsLayout);

    exportButton = new QPushButton("Export Image");
    exportButton->setEnabled(false);
    connect(exportButton, SIGNAL(clicked()), this, SLOT(exportImage()));

    statsLayout->addWidget(statsFileLabel = new QLabel(QString("Image: N/A")));
    statsLayout->addWidget(statsTotalLabel = new QLabel(QString("Pixels: N/A")));
    statsLayout->addWidget(statsNonBackgroundLabel = new QLabel(QString("Non-background pixels: N/A")));
    statsLayout->addWidget(statsMatchingLabel = new QLabel(QString("Matching pixels: N/A")));
    statsLayout->addWidget(statsPercentLabel = new QLabel(QString("Matching percent: N/A")));
    statsLayout->addWidget(exportButton);
    statsWidget->setMaximumHeight(200);

    this->addDockWidget(Qt::RightDockWidgetArea, colorDockWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, statsDockWidget);

    this->setAcceptDrops(true);
    setWindowTitle(tr(WINDOW_TITLE));
    resize(QGuiApplication::primaryScreen()->availableSize());
}

void ColorWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void ColorWindow::dropEvent(QDropEvent* e) {
    loadFile(e->mimeData()->text());
}
void ColorWindow::loadFile(QString file) {
    //load blobs w/ imagemagick
    try {
      filename = file.split("/").last();

      int trim;
      #ifdef _WIN64
        trim = 8;
      #else
        trim = 7;
      #endif

      image.read(file.mid(trim).toStdString());
      filepath = file.mid(trim);
      filepath.chop(filename.length());

      image.write(&originalBlob);
      backgroundMask = ColorLevels::matchingPixels(&image, Magick::ColorRGB("#FFFFFF"), 0.1);

      clearConstraints();

      setWindowTitle(QString(tr(WINDOW_TITLE) + " - %1").arg(filename));
    } catch (Magick::Exception &e) {
      throw std::runtime_error(e.what());
    }
}

void ColorWindow::displayBlob(Magick::Blob* blob) {
    QByteArray imageData((char*)blob->data(), blob->length());

    QPixmap pixmap;
    pixmap.loadFromData(imageData);

    imageArea->setPixmap(pixmap);
    imageArea->resize(scaleFactors[scaleIndex] * imageArea->pixmap()->size());
}

void ColorWindow::keyPressEvent(QKeyEvent* event) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if(keyEvent->key() == 32) { //space
        displayOriginal = !displayOriginal;
        if(displayOriginal) { displayBlob(&originalBlob); } else { displayBlob(&blob); }
    }
}

static int smoothFactor = 0;
bool ColorWindow::eventFilter(QObject *, QEvent *evt) {
    if(evt->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);

        if(mouseEvent->button() == Qt::RightButton) {
            QPoint scaledPos = mouseEvent->pos() / scaleFactors[scaleIndex];
            Magick::ColorRGB rgb = ColorLevels::colorAtPixel(&image, scaledPos.x(), scaledPos.y());
            addConstraint(new ColorConstraint(&image, rgb, 10.0, this));

            return true;
        } else {
            QApplication::setOverrideCursor(Qt::ClosedHandCursor);

            isDragging = true;
            dragStart = mouseEvent->pos();
            scrollStart = QPoint(scrollArea->horizontalScrollBar()->value(),
                                 scrollArea->verticalScrollBar()->value());
        }
    } else if( isDragging && evt->type() == QEvent::MouseButtonRelease ) {
        QApplication::setOverrideCursor(Qt::CrossCursor);

        isDragging = false;
    } else if( isDragging && evt->type() == QEvent::MouseMove ) {
        if(smoothFactor++ % 2 == 0) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
            QPoint delta = (dragStart - mouseEvent->pos()) / 1.5;
            QPoint scrollEnd = scrollStart + delta;

            scrollArea->horizontalScrollBar()->setValue(scrollEnd.x());
            scrollArea->verticalScrollBar()->setValue(scrollEnd.y());
            scrollArea->update();
        }
    } else if(evt->type() == QEvent::Wheel ) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(evt);

        if(wheelEvent->delta() > 0 && scaleIndex < SCALE_FACTOR_SIZE-1)
            scaleImage(wheelEvent->pos(), scaleFactors[++scaleIndex]);
        else if(wheelEvent->delta() < 0 && scaleIndex > 0)
            scaleImage(wheelEvent->pos(), scaleFactors[--scaleIndex]);

        return true;
    }
    return false;
}

void ColorWindow::scaleImage(QPoint point, double factor)
{
    Q_ASSERT(imageArea->pixmap());

    double hp = (double)point.x() / imageArea->width();
    double vp = (double)point.y() / imageArea->height();
    imageArea->resize(factor * imageArea->pixmap()->size());
    scrollArea->horizontalScrollBar()->setValue( (int)(scrollArea->horizontalScrollBar()->maximum()*hp) );
    scrollArea->verticalScrollBar()->setValue( (int)(scrollArea->verticalScrollBar()->maximum()*vp) );
}

void ColorWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + (abs(factor - 1) * scrollBar->pageStep()/2)));
}

void ColorWindow::addConstraint(ColorConstraint* constraint) {
    colorsLayout->insertWidget(colorsSet->size(), constraint);
    colorsSet->insert(constraint);

    constraintsUpdated();
    connect(constraint, SIGNAL(updated()), this, SLOT(constraintsUpdated()));
    connect(constraint, SIGNAL(remove(ColorConstraint*)), this, SLOT(removeConstraint(ColorConstraint*)));
}

void ColorWindow::removeConstraint(ColorConstraint* c) {
    colorsSet->remove(c);
    colorsLayout->removeWidget(c);
    c->deleteLater();

    constraintsUpdated();
}

void ColorWindow::clearConstraints() {
    QSet<ColorConstraint*>::Iterator it = colorsSet->begin();
    while(it != colorsSet->end()) {
        colorsLayout->removeWidget(*it);
        (*it)->deleteLater();
        it++;
    }
    colorsSet->clear();

    constraintsUpdated();
}

void ColorWindow::constraintsUpdated() {
    QSet<int> mask;
    QSet<ColorConstraint*>::Iterator constraint = colorsSet->begin();
    while(constraint != colorsSet->end()) {
        mask.unite( *(*constraint)->getPixelMask() );
        constraint++;
    }

    maskedImage = Magick::Image(image);
    int width = maskedImage.size().width();
    int height = maskedImage.size().height();
    Magick::PixelPacket* pixels = maskedImage.getPixels(0, 0, width, height);

    QSet<int>::Iterator pixelIt;

    const Magick::ColorRGB backgroundMaskColor("#000000");
    pixelIt = backgroundMask->begin();
    while(pixelIt != backgroundMask->end()) {
        int offset = (*pixelIt);
        pixels[offset] = backgroundMaskColor;
        pixelIt++;
    }

    const Magick::ColorRGB maskColor("#00FF00");
    pixelIt = mask.begin();
    while(pixelIt != mask.end()) {
        int offset = (*pixelIt);
        pixels[offset] = maskColor;
        pixelIt++;
    }

    maskedImage.syncPixels();
    maskedImage.write(&blob);
    if(displayOriginal) { displayBlob(&originalBlob); } else { displayBlob(&blob); }

    int total = width*height;
    int background = backgroundMask->size();
    int net = total-background;
    int matchingTotal = mask.size();
    double matchingPercent = 100.0*matchingTotal / net;

    statsFileLabel->setText(QString("Image: %1").arg(filename));
    statsTotalLabel->setText(QString("Pixels: %1").arg(total));
    statsNonBackgroundLabel->setText(QString("Non-background pixels: %1").arg(net));
    statsMatchingLabel->setText(QString("Matching pixels: %1").arg(matchingTotal));
    statsPercentLabel->setText(QString("Matching percent: %1\%").arg(matchingPercent));
    exportButton->setEnabled(true);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1\t%2").arg(filename).arg(matchingPercent/100.0));
}

void ColorWindow::exportImage() {
    exportButton->setEnabled(false);

    QString exportFn = QString("%1%2_masked.png").arg(filepath, filename.mid(0, filename.lastIndexOf('.')));

    maskedImage.write(exportFn.toStdString());
}
