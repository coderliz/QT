/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
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

/****************************************************************************
**
** Copyright (c) 2007-2008, Apple, Inc.
**
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**   * Redistributions of source code must retain the above copyright notice,
**     this list of conditions and the following disclaimer.
**
**   * Redistributions in binary form must reproduce the above copyright notice,
**     this list of conditions and the following disclaimer in the documentation
**     and/or other materials provided with the distribution.
**
**   * Neither the name of Apple, Inc. nor the names of its contributors
**     may be used to endorse or promote products derived from this software
**     without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/

#include "qplatformdefs.h"
#include "private/qt_mac_p.h"
#include "qeventdispatcher_mac_p.h"
#include "qapplication.h"
#include "qevent.h"
#include "qdialog.h"
#include "qhash.h"
#include "qsocketnotifier.h"
#include "private/qwidget_p.h"
#include "private/qthread_p.h"
#include "private/qapplication_p.h"

#include <private/qcocoaapplication_mac_p.h>
#include "private/qt_cocoa_helpers_mac_p.h"

#ifndef QT_NO_THREAD
#  include "qmutex.h"

QT_BEGIN_NAMESPACE

QT_USE_NAMESPACE
#endif

/*****************************************************************************
  Externals
 *****************************************************************************/
extern void qt_event_request_timer(MacTimerInfo *); //qapplication_mac.cpp
extern MacTimerInfo *qt_event_get_timer(EventRef); //qapplication_mac.cpp
extern void qt_event_request_select(QEventDispatcherMac *); //qapplication_mac.cpp
extern void qt_event_request_updates(); //qapplication_mac.cpp
extern OSWindowRef qt_mac_window_for(const QWidget *); //qwidget_mac.cpp
extern bool qt_is_gui_used; //qapplication.cpp
extern bool qt_sendSpontaneousEvent(QObject*, QEvent*); // qapplication.cpp
extern bool qt_mac_is_macsheet(const QWidget *); //qwidget_mac.cpp

static inline CFRunLoopRef mainRunLoop()
{
#ifndef QT_MAC_USE_COCOA
    return reinterpret_cast<CFRunLoopRef>(const_cast<void *>(GetCFRunLoopFromEventLoop(GetMainEventLoop())));
#else
    return CFRunLoopGetMain();
#endif
}

/*****************************************************************************
  Timers stuff
 *****************************************************************************/

/* timer call back */
void QEventDispatcherMacPrivate::activateTimer(CFRunLoopTimerRef, void *info)
{
    int timerID =
#ifdef Q_OS_MAC64
    qint64(info);
#else
    int(info);
#endif

    MacTimerInfo *tmr;
    tmr = macTimerHash.value(timerID);
    if (tmr == 0 || tmr->pending == true)
        return; // Can't send another timer event if it's pending.

    tmr->pending = true;
    QTimerEvent e(tmr->id);
    qt_sendSpontaneousEvent(tmr->obj, &e);

    // Get the value again in case the timer gets unregistered during the sendEvent.
    tmr = macTimerHash.value(timerID);
    if (tmr != 0)
        tmr->pending = false;
}

void QEventDispatcherMac::registerTimer(int timerId, int interval, QObject *obj)
{
#ifndef QT_NO_DEBUG
    if (timerId < 1 || interval < 0 || !obj) {
        qWarning("QEventDispatcherMac::registerTimer: invalid arguments");
        return;
    } else if (obj->thread() != thread() || thread() != QThread::currentThread()) {
        qWarning("QObject::startTimer: timers cannot be started from another thread");
        return;
    }
#endif

    MacTimerInfo *t = new MacTimerInfo();
    t->id = timerId;
    t->interval = interval;
    t->obj = obj;
    t->runLoopTimer = 0;
    t->pending = false;

    CFAbsoluteTime fireDate = CFAbsoluteTimeGetCurrent();
    CFTimeInterval cfinterval = qMax(CFTimeInterval(interval) / 1000, 0.0000001);
    fireDate += cfinterval;
    QEventDispatcherMacPrivate::macTimerHash.insert(timerId, t);
    CFRunLoopTimerContext info = { 0, (void *)timerId, 0, 0, 0 };
    t->runLoopTimer = CFRunLoopTimerCreate(0, fireDate, cfinterval, 0, 0,
                                           QEventDispatcherMacPrivate::activateTimer, &info);
    if (t->runLoopTimer == 0) {
        qFatal("QEventDispatcherMac::registerTimer: Cannot create timer");
    }
    CFRunLoopAddTimer(mainRunLoop(), t->runLoopTimer, kCFRunLoopCommonModes);
}

