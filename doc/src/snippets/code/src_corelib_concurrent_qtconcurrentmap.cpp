/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
U function(const T &t);
//! [0]


//! [1]
QImage scaled(const QImage &image)
{
    return image.scaled(100, 100);
}

QList<QImage> images = ...;
QFuture<QImage> thumbnails = QtConcurrent::mapped(images, scaled);
//! [1]


//! [2]
U function(T &t);
//! [2]


//! [3]
void scale(QImage &image)
{
    image = image.scaled(100, 100);
}

QList<QImage> images = ...;
QFuture<void> future = QtConcurrent::map(images, scale);
//! [3]


//! [4]
V function(T &result, const U &intermediate)
//! [4]


//! [5]
void addToCollage(QImage &collage, const QImage &thumbnail)
{
    QPainter p(&collage);
    static QPoint offset = QPoint(0, 0);
    p.drawImage(offset, thumbnail);
    offset += ...;
}

QList<QImage> images = ...;
QFuture<QImage> collage = QtConcurrent::mappedReduced(images, scaled, addToCollage);
//! [5]


//! [6]
QList<QImage> images = ...;

QFuture<QImage> thumbnails = QtConcurrent::mapped(images.constBegin(), images.constEnd(), scaled);

// map in-place only works on non-const iterators
QFuture<void> future = QtConcurrent::map(images.begin(), images.end(), scale);

QFuture<QImage> collage = QtConcurrent::mappedReduced(images.constBegin(), images.constEnd(), scaled, addToCollage);
//! [6]


//! [7]
QList<QImage> images = ...;

// each call blocks until the entire operation is finished
QList<QImage> future = QtConcurrent::blockingMapped(images, scaled);

QtConcurrent::blockingMap(images, scale);

QImage collage = QtConcurrent::blockingMappedReduced(images, scaled, addToCollage);
//! [7]


//! [8]
// squeeze all strings in a QStringList
QStringList strings = ...;
QFuture<void> squeezedStrings = QtConcurrent::map(strings, &QString::squeeze);

// swap the rgb values of all pixels on a list of images
QList<QImage> images = ...;
QFuture<QImage> bgrImages = QtConcurrent::mapped(images, &QImage::rgbSwapped);

// create a set of the lengths of all strings in a list
QStringList strings = ...;
QFuture<QSet<int> > wordLengths = QtConcurrent::mappedReduced(string, &QString::length, &QSet<int>::insert);
//! [8]


//! [9]
// can mix normal functions and member functions with QtConcurrent::mappedReduced()

// compute the average length of a list of strings
extern void computeAverage(int &average, int length);
QStringList strings = ...;
QFuture<int> averageWordLength = QtConcurrent::mappedReduced(strings, &QString::length, computeAverage);

// create a set of the color distribution of all images in a list
extern int colorDistribution(const QImage &string);
QList<QImage> images = ...;
QFuture<QSet<int> > totalColorDistribution = QtConcurrent::mappedReduced(images, colorDistribution, QSet<int>::insert);
//! [9]


//! [10]
QImage QImage::scaledToWidth(int width, Qt::TransformationMode) const;
//! [10]


//! [11]
boost::bind(&QImage::scaledToWidth, 100, Qt::SmoothTransformation)
//! [11]


//! [12]
QImage scaledToWith(const QImage &image)
//! [12]


//! [13]
QList<QImage> images = ...;
QFuture<QImage> thumbnails = QtConcurrent::mapped(images, boost::bind(&QImage::scaledToWidth, 100 Qt::SmoothTransformation));
//! [13]

//! [14]
struct Scaled
{
    Scaled(int size)
    : m_size(size) { }

    typedef QImage result_type;

    QImage operator()(const QImage &image)
    {
        return image.scaled(m_size, m_size);
    }

    int m_size;
};

QList<QImage> images = ...;
QFuture<QImage> thumbnails = QtConcurrent::mapped(images, Scaled(100));
//! [14]
