/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
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

#include <qimage.h>
#include <qcolor.h>

static inline int blendComponent( int v, int av, int s, int as )
{
    return as*s + av*v -(av*as*s)/255;  
}
    
static inline QRgb blendShade( QRgb v, QRgb s )
{
    //shadow image is already reduced and blurred
    int as = qAlpha(s); 
    int av = qAlpha(v);
    if ( as == 0 || av == 255 )
	return v;

    int a = as + av -(as*av)/255;
    
    int r = blendComponent( qRed(v),av, qRed(s), as)/a;
    int g = blendComponent( qGreen(v),av, qGreen(s), as)/a;
    int b = blendComponent( qBlue(v),av, qBlue(s), as)/a;

    return qRgba(r,g,b,a);
}

 

int main( int*, char**)
{
    QImage image( "out.png" );
    image.convertDepth( 32 );
    QImage shade( "outshade.png" );
    shade.convertDepth( 32 );
    int dx = 10;
    int dy = 5;
	
    int w = image.width();
    int h = image.height();
	
    QImage img( w+dx, h+dy, 32 );
    img.setAlphaBuffer( TRUE );

    for ( int y = 0; y < h+dy; y++ ) {
	for ( int x = 0; x < w+dx; x++ ) {
	    QRgb sh =  (x<dx||y<dy) ? 0 : shade.pixel( x-dx, y-dy );
	    QRgb pixel = (x<w&y<h) ? image.pixel( x, y ) : 0;
	    img.setPixel( x, y, blendShade( pixel, sh ) ); 
	}
    }
    img.save("blend.png", "PNG" );
}