bool QEventDispatcherMac::unregisterTimer(int identifier)
{
#ifndef QT_NO_DEBUG
    if (identifier < 1) {
        qWarning("QEventDispatcherMac::unregisterTimer: invalid argument");
        return false;
    } else if (thread() != QThread::currentThread()) {
        qWarning("QObject::killTimer: timers cannot be stopped from another thread");
        return false;
    }
#endif
    if (identifier <= 0)
        return false;                                // not init'd or invalid timer

    MacTimerInfo *timerInfo = QEventDispatcherMacPrivate::macTimerHash.take(identifier);
    if (timerInfo == 0)
        return false;

    if (!QObjectPrivate::get(timerInfo->obj)->inThreadChangeEvent)
        QAbstractEventDispatcherPrivate::releaseTimerId(identifier);
    CFRunLoopTimerInvalidate(timerInfo->runLoopTimer);
    CFRelease(timerInfo->runLoopTimer);
    delete timerInfo;

    return true;
}

bool QEventDispatcherMac::unregisterTimers(QObject *obj)
{
#ifndef QT_NO_DEBUG
    if (!obj) {
        qWarning("QEventDispatcherMac::unregisterTimers: invalid argument");
        return false;
    } else if (obj->thread() != thread() || thread() != QThread::currentThread()) {
        qWarning("QObject::killTimers: timers cannot be stopped from another thread");
        return false;
    }
#endif

    MacTimerHash::iterator it = QEventDispatcherMacPrivate::macTimerHash.begin();
    while (it != QEventDispatcherMacPrivate::macTimerHash.end()) {
        MacTimerInfo *timerInfo = it.value();
        if (timerInfo->obj != obj) {
            ++it;
        } else {
            if (!QObjectPrivate::get(timerInfo->obj)->inThreadChangeEvent)
                QAbstractEventDispatcherPrivate::releaseTimerId(timerInfo->id);
            CFRunLoopTimerInvalidate(timerInfo->runLoopTimer);
            CFRelease(timerInfo->runLoopTimer);
            delete timerInfo;
            it = QEventDispatcherMacPrivate::macTimerHash.erase(it);
        }
    }
    return true;
}

QList<QEventDispatcherMac::TimerInfo>
QEventDispatcherMac::registeredTimers(QObject *object) const
{
    if (!object) {
        qWarning("QEventDispatcherMac:registeredTimers: invalid argument");
        return QList<TimerInfo>();
    }

    QList<TimerInfo> list;

    MacTimerHash::const_iterator it = QEventDispatcherMacPrivate::macTimerHash.constBegin();
    while (it != QEventDispatcherMacPrivate::macTimerHash.constEnd()) {
        MacTimerInfo *t = it.value();
        if (t->obj == object)
            list << TimerInfo(t->id, t->interval);
        ++it;
    }
    return list;
}

/**************************************************************************
    Socket Notifiers
 *************************************************************************/
void qt_mac_socket_callback(CFSocketRef s, CFSocketCallBackType callbackType, CFDataRef,
                            const void *,  void *info) {
    QEventDispatcherMacPrivate *const eventDispatcher
                                    = static_cast<QEventDispatcherMacPrivate *>(info);
    int nativeSocket = CFSocketGetNative(s);
    MacSocketInfo *socketInfo = eventDispatcher->macSockets.value(nativeSocket);
    QEvent notifierEvent(QEvent::SockAct);

    // There is a race condition that happen where we disable the notifier and
    // the kernel still has a notification to pass on. We then get this
    // notification after we've successfully disabled the CFSocket, but our Qt
    // notifier is now gone. The upshot is we have to check the notifier
    // everytime.
    if (callbackType == kCFSocketReadCallBack) {
        if (socketInfo->readNotifier)
            QApplication::sendEvent(socketInfo->readNotifier, &notifierEvent);
    } else if (callbackType == kCFSocketWriteCallBack) {
        if (socketInfo->writeNotifier)
            QApplication::sendEvent(socketInfo->writeNotifier, &notifierEvent);
    }
}

