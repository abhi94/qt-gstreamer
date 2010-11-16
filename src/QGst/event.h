/*
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef QGST_EVENT_H
#define QGST_EVENT_H

#include "miniobject.h"
#include "structure.h"

namespace QGst {

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for GstEvent
 *
 * Events are passed between elements in parallel to the data stream. Some events are serialized
 * with buffers, others are not. Some events only travel downstream, others only upstream.
 * Some events can travel both upstream and downstream.
 *
 * The events are used to signal special conditions in the datastream such as EOS (end of stream) or
 * the start of a new stream-segment. Events are also used to flush the pipeline of pending data.
 *
 * Events are implemented as a subclass of MiniObject with a generic GstStructure as the
 * content. Notice that the source property is set by GStreamer when the event is passed to the
 * Pad with send() or push(). In the case of Element::sendEvent() the behavior is similar, as this
 * internally translates to searching for a random pad with the correct direction and then pushing
 * the event to it. So there is no need to set the source of the event in QtGStreamer.
 *
 * In these bindings, for convenience, each event type has its own Event subclass. This
 * does not reflect 1-1 the native C API, where there is only one Event class with tens of
 * 'new_foo' and 'parse_foo' methods. You can use RefPointer::dynamicCast() to cast a EventPtr
 * to a RefPointer of one of the Event subclasses and it will behave as expected (i.e. it will
 * only succeed if the event type matches the event type that the subclass handles). Note
 * however that the Event subclasses \em cannot be used with ValueBase::get(), since a GValue
 * will actually contain a GstEvent (the subclasses do not exist in C) and ValueBase::get()
 * is not able to do dynamic casts. As a result of that, Event subclasses also \em cannot be
 * used as arguments in slots connected to GObject signals, even though you may know that your
 * slot will only be called with that type of event.
 */
class Event : public MiniObject
{
    QGST_WRAPPER(Event)
public:
    static EventPtr create(EventType type,
                           const StructureBase & structure = SharedStructure(NULL));

    ObjectPtr source() const;
    quint64 timestamp() const;
    EventType type() const;
    QString typeName() const;

    SharedStructure structure();
    const SharedStructure structure() const;

    quint32 sequenceNumber() const;
    void setSequenceNumber(quint32 num);

    EventPtr copy() const;

protected:
    //Workaround while waiting for global refCount hash, REMOVE in the near future
    virtual void ref();
    virtual void unref();
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::FlushStartEvent
 */
class FlushStartEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(FlushStartEvent, Event)
public:
    static FlushStartEventPtr create();
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::FlushStopEvent
 */
class FlushStopEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(FlushStopEvent, Event)
public:
    static FlushStopEventPtr create();
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::EosEvent
 */
class EosEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(EosEvent, Event)
public:
    static EosEventPtr create();
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::NewSegmentEvent
 */
class NewSegmentEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(NewSegmentEvent, Event)
public:
    static NewSegmentEventPtr create(bool update, double rate, double appliedRate, Format format,
                                     qint64 start, qint64 stop, qint64 position);

    bool isUpdate() const;
    double rate() const;
    double appliedRate() const;
    Format format() const;
    qint64 start() const;
    qint64 stop() const;
    qint64 position() const;
};

//TODO GST_EVENT_TAG

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::BufferSizeEvent
 */
class BufferSizeEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(BufferSizeEvent, Event)
public:
    static BufferSizeEventPtr create(Format format, qint64 minSize, qint64 maxSize, bool isAsync);

    Format format() const;
    qint64 minSize() const;
    qint64 maxSize() const;
    bool isAsync() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::BufferSizeEvent
 */
class SinkMessageEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(SinkMessageEvent, Event)
public:
    static SinkMessageEventPtr create(const MessagePtr & msg);

    MessagePtr message() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::QosEvent
 */
class QosEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(QosEvent, Event)
public:
    static QosEventPtr create(double proportion, ClockTimeDiff diff, ClockTime timestamp);

    double proportion() const;
    ClockTimeDiff diff() const;
    ClockTime timestamp() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::SeekEvent
 */
class SeekEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(SeekEvent, Event)
public:
    static SeekEventPtr create(double rate, Format format, SeekFlags flags, SeekType startType,
                               qint64 start, SeekType stopType, qint64 stop);

    double rate() const;
    Format format() const;
    SeekFlags flags() const;
    SeekType startType() const;
    qint64 start() const;
    SeekType stopType() const;
    qint64 stop() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::NavigationEvent
 */
class NavigationEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(NavigationEvent, Event)
public:
    static NavigationEventPtr create(const StructureBase & structure  = SharedStructure(NULL));
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::LatencyEvent
 */
class LatencyEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(LatencyEvent, Event)
public:
    static LatencyEventPtr create(ClockTime latency);

    ClockTime latency() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::StepEvent
 */
class StepEvent : public Event
{
    QGST_WRAPPER_DIFFERENT_C_CLASS(StepEvent, Event)
public:
    static StepEventPtr create(Format format, quint64 amount, double rate,
                            bool flush, bool intermediate);

    Format format() const;
    quint64 amount() const;
    double rate() const;
    bool flush() const;
    bool intermediate() const;
};

} //namespace QGst


#define QGST_EVENT_SUBCLASS_REGISTER_CONVERTERS(CLASS, EVTTYPE) \
    namespace QGlib { \
    namespace Private { \
        template <> \
        struct CanConvertTo<CLASS> \
        { \
            static inline bool from(void *instance) \
            { \
                return (Type::fromInstance(instance).isA(GetType<QGst::Event>()) && \
                        QGst::EventPtr::wrap(static_cast<GstEvent*>(instance))->type() == EVTTYPE); \
            } \
            /* NO 'static inline bool from(Type t)' to disallow usage from Value::get */ \
            /* ValueBase::get is not supposed to provide dynamic_cast capabilities */ \
        }; \
        \
        template <> \
        struct CanConvertFrom<CLASS##Ptr> \
        { \
            static inline bool to(Type t) \
            { \
                return GetType<QGst::Event>().isA(t); \
            } \
        }; \
    } /* namespace Private */ \
    } /* namespace QGlib */

#define QGST_EVENT_SUBCLASS_REGISTER_VALUEIMPL(CLASSPTR, EVTTYPE) \
    namespace QGlib { \
        template<> \
        struct ValueImpl<CLASSPTR> \
        { \
            static void set(ValueBase & value, const CLASSPTR & data) { \
                ValueImpl<QGst::EventPtr>::set(value, data); \
            } \
        }; \
    } /* namespace QGlib */

#define QGST_REGISTER_EVENT_SUBCLASS(TYPE) \
    QGST_EVENT_SUBCLASS_REGISTER_CONVERTERS(QGst::TYPE##Event, QGst::Event##TYPE) \
    QGST_EVENT_SUBCLASS_REGISTER_VALUEIMPL(QGst::TYPE##EventPtr, QGst::Event##TYPE)

QGLIB_REGISTER_TYPE(QGst::Event)
QGLIB_REGISTER_VALUEIMPL(QGst::EventPtr)
QGST_REGISTER_EVENT_SUBCLASS(FlushStart)
QGST_REGISTER_EVENT_SUBCLASS(FlushStop)
QGST_REGISTER_EVENT_SUBCLASS(Eos)
QGST_REGISTER_EVENT_SUBCLASS(NewSegment)
//TODO QGST_REGISTER_EVENT_SUBCLASS(Tag)
QGST_REGISTER_EVENT_SUBCLASS(BufferSize)
QGST_REGISTER_EVENT_SUBCLASS(SinkMessage)
QGST_REGISTER_EVENT_SUBCLASS(Qos)
QGST_REGISTER_EVENT_SUBCLASS(Seek)
QGST_REGISTER_EVENT_SUBCLASS(Navigation)
QGST_REGISTER_EVENT_SUBCLASS(Latency)
QGST_REGISTER_EVENT_SUBCLASS(Step)

#undef QGST_REGISTER_EVENT_SUBCLASS
#undef QGST_EVENT_SUBCLASS_REGISTER_VALUEIMPL
#undef QGST_EVENT_SUBCLASS_REGISTER_CONVERTERS

/*! \relates QGst::Event */
QDebug operator<<(QDebug debug, QGst::EventType type);

/*! \relates QGst::Event */
QDebug operator<<(QDebug debug, const QGst::EventPtr & event);

#endif