/*
    Adds a loop source for the given socket to the current run loop.
*/
CFRunLoopSourceRef qt_mac_add_socket_to_runloop(const CFSocketRef socket)
{
    CFRunLoopSourceRef loopSource = CFSocketCreateRunLoopSource(kCFAllocatorDefault, socket, 0);
    if (!loopSource)
        return 0;

    CFRunLoopAddSource(mainRunLoop(), loopSource, kCFRunLoopCommonModes);
    return loopSource;
}

/*
    Removes the loop source for the given socket from the current run loop.
*/
void qt_mac_remove_socket_from_runloop(const CFSocketRef socket, CFRunLoopSourceRef runloop)
{
    Q_ASSERT(runloop);
    CFRunLoopRemoveSource(mainRunLoop(), runloop, kCFRunLoopCommonModes);
    CFSocketDisableCallBacks(socket, kCFSocketReadCallBack);
    CFSocketDisableCallBacks(socket, kCFSocketWriteCallBack);
    CFRunLoopSourceInvalidate(runloop);
}

/*
    Register a QSocketNotifier with the mac event system by creating a CFSocket with
    with a read/write callback.

    Qt has separate socket notifiers for reading and writing, but on the mac there is
    a limitation of one CFSocket object for each native socket.
*/
void QEventDispatcherMac::registerSocketNotifier(QSocketNotifier *notifier)
{
    Q_ASSERT(notifier);
    int nativeSocket = notifier->socket();
    int type = notifier->type();
#ifndef QT_NO_DEBUG
    if (nativeSocket < 0 || nativeSocket > FD_SETSIZE) {
        qWarning("QSocketNotifier: Internal error");
        return;
    } else if (notifier->thread() != thread()
               || thread() != QThread::currentThread()) {
        qWarning("QSocketNotifier: socket notifiers cannot be enabled from another thread");
        return;
    }
#endif

    Q_D(QEventDispatcherMac);

    if (type == QSocketNotifier::Exception) {
        qWarning("QSocketNotifier::Exception is not supported on Mac OS X");
        return;
    }

    // Check if we have a CFSocket for the native socket, create one if not.
    MacSocketInfo *socketInfo = d->macSockets.value(nativeSocket);
    if (!socketInfo) {
        socketInfo = new MacSocketInfo();

        // Create CFSocket, specify that we want both read and write callbacks (the callbacks
        // are enabled/disabled later on).
        const int callbackTypes = kCFSocketReadCallBack | kCFSocketWriteCallBack;
        CFSocketContext context = {0, d, 0, 0, 0};
        socketInfo->socket = CFSocketCreateWithNative(kCFAllocatorDefault, nativeSocket, callbackTypes, qt_mac_socket_callback, &context);
        if (CFSocketIsValid(socketInfo->socket) == false) {
            qWarning("QEventDispatcherMac::registerSocketNotifier: Failed to create CFSocket");
            return;
        }

        CFOptionFlags flags = CFSocketGetSocketFlags(socketInfo->socket);
        flags |= kCFSocketAutomaticallyReenableWriteCallBack; //QSocketNotifier stays enabled after a write
        flags &= ~kCFSocketCloseOnInvalidate; //QSocketNotifier doesn't close the socket upon destruction/invalidation
        CFSocketSetSocketFlags(socketInfo->socket, flags);

        // Add CFSocket to runloop.
        if(!(socketInfo->runloop = qt_mac_add_socket_to_runloop(socketInfo->socket))) {
            qWarning("QEventDispatcherMac::registerSocketNotifier: Failed to add CFSocket to runloop");
            CFSocketInvalidate(socketInfo->socket);
            CFRelease(socketInfo->socket);
            return;
        }

        // Disable both callback types by default. This must be done after
        // we add the CFSocket to the runloop, or else these calls will have
        // no effect.
        CFSocketDisableCallBacks(socketInfo->socket, kCFSocketReadCallBack);
        CFSocketDisableCallBacks(socketInfo->socket, kCFSocketWriteCallBack);

        d->macSockets.insert(nativeSocket, socketInfo);
    }

    // Increment read/write counters and select enable callbacks if necessary.
    if (type == QSocketNotifier::Read) {
        Q_ASSERT(socketInfo->readNotifier == 0);
        socketInfo->readNotifier = notifier;
        CFSocketEnableCallBacks(socketInfo->socket, kCFSocketReadCallBack);
    } else if (type == QSocketNotifier::Write) {
        Q_ASSERT(socketInfo->writeNotifier == 0);
        socketInfo->writeNotifier = notifier;
        CFSocketEnableCallBacks(socketInfo->socket, kCFSocketWriteCallBack);
    }
}

/*
    Unregister QSocketNotifer. The CFSocket correspoding to this notifier is
    removed from the runloop of this is the last notifier that users
    that CFSocket.
*/
void QEventDispatcherMac::unregisterSocketNotifier(QSocketNotifier *notifier)
{
    Q_ASSERT(notifier);
    int nativeSocket = notifier->socket();
    int type = notifier->type();
#ifndef QT_NO_DEBUG
    if (nativeSocket < 0 || nativeSocket > FD_SETSIZE) {
        qWarning("QSocketNotifier: Internal error");
        return;
    } else if (notifier->thread() != thread() || thread() != QThread::currentThread()) {
        qWarning("QSocketNotifier: socket notifiers cannot be disabled from another thread");
        return;
    }
#endif

    Q_D(QEventDispatcherMac);

    if (type == QSocketNotifier::Exception) {
        qWarning("QSocketNotifier::Exception is not supported on Mac OS X");
        return;
    }
    MacSocketInfo *socketInfo = d->macSockets.value(nativeSocket);
    if (!socketInfo) {
        qWarning("QEventDispatcherMac::unregisterSocketNotifier: Tried to unregister a not registered notifier");
        return;
    }

    // Decrement read/write counters and disable callbacks if necessary.
    if (type == QSocketNotifier::Read) {
        Q_ASSERT(notifier == socketInfo->readNotifier);
        socketInfo->readNotifier = 0;
        CFSocketDisableCallBacks(socketInfo->socket, kCFSocketReadCallBack);
    } else if (type == QSocketNotifier::Write) {
        Q_ASSERT(notifier == socketInfo->writeNotifier);
        socketInfo->writeNotifier = 0;
        CFSocketDisableCallBacks(socketInfo->socket, kCFSocketWriteCallBack);
    }

    // Remove CFSocket from runloop if this was the last QSocketNotifier.
    if (socketInfo->readNotifier == 0 && socketInfo->writeNotifier == 0) {
        if (CFSocketIsValid(socketInfo->socket))
            qt_mac_remove_socket_from_runloop(socketInfo->socket, socketInfo->runloop);
        CFRunLoopSourceInvalidate(socketInfo->runloop);
        CFRelease(socketInfo->runloop);
        CFSocketInvalidate(socketInfo->socket);
        CFRelease(socketInfo->socket);
        delete socketInfo;
        d->macSockets.remove(nativeSocket);
    }
}

bool QEventDispatcherMac::hasPendingEvents()
{
    extern uint qGlobalPostedEventsCount();
    return qGlobalPostedEventsCount() || (qt_is_gui_used && GetNumEventsInQueue(GetMainEventQueue()));
}


static bool qt_mac_send_event(QEventLoop::ProcessEventsFlags, OSEventRef event, OSWindowRef pt)
{
#ifndef QT_MAC_USE_COCOA
    if(pt && SendEventToWindow(event, pt) != eventNotHandledErr)
        return true;
    return !SendEventToEventTarget(event, GetEventDispatcherTarget());
#else // QT_MAC_USE_COCOA
    if (pt)
        [pt sendEvent:event];
    else
        [NSApp sendEvent:event];
    return true;
#endif
}

#ifdef QT_MAC_USE_COCOA
static bool IsMouseOrKeyEvent( NSEvent* event )
{
    bool    result    = false;
    
    switch( [event type] )
    {
        case NSLeftMouseDown:              
        case NSLeftMouseUp:      
        case NSRightMouseDown:   
        case NSRightMouseUp:     
        case NSMouseMoved:                // ??
        case NSLeftMouseDragged: 
        case NSRightMouseDragged:
        case NSMouseEntered:     
        case NSMouseExited:      
        case NSKeyDown:          
        case NSKeyUp:            
        case NSFlagsChanged:            // key modifiers changed?
        case NSCursorUpdate:            // ??
        case NSScrollWheel:      
        case NSTabletPoint:      
        case NSTabletProximity:  
        case NSOtherMouseDown:   
        case NSOtherMouseUp:     
        case NSOtherMouseDragged:
            result    = true;
        break;

        default:
        break;
    }
    return result;
}
#endif

bool QEventDispatcherMac::processEvents(QEventLoop::ProcessEventsFlags flags)
{
    Q_D(QEventDispatcherMac);
    d->interrupt = false;
    // In case we end up recursing while we now process events, make sure
    // that we send remaining posted Qt events before this call returns:
    wakeUp();
    emit awake();

#ifndef QT_MAC_NO_QUICKDRAW
    if(!qt_mac_safe_pdev) { //create an empty widget and this can be used for a port anytime
        QWidget *tlw = new QWidget;
        tlw->setAttribute(Qt::WA_DeleteOnClose);
        tlw->setObjectName(QLatin1String("empty_widget"));
        tlw->hide();
        qt_mac_safe_pdev = tlw;
    }
#endif

    bool retVal = false;
    forever {
        if (d->interrupt)
            break;

#ifdef QT_MAC_USE_COCOA
        QMacCocoaAutoReleasePool pool;
        NSEvent* event = 0;

        if (flags & QEventLoop::DialogExec || flags & QEventLoop::EventLoopExec) {
            // The point of the CocoaRequestModal event is to make sure that a
            // non-execed app modal window recurses into it's own dialog exec
            // once cocoa is spinning the event loop for us (e.g on top of [NSApp run]).
            // We expect only one event to notify us about this, regardless of how many
            // widgets that are waiting to be modal. So we remove all other pending
            // events, if any. And since cocoa will now take over event processing for us,
            // we allow new app modal widgets to recurse on top of us, hence the release of
            // the block:
            QBoolBlocker block(d->blockCocoaRequestModal, false);
            QCoreApplication::removePostedEvents(qApp, QEvent::CocoaRequestModal);

            if (NSModalSession session = d->activeModalSession())
                while ([NSApp runModalSession:session] == NSRunContinuesResponse) {
                    // runModalSession will not wait for events, so we do it
                    // ourselves (otherwise we would spend 100% CPU inside this loop):
                    event = [NSApp nextEventMatchingMask:NSAnyEventMask
                        untilDate:[NSDate distantFuture] inMode:NSModalPanelRunLoopMode dequeue:YES];
                    if (event)
                        [NSApp postEvent:event atStart:YES];
                }
            else
                [NSApp run];

            d->rebuildModalSessionStack(false);
            retVal = true;
        } else do {
            // Since we now are going to spin the event loop just _one_ round
            // we need to block all incoming CocoaRequestModal events to ensure
            // that we don't recurse into a new exec-ing event loop while doing
            // so (and as such, 'hang' the thread inside the recursion):
            QBoolBlocker block(d->blockCocoaRequestModal, true);
            bool mustRelease = false;

            if (!(flags & QEventLoop::ExcludeUserInputEvents) && !d->queuedUserInputEvents.isEmpty()) {
                // process a pending user input event
                mustRelease = true;
                event = static_cast<NSEvent *>(d->queuedUserInputEvents.takeFirst());
            } else {
                if (NSModalSession session = d->activeModalSession()) {
                    // There's s a modal widget showing, run it's session:
                    if (flags & QEventLoop::WaitForMoreEvents) {
                        // Wait for at least one event
                        // before spinning the session:
                        event = [NSApp nextEventMatchingMask:NSAnyEventMask
                            untilDate:[NSDate distantFuture] inMode:NSModalPanelRunLoopMode dequeue:YES];
                        if (event)
                            [NSApp postEvent:event atStart:YES];
                    }
                    [NSApp runModalSession:session];
                    retVal = true;
                    break;
                } else {
                    event = [NSApp nextEventMatchingMask:NSAnyEventMask
                        untilDate:nil
                        inMode:NSDefaultRunLoopMode
                        dequeue: YES];

                    if (event != nil) {
                        if (flags & QEventLoop::ExcludeUserInputEvents) {
                            if (IsMouseOrKeyEvent(event)) {
                                // retain event here?
                                [event retain];
                                d->queuedUserInputEvents.append(event);
                                continue;
                            }
                        }
                    }
                }
            }
            if (event) {
                if (!filterEvent(event) && qt_mac_send_event(flags, event, 0))
                    retVal = true;
                if (mustRelease)
                    [event release];
            }
        } while(!d->interrupt && event != nil);

#else
        do {
            EventRef event;
            if (!(flags & QEventLoop::ExcludeUserInputEvents)
                    && !d->queuedUserInputEvents.isEmpty()) {
                // process a pending user input event
                event = static_cast<EventRef>(d->queuedUserInputEvents.takeFirst());
            } else {
                OSStatus err = ReceiveNextEvent(0,0, kEventDurationNoWait, true, &event);
                if(err != noErr)
                    continue;
                // else
                if (flags & QEventLoop::ExcludeUserInputEvents) {
                    UInt32 ekind = GetEventKind(event),
                           eclass = GetEventClass(event);
                    switch(eclass) {
                        case kEventClassQt:
                            if(ekind != kEventQtRequestContext)
                                break;
                            // fall through
                        case kEventClassMouse:
                        case kEventClassKeyboard:
                            d->queuedUserInputEvents.append(event);
                            continue;
                    }
                }
            }

            if (!filterEvent(&event) && qt_mac_send_event(flags, event, 0))
                retVal = true;
            ReleaseEvent(event);
        } while(!d->interrupt && GetNumEventsInQueue(GetMainEventQueue()) > 0);

#endif

        bool canWait = (d->threadData->canWait
                && !retVal
                && !d->interrupt
                && (flags & QEventLoop::WaitForMoreEvents));
        if (canWait) {
            // INVARIANT: We haven't processed any events yet. And we're told
            // to stay inside this function until at least one event is processed
            // (WaitForMoreEvents). So we wait on the window server:
#ifndef QT_MAC_USE_COCOA
            while(CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1.0e20, true) == kCFRunLoopRunTimedOut);
#else
            QMacCocoaAutoReleasePool pool;
            NSEvent *manualEvent = [NSApp nextEventMatchingMask:NSAnyEventMask
                untilDate:[NSDate distantFuture] inMode:NSDefaultRunLoopMode
                dequeue:YES];
            if (manualEvent)
                [NSApp sendEvent:manualEvent];
#endif
            flags &= ~QEventLoop::WaitForMoreEvents;
        } else {
            // Done with event processing for now. Leave the function:
            break;
        }
    }

    // Because pending deffered-delete events are only sendt after
    // returning from the loop level they were posted in, we schedule
    // an extra wakup to force the _current_ run loop to process them (in
    // case the application stands idle waiting for the delete event):
    wakeUp();

    if (d->interrupt){
        // We restart NSApplication by first stopping it, and then call 'run'
        // again (NSApplication is actually already stopped, hence the need
        // for a restart, but calling stop again will also make the call
        // return from the current recursion). When the call returns to
        // QEventLoop (mind, not from this recursion, but from the one we're
        // about to stop), it will just call QEventDispatcherMac::processEvents()
        // again.
        interrupt();
    }
    return retVal;
}

void QEventDispatcherMac::wakeUp()
{
    Q_D(QEventDispatcherMac);
    d->serialNumber.ref();
    CFRunLoopSourceSignal(d->postedEventsSource);
    CFRunLoopWakeUp(mainRunLoop());
}

void QEventDispatcherMac::flush()
{
    if(qApp) {
        QWidgetList tlws = QApplication::topLevelWidgets();
        for(int i = 0; i < tlws.size(); i++) {
            QWidget *tlw = tlws.at(i);
            if(tlw->isVisible())
                macWindowFlush(qt_mac_window_for(tlw));
        }
    }
}

/*****************************************************************************
  QEventDispatcherMac Implementation
 *****************************************************************************/
MacTimerHash QEventDispatcherMacPrivate::macTimerHash;
bool QEventDispatcherMacPrivate::blockSendPostedEvents = false;

#ifdef QT_MAC_USE_COCOA
QStack<QCocoaModalSessionInfo> QEventDispatcherMacPrivate::cocoaModalSessionStack;
bool QEventDispatcherMacPrivate::blockCocoaRequestModal = false;

static void qt_mac_setChildDialogsResponsive(QWidget *widget, bool responsive)
{
    QList<QDialog *> dialogs = widget->findChildren<QDialog *>();
    for (int i=0; i<dialogs.size(); ++i){
        NSWindow *window = qt_mac_window_for(dialogs[i]);
        if (window && [window isKindOfClass:[NSPanel class]]) {
            [static_cast<NSPanel *>(window) setWorksWhenModal:responsive];
            if (responsive && dialogs[i]->isVisible()){
                [window orderFront:window];
            }
        }
    }
}

NSModalSession QEventDispatcherMacPrivate::activeModalSession()
{
    // Create (if needed) and return the modal session
    // for the  top-most modal dialog, if any:
    if (cocoaModalSessionStack.isEmpty())
        return 0;
    QCocoaModalSessionInfo &info = cocoaModalSessionStack.last();
    if (!info.widget)
        return 0;
    if (info.widget->testAttribute(Qt::WA_DontShowOnScreen)){
        // INVARIANT: We have a modal widget, but it's not visible on screen.
        // This will e.g. be true for native dialogs. Make the dialog children
        // of the previous modal dialog unresponsive, so that the current dialog
        // (native or not) is the only reponsive dialog on screen:
        int size = cocoaModalSessionStack.size();
        if (size > 1){
            if (QWidget *prevModal = cocoaModalSessionStack[size-2].widget)
                qt_mac_setChildDialogsResponsive(prevModal, false);
        }
        return 0;
    }

    if (!info.session) {
        QMacCocoaAutoReleasePool pool;
        NSWindow *window = qt_mac_window_for(info.widget);
        if (!window)
            return 0;
        // 'beginModalSessionForWindow' will give the event loop a spin, and as
        // such, deliver Qt events. This might lead to inconsistent behaviour
        // (especially if CocoaRequestModal is delivered), so we need to block:
        QBoolBlocker block(blockSendPostedEvents, true);
        info.session = [NSApp beginModalSessionForWindow:window];
        // Make the dialog children of the current modal dialog
        // responsive. And make the dialog children of
        // the previous modal dialog unresponsive again:
        qt_mac_setChildDialogsResponsive(info.widget, true);
        int size = cocoaModalSessionStack.size();
        if (size > 1){
            if (QWidget *prevModal = cocoaModalSessionStack[size-2].widget)
                qt_mac_setChildDialogsResponsive(prevModal, false);
        }
    }
    return info.session;
}

void QEventDispatcherMacPrivate::rebuildModalSessionStack(bool pop)
{
    // Calling [NSApp stopModal], or [NSApp stop], will stop all modal dialogs
    // in one go. So to to not confuse cocoa, we need to stop all our modal
    // sessions as well. QMacEventDispatcher will make them modal again
    // in the correct order as long as they are left on the cocoaModalSessionStack
    // and a CocoaRequestModal is posted:
    if (cocoaModalSessionStack.isEmpty())
        return;

    QMacCocoaAutoReleasePool pool;
    [NSApp stopModal];
    [NSApp postEvent:[NSEvent otherEventWithType:NSApplicationDefined location:NSZeroPoint
        modifierFlags:0 timestamp:0. windowNumber:0 context:0
        subtype:SHRT_MAX data1:0 data2:0] atStart:NO];

    for (int i=0; i<cocoaModalSessionStack.size(); ++i){
        QCocoaModalSessionInfo &info = cocoaModalSessionStack[i];
        if (info.session) {
            [NSApp endModalSession:info.session];
            info.session = 0;
        }
    }

    if (pop) {
        QCocoaModalSessionInfo info = cocoaModalSessionStack.pop();
        if (info.widget)
            qt_mac_setChildDialogsResponsive(info.widget, false);
    }

    if (!cocoaModalSessionStack.isEmpty()) {
        // Since we now have pending modal sessions again, make
        // sure that we enter modal for the one on the top later:
        qApp->postEvent(qApp, new QEvent(QEvent::CocoaRequestModal));
    } else {
        QCoreApplication::removePostedEvents(qApp, QEvent::CocoaRequestModal);
    }
}

#endif

QEventDispatcherMacPrivate::QEventDispatcherMacPrivate()
    : interrupt(false)
{
}

QEventDispatcherMac::QEventDispatcherMac(QObject *parent)
    : QAbstractEventDispatcher(*new QEventDispatcherMacPrivate, parent)
{
    Q_D(QEventDispatcherMac);
    CFRunLoopSourceContext context;
    bzero(&context, sizeof(CFRunLoopSourceContext));
    context.info = d;
    context.equal = QEventDispatcherMacPrivate::postedEventSourceEqualCallback;
    context.perform = QEventDispatcherMacPrivate::postedEventsSourcePerformCallback;
    d->postedEventsSource = CFRunLoopSourceCreate(0, 0, &context);
    Q_ASSERT(d->postedEventsSource);
    CFRunLoopAddSource(mainRunLoop(), d->postedEventsSource, kCFRunLoopCommonModes);

    CFRunLoopObserverContext observerContext;
    bzero(&observerContext, sizeof(CFRunLoopObserverContext));
    observerContext.info = this;
    d->waitingObserver = CFRunLoopObserverCreate(kCFAllocatorDefault,
                                                 kCFRunLoopBeforeWaiting | kCFRunLoopAfterWaiting,
                                                 true, 0,
                                                 QEventDispatcherMacPrivate::waitingObserverCallback,
                                                 &observerContext);
    CFRunLoopAddObserver(mainRunLoop(), d->waitingObserver, kCFRunLoopCommonModes);
}

void QEventDispatcherMacPrivate::waitingObserverCallback(CFRunLoopObserverRef,
                                                          CFRunLoopActivity activity, void *info)
{
    if (activity == kCFRunLoopBeforeWaiting)
        emit static_cast<QEventDispatcherMac*>(info)->aboutToBlock();
    else
        emit static_cast<QEventDispatcherMac*>(info)->awake();
}

Boolean QEventDispatcherMacPrivate::postedEventSourceEqualCallback(const void *info1, const void *info2)
{
    return info1 == info2;
}

void QEventDispatcherMacPrivate::postedEventsSourcePerformCallback(void *info)
{
    QEventDispatcherMacPrivate *d = static_cast<QEventDispatcherMacPrivate *>(info);
    if (blockSendPostedEvents) {
        CFRunLoopSourceSignal(d->postedEventsSource);
    } else {
        if (!d->threadData->canWait || (d->serialNumber != d->lastSerial)) {
            d->lastSerial = d->serialNumber;
            QApplicationPrivate::sendPostedEvents(0, 0, d->threadData);
        }
    }
}

#ifdef QT_MAC_USE_COCOA
static void stopNSApp()
{
    QMacCocoaAutoReleasePool pool;
    static const short NSAppShouldStopForQt = SHRT_MAX;
    [NSApp stop:NSApp];
    [NSApp postEvent:[NSEvent otherEventWithType:NSApplicationDefined location:NSZeroPoint
                                   modifierFlags:0 timestamp:0. windowNumber:0 context:0
                                         subtype:NSAppShouldStopForQt data1:0 data2:0] atStart:NO];
}
#endif

void QEventDispatcherMac::interrupt()
{
    Q_D(QEventDispatcherMac);
    d->interrupt = true;
    wakeUp();
#ifndef QT_MAC_USE_COCOA
    CFRunLoopStop(mainRunLoop());
#else
    stopNSApp();
#endif
}

QEventDispatcherMac::~QEventDispatcherMac()
{
    Q_D(QEventDispatcherMac);
    //timer cleanup
    MacTimerHash::iterator it = QEventDispatcherMacPrivate::macTimerHash.begin();
    while (it != QEventDispatcherMacPrivate::macTimerHash.end()) {
        MacTimerInfo *t = it.value();
        if (t->runLoopTimer) {
            CFRunLoopTimerInvalidate(t->runLoopTimer);
            CFRelease(t->runLoopTimer);
        }
        delete t;
        ++it;
    }
    QEventDispatcherMacPrivate::macTimerHash.clear();

    // Remove CFSockets from the runloop.
    for (MacSocketHash::ConstIterator it = d->macSockets.constBegin(); it != d->macSockets.constEnd(); ++it) {
        MacSocketInfo *socketInfo = (*it);
        if (CFSocketIsValid(socketInfo->socket)) {
            qt_mac_remove_socket_from_runloop(socketInfo->socket, socketInfo->runloop);
            CFRunLoopSourceInvalidate(socketInfo->runloop);
            CFRelease(socketInfo->runloop);
            CFSocketInvalidate(socketInfo->socket);
            CFRelease(socketInfo->socket);
        }
    }
    CFRunLoopRemoveSource(mainRunLoop(), d->postedEventsSource, kCFRunLoopCommonModes);
    CFRelease(d->postedEventsSource);

    CFRunLoopObserverInvalidate(d->waitingObserver);
    CFRelease(d->waitingObserver);
}


QT_END_NAMESPACE
